#include "Channel.h"
#include <sys/epoll.h>
#include "EventLoop.h"
#include "Logger.h"

namespace Server::Net{
            const int Channel::kNoneEvent = 0;
            const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
            const int Channel::kWriteEvent = EPOLLOUT;

            Channel::Channel(EventLoop *loop, int fd)
                :loop_(loop)
                , fd_(fd)
                , events_(0)
                , revents_(0)
                , index_(-1){}
            Channel::~Channel(){}
            void Channel::handleEvent(Timestamp receiveTime){
                //TODO handle tie
                handleEventWithGuard(receiveTime);
            }
            void Channel::setReadCallBack(ReadEventCallBack cb){readCallBack_ = std::move(cb);}
            void Channel::setWriteCallBack(EventCallBack cb){writeCallBack_ = std::move(cb);}
            void Channel::setCloseCallBack(EventCallBack cb){closeCallBack_ = std::move(cb);}
            void Channel::setErrorCallBack(EventCallBack cb){errorCallBack_ = std::move(cb);}
            int Channel::events() const{return events_;}
            void Channel::set_revents(int revents){revents_ |= revents;}
  
            void Channel::enableReading(){
                events_ |= kReadEvent; 
                update();
            }
            void Channel::disableReading(){
                events_ &= (~kReadEvent);
                update();
            }
            void Channel::enableWriting(){
                events_ |= kWriteEvent;
                update();
            }
            void Channel::disableWriting(){
                events_ &= (~kWriteEvent);
                update();
            }
            void Channel::disableAll(){events_ = kNoneEvent;}
            
            //check current state
            bool Channel::isNoneEvent() const{return events_==kNoneEvent;}
            bool Channel::isWriting() const{return (events_ & kWriteEvent);}
            bool Channel::isReading() const{return (events_ & kReadEvent);}

            int Channel::index() const{return index_;}
            void Channel::set_index(int index){index_ = index;}

            int Channel::fd() const{return fd_;}

            EventLoop* Channel::ownerLoop() const{return loop_;}
            void Channel::remove(){
                //TODO
                //loop_->removeChannel(this);
            }

            //update poller through eventloop
            void Channel::update(){
                //TODO
                //loop_->updateChannel(this);
            }

            //invoke call back according to revents
            void Channel::handleEventWithGuard(Timestamp receiveTime){
                LOG_INFO("channel handle events: %d", revents_);
                //disconnect
                if( (revents_ & EPOLLHUP) && !(revents_ & EPOLLIN) ){
                    if(closeCallBack_){
                        closeCallBack_();
                    }
                }

                if(revents_ & EPOLLERR){
                    if(errorCallBack_){
                        errorCallBack_();
                    }
                }

                if(revents_ & kReadEvent){
                    if(readCallBack_){
                        readCallBack_(receiveTime);
                    }
                }

                if(revents_ & kWriteEvent){
                    if(writeCallBack_){
                        writeCallBack_();
                    }
                }
            }
}