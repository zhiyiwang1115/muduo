#pragma once
#include "Noncopyable.h"
#include <vector>
#include <unordered_map>
#include "Timestamp.h"

namespace Server::Net{
    class Channel;
    class EventLoop;

    class Poller: Noncopyable{
        using ChannelList = std::vector<Channel*>;
        using ChannelMap = std::unordered_map<int, Channel*>;
        public:
            explicit Poller(EventLoop* loop);
            virtual ~Poller();
            virtual Timestamp poll(int timeOut, ChannelList &activeChannels) = 0;
            virtual void updateChannel(Channel* channel) = 0;
            virtual void removeChannel(Channel* channel) = 0;
            bool hasChannel(Channel *channel);
            static Poller* newDefaultPoller(EventLoop* loop);

        protected:
            ChannelMap channelMap_;
        private:
            EventLoop* loop_;
    };    
}