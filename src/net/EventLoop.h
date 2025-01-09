#pragma once
#include "Noncopyable.h"
#include <functional>
#include <vector>
#include <atomic>
#include "Timestamp.h"
#include <memory>
#include <mutex>

namespace Server::Net{
    class Channel;
    class Poller;

    class EventLoop: Noncopyable{
        using ChannelList = std::vector<Channel*>;
        public:
            using Functor = std::function<void()>;
            EventLoop();
            ~EventLoop();

            void loop();
            void quit();
            Timestamp pollReturnTime() const;

            //comments: 
            void runInLoop(Functor cb);
            void queueInLoop(Functor cb);

            void wakeup();
            void updateChannel(Channel* channel);
            void removeChannel(Channel* channel);
            //comments:
            bool isInLoopThread() const;
        private:
            void handleRead(Timestamp ts);
            void doPendingFunctors();
            //implemented with (Compare-And-Swap) CAS 
            std::atomic_bool looping_;
            std::atomic_bool quit_;
            // the thread id of current loop
            const pid_t threadId_; 
            //the time that poller return events
            Timestamp pollReturnTime_;
            std::unique_ptr<Poller> poller_;
            //used to notify current eventloop to accept new channels created by eventfd()
            //since most time eventloop is blocked
            int wakeupFd_;
            std::unique_ptr<Channel> wakeupChannel_;
            ChannelList activeChannels_;


            //check if there is pending functors stored in the following vector
            std::atomic_bool callingPendingFunctors_; 
            std::vector<Functor> pendingFunctors_;
            std::mutex mutex_; //protect pending Functors_
    };
}
