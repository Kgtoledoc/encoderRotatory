#define ENC_A 2
#define ENC_B 3
#define RELAY_PIN 12
#define OUTPUT_SIGNAL_PIN 11
#define SIGNAL_BUTTON 10
#define REFRESH_MS 5
#define BAUD_RATE 115200


//volatile unsigned int temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
volatile boolean state_a = 0;
volatile boolean state_b = 0;
volatile int revolution;

volatile int enc_pos = 0;
int enc_pos_prev = 0;
int enc_pos_change = 0;

volatile boolean flag = 0;
volatile boolean flagInitial = 0;

//Timing
unsigned long micros_current = 0;
unsigned long micros_prev = 0;
long micros_change = 0;
unsigned long last_measurement = millis();
unsigned long current_millis= 0;
unsigned long t = 0;
    
int state = 0;
int exitSignal = 0;
int lastStateSignal = 0;

boolean variable = false;
unsigned int currentStatePins = 0;
boolean state_pinRelay = false;
boolean state_pinRelayInit = false;
 
void setup() {
  
  Serial.begin (BAUD_RATE);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(OUTPUT_SIGNAL_PIN, OUTPUT);
  pinMode(SIGNAL_BUTTON, INPUT);
  pinMode(13, OUTPUT);
  pinMode(ENC_A, INPUT_PULLUP); // internal pullup input pin 2   
  pinMode(ENC_B, INPUT_PULLUP); // internalเป็น pullup input pin 3

  state_a = (boolean) digitalRead(ENC_A);
  state_b = (boolean) digitalRead(ENC_B);
  
  attachInterrupt(0, interrupt_enc_a, CHANGE);
  attachInterrupt(1, interrupt_enc_b, CHANGE);

  micros_prev = micros();
  
}
  
  void loop() {

    if(state_pinRelay == true){
      //Serial.print("ADSFDSDFSFDSDF");
      digitalWrite(RELAY_PIN, HIGH);
      //digitalWrite(OUTPUT_SIGNAL_PIN, HIGH);
      //last_measurement = current_millis
      digitalWrite(13, HIGH);
      
    }else {
      //Serial.println("ESTOY AQUI");
      digitalWrite(RELAY_PIN, LOW);
      //digitalWrite(OUTPUT_SIGNAL_PIN, LOW);
      digitalWrite(13, LOW);
    }

    micros_current = micros();
    if (micros_current < micros_prev) {
        micros_change = micros_current + (4294967295 - micros_prev);
    } else {
        micros_change = micros_current - micros_prev;
    }

    // Calculate change in encoder position.
    enc_pos_change = enc_pos - enc_pos_prev;
    enc_pos_change = abs(enc_pos_change);

    // Emit data
    //Serial.println(enc_pos);
    /*Serial.print("\t");    
    Serial.print(enc_pos_change);
    Serial.print("\t");
    Serial.print(micros_current);
    Serial.print("\t");
    Serial.print(micros_change);
    Serial.print("\t");
    Serial.print(enc_pos_change / (micros_change / 1e6));
    Serial.print("\n"); */

    enc_pos_prev = enc_pos;
    micros_prev = micros_current;

    
    
    current_millis = millis();
    //t = abs(current_millis - last_measurement);
    //Waiting time for change of state and begin again
    if((abs(current_millis - last_measurement) >= 5000) && flag == true && (abs(t) < 4294967000)){
      enc_pos = 0;  
      last_measurement = current_millis;
      flag = false;
      flagInitial = false;
      //state_a = (boolean) digitalRead(ENC_A);
      //state_b = (boolean) digitalRead(ENC_B);
    }

    state = digitalRead(SIGNAL_BUTTON);
//    Serial.print("STATE");
  //  Serial.println(state);
    

    if(state == true && lastStateSignal == false){
      exitSignal = 1 - exitSignal;
      //Serial.print("Estoy Aca");
    }

    lastStateSignal = state;
    //Serial.println("ExitSignal");
    //Serial.println(exitSignal);

    if(exitSignal == 1){
      variable = true;
      //Serial.println("LSKDFÑLSKFÑDSÑDFKSJÑDFKSÑKDFÑSDF");
    }
    
    

   

    
    //Serial.print("El estado es ");
    //Serial.println(state_pinRelay);
//    if(state_pinRelayInit == true && variable == true){
//      digitalWrite(RELAY_PIN, HIGH);
//      digitalWrite(13, HIGH);
//    }
    
    
    
    
    
    //if((abs(currentMillis -lastMeasurem  >= 100)){
      //Serial.print("Hola mundo");
      //digitalWrite(
    //}
    

    

    //delay(REFRESH_MS);
  

  }  
  void interrupt_enc_a() {
    //if(variable){
       
    if(!state_a) {
      if(state_b){
          enc_pos++;
          flag = true;
          last_measurement = current_millis;
          
          
      } else {
        enc_pos++;
        flag = true;
        last_measurement = current_millis;
               
      }
      
    }
    //Position initial cut
    if((enc_pos) == 150  && flagInitial == false){
      enc_pos = 0;
      flagInitial = true;
      state_pinRelay = true;
    }
    //Position 20 cm cut
    if((enc_pos == 600) && flagInitial == true){
      enc_pos = 0;
      state_pinRelay = true;
    }

    if((enc_pos) >=50 && state_pinRelay == true){
      state_pinRelay = false;
      
    }    
    
    state_a = !state_a;
                    
}

void interrupt_enc_b(){
  //if(variable){
  state_b = !state_b;
}



























