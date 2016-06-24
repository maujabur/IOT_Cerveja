// By Mauricio Jabur for Spark Core
// Workshop Garagem fabLab Cerveja IOT

#ifndef ROLLING_H
#define ROLLING_H

#include "application.h"

#define F_OLD 19
#define F_NEW 1

class Rolling {
    public:
        int f_old;
        int f_new;
        float reading;
        float average;
        
        Rolling();
        Rolling(float value);
        
        float set(float value);
        float put(float value);
};

//////////////////////////////////////////////////////////
// Interval.cpp
//////////////////////////////////////////////////////////

//#include "Rolling.h"

Rolling::Rolling(){
    f_old = F_OLD;
    f_new = F_NEW;
    reading = 0;
    average = 0;
}

Rolling::Rolling(float value){
    reading = value;
    average = value;
}

float Rolling::set(float value) {
    reading = value;
    average = value;
    return average;
}
float Rolling::put(float value) {
    reading = value;
    average = (average*f_old+value*f_new)/(f_old+f_new);
    return average;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
#endif //ROLLING_H
