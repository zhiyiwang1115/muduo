#include "EpollPoller.h"
#include "Logger.h"
#include <error.h>
#include <unistd.h>
#include "Channel.h"
#include <cstring>

namespace Server::Net
{           
            const int kNew = -1; //channel has not been added into poller
            const int kAdded = 1; //channel has been added into poller
            const int kDeleted = 2; //channel have been deleted from poller
            
            const int EpollPoller::kInitEventListSize = 16;
            EpollPoller::EpollPoller(EventLoop* loop)
            :Poller(loop)
            // EPOLL_CLOEXEC flag
            // when a subprocess is forked, the subprocess would close this socketfd
            ,epollfd_(epoll_create1(EPOLL_CLOEXEC))
            ,events_(kInitEventListSize)
            {
                if(epollfd_<0){
                    LOG_ERROR("Epoll create failed with error no: %d \n", errno);
                }
            }
            EpollPoller::~EpollPoller(){
                close(epollfd_);
            }
            //channellist returns channels having events
            Timestamp EpollPoller::poll(int timeOut, ChannelList &activeChannels){
                LOG_INFO("fun=%s => fd total count: %ld\n",__FUNCTION__, channelMap_.size());
                int numEvents = epoll_wait(epollfd_, &*events_.begin(), events_.size(), timeOut);
                //is error no global for all thread or local for each thread?
                int curErrno = errno;
                Timestamp now = Timestamp::now();
                if(numEvents > 0){
                    LOG_INFO("%d events happened \n", numEvents);
                    fillActiveChannels(numEvents, activeChannels);
                    if(numEvents == events_.size()){
                        events_.resize(events_.size()*2);
                    }
                }
                else if (numEvents==0){
                    LOG_DEBUG("time out \n");
                }
                else{
                    //interrupt
                    if(curErrno != EINTR){
                        LOG_WARN("Something wrong when polling");
                    }
                }
                return now;
            }
            void EpollPoller::updateChannel(Channel* channel){
                const int index = channel->index();
                int fd = channel->fd();
                LOG_INFO("func=%s, fd=%d events=%d index=%d \n", __FUNCTION__, fd, channel->events(), channel->index());
                if(index==kNew || index==kDeleted){
                    if(index==kNew){
                        channelMap_[fd] = channel;
                    }
                    //when index==kDelete, that means it's in the map, but no events of interest
                    //as shown in the else statement
                    channel->set_index(kAdded);
                    update(EPOLL_CTL_ADD, channel);
                }
                else{
                    if(channel->isNoneEvent()){
                        update(EPOLL_CTL_DEL, channel);
                        channel->set_index(kDeleted);
                    }
                    else{
                        update(EPOLL_CTL_MOD, channel);
                    }
                }

            }
            void EpollPoller::removeChannel(Channel* channel){
                int fd = channel->fd();
                LOG_INFO("func=%s, fd=%d \n", __FUNCTION__, fd);
                channelMap_.erase(fd);
                if(channel->index()==kAdded){
                    update(EPOLL_CTL_DEL, channel);
                }
                channel->set_index(kNew);
            }
            void EpollPoller::fillActiveChannels(int numEvents, ChannelList &activeChannels) const{
                for(int i = 0;i<numEvents;++i){
                    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
                    channel->set_revents(events_[i].events);
                    activeChannels.emplace_back(channel);
                }
            }
            void EpollPoller::update(int operation, Channel* channel){
                int fd = channel->fd();
                struct epoll_event event;
                memset(&event, 0, sizeof event);
                event.events = channel->events();
                event.data.ptr = channel; //equivalent to event.data.df = fd;
                //let epollfd check event at fd
                if(epoll_ctl(epollfd_,operation,fd,&event) < 0){
                    if(operation == EPOLL_CTL_DEL){
                        LOG_WARN("epoll_ctl delete error: %d\n", errno);
                    }
                    else{
                        LOG_ERROR("epoll_ctl add/modify error: %d\n", errno);
                    }
                }
            }
             
}
