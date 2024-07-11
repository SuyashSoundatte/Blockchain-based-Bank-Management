#include "include/CurrentTime.h"

std::string CurrentTime::getCurrentDate(){
            time_t tt;

            // Declaring variable to store return value of 
            // localtime() 
            struct tm* ti;

            // Applying time() 
            time(&tt);

            // Using localtime() 
            ti = localtime(&tt);
            
            std::string time = asctime(ti);
            return time;
        }