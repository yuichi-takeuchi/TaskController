#include "mbed.h"

// mbed fuctions
PortOut hPortA(PortA, 0xFFFF); // PA_0-15
PortOut hPortB(PortB, 0xFFFF); //PB_0-15
DigitalIn exitTrigger (PC_4); // trigger from C1
DigitalOut mainled(PC_8);  // (main function is on)
DigitalOut userled(PC_9);	// (user button is on)
DigitalOut reset_TTL(PC_10); // (reset TTL)
DigitalOut userbutton_TTL(PC_11); // (userbutton TTL)
Ticker tic_ticker; // main ticker
Ticker led_ticker1; // main LED ticker
Ticker led_ticker2; // user LED ticker
Ticker exit_ticker; // monitor of exit trigger
Ticker iosout_ticker_a; // ios input ticker
Ticker iosout_ticker_b; // ios output ticker
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

void reset_C1C2(void){
	if(exitTrigger){
		userbutton_TTL = 1;
		wait(0.1);
		reset_TTL = 1;
		wait(0.1);
	}
	else{
		reset_TTL = 0;
		userbutton_TTL = 0;
	}
}	

// user button is on
void blink_userled(void){
	userled = !userled;
}

// write PortA
void ios_out_a(void){
	PortAValue = 0x0000;
	
//	if(tic < 1){
//		PortAValue += 0b0000000001;
//	}
	
	hPortA = PortAValue;
}

// write PortB
void ios_out_b(void){
	PortBValue = 0x0000;

//	if(tic < 1){
//		PortBValue += 0b00000001;
//	}
	
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
		iosout_ticker_a.attach_us(&ios_out_a, 1000);
 		iosout_ticker_b.attach_us(&ios_out_b, 1000);
		
		// wait exit trigger
		exit_ticker.attach_us(&reset_C1C2, 1000);
	}
	else {
		// count tic off
		tic_ticker.detach();
		tic = 0;
				
		// userled off
		led_ticker2.detach();
		userled = 0;
			
		// ios output off
		iosout_ticker_a.detach();
		iosout_ticker_b.detach();
		hPortA = 0b0000000000;
		hPortB = 0b00000000;
		
		// wait exit trigger
		exit_ticker.detach();
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
