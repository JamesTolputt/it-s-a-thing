// Headers for Screen
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

// Headers for Motors
#include <ENGG1500Lib.h>
#include <Servo.h>

// Pins
#define ENA 5 // LEFT
#define ENB 6 // RIGHT
#define IN1 8
#define IN2 1
#define IN3 10
#define IN4 11
#define ECHO 12 // Pin that senses ultrasonic pulse
#define TRIG 7 // Pin that sends ultrasonic pulse
#define SONICSTEPPER 9 // Ultrasonic sensor's stepper motor

//#define LEFT 1
//#define RIGHT 0

// START SCREEN STUFF
  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 64
  #define STD_TEXT_SIZE 1
  #define LRG_TEXT_SIZE 2
  
  #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  enum quarter {
    topleft, topright, bottomleft, bottomright
  };
// END SCREEN STUFF

// Enum for direction of motors
enum direction{
  forward, backward, noward
};

const float speed_sound = 340.29; // m/s
Servo sonic_servo; // Initialise servo motor driver object

// Function prototypes
void left_motor(direction dir=forward);
void right_motor(direction dir=forward);
unsigned int sonar_distance(byte checks=10); // Assumes you don't want to average >255 values for REAL accurate reading
void clear_corner(quarter qrt);
void cursor_to_corner(quarter qrt);
void init_display(void);

//START
void setup() {
  // put your setup code here, to run once:
  pinMode(ENA,OUTPUT); //set ENA as an output
  pinMode(ENB,OUTPUT); //set ENB as an output
  pinMode(IN1,OUTPUT); //set IN1 as an output
  pinMode(IN2,OUTPUT); //set IN2 as an output
  pinMode(IN3,OUTPUT); //set IN3 as an output
  pinMode(IN4,OUTPUT); //set IN4 as an output
  pinMode(A0, INPUT); //right IR sensor
  pinMode(A1, INPUT); //left IR sensor
  pinMode(ECHO,INPUT); //Initialise ECHO as an input
  pinMode(TRIG,OUTPUT); //Initialise TRIG as an output
  
  Serial.begin(9600); // Setup console to prtin. Set the baud rate on the serial connection
  enc_init(); // Initialise motor encoder library
  sonic_servo.attach(SONICSTEPPER); // Attach pin SONICSWEEPER to sonic_servo object

  // Setup screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // If Screen can not be initialised...
    Serial.println(F("SSD1306 allocation failed")); // Print that it did not work
    for(;;); // Loop forever
  } else { // Else show a smiley face and set up for text
    display.clearDisplay();
    display.fillCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_HEIGHT/2, WHITE);
    display.fillCircle(SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-(SCREEN_HEIGHT/12), SCREEN_HEIGHT/2+(SCREEN_HEIGHT/32), BLACK);
    display.fillCircle((SCREEN_WIDTH/2)-(SCREEN_WIDTH / 6), 20, SCREEN_HEIGHT / 8, WHITE);
    display.fillCircle((SCREEN_WIDTH/2)+(SCREEN_WIDTH / 6), 20, SCREEN_HEIGHT / 8, WHITE);
    display.display(); 

    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.setTextSize(STD_TEXT_SIZE);
    display.setTextWrap(false);
  }
  
  delay(5000);//delay before starting loop and to show all-is-good smiley
  init_display(); // init cross display
}

void loop()
{
  // Clear the amount of "clicks" the motors have run
  enc_clear();

  sonic_servo.write(0);
  delay(300);
  // BEGIN PRINTING TO SCREEN
    clear_corner(topleft); // Clear area 
    cursor_to_corner(topleft); // Reset cursor to top left
    display.println("LEFT DIST");
    display.setTextSize(LRG_TEXT_SIZE);
    display.println(sonar_distance()); // Print data
    display.setTextSize(STD_TEXT_SIZE);
    display.display(); // Clear
  // END PRINTING TO SCREEN

  sonic_servo.write(90);
  delay(300);
  // BEGIN PRINTING TO SCREEN
    clear_corner(topright); // Clear area 
    cursor_to_corner(topright); // Reset cursor to top left
    display.println("CENTER DIST");
    display.setTextSize(LRG_TEXT_SIZE);
    display.setCursor(SCREEN_WIDTH/2, 8);
    display.println(sonar_distance()); // Print data
    display.setTextSize(STD_TEXT_SIZE);
    display.display(); // Clear
  // END PRINTING TO SCREEN

  sonic_servo.write(180);
  delay(300);
  // BEGIN PRINTING TO SCREEN
    clear_corner(bottomleft); // Clear area 
    cursor_to_corner(bottomleft); // Reset cursor to top left
    display.println("RIGHT DIST");
    display.setTextSize(LRG_TEXT_SIZE);
    display.println(sonar_distance()); // Print data
    display.setTextSize(STD_TEXT_SIZE);
    display.display(); // Clear
  // END PRINTING TO SCREEN
  
  delay(1000);
}


// Function definitions
//This function sets IN1 = LOW and IN2 = HIGH in order to set the direction to forwards for motor 1
void left_motor(direction dir) {
  //Serial.println("forward");
  if(dir == forward)
  {
    digitalWrite(IN1,LOW); //IN1
    digitalWrite(IN2,HIGH); //IN2
  }
  else if(dir == backward)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if(dir == noward)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  
}

//This function sets IN3 = LOW and IN4 = HIGH in order to set the direction to forwards for motor 2
void right_motor(direction dir) {
  //Serial.println("backward");
  if(dir == forward)
  {
    digitalWrite(IN3,LOW); //IN3
    digitalWrite(IN4,HIGH); //IN4
  }
  else if(dir == backward)
  {
    digitalWrite(IN3,HIGH); //IN3
    digitalWrite(IN4,LOW); //IN4
  }
  else if(dir == noward)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

unsigned int sonar_distance(byte checks) {
  // Sum of all sensor readings so that they can be "smoothed"
  unsigned long average_duration = 0L;
  
  for (byte i = 0; i < checks;i++) {
    // Make a 10 microsecond pulse
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    // half (cause we don't want back AND forth) * duration (micro s) * 1e-6(conversion to s) * speed of sound (m/s) * 1e3 (conversion to mm)
    average_duration += pulseIn(ECHO, HIGH); 
  }
  
  average_duration /= checks; // Average all the readings so we get a "smoothed" (more accurate) reading
  
  Serial.print("Distance=");Serial.println((unsigned int)(0.5 * average_duration * 1e-6 * speed_sound * 1e3)); //print to console
  
  return (unsigned int)(0.5 * average_duration * 1e-6 * speed_sound * 1e3);  
}

void clear_corner(quarter qrt) {
  switch(qrt) {
    case topleft:
    display.fillRect(0, 0, SCREEN_WIDTH/2-1, SCREEN_HEIGHT/2-1, BLACK);
    break;
    case topright:
    display.fillRect(SCREEN_WIDTH/2+1, 0, SCREEN_WIDTH/2-1, SCREEN_HEIGHT/2-1, BLACK);
    break;
    case bottomleft:
    display.fillRect(0, SCREEN_HEIGHT/2+1, SCREEN_WIDTH/2-1, SCREEN_HEIGHT/2-1, BLACK);
    break;
    case bottomright:
    display.fillRect(SCREEN_WIDTH/2+1, SCREEN_HEIGHT/2+1, SCREEN_WIDTH/2-1, SCREEN_HEIGHT/2-1, BLACK);
    break;
  }
}

void cursor_to_corner(quarter qrt) {
  switch(qrt) {
    case topleft:
    display.setCursor(0, 0);
    break;
    case topright:
    display.setCursor(SCREEN_WIDTH/2+1, 0);
    break;
    case bottomleft:
    display.setCursor(0, SCREEN_HEIGHT/2+1);
    break;
    case bottomright:
    display.setCursor(SCREEN_WIDTH/2+1, SCREEN_HEIGHT/2+1);
    break;
  }
}

void init_display(void) {
  display.clearDisplay();
  display.drawFastVLine(SCREEN_WIDTH/2, 0, SCREEN_HEIGHT, WHITE);
  display.drawFastHLine(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, WHITE);
  display.display();
}
//END
