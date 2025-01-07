#pragma once
#include "Noncopyable.h"
#include <string>

//input similar to printf
//do{}while(0) to ensure that the macro behaves like a single statement
#define LOG_INFO(logMsgFormat, ...) \
    do{ \
        Logger &logger = Server::Logger::instance(); \
        logger.setLogLevel(Server::LogLevel::INFO); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logMsgFormat, ##__VA_ARGS__);\
        logger.log(buf); \
    } while(0)

#define LOG_WARN(logMsgFormat, ...) \
    do{ \
        Logger &logger = Server::Logger::instance(); \
        logger.setLogLevel(Server::LogLevel::WARN); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logMsgFormat, ##__VA_ARGS__);\
        logger.log(buf); \
    } while(0)

#define LOG_ERROR(logMsgFormat, ...) \
    do{ \
        Logger &logger = Server::Logger::instance(); \
        logger.setLogLevel(Server::LogLevel::ERROR); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logMsgFormat, ##__VA_ARGS__);\
        logger.log(buf); \
    } while(0)

#ifdef DEBUG
#define LOG_DEBUG(logMsgFormat, ...) \
    do{ \
        Logger &logger = Server::Logger::instance(); \
        logger.setLogLevel(Server::LogLevel::DEBUG); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logMsgFormat, ##__VA_ARGS__);\
        logger.log(buf); \
    } while(0)
#else
#define LOG_DEBUG(logMsgFormat, ...)
#endif

namespace Server {
    // INFO ERROR FATAL DEBUG
    enum LogLevel{
        INFO,
        WARN,
        ERROR,
        DEBUG
    };

    //skeleton
    class Logger: Server::Noncopyable {
        public:
            static Logger& instance();
            void setLogLevel(LogLevel level);
            void log(const std::string &msg);
        private:
            int logLevel_;
            Logger(){};
    };
}
