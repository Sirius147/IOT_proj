#include <stdio.h>
#include <pigpio.h>
int main(){

	int pins[3] = {25,18,23};
	int pin_cnt = 0;
	gpioInitialise();
	gpioSetMode(pins[0], PI_OUTPUT);
	gpioSetMode(pins[1], PI_OUTPUT);
	gpioSetMode(pins[2], PI_OUTPUT);

	while(1){
		if(pin_cnt%3 == 0){
			gpioWrite(pins[0],1);
			gpioDelay(1000000);
			gpioWrite(pins[0],0);
		}
		else if(pin_cnt%3 == 1){
			gpioWrite(pins[1],1);
                        gpioDelay(1000000);
                        gpioWrite(pins[1],0);
		}
		else{
			gpioWrite(pins[2],1);
                        gpioDelay(1000000);
                        gpioWrite(pins[2],0);
		}
		pin_cnt+=1;
	}
	gpioTerminate();
	return 0;
}
