#pragma once

namespace Server{
    class Noncopyable{
        public:
            Noncopyable(const Noncopyable&) = delete;
            Noncopyable& operator=(const Noncopyable&) = delete;

        //make sure it can only used as a base class
        protected:
            Noncopyable() = default;
            ~Noncopyable() = default;
    };
}