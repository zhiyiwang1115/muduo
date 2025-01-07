#include "Timestamp.h"
#include <time.h>

namespace Server{
    Timestamp::Timestamp():microSecondsSinceEpoch_(0){}
    
    explicit Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
    :microSecondsSinceEpoch_(microSecondsSinceEpoch){}

    Timestamp Timestamp::now(){
        return Timestamp(time(nullptr));
    }
    std::string Timestamp::toString() const{
        char buf[128] = {0};
        tm* time = localtime(&microSecondsSinceEpoch_);
        //year/month/minute hour:minute:second
        snprintf(buf,128,"%4d/%02d/%02d %02d:%02d:%02d", 
        time->tm_year+1900,
        time->tm_mon + 1,
        time->tm_mday,
        time->tm_hour,
        time->tm_min,
        time->tm_sec);
        return buf;
    }
}