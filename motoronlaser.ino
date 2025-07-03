#include <Motoron.h>
MotoronI2C mc(16);
const int button = 12;
bool recalculatepercent = true;
const int pot1 = A0;
const int pot2 = A1;
const int pot3 = A3;
const int pot1min = 48;
const int pot2min = 48;
const int pot3min = 50;
const int pot1max = 975;
const int pot2max = 995;
const int pot3max = 915;
int pot1val = 0;
int pot2val = 0;
int pot3val = 0;
double scaledpot1val = 0;
double scaledpot2val = 0;
double scaledpot3val = 0;
const double expon = 1.5;
int mot1actspeed = 0;
int mot2actspeed = 0;
int mot3actspeed = 0;
const int maxspeed = 150;
const int minspeed = 30;
int mot1percentspeed = 0;
int mot2percentspeed = 0;
int mot3percentspeed = 0;
int mot1percentspeedunconstrained = 0;
int mot2percentspeedunconstrained = 0;
int mot3percentspeedunconstrained = 0;
bool potmode = true;
bool serialmode = false;
bool seriallist = false;
String receivedstring = "";



void setup(){
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
  Wire.begin();
  mc.reinitialize();
  mc.disableCrc();
  mc.clearResetFlag();
  mc.disableCommandTimeout();
  mc.setMaxAcceleration(1, 80);
  mc.setMaxDeceleration(1, 80);
  mc.setMaxAcceleration(2, 80);
  mc.setMaxDeceleration(2, 80);
  mc.setMaxAcceleration(3, 80);
  mc.setMaxDeceleration(3, 80);
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n----------------Henry's Arduino Motoron Geometric Laser Projector!!!---------------\n");
  printpotinstfunc();
}


void loop(){
  if (potmode == true){
    potmodefunc();
  } else if (serialmode == true){
    serialmodefunc();
  }
}


void potmodefunc(){
  potvaltomotactspeedfunc();
  recalculatepercent = true;
  setmotorspeedsfunc();
  potselectmodebuttonfunc();
  potmodeserialfunc();
}


void serialmodefunc(){
  serialselectmodebuttonfunc();
  if (Serial.available()){
    receivedstring = Serial.readStringUntil('\n');
    receivedstring.trim();
    receivedstring.toLowerCase();
    quitfunc();
    examplefunc();
    serialselectmodeserialfunc();
    potandserialseriallistfunc();
    serialdispinstserialfunc();
    mxsformatfunc();
    mxformatfunc();
    sssformatfunc();
  }
  unconstrainedpercenttoactspeedfunc();
  recalculatepercent = false;
  setmotorspeedsfunc();
}


void quitfunc() {
  if (receivedstring.equals("q")){
    mot1percentspeedunconstrained = 0;
    mot2percentspeedunconstrained = 0;
    mot3percentspeedunconstrained = 0;
  }
}


void examplefunc() {
  if (receivedstring.equals("example")){
    mot1percentspeedunconstrained = 50;
    mot2percentspeedunconstrained = 51;
    mot3percentspeedunconstrained = 49;
  }
}


void printserialinstfunc(){
  Serial.println("\n----------------serial monitor control mode----------------");
  Serial.println("\nSend 'm-s', where 'm' is the motor number (1, 2, or 3) and 's' is the speed (between 0 and 100), to make the motor go that speed.");
  Serial.println("Send 'm-' or 'm+', where 'm' is the motor number, to make the motor's speed increase (+) or decrease (-) by 1.");
  Serial.println("In either command, type 'm' in place of the motor number to apply the command to all motors.");
  Serial.println("Send commands in the format 's\u2081, s\u2082, s\u2083' to apply the speed 's\u2081' to motor 1, 's\u2082' to motor 2, and 's\u2083' to motor 3.");
  Serial.println("Send 'q' to stop the motors.");
  Serial.println("Send 'example' to see an example of a pattern that the laser can create.");
  Serial.println("Press the black button or send 'p' to the serial monitor to switch to potentiometer control mode.");
  Serial.println("Send 'start' or 'stop' to start or stop receiving motor speed data.");
  Serial.println("Send 'disp' to display these instructions again.");
}


void sssformatfunc(){
  int comma1pos = receivedstring.indexOf(", ");
  int comma2pos = receivedstring.indexOf(", ", comma1pos + 2);
  if (comma1pos != -1 && comma2pos != -1) {
    String speed1 = receivedstring.substring(0, comma1pos);
    String speed2 = receivedstring.substring(comma1pos + 2, comma2pos);
    String speed3 = receivedstring.substring(comma2pos + 2);
    mot1percentspeedunconstrained = speed1.toInt();
    mot2percentspeedunconstrained = speed2.toInt();
    mot3percentspeedunconstrained = speed3.toInt();
  } else {
    int comma1pos = receivedstring.indexOf(",");
    int comma2pos = receivedstring.indexOf(",", comma1pos + 1);
    if (comma1pos != -1 && comma2pos != -1) {
      String speed1 = receivedstring.substring(0, comma1pos);
      String speed2 = receivedstring.substring(comma1pos + 1, comma2pos);
      String speed3 = receivedstring.substring(comma2pos + 1);
      mot1percentspeedunconstrained = speed1.toInt();
      mot2percentspeedunconstrained = speed2.toInt();
      mot3percentspeedunconstrained = speed3.toInt();
    }
  }
}


void mxsformatfunc(){
  if ((receivedstring.length() > 2) && (receivedstring.substring(1, 2) == "-")){
    String motornumber = receivedstring.substring(0, 1);
    String sspeed = receivedstring.substring(2);
    int ispeed = sspeed.toInt();
    if (motornumber.equals("1")){
      mot1percentspeedunconstrained = ispeed;
    } else if (motornumber.equals("2")){
      mot2percentspeedunconstrained = ispeed;
    } else if (motornumber.equals("3")){
      mot3percentspeedunconstrained = ispeed;
    } else if (motornumber.equals("m")){
      mot1percentspeedunconstrained = ispeed;
      mot2percentspeedunconstrained = ispeed;
      mot3percentspeedunconstrained = ispeed;
    }
  }
}


void mxformatfunc(){
  if (receivedstring.equals("1+")){
    mot1percentspeedunconstrained++;
  } else if (receivedstring.equals("2+")){
    mot2percentspeedunconstrained++;
  } else if (receivedstring.equals("3+")){
    mot3percentspeedunconstrained++;
  } else if (receivedstring.equals("m+")){
    mot1percentspeedunconstrained++;
    mot2percentspeedunconstrained++;
    mot3percentspeedunconstrained++;
  } else if (receivedstring.equals("1-")){
    mot1percentspeedunconstrained--;
  } else if (receivedstring.equals("2-")){
    mot2percentspeedunconstrained--;
  } else if (receivedstring.equals("3-")){
    mot3percentspeedunconstrained--;
  } else if (receivedstring.equals("m-")){
    mot1percentspeedunconstrained--;
    mot2percentspeedunconstrained--;
    mot3percentspeedunconstrained--;
  }
}


void potvaltomotactspeedfunc(){
  pot1val = analogRead(pot1);
  scaledpot1val = pow(pot1val, expon);
  mot1actspeed = map(scaledpot1val, pow(pot1max, expon), pow(pot1min, expon), minspeed, maxspeed);
  pot2val = analogRead(pot2);
  scaledpot2val = pow(pot2val, expon);
  mot2actspeed = map(scaledpot2val, pow(pot2max, expon), pow(pot2min, expon), minspeed, maxspeed);
  pot3val = analogRead(pot3);
  scaledpot3val = pow(pot3val, expon);
  mot3actspeed = map(scaledpot3val, pow(pot3max, expon), pow(pot3min, expon), minspeed, maxspeed);
}


void potselectmodebuttonfunc(){
  if (digitalRead(button) == LOW){
    serialmode = true;
    potmode = false;
    seriallist = false;
    printserialinstfunc();
  }
}


void serialselectmodebuttonfunc(){
  if (digitalRead(button) == LOW){
    serialmode = false;
    potmode = true;
    seriallist = false;
    printpotinstfunc();
  }
}


void printpotinstfunc(){
  Serial.println("\n----------------potentiometer control mode----------------");
  Serial.println("\nAdjust the yellow potentiometers to control the motors.");
  Serial.println("Press the black button or send 's' to the serial monitor to switch to serial monitor control mode.");
  Serial.println("Send 'start' or 'stop' to start or stop receiving motor speed data.");
  Serial.println("Send 'disp' to display these instructions again.");
}


void unconstrainedpercenttoactspeedfunc(){
  mot1percentspeed = constrain(mot1percentspeedunconstrained, 0, 100);
  mot2percentspeed = constrain(mot2percentspeedunconstrained, 0, 100);
  mot3percentspeed = constrain(mot3percentspeedunconstrained, 0, 100);
  mot1actspeed = map(mot1percentspeed, 0, 100, 30, 148);
  mot2actspeed = map(mot2percentspeed, 0, 100, 30, 148);
  mot3actspeed = map(mot3percentspeed, 0, 100, 30, 148);
}


void acttopercentspeedfunc(){
  mot1percentspeedunconstrained = map(mot1actspeed, 30, 148, 0, 100);
  mot2percentspeedunconstrained = map(mot2actspeed, 30, 148, 0, 100);
  mot3percentspeedunconstrained = map(mot3actspeed, 30, 148, 0, 100);
  mot1percentspeed = constrain(mot1percentspeedunconstrained, 0, 100);
  mot2percentspeed = constrain(mot2percentspeedunconstrained, 0, 100);
  mot3percentspeed = constrain(mot3percentspeedunconstrained, 0, 100);
}


void setmotorspeedsfunc(){
  mc.setSpeed(1, mot1actspeed);
  mc.setSpeed(2, mot2actspeed);
  mc.setSpeed(3, mot3actspeed);
  if (seriallist == true){
    if (recalculatepercent == true){
      acttopercentspeedfunc();
    }
    Serial.print(mot1percentspeed);
    Serial.print(", ");
    Serial.print(mot2percentspeed);
    Serial.print(", ");
    Serial.println(mot3percentspeed);
  }
}


void potmodeserialfunc(){
  if (Serial.available()){
    receivedstring = Serial.readStringUntil('\n');
    receivedstring.trim();
    receivedstring.toLowerCase();
    potandserialseriallistfunc();
    potselectmodeserialfunc();
    potdispinstserialfunc();
  }
}


void potandserialseriallistfunc(){
  if (receivedstring.equals("start")){
    seriallist = true;
  } else if (receivedstring.equals("stop")){
    seriallist = false;
  }
}


void potselectmodeserialfunc(){
  if (receivedstring.equals("s")){
    serialmode = true;
    potmode = false;
    seriallist = false;
    printserialinstfunc();
  }
}


void serialselectmodeserialfunc(){
  if (receivedstring.equals("p")){
    serialmode = false;
    potmode = true;
    seriallist = false;
    printpotinstfunc();
  }
}


void potdispinstserialfunc(){
  if (receivedstring.equals("disp")){
    printpotinstfunc();
  }
}


void serialdispinstserialfunc(){
  if (receivedstring.equals("disp")){
    printserialinstfunc();
  }
}
