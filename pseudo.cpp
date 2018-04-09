// Here is pseudo code for arduino

int state 	= 0; // off = 0	idle = 10	booting = 100	on = 1000	customization = 5000
int medium 	= 0; // off = 0	keyboard = 10	paper = 100	detail = 1000	social = 5000
int angle 	= 0; // TODO -- get these values

int time_customize_up 	= 0;
int time_customize_on 	= 0;
int time_customize_down = 0;
int time_boot 	= 0;
int time_wake 	= 0;
int time_sleep 	= 0;

void setup(){
	lights = [l1, l2, l3, ...]
	values = [[r,g,b], [r,g,b], [r,g,b] ...]
}

void loop(){
	timedFunctions();
	controlFunctions();
}

void timedFunctions(){

}
void controlFunctions(){
	switch(sate){
		case 0:
			//set to idle state
		break;
		case 10:
			// read proximity sensor for activity
		break;
		case 100:
			//boot up
		break;
	}
}


// Proximity Sensor
void idleProximity(){
	readProximity();
}
int readProximity(){
	return value;
}


void fadeTo(pin, target, speed){

}
