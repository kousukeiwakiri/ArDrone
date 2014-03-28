#include "ardrone/ardrone.h"
#include <time.h>

void Init(void);
bool KeyInput();
void Forward(int interval_sec , double move_val);
void Roll(int interval_sec , double move_val);
void Stop(int interval_sec);
void MoveVal(int interval_sec , double vx , double vy , double vz , double vr); //汎用的な関数

// AR.Drone class
ARDrone ardrone;

//パラメータ
struct{
	double vx;
	double vy;
	double vz;
	double vr;
}drone_parameter;


int main(int argc, char **argv)
{

	if (!ardrone.open()) return -1;
	Init();

	ardrone.takeoff();
	Stop(5);
	Forward(10 , 10.0);
	Stop(5);
	Roll(5 , 1.0);
	Stop(5);
	Forward(10 , 10.0);
	ardrone.landing();

	ardrone.close();

	return 0;
}



void Init(void)
{



	// Battery
	printf("Battery = %d%%\n", ardrone.getBatteryPercentage());

	// Instructions
	printf("***************************************\n");
	printf("*       CV Drone sample program       *\n");
	printf("*           - How to Play -           *\n");
	printf("***************************************\n");
	printf("*                                     *\n");
	printf("* - Controls -                        *\n");
	printf("*    'Space' -- Takeoff/Landing       *\n");
	printf("*    'Up'    -- Move forward          *\n");
	printf("*    'Down'  -- Move backward         *\n");
	printf("*    'Left'  -- Turn left             *\n");
	printf("*    'Right' -- Turn right            *\n");
	printf("*    'Q'     -- Move upward           *\n");
	printf("*    'A'     -- Move downward         *\n");
	printf("*                                     *\n");
	printf("* - Others -                          *\n");
	printf("*    'C'     -- Change camera         *\n");
	printf("*    'Esc'   -- Exit                  *\n");
	printf("*                                     *\n");
	printf("***************************************\n\n");
}


void Forward(int interval_sec , double move_val)
{
	time_t old_time = time(NULL);
	while(1)
	{
		drone_parameter.vx = move_val;
		drone_parameter.vy = 0.0;
		drone_parameter.vz = 0.0;
		drone_parameter.vr = 0.0;
		bool control_flag = KeyInput();
		if(control_flag == false) ardrone.close();
		ardrone.move3D( drone_parameter.vx , drone_parameter.vy , drone_parameter.vz , drone_parameter.vr );
		time_t now_time = time(NULL);
		if(now_time - old_time > interval_sec)break;
	}
	old_time = time(NULL);
}

void Roll(int interval_sec , double move_val)
{
	time_t old_time = time(NULL);
	while(1)
	{
		drone_parameter.vx = 0.0;
		drone_parameter.vy = 0.0;
		drone_parameter.vz = 0.0;
		drone_parameter.vr = move_val;
		bool control_flag = KeyInput();
		if(control_flag == false) ardrone.close();
		ardrone.move3D( drone_parameter.vx , drone_parameter.vy , drone_parameter.vz , drone_parameter.vr );
		time_t now_time = time(NULL);
		if(now_time - old_time > interval_sec)break;
	}
	old_time = time(NULL);
}

void Stop(int interval_sec)
{
	time_t old_time = time(NULL);
	while(1)
	{
		drone_parameter.vx = 0.0;
		drone_parameter.vy = 0.0;
		drone_parameter.vz = 0.0;
		drone_parameter.vr = 0.0;
		bool control_flag = KeyInput();
		if(control_flag == false) ardrone.close();
		ardrone.move3D( drone_parameter.vx , drone_parameter.vy , drone_parameter.vz , drone_parameter.vr );
		time_t now_time = time(NULL);
		if(now_time - old_time > interval_sec)break;
	}
	old_time = time(NULL);
}

void MoveVal(int interval_sec , double vx , double vy , double vz , double vr)
{
	time_t old_time = time(NULL);
	while(1)
	{
		drone_parameter.vx = vx;
		drone_parameter.vy = vy;
		drone_parameter.vz = vz;
		drone_parameter.vr = vr;
		bool control_flag = KeyInput();
		if(control_flag == false) ardrone.close();
		ardrone.move3D( drone_parameter.vx , drone_parameter.vy , drone_parameter.vz , drone_parameter.vr );
		time_t now_time = time(NULL);
		if(now_time - old_time > interval_sec)break;
	}
	old_time = time(NULL);
}

bool KeyInput(void)
{
	// Key input
	int key = cvWaitKey(33);
	if (key == 0x1b) return false;

	// Update
	if (!ardrone.update()) return false;

	// Get an image
	IplImage *image = ardrone.getImage();

	// Take off / Landing 
	if (key == ' ') {
		if (ardrone.onGround()) ardrone.takeoff();
		else                    ardrone.landing();
	}

	if (key == 0x260000) drone_parameter.vx =  1.0;
	if (key == 0x280000) drone_parameter.vx = -1.0;
	if (key == 0x250000) drone_parameter.vr =  1.0;
	if (key == 0x270000) drone_parameter.vr = -1.0;
	if (key == 'q')      drone_parameter.vz =  1.0;
	if (key == 'a')      drone_parameter.vz = -1.0;

	// Change camera
	static int mode = 0;
	if (key == 'c') ardrone.setCamera(++mode%4);

	// Display the image
	cvShowImage("camera", image);

	return true;
}
