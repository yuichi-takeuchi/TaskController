#include "mbed.h"

// mbed fuctions
AnalogIn aLick1(PA_0);
AnalogIn aLick2(PA_1);
DigitalOut dLick1(PB_2);
DigitalOut dLick2(PB_3);
DigitalOut mainled(PC_8);  // (main function is on)
DigitalOut userled(PC_9);	// (user button is on)
Ticker tic_ticker; // main ticker
Ticker led_ticker1; // main LED ticker
Ticker led_ticker2; // user LED ticker
Ticker readwrite_ticker1; // basic cycle ticker 1
Ticker readwrite_ticker2; // basic cycle ticker 1
InterruptIn userbutton(USER_BUTTON); // (PC13)

float vrange = 3.3; // in V
float threshold1 = 0; // in V
float threshold2 = 0; // in V

// ios value
bool taskrun = 0;

void blink_mainled(void){
	mainled = !mainled;
}

// user button is on
void blink_userled(void){
	userled = !userled;
}

// readwrite1
void readwrite1(void){
	float adc_v1;
	adc_v1 = vrange * aLick1.read(); // Converts and read the analog input value (0.0 to 3.3 V to 0.0 to 1.0)
	
	if(adc_v1 > threshold1){
		dLick1 = 1;
	} else {
		dLick1 = 0;
	}
}

// readwrite2
void readwrite2(void){
	float adc_v2;
	adc_v2 = vrange * aLick2.read(); // Converts and read the analog input value (0.0 to 3.3 V to 0.0 to 1.0)
	
	if(adc_v2 > threshold2){
		dLick2 = 1;
	} else {
		dLick2 = 0;
	}	
}

// button control
void userbuttoncallback(void){
	taskrun = !taskrun;

	if(taskrun){			
		// userled on
		led_ticker2.attach(&blink_userled, 0.1);
		
		// read write 1 on
		readwrite_ticker1.attach_us(&readwrite1, 100);
		
		// read write 2 on
		readwrite_ticker2.attach_us(&readwrite2, 100);
	}
	else {
		// userled off
		led_ticker2.detach();
		userled = 0;
			
		// readwrite off
		readwrite_ticker1.detach();
		readwrite_ticker2.detach();
		dLick1 = 0;
		dLick2 = 0;
	}
}

int main() {
		// set button
		userbutton.fall(&userbuttoncallback);
		
		// ticker
		led_ticker1.attach(&blink_mainled, 0.1);
    while(1) {
    }
}
