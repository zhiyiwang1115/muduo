#include "Logger.h"
#include <iostream>
#include "Timestamp.h"

namespace Server{
    Logger& Logger::instance(){
        static Logger logger;
        return logger;
    }

    void Logger::setLogLevel(LogLevel level){
        logLevel_ = level;
    }

    void Logger::log(const std::string &msg){
        if(logLevel_==LogLevel::INFO){
            std::cout << "[INFO]";
        }else if(logLevel_==LogLevel::ERROR){
            std::cout << "[ERROR]";
        }else if(logLevel_==LogLevel::WARN){
            std::cout << "[WARN]";
        }else if(logLevel_==LogLevel::DEBUG){
            std::cout << "[DEBUG]";
        }

        std:: cout << Timestamp::now().toString() << " : " << msg << std::endl;
    }
}