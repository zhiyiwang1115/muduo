#include "Poller.h"
#include "EpollPoller.h"

namespace Server::Net{
    Poller* Poller::newDefaultPoller(EventLoop* loop){
        return new EpollPoller(loop);
    }
}