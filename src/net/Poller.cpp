#include "Poller.h"
#include "Channel.h"


namespace Server::Net{
            Poller::Poller(EventLoop* loop):loop_(loop){}
            bool Poller::hasChannel(Channel* channel){
                auto it = channelMap_.find(channel->fd());
                return it != channelMap_.end() && it->second == channel;
            }
            //do not implement newDefaultPoller here since it would need to import child class
            //to create an instance
}