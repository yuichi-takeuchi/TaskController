#include "mbed.h"

// mbed fuctions
PortIn hPortA(PortA, 0xFFFF); // PA_0-15
PortOut hPortB(PortB, 0xFFFF); //PB_0-15
DigitalIn din_PC0(PC_0); // aux 0
DigitalIn din_PC1(PC_1); // aux 1
DigitalIn din_PC2(PC_2); // aux 2
DigitalIn din_PC3(PC_3); // aux 3
DigitalIn din_PC4(PC_4); // switch 1 (valve right)
DigitalIn din_PC5(PC_5); // switch 2 
DigitalIn din_PC6(PC_6); // switch 3 (valve left)
DigitalIn din_PC7(PC_7); // switch 4
DigitalOut mainled(PC_8);  // (main function is on)
DigitalOut userled(PC_9);	// (user button is on)
Ticker tic_ticker; // main ticker
Ticker led_ticker1; // main LED ticker
Ticker led_ticker2; // user LED ticker
Ticker iosin_ticker; // ios input ticker
Ticker iosout_ticker; // ios output ticker
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
	
	if(tic >= 0.1 && (din_PC4|| din_PC6)){
		PortBValue += 0b0001;
	}
	
  if(0 ){ // valve left && (din_PC6)
		PortBValue += 0b0010;
	}
	
	if(0){ // valve center
		PortBValue += 0b0100;
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

int main() {
	// set button
	userbutton.fall(&userbuttoncallback);
		
	// ticker
	led_ticker1.attach(&blink_mainled, 0.1);
	iosin_ticker.attach_us(&ios_in, 1000);
		
    while(1) {
    }
}
