#pragma once

#include "Noncopyable.h"
#include <functional>
#include <thread>
#include <memory>
#include <string>
#include <atomic>

namespace Server::Thread
{
    class Thread : Noncopyable{
        using ThreadFunc = std::function<void()>;
        public:
            explicit Thread(ThreadFunc, const std::string& name = "");
            ~Thread();
            void start();
            void join();
            bool started() const;
            bool getTid() const;
            const std::string& name() const;
            static int numCreated();
        private:
            void setDefaultName();
            bool started_;
            bool joined_;
            //we cannot define std::thread thread_ to define a thread
            //since a thread would immediately start once executed
            //so we need a smart pointer to control
            std::shared_ptr<std::thread> thread_;
            pid_t tid_;
            ThreadFunc func_;
            std::string name_;
            static std::atomic_int numCreated_;
    };
} 
