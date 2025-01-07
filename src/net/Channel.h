#pragma once
#include "Noncopyable.h"
#include "Timestamp.h"
#include <functional>

namespace Server::Net{
    //so dont need to expose EventLoop header file
    //since we include eventloop as a pointer, so we can not define eventloop here
    //but we need to include timesteamp header file here since we do not pass it by pointer
    class EventLoop;
    //pack socketfd and events of interest (e.g. EPOLLIN)
    class Channel : Noncopyable{
        public:
            using EventCallBack = std::function<void()>;
            using ReadEventCallBack = std::function<void(Timestamp)>;
            Channel(EventLoop *loop, int fd);
            ~Channel();
            void handleEvent(Timestamp receiveTime);
            void setReadCallBack(ReadEventCallBack cb);
            void setWriteCallBack(EventCallBack cb);
            void setCloseCallBack(EventCallBack cb);
            void setErrorCallBack(EventCallBack cb);
            int events() const;
            void set_revents(int revt);
  
            void enableReading();
            void disableReading();
            void enableWriting();
            void disableWriting();
            void disableAll();
            
            //check current state
            bool isNoneEvent() const;
            bool isWriting() const;
            bool isReading() const;

            int index() const;
            void set_index(int index);

            EventLoop* ownerLoop() const;
            void remove();

        private:
            static const int kNoneEvent;
            static const int kReadEvent;
            static const int kWriteEvent;

            //one loop has multiple channels
            EventLoop *loop_;
            const int fd_; //file descriptor that the channel is listening
            int events_; //the events of interest
            int revents_; //returned events of interest
            int index_;

            ReadEventCallBack readCallBack_;
            EventCallBack writeCallBack_;
            EventCallBack closeCallBack_;
            EventCallBack errorCallBack_;

            void update();
            void handleEventWithGuard(Timestamp receiveTime);
    };
}