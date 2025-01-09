#include "EventLoop.h"
#include "CurrentThread.h"
#include <fcntl.h>
#include <sys/eventfd.h>
#include "Logger.h"
#include "Poller.h"
#include "Channel.h"
#include <unistd.h>

namespace Server::Net{
            //global variable to prevent a thread creating more than one event loop
            //singleton in thread
            thread_local EventLoop *t_loopInThisThread = nullptr;

            const int kPollTimeMs = 10000;

            int createEventfd(){
                //EFD_NONBLOCK: This flag sets the file descriptor to non-blocking mode
                //EFD_CLOEXEC: This flag ensures that the file descriptor is automatically closed if a new process is created with execve()
                //we also have a EPOLL_CLOEXEC
                int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
                if(evtfd){
                    LOG_ERROR("event fd error: %d", errno);
                }
                return evtfd;
            }

            EventLoop::EventLoop()
            :looping_(false)
            , quit_(false)
            , callingPendingFunctors_(false)
            , threadId_(Thread::tid())
            , poller_(Poller::newDefaultPoller(this))
            , wakeupFd_(createEventfd())
            , wakeupChannel_(new Channel(this, wakeupFd_))
            {
                LOG_DEBUG("EventLopp created %p in thread %d \n", this, threadId_);
                if(t_loopInThisThread){
                    LOG_ERROR("Another EventLoop %p existed in this thread% d \n", t_loopInThisThread, threadId_);
                }else{
                    t_loopInThisThread = this;
                }
                // need to further set the event that wakeUp is interested and callback function
                auto handleReadFunc = [this](Timestamp ts) { this->handleRead(ts); };
                wakeupChannel_->setReadCallBack(handleReadFunc);
                //every eventloop would listen to wakup channel read event
                wakeupChannel_->enableReading();
            }

            EventLoop::~EventLoop(){
                wakeupChannel_->disableAll();
                wakeupChannel_->remove();
                close(wakeupFd_);
                t_loopInThisThread = nullptr;
            }

            void EventLoop::loop(){
                looping_ = true;
                quit_ = false;
                LOG_INFO("EventLoop %p start looping \n", this);
                while(!quit_){
                    activeChannels_.clear();
                    pollReturnTime_ = poller_->poll(kPollTimeMs, activeChannels_);
                    for(auto channel: activeChannels_){
                        channel->handleEvent(pollReturnTime_);
                    }
                    //mainLoop would distribute connections (channels) to differnt loops
                    doPendingFunctors();
                }
                LOG_INFO("EventLoop %p stop looping", this);
                looping_ = false;
            }

            //quit two cases
            //1. eventloop quit by itself
            //2. sub loop quits a main loop or a main loop quits sub loop (thread id is different from current one)?
            //some loop "l" -> wakeup would wake up this l then
            //when l wake up, its quit_ has been set to false
            //we can do some optimization to decouple main loop and sub loop by using a message queue
            //main loop produces channels, and sub loop consumes channels
            void EventLoop::quit(){
                quit_ = true;
                if(!isInLoopThread()){
                    wakeup();
                }
            }
            Timestamp EventLoop::pollReturnTime() const{return pollReturnTime_;}

            //this is similar to quit logic
            void EventLoop::runInLoop(Functor cb){
                if(isInLoopThread()){
                    cb();
                }else{
                    queueInLoop(cb);
                }
            }

            void EventLoop::queueInLoop(Functor cb){
                //smart lock, would release when out of scope
                // since multiple threads can insert element to vector, make it thread safe
                //scope of mutex
                {
                std::unique_lock<std::mutex> lock(mutex_);
                pendingFunctors_.emplace_back(cb);
                }
                //when callingPendingFunctors_ is true, then, it is doing callback in the vector
                //we need to wakeup it again to let it cb just written in pendingFunctors_
                if(!isInLoopThread() || callingPendingFunctors_){
                    wakeup();
                }
            }

            void EventLoop::wakeup(){
                uint64_t one = 1;
                ssize_t n = write(this->wakeupFd_, &one, sizeof one);
                if(n!=sizeof one){
                    LOG_WARN("%s writes %ld bytes rather than 8", __FUNCTION__, n);
                }
            }

            void EventLoop::updateChannel(Channel* channel){
                poller_->updateChannel(channel);
            }
            void EventLoop::removeChannel(Channel* channel){
                poller_->removeChannel(channel);
            }
            //comments:
            bool EventLoop::isInLoopThread() const{
                return threadId_ == Thread::tid();
            }
            void EventLoop::handleRead(Timestamp ts){
                //data is not important, just wakeup eventloop
                uint64_t one = 1;
                ssize_t n = read(wakeupFd_, &one, sizeof one);
                if(n!=sizeof one){
                    LOG_WARN("%s read s %ld bytes rather than 8 at %s", __FUNCTION__, n, ts.toString().c_str());
                }
            }

            void EventLoop::doPendingFunctors(){
                std::vector<Functor> tmpFunctors;
                callingPendingFunctors_ = true;
                //we just do swap one time, so that the lock is released, which do not block other threads
                {
                std::unique_lock<std::mutex> lock(mutex_);
                tmpFunctors.swap(pendingFunctors_);
                }
                for(auto& functor: tmpFunctors){
                    functor();
                }
                callingPendingFunctors_ = false;
            }

}