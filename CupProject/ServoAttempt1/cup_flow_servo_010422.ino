// INCLUDE files
/* 
  Sweep by BARRAGAN <http://barraganstudio.com>
     This example codce is in the public domain.
     modified 8 Nov 2013 by Scott Fitzgerald
     https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep

  Arduino LSM9DS1 - Simple Accelerometer
    This example reads the acceleration values from the LSM9DS1
    sensor and continuously prints them to the Serial Monitor
    or Serial Plotter.
    The circuit:  - Arduino Nano 33 BLE Sense
    created 10 Jul 2019 by Riccardo Rizzo
    This example code is in the public domain.
*/
#include <Servo.h>
#include <Arduino_LSM9DS1.h>
#include <SmartCup.h>
#//include <Complex.h>

Servo valveServo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

int servoOpen = 0;
int servoClose = 30;
int servoPin = 11;

int botsensPin = 9;
int sidsensPin = 10;
int xacc_aPin = 0;
int yacc_aPin = 1;

// Initialize a cup.
// radius=2.85 cm; height=6.9 cm; 
// drink diameter=.9 cm; vent diameter=0.2 cm; 
// radial dist for drink = .9 cm; radial dist for vent =0.0 cm 
cup basecup(2.85,6.9,0.158,0.2,0.9,0.0);

// Initialize the liquid as water.
// 200 ml of water in the basecup
liquid water(90,1,1,basecup);
liquid psure(180,1.06,0.78,basecup);
liquid fiber(180,1.04,0.65,basecup);
liquid cupLiq = water;

// a boolean to track if control is active.
boolean controlled = false;

int vactrack=0; // tracks when vacuum is building up =1, otherwise =0.
float vac_area = 0.0;
float vol_air1 = 0.0, vol_air2 = 0.0;
float p_atm =1013250; // in g/(cm*s^2)
float p_air = p_atm; // Set internal air pressure to atmospheric pressure.
float surf_velocity = 0.0;
float spout_velocity = 0.0;
float Q_desired = 5.0; // Set desired flow rate in ml/sec.
float Q_offset = 1.0; // set control offset to 1 ml/sec.
float gravity = 981; //gravitational constant in cm/s^2.

float trackTime=0, deltaTime=0.9, prevTime, zeroTime; // in sec.
float cupAngle=0;// Initialize the cup angle.
float rotateTime=4; // time in sec. to rotate from 0 to PI/2.
float angle_velocity= PI/4; // Initialized in radians/sec.

float cuphead =0;
float c_velocity = 0.97;
float c_contraction = 0.8;
float Q=0; // calculated Flow rate from aperture in ml/sec.
float p=0; // ????
float prevY=0, prevY2 = 0, prevY3=0;
float viewscale=20.0;

// pulse width modulation terms
float dutycycle = 50.0;
float period = 1.0;
float perstart = trackTime;
float perend = perstart+period;
float perswitch = perend*(dutycycle/100.0);

void setup() {
    cupLiq.volume = realVol();
    cupLiq.volume = min(basecup.volume(),cupLiq.volume);
    cupLiq.volume = max(0,cupLiq.volume);
    cupAngle = realAng(cupAngle);
    zeroTime = float(millis())/1000;
    valveServo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
}

void loop() { 
  // Calculate the height of the liquid on the drinking aperture face of the cup. 
   cupLiq.hl_solve(basecup,cupAngle);
  
  // Determine the head when the height exceeds the drinking aperature.
  cuphead=head(basecup,cupLiq,cupAngle);
        
  if(Q>Q_desired&&controlled) {
         /* Check if liquid level is above drinking aperature and the valve is closed.
         If true, the change in internal air pressure is calculated. */ 
    if(cupLiq.zdim(cupAngle)>basecup.dr_max(cupAngle)) {
      if (vactrack==0) {
          perstart=millis();
          vol_air1=basecup.volume()-cupLiq.volume;
          vactrack=1;
          p_air = p_atm;
          valveServo.write(servoClose); // tell servo to go to Closed position
      } else {
          vol_air2=basecup.volume()-cupLiq.volume;
          p_air = p_atm*vol_air1/vol_air2;
      }     
    } else {
        p_air = p_atm;
        vactrack=0;
        valveServo.write(servoOpen); // tell servo to go to Open position
    }
  } else {
      p_air=p_atm;
      vactrack=0;
      valveServo.write(servoOpen); // tell servo to go to Open position
  }
  
  if(cuphead > 0) {
      // baseflow.iterate;
      vac_area=vacalc(basecup,cupLiq,cupAngle);
      Q = Qcalc(basecup,cupLiq,cuphead);
      //Q = basecup.dr_area()*c_velocity*c_contraction*sqrt(2*gravity*(cuphead+p/cupLiq.density));
      cupLiq.volume=cupLiq.volume-Q*deltaTime;
  } else {
    Q=0;
  }

 //estimate actual volume, calculate cup angle from sensor, and set time tracking. 
    cupLiq.volume = realVol();
    cupLiq.volume = min(basecup.volume(),cupLiq.volume);
    cupLiq.volume = max(0,cupLiq.volume);
    cupAngle = realAng(cupAngle);
    trackTime = float(millis())/1000 - zeroTime;
}
 
// Function to calculate the head above the drinking aperature.
float head(cup hcup, liquid hliq, float a) {
  float h;
  h=hliq.zdim(a)-hcup.dr_min(a);
  h=max(h,0);
  return h;
}

float realAng(float prevAng) {
  float accXin, accYin, accZin;
  float rAngle;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accXin, accYin, accZin);
    rAngle = PI-atan2(accXin,accYin);
  } else {
    rAngle = prevAng;
  }
  return rAngle; 
}

float realVol() {
  float bprs=1, sprs=1;
  float ba,bb,bc;
  float sa,sb,sc;
  float rVolume;
/*
  ba = 0.001764;
  bb = 0.055;
  bc = -0.511;
  sa = 0;
  sb = 1;
  sc = 0;
*/
  ba = -0.00132;
  bb = 1.34;
  bc = -120;
  // ba=0; bb=1; bc =0;
  sa = 0.0004235;
  sb = 0.283;
  sc = -2.386;
  bprs = float(analogRead(botsensPin));
  //bprs = 25.4*exp(0.00382*float(smartCup.analogRead(10)));
  bprs = exp((bprs+787)/250);
  //bprs = max(0,ba*float(analogRead(10))*float(analogRead(10))+bb*float(smartCup.analogRead(10))+bc);
  //sprs = sa*float(smartCup.analogRead(9))*float(smartCup.analogRead(9))
  //       +sb*float(smartCup.analogRead(9))+sc;
  sprs = float(analogRead(sidsensPin));
  //realvol = sqrt((bprs)*(bprs)+(sprs)*(sprs));
  rVolume = bprs;
  return rVolume;   
 }
 
 // Function to calculate the vacuum area within the cup.
float vacalc(cup vacup, liquid valiq, float vangle) {
  float va, chord,segment,beta;
  beta=1;
  if(vangle==0){
    // circular area when not tilted.
    va = PI*vacup.Rc*vacup.Rc;
  } else if(valiq.xl==0&&valiq.xt==0) {
    // elliptical area when in contact with both walls.
    va = PI*vacup.Rc*vacup.Rc/cos(vangle);
  } else if(vangle==PI/2) {
    // rectangular are when horizontal.
    if(valiq.xl>vacup.Rc) {
      chord=2*sqrt((vacup.Rc*vacup.Rc)-((valiq.xl-vacup.Rc)*(valiq.xl-vacup.Rc)));
    } else {
      chord=2*sqrt((vacup.Rc*vacup.Rc)-(valiq.xl*valiq.xl));
    }
    va=vacup.hc*chord;
  } else if(valiq.xt!=0) {
    // elliptical area missing a segment on one end when touching the bottom.
    va = PI*vacup.Rc*vacup.Rc/cos(vangle);
    segment=0.5*vacup.Rc*vacup.Rc/cos(vangle); 
    beta=atan((vacup.Rc-valiq.xt)/cos(vangle)/sqrt(2*vacup.Rc*valiq.xt-(valiq.xt*valiq.xt)));
    va=va-segment*((PI-2*beta)-sin(PI-2*beta));
  } else if(valiq.xl!=0) {
    // elliptical area missing a segment on one end when touching the top.
    va = PI*vacup.Rc*vacup.Rc/cos(vangle);
    segment=0.5*vacup.Rc*vacup.Rc/cos(vangle); 
    beta=atan((vacup.Rc-valiq.xl)/cos(vangle)/sqrt(2*vacup.Rc*valiq.xl-(valiq.xl*valiq.xl)));
    va=va-segment*((PI-2*beta)-sin(PI-2*beta));
  } else {
    // elliptical area missing a segment on both ends when touching the top and bottom.
    // area of ellipse.
    va = PI*vacup.Rc*vacup.Rc/cos(vangle);  
    // subtract area missing from the top.
    segment=0.5*vacup.Rc*vacup.Rc/cos(vangle); 
    beta=atan((vacup.Rc-valiq.xl)/cos(vangle)/sqrt(2*vacup.Rc*valiq.xl-(valiq.xl*valiq.xl)));
    va=va-segment*((PI-2*beta)-sin(PI-2*beta));
    // subtract are missing from the bottom.
    va = PI*vacup.Rc*vacup.Rc/cos(vangle);
    segment=0.5*vacup.Rc*vacup.Rc/cos(vangle); 
    beta=atan((vacup.Rc-valiq.xt)/cos(vangle)/sqrt(2*vacup.Rc*valiq.xt-(valiq.xt*valiq.xt)));
    va=va-segment*((PI-2*beta)-sin(PI-2*beta)); 
  }
  return va;
}

// Calculates the flow rate.
float Qcalc(cup qcup, liquid qliq, float qhead) {
  float Qval,num,den;
  //Q = basecup.dr_area()*c_velocity*c_contraction*sqrt(2*gravity*(cuphead+p/cupLiq.density));
  if(controlled) {
    num=2*gravity*qhead+2*(p_air-p_atm)/qliq.density;
  }
  else {
    num=2*gravity*qhead+2*(p_air-p_atm)/qliq.density;
  }
  num=max(num,0);
  den=1-(qcup.dr_area()/vac_area)*(qcup.dr_area()/vac_area);
  Qval = sqrt(num/den);
  Qval=Qval*qliq.flowFactor*qcup.dr_area()*c_velocity*c_contraction;
  return Qval;
}
