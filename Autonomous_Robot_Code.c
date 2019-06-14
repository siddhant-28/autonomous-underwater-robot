#pragma config(Sensor, in4,    InfraCollector, sensorReflection)
#pragma config(Sensor, dgtl7,  RedLED1,         sensorDigitalOut)
#pragma config(Sensor, dgtl9,  RedLED2,         sensorDigitalOut)
#pragma config(Sensor, dgtl12,  RedLED3,         sensorDigitalOut)
#pragma config(Sensor, dgtl3,  button1,        sensorTouch)
#pragma config(Motor,  port1,           motor1,        tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,           motor2,        tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port3,           motor3,        tmotorVex393_HBridge, openLoop)
#pragma config(Sensor, dgtl1,  ultrasonic,     sensorSONAR_inch)

//Increment this value after each task
#define TASK_NUMBER 1

bool button1_pushed;
void detectAgain();
void connect();
void finish();

//Threshold of values read from the phototransistor.
const   int IR_SENSOR_THRESHOLD = 500;
const   int IR_SENSOR_THRESHOLD_2 = 1500;

//Control LED values
const   int OFF = 0;
const   int ON  = 1;

void init_light_status()
{
  //Turn off all the LED's
  SensorValue(RedLED1)= OFF;
  SensorValue(RedLED2)= OFF;
  SensorValue(RedLED3)= OFF;
}

/*This function is used to flag button
 *inputs
 */
void monitorInput()
{
  if(SensorValue(button1) && !button1_pushed)
  {
    button1_pushed = true;
  }
}

/*This function is used for detecting and moving towards
 *the object. When the robot is in the vicinity of
 *the target object, it turns to face the target object and then
 *moves towards it.
 *There are 3 LED's installed on the robot. These LED's turn
 *on depending upon the distance and orientation of the robot.
 *LED's only function when the IR sensor detects the beacon
 *LED 1 - turns on when robot is too far away for connection
 *LED 2 - turns on when robot is at the perfect distance for connection
 *LED 3 - turns on when the robot is too close for connection
*/
void detectObject()
{
	enum T_system_state
	{
  	ALL_OFF = 0,  //base state - everything off
  	DETECT_OBJECT, //Turn and move towards the object
  	LED1,
  	LED2,
  	LED3
	};
  //Declare and intialize system state
  T_system_state system_state = ALL_OFF;

  //Initialize lighting status.
  init_light_status();


  while (true)
  {
  	monitorInput();

    switch(system_state)
    {
   		case(ALL_OFF):

        // All motors and LED's should be off when in this state
        SensorValue(RedLED1) = OFF;
        SensorValue(RedLED2) = OFF;
        SensorValue(RedLED3) = OFF;
        motor[motor1] = 0;
        motor[motor2] = 0;
        monitorInput();

        //Turn on the LED's depending upon the condition
        if(SensorValue(ultrasonic)>10&&SensorValue(InfraCollector)>IR_SENSOR_THRESHOLD)
      	{
         system_state = LED1; //Robot detects beacon but is too far away for connection - turn on LED 1
      	}
      	if(SensorValue(ultrasonic)<10&&SensorValue(ultrasonic)>5&&SensorValue(InfraCollector)>IR_SENSOR_THRESHOLD)
      	{
         system_state = LED2; //Robot detects beacon and is at perfect distance for connection - turn on LED 2
      	}
      	if(SensorValue(ultrasonic)<5&&SensorValue(InfraCollector)>IR_SENSOR_THRESHOLD)
      	{
         system_state = LED3; //Robot detects beacon but is too close for connection - turn on LED 3
      	}

        if(button1_pushed)
        {
        	//If button 1 is pushed, go to the DETECT_OBJECT state
        	system_state = DETECT_OBJECT;

        	//Clear button 1 flag
        	button1_pushed = false;

        }
				break;

			/*When the phototransistor detects the IR LED from the beacon,
			 *the InfraCollector port goes above the
       *IR_SENSOR_THRESHOLD value.
      */
      case(DETECT_OBJECT):

     /*Rotate the robot WHILE the InfraCollector port value is LESS
      *than the IR_SENSOR_THRESHOLD value
     */
     while(SensorValue[InfraCollector] < IR_SENSOR_THRESHOLD)
      {
      		motor[motor1] = 50;
        	motor[motor2] = 50;
      }

      //After value of the InfraCollector port is greater than IR_SENSOR_THRESHOLD, stop the motors
      motor[motor1] = 0;
     	motor[motor2] = 0;
     	wait10Msec(60);

     	/*If the InfraCollector port value of the IR sensor is greater than
     	 *IR_SENSOR_THRESHOLD and WHILE the value of the
     	 *ultrasonic sensor is greater than 20 (too far away
     	 *for connection), travel towards the beacon
     	*/
      if(SensorValue[InfraCollector] > IR_SENSOR_THRESHOLD)
      {
      	while(SensorValue(ultrasonic)>20)
      	{
      		motor[motor1] = 50;
      		motor[motor2] = -50;
      	}

      }

      /*After the value of the ultrasonic sensor is less than 20
       *go to the detectAgain() function
      */
      detectAgain();

      /*After the robot is in perfect orientation with respect to
       *the beacon go to the connect() function
      */
      connect();

      /*After the robot successfully connects to the target, go to the
       *finish() function
      */
      finish();

      //When the connection process is finished, switch to the ALL_OFF state
      system_state = ALL_OFF;
    	break;

    	//Turns on LED 1 when robot detects beacon but is too far away for connection
    	case(LED1):
    		SensorValue(RedLED1) = ON;
    			wait10Msec(60);
    			system_state = ALL_OFF;
      		break;

      //Turns on LED 2 when robot detects beacon and is at perfect distance for connection
      case(LED2):
      	SensorValue(RedLED2) = ON;
      	wait10Msec(60);
    			system_state = ALL_OFF;
    			break;

    	//Turn on LED 3 when robot detects beacon but is too close for connection
    	case(LED3):
    		SensorValue(RedLED3) = ON;
      	wait10Msec(60);
    			system_state = ALL_OFF;
    			break;

    	default:

    } // end switch

  } // end while

}

/*This function is triggered when the value of ultrasonic
 *sensor is less than 20. It instructs the robot to stop
 *and then look for the target object again, with a higher
 *threshold (IR_SENSOR_THRESHOLD_2) to increase precision.
 *After detecting the target object, the robot moves towards it.
 */
void detectAgain()
{
	//stop the motors
	motor[motor1] = 0;
	motor[motor2] = 0;

	wait10Msec(65);

	//offset the robot from the target object
	motor[motor1] = 50;
	motor[motor2] = 50;

	wait10Msec(45);

	//stop the motors
	motor[motor1] = 0;
	motor[motor2] = 0;

	wait10Msec(45);

	/*Rotate the robot WHILE the InfraCollector port value is LESS
   *than the IR_SENSOR_THRESHOLD_2 value
  */
	while(SensorValue[InfraCollector] < IR_SENSOR_THRESHOLD_2)
      {
      		motor[motor1] = 50;
        	motor[motor2] = 50;
      }

  /*If the InfraCollector port value of the IR sensor is greater than
   *IR_SENSOR_THRESHOLD_2 and WHILE the value of the
   *ultrasonic sensor is greater than 10, travel towards the beacon
  */
  if(SensorValue[InfraCollector] > IR_SENSOR_THRESHOLD_2)
      {
      	while(SensorValue(ultrasonic)>10)
      	{
      		motor[motor1] = 50;
      		motor[motor2] = -50;
      	}
      }
}

/*This function is triggered once the value of the ultrasonic
 *sensor is less than 10 and the robot is at perfect distance
 *for connection. This function triggers the connection process
 *completing the connection.
*/
void connect()
 {
   //stop the motors
   motor[motor1] = 0;
   motor[motor2] = 0;

   wait10Msec(50);

   //connect to the target object
   motor[motor3] = -30;
	 wait10Msec(145);
	 motor[motor3] = 0;
}


/*This function is triggered after the connection process is
 *completed. This function signals task completion by retracting
 *the arm, and moving back by some distance
*/
 void finish()
 {
   wait10Msec(50);

   //retract the connection arm
   motor[motor3] = 30;
   wait10Msec(135);
   motor[motor3] = 0;

   wait10Msec(50);

   //move back
   motor[motor1] = -40;
   motor[motor2] = 40;

   wait10Msec(40);

   button1_pushed = false;
   monitorInput();
  }


//Main Function - Used to switch between tasks(if more than one)
task main()
{
	//Turn everything off
	button1_pushed=false;
	SensorValue(RedLED1) = OFF;
  SensorValue(RedLED2) = OFF;
  SensorValue(RedLED3) = OFF;
 	motor[motor1] = 0;
  motor[motor2] = 0;

  //Switch TASK_NUMVER
  switch(TASK_NUMBER)
  {
  	case 1:
  		detectObject();
  		break;
  	default:
  }//end switch
}
