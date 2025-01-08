#pragma once

namespace Server::Thread{
    //extern means it would be initialized in other files (since it is not a class variable)
    //thread_local means id is a local variable in each thread
    extern thread_local int t_cachedTid;

    void cacheTid();

    //cache the id, since system call is not efficient
    inline int tid(){
        // The __builtin_expect is a compiler hint provided by GCC and Clang to indicate the likely outcome of a condition
        if(__builtin_expect(t_cachedTid==0,0)){
            cacheTid();
        }
        return t_cachedTid;
    }
}