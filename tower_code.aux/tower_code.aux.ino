// Rotary Encoder Inputs
int counter[] = {0,0};
int currentStateCLK[] = {0,0};
int lastStateCLK[] = {0,0};
unsigned long lastButtonPress[] = {0,0};
// For the red, blue, and green pins of the RGB light that will sign the color the roterary is going to change
int red_light_pin= 11;
int green_light_pin = 12;
int blue_light_pin = 13;

char zone[1];

int CLK[] = {3, 0}; 
int DT[]  = {4, 1};
int SW[]  = {5, 2};

// LED light strip
int GREEN_LED = 10;
int RED_LED   = 9;
int BLUE_LED  = 6;

int GRGB = 0;
int RRGB = 0;
int BRGB = 0;


// encoder dynamic variables 
float power=0.0;
int color=0;


void setup() {
  
  //Zone color variable to reset encoder when color is switched
  zone[0] = 'r';
  // set the pins for the RGB to output bc it is a common cathode RGB   
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  
  pinMode(CLK[0],INPUT);
  pinMode(DT[0],INPUT);
  pinMode(SW[0], INPUT_PULLUP);

  pinMode(CLK[1],INPUT);
  pinMode(DT[1],INPUT);
  pinMode(SW[1], INPUT_PULLUP);
  
  Serial.begin(9600);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  delay(5000);
}

void loop() {
//Step 1 get input values
  power = 15*encoder(17, 0);
//  Serial.print("Power:");
// Serial.println(power);
  color = encoder(2, 1);
//Step 2  modify lights
  switch (color){
    // RED    
    case 0:
      if (zone[0] !='r'){
         Serial.println("Red!!");
         // Reset counter        
         counter[0] =0;
         currentStateCLK[0]= 0;
         lastStateCLK[0] = 0;
         zone[0] = 'r';
         if(RRGB !=0){
          power = RRGB;
         } else {
          power = 0; 
         }
      }
      RGB_color(255, 0, 0); // Red
      RRGB = power;
      break;
    // GREEN  
    case 1:
     if (zone[0] != 'g'){
         Serial.println("Green!!");
         // Reset counter        
         counter[0] =0;
         currentStateCLK[0]= 0;
         lastStateCLK[0] = 0;
         zone[0] = 'g';
         if(GRGB !=0){
          power = GRGB;
         } else {
          power = 0; 
         }
      }
      RGB_color(0, 255, 0); // Green
      GRGB = power; 
      break;
    // Blue     
    case 2:
    if (zone[0] !='b'){
         // Reset counter
         Serial.println("Blue!!");        
         counter[0] =0;
         currentStateCLK[0]= 0;
         lastStateCLK[0] = 0;
         zone[0] = 'b';
         if(BRGB !=0){
          power = BRGB;
         } else {
          power = 0; 
         }
      }
      RGB_color(0, 0, 255); // Blue
      BRGB = power;
      break;
  }
  analogWrite(GREEN_LED, GRGB);
  analogWrite(RED_LED, RRGB);
  analogWrite(BLUE_LED, BRGB);
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

int encoder(int max_val, int encoderID){
  // Read the current state of CLK
  currentStateCLK[encoderID] = digitalRead(CLK[encoderID]);
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK[encoderID] != lastStateCLK[encoderID]  && currentStateCLK[encoderID] == 1){
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT[encoderID]) != currentStateCLK[encoderID]) {
      counter[encoderID] --;
    } else {
      // Encoder is rotating CW so increment
      counter[encoderID] ++;
    }
    if (counter[encoderID] > max_val){
        counter[encoderID] = 0;
    }
    // no negative numbers 
    if (counter[encoderID] < 0){
        counter[encoderID] = max_val;
    }
    Serial.print("Counter");
    Serial.print(encoderID);
    Serial.print(":\n");
    Serial.println(counter[encoderID]);
  }
  // Remember last CLK state
  lastStateCLK[encoderID] = currentStateCLK[encoderID];
  // Read the button state
  int btnState = digitalRead(SW[encoderID]);
  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress[encoderID] > 50) {
      Serial.println("Button pressed!");
    }
    // Remember last button press event
    lastButtonPress[encoderID] = millis();
  }
  // Put in a slight delay to help debounce the reading
  delay(1);
  return counter[encoderID];
}
