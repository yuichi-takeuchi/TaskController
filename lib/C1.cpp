#include "mbed.h"

// mbed fuctions
PortIn hPortA(PortA, 0xFFFF); // PA_0-15
PortOut hPortB(PortB, 0xFFFF); //PB_0-15
DigitalOut PanelC3_TTL(PC_4); // (reset and userbutton for C3)
DigitalOut mainled(PC_8);  // (main function is on)
DigitalOut userled(PC_9);	// (user button is on)
//DigitalOut resetTTL(PC_10); // (resetTTL)
//DigitalOut userbuttonTTL(PC_11); // (userbuttonTTL)
Ticker tic_ticker; // main ticker
Ticker led_ticker1; // main LED ticker
Ticker led_ticker2; // user LED ticker
Ticker iosin_ticker; // ios input ticker
Ticker iosout_ticker; // ios output ticker
Ticker exit_ticker; // exit TTL
InterruptIn userbutton(USER_BUTTON); // (PC13)

// main tic
float tic = 0;

// ios value
int PortAValue = 0x0000;
int PortBValue = 0x0000;
bool taskrun = 0;

void count_tic(void){
		tic += 0.0001; // s
}

void blink_mainled(void){
		mainled = !mainled;
}

// user button is on
void blink_userled(void){
		userled = !userled;
}

// read PortA
void ios_in(void){
	if(tic == 0){
		PortAValue = hPortA.read();
	}
}

// write PortB
void ios_out(void){
	PortBValue = 0x0000;
		
	if(tic < 1){
		PortBValue += 0b00000001;
	}
	

	if(tic < 2){
		PortBValue += 0b01100000;
}
		
	hPortB = PortBValue;
}

// button control
void userbuttoncallback(void){
	taskrun = !taskrun;

	if(taskrun){
		// count tic on
		tic_ticker.attach_us(&count_tic, 100);
			
		// userled on
		led_ticker2.attach(&blink_userled, 0.1);
			
		// ios output on
		iosout_ticker.attach_us(&ios_out, 1000);
	}
	else {
		// count tic off
		tic_ticker.detach();
		tic = 0;
				
		// userled off
		led_ticker2.detach();
		userled = 0;
			
		// ios output off
		iosout_ticker.detach();
		hPortB = 0;
	}
}

void exit_TTL(void){
	if(tic >  5){
		PanelC3_TTL = 1;
	}
	else {
		PanelC3_TTL = 0;
	}
}

int main() {
	// set button
	userbutton.fall(&userbuttoncallback);
		
	// ticker
	led_ticker1.attach(&blink_mainled, 0.1);
	iosin_ticker.attach_us(&ios_in, 1000);
	exit_ticker.attach(&exit_TTL, 0.1);
	
    while(1) {
	}
}
