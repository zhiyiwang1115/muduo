#pragma once
#include "Poller.h"
#include <vector>
#include <sys/epoll.h>

namespace Server::Net
{   
    class EpollPoller: Poller{
        using EventList = std::vector<epoll_event>;
        public:
            EpollPoller(EventLoop* loop);
            ~EpollPoller() override;
            Timestamp poll(int timeOut, ChannelList &activeChannels) override;
            virtual void updateChannel(Channel* channel) override;
            virtual void removeChannel(Channel* channel) override;
        private:
            void fillActiveChannels(int numEvents, ChannelList &activeChannels) const;
            void update(int operation, Channel* channel);
            static const int kInitEventListSize;
            EventList events_;
            // epollfd_ is the file descriptor for the epoll instance as watcher
            int epollfd_; 
    };
}
