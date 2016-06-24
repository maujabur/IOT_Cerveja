// This #include statement was automatically added by the Spark IDE.
#include "Rolling.h"
Rolling temp1;

#include "Interval.h"
Interval ticker(100);
Interval sender(60000);

#include "thingspeak/thingspeak.h"
ThingSpeakLibrary::ThingSpeak thingspeak ("KPCFK2OSA15E2J74");

#include "Thermistor.h"
Thermistor therm1(A0);

#include "ht16k33_bubble.h"

#if defined(ARDUINO)
#include "Wire.h"
#endif


int enabled = 1;

long count = 0;
double get_temperature;

void setup()
{
    Serial.begin(115200);
    
    setup_metodos();

    setup_display();
    
    therm1.seriesResistor = 9970;
    get_temperature=temp1.set(therm1.update());

 }
 
void loop ()
{
    if (ticker.check() ){
        ticker.reset(); // reinicia contagem de tempo
        
        get_temperature = temp1.put(therm1.update());
        Serial.print("temp1: "); 
        Serial.println(temp1.average);
        
        update_display();
      }

    
     if (sender.check() && enabled !=0){
        post_measure("mauricio");
   }
}

int post_measure(String args) {
    int result = -1;
    
    bool valSet;
 
     valSet = thingspeak.recordValue(1, String(temp1.average, DEC));
     if(valSet) {
        Serial.println("Campo 1 (temp) : " + String(temp1.average, DEC));
    } else {
        Serial.println("Erro ao gravar campo 1");
    }
    
    bool valsSent = thingspeak.sendValues();
    if(valsSent) {
        Serial.println("Enviado com sucesso ao thingspeak");
        count = count + 1;
        result = count;
    } else {
        Serial.println("Falha ao enviar para thingspeak ");
    }

    return result; 
}

void setup_metodos() {
    Spark.variable("temperatura", &get_temperature, DOUBLE);

    Spark.variable("contagens", &count, INT);
    Spark.variable("habilitado", &enabled, INT);
    
    Spark.function("conta", set_count);
    Spark.function("publica", post_measure);
    Spark.function("habilita", enabler);
}

int set_count (String args){
    count = args.toInt();
    return 1;
}

int enabler (String args) {
    int result = args.toInt();
    enabled = result;
    return result;
}

void setup_display() {
    Wire.begin();
    initHT16K33();  // init display
    clearDsplay(1);  // clear display 1
    clearDsplay(2);  // clear display 2
    clearDsplay(3);  // clear display 3
    clearDsplay(4);  // clear display 4
    
    for (uint8_t d = 1; d <= 4; d++) {
        for (uint8_t p = 1; p <= 4; p++) {
            writeDigit(d, p, 8, 1)
            delay(250);
        }
    }

    delay(5000);
    Serial.println("Iniciado");
    
    for (uint8_t d = 1; d <= 4; d++) {
        writeInteger(d, 8888);
        blinkHT16K33(2);
        clearDsplay(d);
    }
}

void update_display() {
         //clearDsplay(4);
        writeFloat(3, temp1.average, 1);
        
        writeDigit(1, 1, 17, 0);  // E = 17
        writeDigit(1, 2, enabled, 0);
        
        writeDigit(4, 2, 32, 0); // degree = 32
        writeDigit(4, 3, 15, 0); // C = 15
       
        writeInteger(2, count%9999);
}
