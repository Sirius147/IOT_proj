#include <stdio.h>
#include <pigpio.h>

int main(){

	int pin = 18;
	int switch_pin = 24;
	int state = 1;
	gpioInitialise();

	gpioSetMode(pin, PI_OUTPUT);
	gpioSetMode(switch_pin, PI_INPUT);

	while(1){
		if(gpioRead(switch_pin) == 0){
			gpioWrite(pin, state%2);

		}
		state+=1;
	}
	gpioTerminate();
	return 0;
}
