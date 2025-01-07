#pragma once

#include<iostream>

namespace Server{
    class Timestamp{
        public:
            Timestamp();
            //when there is parameter, add explicit
            explicit Timestamp(int64_t microSecondsSinceEpoch);
            static Timestamp now();
            std::string toString() const;
        private:
            int64_t microSecondsSinceEpoch_;
    };
}