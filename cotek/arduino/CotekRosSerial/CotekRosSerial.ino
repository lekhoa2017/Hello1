#include "Wire.h"
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#define address 0x50 



ros::NodeHandle  nh;

std_msgs::Float32MultiArray power_msg;
ros::Publisher power_pub("power", &power_msg);
float power[]={0,0};

void setup()
{
  nh.initNode(); 
  nh.advertise(power_pub);

  Wire.begin(); // wake up I2C bus
}

void getdata(byte *a, byte *b, byte *c, byte *d)
{
  byte error;
  // move the register pointer back to the first register
  Wire.beginTransmission(address); 
  Wire.write(0x60); 
  error = Wire.endTransmission(); 
  
  if (error == 0){
    Wire.requestFrom(address, 4); 
    *a = Wire.read(); // first received byte stored here
    *b = Wire.read(); // second received byte stored here
    *c = Wire.read(); // third received byte stored here
    *d = Wire.read(); // forth received byte stored here
  }else if (error==4){
    *a = 0;
    *b = 0;
    *c = 0;
    *d = 0;
  }
}



void loop()
{
  power_msg.data_length=2;
  byte aa,bb,cc,dd;
  getdata(&aa,&bb,&cc,&dd);
  float voltage = word(bb, aa)/100.0;
  float current = word(dd, cc)/100.0;
  power[0] = voltage;
  power[1] = current;
  power_msg.data = power;
  power_pub.publish( &power_msg );
  nh.spinOnce();
  delay(100);
}
