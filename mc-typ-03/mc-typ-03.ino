/*
 mc-typ-03 ver :1.0
*/

#include <MsTimer2.h>

String MC_ID ="0";

#define SENSOR 0
#define SENSOR_1 1
#define SENSOR_2 2
#define SENSOR_3 3

#define RELAY_PIN    5
#define RELAY_PIN_2  6
#define RELAY_PIN_3  7
#define RELAY_PIN_4  8

#define mSTAT_CONECT  1
#define mSTAT_VALVE   2

String mResponse3 ="";
int mSTAT =0;
int mCtOpen   = 0;
int mValveGet = 20;
//Struct
struct stParam{
 String stat;
 String statCode;
 String moi_num;
 String vnum_1;
 String vnum_2;
 String vnum_3;
 String vnum_4;
 String kai_num_1;
 String kai_num_2;
 String kai_num_3;
 String kai_num_4;
};

String get_zeroStr(String sStr, int nWidth){
  String ret="";
  String buf="000000";
  buf=buf + sStr;
  int nLen = buf.length();  
  int nPos= nLen -nWidth;
  ret=buf.substring(nPos);
  return ret;
}
void timer_func() {
Serial.println("#timer_func#" + String(mSTAT));
  if(mSTAT == mSTAT_CONECT){
    mResponse3="";
    proc_start();
  }
}

void proc_start() {
  int iSen =0;
  int iSen2=0;
  int iSen3=0;
  int iSen4=0;  
  iSen=analogRead(SENSOR);
  iSen2 =analogRead(SENSOR_1);
  iSen3 =analogRead(SENSOR_2);
  iSen4 =analogRead(SENSOR_3);
  
  String sSen  = String(iSen);
  String sSen2  = String(iSen2);
  String sSen3  = String(iSen3);
  String sSen4  = String(iSen4);  
  sSen  = get_zeroStr(sSen ,4);
  sSen2 = get_zeroStr(sSen2 ,4);
  sSen3 = get_zeroStr(sSen3 ,4);
  sSen4 = get_zeroStr(sSen4 ,4);
  String sMc_id = get_zeroStr(MC_ID, 4);
  
  String sPut="put_dat=";
  sPut.concat(sMc_id); //mc
  sPut.concat(sSen);
  sPut.concat(sSen2);
  sPut.concat(sSen3);
  sPut.concat(sSen4);  
  sPut.concat("0000");
  Serial.println(sPut);
}

void proc_valve(){
  String sCt = String(mCtOpen);
Serial.println("proc_valve=" + sCt);
  if(mCtOpen >= mValveGet){
    digitalWrite(RELAY_PIN   , LOW);
    digitalWrite(RELAY_PIN_2 , LOW);
    digitalWrite(RELAY_PIN_3 , LOW);
    digitalWrite(RELAY_PIN_4 , LOW);
    mResponse3="";
    mSTAT = mSTAT_CONECT;
    mCtOpen=0;
  }
  mCtOpen ++;
  delay( 1000 );
}
void proc_ser(){
  if(Serial.available() > 0){
       char c = Serial.read();
       mResponse3.concat(c);
  }else{
    if(mResponse3.length() > 31){
  Serial.print("==== response ==== \n");
  String sRes4 = mResponse3.substring( 8 );
  Serial.println("res=" + sRes4);
  Serial.print("==== response.END ==== \n");
      struct stParam param;
      param.stat     = sRes4.substring(0,1);
      param.statCode = sRes4.substring(1,4);
      param.moi_num  = sRes4.substring(4,8);
      param.vnum_1   = sRes4.substring(8, 9);
      param.vnum_2   = sRes4.substring(9, 10);
      param.vnum_3   = sRes4.substring(10,11);
      param.vnum_4   = sRes4.substring(11,12);      
      param.kai_num_1= sRes4.substring(12,15);      
      param.kai_num_2= sRes4.substring(15,18);      
      param.kai_num_3= sRes4.substring(18,21);      
      param.kai_num_4= sRes4.substring(21,24);  
Serial.println("stat=" + param.stat);
      if(param.stat=="1"){
        if((param.vnum_1=="1") || (param.vnum_2=="1") || (param.vnum_3=="1") || (param.vnum_4=="1")){
          mSTAT =mSTAT_VALVE;
          mValveGet =param.kai_num_1.toInt();
          if(param.vnum_1=="1"){
  Serial.print( "vnum_1.STart\n" );
            digitalWrite(RELAY_PIN   , HIGH);
          }
          if(param.vnum_2=="1"){
  Serial.print( "vnum_2.STart\n" );
            digitalWrite(RELAY_PIN_2 , HIGH);
          }
          if(param.vnum_3=="1"){
  Serial.print( "vnum_3.STart\n" );
            digitalWrite(RELAY_PIN_3 , HIGH);
          }
          if(param.vnum_4=="1"){
  Serial.print( "vnum_4.STart\n" );
            digitalWrite(RELAY_PIN_4 , HIGH);
          }
          
        }
      } //stat=1
      mResponse3="";
    } //mResponse3.length
  } //Serial.available
  
}

void setup() {
  mSTAT = mSTAT_CONECT;
  pinMode(SENSOR ,INPUT);
  pinMode(SENSOR_1 ,INPUT);  
  pinMode(SENSOR_2 ,INPUT);
  pinMode(SENSOR_3 ,INPUT); 
  pinMode(RELAY_PIN   ,OUTPUT);
  pinMode(RELAY_PIN_2 ,OUTPUT);
  pinMode(RELAY_PIN_3 ,OUTPUT);
  pinMode(RELAY_PIN_4 ,OUTPUT);  
  Serial.begin(9600);
  
  MsTimer2::set( 10000, timer_func); // xx msごとにオンオフ
  MsTimer2::start();  
}

void loop() {
  if(mSTAT == mSTAT_CONECT){
    proc_ser();
  }
  else if(mSTAT == mSTAT_VALVE){
    proc_valve();
  }  
}

