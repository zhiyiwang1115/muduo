#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>
namespace Server::Thread
{
            //cannot initilize with std::atomic_int Thread::numCreated_ = 0;
            //since atomic_int is not copyable
            std::atomic_int Thread::numCreated_{0};
            Thread::Thread(ThreadFunc func, const std::string& name)
            : func_(std::move(func))
            , name_(name)
            , started_(false)
            , joined_(false)
            , tid_(0)
            {
                setDefaultName();
            }
            Thread::~Thread(){
                // that means it is a daemon thread
                if(started_ && !joined_){
                    //detach the thread
                    thread_->detach();
                }
            }
            void Thread::start(){
                started_ = true;
                sem_t semaphore;
                sem_init(&semaphore, false, 0);
                thread_ = std::make_shared<std::thread>([this,&semaphore]() {
                    tid_ = tid();
                    sem_post(&semaphore);
                    func_();
                });            

                //here in the main thread, need to wait new thread created, we then check its tid
                sem_wait(&semaphore);

            }
            void Thread::join(){
                joined_ = true;
                thread_->join();
            }
            bool Thread::started() const{return started_;}
            bool Thread::getTid() const{return tid_;}
            const std::string& Thread::name() const{return name_;}
            int Thread::numCreated(){return numCreated_;}
            void Thread::setDefaultName(){
                int num = ++numCreated_;
                if(name_.size()==0){
                    name_ = "Thread" + std::to_string(num);
                }
            }
} 
