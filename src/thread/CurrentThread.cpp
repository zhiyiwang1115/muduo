#include "CurrentThread.h"
#include <unistd.h>
#include <sys/syscall.h>

namespace Server::Thread{
    thread_local int t_cachedTid = 0;
    
    void cacheTid(){
        if(t_cachedTid==0){
            t_cachedTid = syscall(SYS_gettid);
        }
    }


}