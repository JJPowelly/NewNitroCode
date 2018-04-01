// runs nitrogenator

#include <MsTimer2.h> 
int tick = 0; //计数值 count value

#include <DHT22.h>
#include <DHT.h>
//Constants
#define DHTPIN A5   // what pin we're connected to
#define blower2 13
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
int n = 0;

int sv[] = {2,3,4,5,6,7,8}; // set pin numbers
int compressor = 9;
int heater = 10;
int blower1= 11;
int fan = 12;


int  i = 0;
int  j = 0; 
const long t0 = 40000; // (end of first t_a)
const long t1 = 41000; // (end of first t_b)
const long t2 = 81000; // (end of second t_a)
const long t3 = 82000; // (end of second t_b)
const long t4 = 984000; // (end of generation 1)
const long t5 = 1968000; // (end of generation 2)
const long t6 = 2968000; // (end of drying 1)
const long t7 = 31488000; // set timer (what is this)

unsigned long t_idle = 3600*3*1000; // time to be off (in ms)

unsigned long pretime = 0;
boolean boolean_t[]={1,1,1,1,1,1,1,1};

int t_times;
int state_time;

void onTimer()
{
 if (hum < 95)
  {
    digitalWrite(blower2,HIGH);
  }
  else
  {
    digitalWrite(blower2,LOW);
  }
  
    //Timer
    
     n = n + 1;
     t_times = n/82;
     state_time = n%82;
     
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor

     Serial.println("--------------------");
     Serial.print("Timer ");  
     Serial.print(tick++); 
     Serial.println(" s"); 

    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    Serial.print("n: ");
    Serial.println(n);
    Serial.print("t_times: ");
    Serial.println(t_times);
    Serial.print("state_time: ");
    Serial.println(state_time);
    
}
  
void setup(){
  Serial.begin(9600);

  MsTimer2::set(1000, onTimer); //设置中断，每1000ms进入一次中断服务程序 onTimer()  set interrupt to every 1000 ms
  MsTimer2::start(); //开始计时  

  Serial.println("DHT12 Humidity & temperature Sensor\n\n");
  pinMode(blower2,OUTPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  
  pinMode(sv[0],OUTPUT);
  pinMode(sv[1],OUTPUT);
  pinMode(sv[2],OUTPUT);
  pinMode(sv[3],OUTPUT);
  pinMode(sv[4],OUTPUT);
  pinMode(sv[5],OUTPUT);
  pinMode(sv[6],OUTPUT);
  pinMode(compressor,OUTPUT);
  pinMode(heater,OUTPUT);
  pinMode(blower1,OUTPUT);
  pinMode(fan,OUTPUT);
  digitalWrite(sv[0],HIGH);//sv1
  digitalWrite(sv[1],LOW); //sv2
  digitalWrite(sv[2],HIGH); //sv3
  digitalWrite(sv[3],LOW); //sv4
  digitalWrite(sv[4],HIGH); //sv5
  digitalWrite(sv[5],LOW); //sv6 
  digitalWrite(sv[6],LOW); //sv7
  digitalWrite(compressor,HIGH);
  digitalWrite(heater,LOW);
  digitalWrite(blower1,LOW);
  digitalWrite(blower2,LOW);
  digitalWrite(fan,HIGH);//set initial IO state
  Serial.println("program starts here...");

} //end "setup()"

void loop(){
    //Start of Program 
 
  unsigned long currentMillis = millis();
  
  while(i<24)
  { //loop 24 times

    currentMillis = millis();
    
    if((currentMillis - pretime >= (j*t_idle + t0 + i * t3)) && (boolean_t[0]==1))//t0=40s
    {  
      digitalWrite(sv[1],HIGH); //sv2 from OFF to ON
      digitalWrite(sv[3],HIGH); //sv4 from OFF to ON
      boolean_t[0]=0;
    }

    if((currentMillis - pretime >= (j*t_idle + t1 + i * t3)) && (boolean_t[1]==1))//t1=41s
    {  
      digitalWrite(sv[0],LOW);   //sv1 from ON to OFF
      digitalWrite(sv[2],LOW);  //sv3 from ON to OFF
      boolean_t[1]=0;
    }

    if((currentMillis - pretime >= (j*t_idle + t2 + i * t3)) && (boolean_t[2]==1))//t2 = 81s
    {  
      digitalWrite(sv[0],HIGH); //sv1 from OFF to ON
      digitalWrite(sv[2],HIGH); //sv3 from OFF to ON
      boolean_t[2]=0;
    }

    if((currentMillis - pretime >= (j*t_idle + t3 + i * t3)) && (boolean_t[3]==1))//t3 = 82s
    {  
      digitalWrite(sv[1],LOW); //sv2 from ON to OFF
      digitalWrite(sv[3],LOW); //sv4 from ON to OFF
      boolean_t[3]=0; 
      i++;
      boolean_t[0]=1;
      boolean_t[1]=1; 
      boolean_t[2]=1; 
      boolean_t[3]=1;
    }  
  }


  if(i>=24)
  {
  digitalWrite(sv[0],LOW);
  digitalWrite(sv[1],LOW);
  digitalWrite(sv[2],LOW);
  digitalWrite(sv[3],LOW);
  }


// switch from Compartment 1 to Compartment 2
currentMillis = millis();
  if((currentMillis - pretime >= j*t_idle + t4) && (boolean_t[4]==1))// t4=984s(1000s)
  { Serial.print("--- MARK: Switching to Compartment 2 ---");
    digitalWrite(sv[4],LOW);  // sv5 from ON to OFF
    digitalWrite(sv[5],HIGH); //sv6 from OFF to ON
    boolean_t[4]=0;
  }

// end of Generation 2

// begin Drying
  if((currentMillis - pretime >= j*t_idle + t5) && (boolean_t[5]==1)) // t5=1968s(2000s)
  { Serial.print("--- MARK: Beginning Drying ---");
    digitalWrite(sv[2],LOW); // sv3 from ON to OFF
    digitalWrite(sv[4],LOW); // sv4 from ON to OFF
    digitalWrite(sv[5],LOW);  //sv6 from ON to OFF
    digitalWrite(sv[6],HIGH);  //sv7 from OFF to ON
    digitalWrite(compressor,LOW); // compressor from ON to OFF
    digitalWrite(heater,HIGH); //  heater from OFF to ON
    digitalWrite(blower1,HIGH); // blower1 from OFF to ON 
    boolean_t[5]=0;
  } // end drying

// begin Cooldown
  if((currentMillis - pretime >= j*t_idle + t6) && (boolean_t[6]==1)) // t6=2968s(3000s)
  { Serial.print("--- MARK: Begin Cooldown ---");
    digitalWrite(sv[6],LOW); // sv7 from ON to OFF
    digitalWrite(heater,LOW); //heater from ON to OFF
    digitalWrite(blower1,LOW); // blower1 from ON to OFF
    boolean_t[6]=0;
  }// end cooldown


// turn off
  if((currentMillis - pretime >= j*t_idle + t7) && (boolean_t[7]==1))  // t4=3148s(3180s)
  { Serial.print("--- MARK: Turning Off ---");
    digitalWrite(fan,LOW);  // fan from ON to OFF
    boolean_t[7]=0;

    // increment j (j is number of generations completed)
    j++;
    // now wait
    Serial.print("MARK: Waiting: ");
    Serial.println(t_idle);
    delay(t_idle);
    Serial.print("j: ");
    Serial.println(j);

    // get ready to go back on
    boolean_t[0] = 0;  
    boolean_t[1] = 0;
    boolean_t[2] = 0;
    boolean_t[3] = 0;
    boolean_t[4] = 0;
    boolean_t[5] = 0;
    boolean_t[6] = 0;
    boolean_t[7] = 0;  
  }



} //end loop()




