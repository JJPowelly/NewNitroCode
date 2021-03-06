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
int m = 0;

int a = 0;
int b = 0;
int c = 0;
int d = 0;

int sv[] = {2, 3, 4, 5, 6, 7, 8}; // set pin numbers
int compressor = 9;
int heater = 10;
int blower1 = 11;
int fan = 12;


int  i = 0;
int  j = 0;
//const long t0 = 40000; // (end of first t_a)
//const long t1 = 41000; // (end of first t_b)
//const long t2 = 81000; // (end of second t_a)
//const long t3 = 82000; // (end of second t_b)
//const long t4 = 984000; // (end of generation 1)
//const long t5 = 1968000; // (end of generation 2, begin drying)
//const long t6 = 2968000; // (end of drying, begin cooldown)
//const long t7 = 31488000; // (end of cooldown, turn off & wait)

const long t0 = 2000; // (end of first t_a)
const long t1 = 3000; // (end of first t_b)
const long t2 = 5000; // (end of second t_a)
const long t3 = 6000; // (end of second t_b)
const long t4 = 72000; // (end of generation 1)
const long t5 = 144000; // (end of generation 2, begin drying)
const long t6 = 149000; // (end of drying, begin cooldown)
const long t7 = 159000; // (end of cooldown, turn off & wait)

const long t_idle = 10000; // time to be off (in ms)
long t_offset = 0;

unsigned long pretime = 0;
boolean boolean_t[] = {1, 1, 1, 1, 1, 1, 1, 1};

int t_times;
int state_time;

void onTimer()
{
  if (hum < 95)
  {
    digitalWrite(blower2, HIGH);
  }
  else
  {
    digitalWrite(blower2, LOW);
  }

  //Timer

  n = n + 1;
  t_times = n / 82;
  state_time = n % 82;


  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  //Print temp and humidity values to serial monitor

  Serial.print(" ");
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

  Serial.print("a: ");
  Serial.println(a);
  Serial.print("b: ");
  Serial.println(b);
  Serial.print("c: ");
  Serial.println(c);
  Serial.print("d: ");
  Serial.println(d);

  Serial.print("Boolean_t: ");
  Serial.print(boolean_t[0]);
  Serial.print(" ");
  Serial.print(boolean_t[1]);
  Serial.print(" ");
  Serial.print(boolean_t[3]);
  Serial.print(" ");
  Serial.print(boolean_t[4]);
  Serial.print(" ");
  Serial.print(boolean_t[5]);
  Serial.print(" ");
  Serial.print(boolean_t[6]);
  Serial.print(" ");
  Serial.print(boolean_t[7]);
  Serial.print(" ");




}

void setup() {
  Serial.begin(9600);

  MsTimer2::set(1000, onTimer); //设置中断，每1000ms进入一次中断服务程序 onTimer()  set interrupt to every 1000 ms
  MsTimer2::start(); //开始计时

  Serial.println("DHT12 Humidity & temperature Sensor\n\n");
  pinMode(blower2, OUTPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();

  pinMode(sv[0], OUTPUT);
  pinMode(sv[1], OUTPUT);
  pinMode(sv[2], OUTPUT);
  pinMode(sv[3], OUTPUT);
  pinMode(sv[4], OUTPUT);
  pinMode(sv[5], OUTPUT);
  pinMode(sv[6], OUTPUT);
  pinMode(compressor, OUTPUT);
  pinMode(heater, OUTPUT);
  pinMode(blower1, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(sv[0], HIGH); //sv1
  digitalWrite(sv[1], LOW); //sv2
  digitalWrite(sv[2], HIGH); //sv3
  digitalWrite(sv[3], LOW); //sv4
  digitalWrite(sv[4], HIGH); //sv5
  digitalWrite(sv[5], LOW); //sv6
  digitalWrite(sv[6], LOW); //sv7
  digitalWrite(compressor, HIGH);
  digitalWrite(heater, LOW);
  digitalWrite(blower1, LOW);
  digitalWrite(blower2, LOW);
  digitalWrite(fan, HIGH); //set initial IO state
  Serial.println("program starts here...");

} //end "setup()"

void loop() {
  //Start of Program

  unsigned long currentMillis = millis();

  while (i < 24)
  { //loop 24 times

    currentMillis = millis();

    if ((currentMillis - pretime) >= (t_offset + t0 + i * t3) && (boolean_t[0] == 1)) //t0=40s
    {
      digitalWrite(sv[1], HIGH); //sv2 from OFF to ON
      digitalWrite(sv[3], HIGH); //sv4 from OFF to ON
      boolean_t[0] = 0;
      a++;
      Serial.print(" ");
      Serial.print("--- MARK: Changing a : ");
      Serial.println(a);

    }

    if ((currentMillis - pretime) >= (t_offset + t1 + i * t3) && (boolean_t[1] == 1)) //t1=41s
    {
      digitalWrite(sv[0], LOW);  //sv1 from ON to OFF
      digitalWrite(sv[2], LOW); //sv3 from ON to OFF
      boolean_t[1] = 0;
      b++;
      Serial.print(" ");
      Serial.print("--- MARK: Changing b :");
      Serial.println(b);
    }

    if ((currentMillis - pretime) >= (t_offset + t2 + i * t3) && (boolean_t[2] == 1)) //t2 = 81s
    {
      digitalWrite(sv[0], HIGH); //sv1 from OFF to ON
      digitalWrite(sv[2], HIGH); //sv3 from OFF to ON
      boolean_t[2] = 0;
      c++;
      Serial.print(" ");
      Serial.print("--- MARK: Changing c :");
      Serial.println(c);
    }

    if ((currentMillis - pretime) >= (t_offset + t3 + i * t3) && (boolean_t[3] == 1)) //t3 = 82s
    {
      digitalWrite(sv[1], LOW); //sv2 from ON to OFF
      digitalWrite(sv[3], LOW); //sv4 from ON to OFF
      boolean_t[3] = 0;
      i++;
      boolean_t[0] = 1;
      boolean_t[1] = 1;
      boolean_t[2] = 1;
      boolean_t[3] = 1;
      d++;
      Serial.print(" ");
      Serial.print("--- MARK: Changing d :");
      Serial.println(d);
      Serial.print("Boolean_t: ");
      Serial.print(boolean_t[0]);
      Serial.print(" ");
      Serial.print(boolean_t[1]);
      Serial.print(" ");
      Serial.print(boolean_t[3]);
      Serial.print(" ");
      Serial.print(boolean_t[4]);
      Serial.print(" ");
      Serial.print(boolean_t[5]);
      Serial.print(" ");
      Serial.print(boolean_t[6]);
      Serial.print(" ");
      Serial.print(boolean_t[7]);
      Serial.print(" ");
    }

    currentMillis = millis();
    
    if ((currentMillis - pretime) >= (t_offset + t4) && (boolean_t[4] == 1)) // t4=984s(1000s)
    { Serial.print (" ");
      Serial.print(" ");
      Serial.print("--- MARK: Switching to Compartment 2 ---");
      digitalWrite(sv[4], LOW); // sv5 from ON to OFF
      digitalWrite(sv[5], HIGH); //sv6 from OFF to ON
      boolean_t[4] = 0;
    }
  }


  if (i >= 24 && m == 0)
  {
    Serial.print("i: ");
    Serial.println(i);
    Serial.println("--- MARK: 24 cycles complete ---");
    digitalWrite(sv[0], LOW);
    digitalWrite(sv[1], LOW);
    digitalWrite(sv[2], LOW);
    digitalWrite(sv[3], LOW);
    m++;
  }




  // end of Generation 2

  // begin Drying

  currentMillis = millis();
  if ((currentMillis - pretime) >= (t_offset + t5) && (boolean_t[5] == 1)) // t5=1968s(2000s)
  { Serial.print("--- MARK: Beginning Drying ---");
    Serial.println(currentMillis, 4);

    digitalWrite(sv[0], LOW); // sv1 from ON to OFF
    digitalWrite(sv[1], LOW); // sv2 from ON to OFF
    digitalWrite(sv[2], LOW); // sv3 from ON to OFF
    digitalWrite(sv[3], LOW); // sv4 from ON to OFF
    digitalWrite(sv[4], LOW); // sv5 from ON to OFF
    digitalWrite(sv[5], LOW); //sv6 from ON to OFF
    digitalWrite(sv[6], HIGH); //sv7 from OFF to ON
    digitalWrite(compressor, LOW); // compressor from ON to OFF
    digitalWrite(heater, HIGH); //  heater from OFF to ON
    digitalWrite(blower1, HIGH); // blower1 from OFF to ON
    boolean_t[5] = 0;
  } // end drying

  currentMillis = millis();
  // begin Cooldown
  if ((currentMillis - pretime) >= (t_offset + t6) && (boolean_t[6] == 1)) // t6=2968s(3000s)
  { Serial.print("--- MARK: Begin Cooldown ---");
    digitalWrite(sv[0], LOW); // sv1 from ON to OFF
    digitalWrite(sv[1], LOW); // sv2 from ON to OFF
    digitalWrite(sv[2], LOW); // sv3 from ON to OFF
    digitalWrite(sv[3], LOW); // sv4 from ON to OFF
    digitalWrite(sv[4], LOW); // sv5 from ON to OFF
    digitalWrite(sv[5], LOW); //sv6 from ON to OFF
    digitalWrite(sv[6], LOW); //sv7 from OFF to ON
    digitalWrite(compressor, LOW); // compressor from ON to OFF
    digitalWrite(heater, LOW); //  heater from OFF to ON
    digitalWrite(blower1, LOW); // blower1 from OFF to ON
    digitalWrite(fan, HIGH); // blower1 from OFF to ON
    boolean_t[6] = 0;
  }// end cooldown

  currentMillis = millis();
  // turn off
  if ((currentMillis - pretime) >= (t_offset + t_idle + t7) && (boolean_t[7] == 1)) // t4=3148s(3180s)
  { Serial.println("--- MARK: Turning Off ---");
    digitalWrite(fan, LOW); // fan from ON to OFF
    boolean_t[7] = 0;

    // increment j (j is number of generations completed)
    j++;
    // now wait
    Serial.print("MARK: Waiting: ");
    Serial.println(t_idle);
    delay(t_idle);
    Serial.print("j: ");
    Serial.println(j);

    // get ready to go back on
    boolean_t[0] = 1;
    boolean_t[1] = 1;
    boolean_t[2] = 1;
    boolean_t[3] = 1;
    boolean_t[4] = 1;
    boolean_t[5] = 1;
    boolean_t[6] = 1;
    boolean_t[7] = 1;

    i = 0;
    a = 0;
    b = 0;
    c = 0;
    d = 0;

    t_offset = millis();

    // SET INITIAL IO STATE AGAIN
   digitalWrite(sv[0], HIGH); //sv1
   delay(10);
   digitalWrite(sv[1], LOW); //sv2
   delay(10);
   digitalWrite(sv[2], HIGH); //sv3
   delay(10);
   digitalWrite(sv[3], LOW); //sv4
   delay(10);
   digitalWrite(sv[4], HIGH); //sv5
   delay(10);
   digitalWrite(sv[5], LOW); //sv6
   delay(10);
   digitalWrite(sv[6], LOW); //sv7
   delay(10);
   digitalWrite(compressor, HIGH); //
   delay(10);
   digitalWrite(heater, LOW); // 
   delay(10);
   digitalWrite(blower1, LOW); //
   delay(10);
   digitalWrite(blower2, LOW);
   delay(10);
   digitalWrite(fan, HIGH); //set initial IO state

    
  }



} //end loop()




