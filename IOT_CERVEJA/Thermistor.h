// Small alteration by Mauricio Jabur for Spark Core
// Workshop Garagem fabLab Cerveja IOT

// from https://learn.adafruit.com/thermistor/using-a-thermistor
// thermistor connected to ground, series resitor to 3.3V

#ifndef THERMISTOR_H
#define THERMISTOR_H


#define NUMSAMPLES    5
#define SERIESRESISTOR 10000
#define MAXREAD  4095
#define COEF     4300
#define NOMINALT 25
#define NOMINALR 47000


#include "application.h"

class Thermistor {
    public:
    
        double raw;
        double res;
        double temp;
        
        int port;
        int numSamples;
        double seriesResistor;
        
        int maxRead;
        double coef;
        double nominalT;
        double nominalR;
        
        Thermistor();
        Thermistor(int analog_port);
        void defaults();
        void init();
        void read();
        double update();
};

//////////////////////////////////////////////////////////
// Thermistor.cpp
//////////////////////////////////////////////////////////

//#include "Thermistor.h"
#include "application.h"
#include <math.h>

Thermistor::Thermistor(){
    port = A0;
    defaults();
}

Thermistor::Thermistor(int analog_port){
    port = analog_port;
    defaults();
}

void Thermistor::defaults() {
    numSamples     = NUMSAMPLES;
    seriesResistor = SERIESRESISTOR;
    
    maxRead  = MAXREAD;
    coef     = COEF;
    nominalT = NOMINALT;
    nominalR = NOMINALR;
    
    raw   = 0;
    res   = 0;
    temp  = 0;
}

void Thermistor::init() {
    pinMode (port, INPUT); // really needed?
}

void Thermistor::read() {
    raw = 0;
    for (int i = 0; i < numSamples; i++ ) {
        setADCSampleTime(ADC_SampleTime_41Cycles5);
        raw += analogRead(port);
    }
    raw /= numSamples;
}

double Thermistor::update() {
    read();
    res = seriesResistor/(maxRead/raw -1);
    // steinhart approximation
    temp = -273.15 + 1.0 /(log(res / nominalR)/coef + 1.0/(nominalT + 273.15));
    return temp;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
#endif //THERMISTOR_H
