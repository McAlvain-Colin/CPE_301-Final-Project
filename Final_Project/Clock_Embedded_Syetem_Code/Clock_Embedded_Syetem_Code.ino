//Allowed Libraries
#include <Stepper.h>       //stepper library
#include <LiquidCrystal.h> //display library
#include <RTClib.h>        //clock library
#include <Servo.h>         //servo library ASK IF THIS IS OKAY


//declarations
void test();
void setup();
void loop();
void U0init(unsigned long U0baud);
unsigned char U0kbhit();
unsigned char U0getchar();
void U0putchar(unsigned char U0pdata);
void adc_init();
unsigned int adc_read(unsigned char adc_channel_num);
void print_int(unsigned int out_num);
void stepper_init();
void servo_init();
void setup_timer_regs();
ISR(TIMER1_OVF_vect);


//BAUD Pointers
#define RDA 0x80
#define TBE 0x20  
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
//UART Pointers
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;


//input/output registers
//Define address pointers
volatile unsigned char* pin_a = (unsigned char*) 0x20;
volatile unsigned char* ddr_a  = (unsigned char*) 0x21; 
volatile unsigned char* port_a = (unsigned char*) 0x22;

volatile unsigned char* pin_b = (unsigned char*) 0x23;
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 
volatile unsigned char* port_b = (unsigned char*) 0x25;

volatile unsigned char* pin_c  = (unsigned char*) 0x26;
volatile unsigned char* ddr_c  = (unsigned char*) 0x27; 
volatile unsigned char* port_c = (unsigned char*) 0x28;

volatile unsigned char* pin_d = (unsigned char*) 0x29;
volatile unsigned char* ddr_d  = (unsigned char*) 0x2A; 
volatile unsigned char* port_d = (unsigned char*) 0x2B;

volatile unsigned char* pin_e = (unsigned char*) 0x2c;
volatile unsigned char* ddr_e  = (unsigned char*) 0x2d; 
volatile unsigned char* port_e = (unsigned char*) 0x2e;

volatile unsigned char* pin_f = (unsigned char*) 0x2F;
volatile unsigned char* ddr_f  = (unsigned char*) 0x30; 
volatile unsigned char* port_f = (unsigned char*) 0x31;

volatile unsigned char* pin_g = (unsigned char*) 0x32;
volatile unsigned char* ddr_g  = (unsigned char*) 0x33; 
volatile unsigned char* port_g = (unsigned char*) 0x34;

volatile unsigned char* pin_h = (unsigned char*) 0x100;
volatile unsigned char* ddr_h  = (unsigned char*) 0x101; 
volatile unsigned char* port_h = (unsigned char*) 0x102;

volatile unsigned char* pin_L = (unsigned char*) 0x3B;
volatile unsigned char* ddr_L  = (unsigned char*) 0x3C; 
volatile unsigned char* port_L = (unsigned char*) 0x3D;


//Analog Registers
volatile unsigned char* analogPort_A1 = (unsigned char*) 0x62; 
volatile unsigned char* analogPin_A1 = (unsigned char*) 0x63;
volatile unsigned char* analogDdr_A1  = (unsigned char*) 0x61; 


//Analog pointers for ADC
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;


// Timer Pointers
volatile unsigned char *myTCCR1A  = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B  = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C  = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1  = (unsigned char *) 0x6F;
volatile unsigned char *myTIFR1   = (unsigned char *) 0x36;
volatile unsigned int  *myTCNT1   = (unsigned  int *) 0x84;


//global ticks counter
unsigned int currentTicks; 
unsigned int timer_running;


//stepper setup
const int stepsPerRevolution = 200;
Stepper gearShiftStepper(stepsPerRevolution, 4, 5, 6, 7);


//servo setup
Servo clockHandServo;
int pos = 0;


//User Arrays
unsigned int user1[] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
unsigned int user2[] = {5, 35, 65, 95, 125, 155, 185, 215, 245, 275, 305, 335};
unsigned int user3[] = {10, 40, 70, 100, 130, 160, 190, 220, 250, 280, 310, 340};
unsigned int user4[] = {15, 45, 75, 105, 135, 165, 195, 225, 255, 285, 315, 345};
unsigned int user5[] = {20, 50, 80, 110, 140, 170, 200, 230, 260, 290, 320, 350};
unsigned int user6[] = {25, 55, 85, 115, 145, 175, 205, 235, 265, 295, 325, 355};

const int numClockPositions = 12;


//High Position Interrupt.


void setup() {
  // setup the UART
  U0init(9600);
  // setup the ADC
  adc_init();
  //setup peripherials
  stepper_init();
  servo_init();
  void setup_timer_regs();

  // set ports to output
  *ddr_a = 0xFF;
  *ddr_b = 0xFF;
  *ddr_c = 0xFF;
  *ddr_d = 0xFF;
  *ddr_e = 0xFF;
  *ddr_f = 0xFF;
  *ddr_g = 0xFF;
  *ddr_h = 0xFF;
  *ddr_L = 0xFF;
  // set ports to input
  test();
}

void loop() {
  // unsigned char cs1;
  // while (U0kbhit()==0){}; // wait for RDA = true
  // cs1 = U0getchar();    // read character
  // U0putchar(cs1);     // echo character


  //Photo Resistor test code. Delete after you prove functionality ***START***
  //get data from photo resister and save it
  unsigned int data;
  data = adc_read(1); //need a value to pass in. 
  //break the int into two pieces and pass each into two outchars
  //Use the Arduino IDE's serial plotter to plot the voltage values
  print_int(data);
  U0getchar();
  //Photo Resistor test code. Delete after you prove functionality ***END***  
}

void test()
{
  //test LED's by setting Port B High
  *port_h |= 0x20;
  *port_h |= 0x40;
  //*port_h = 0x00;
  *port_b |= 0x04;
  *port_b |= 0x08;
  *port_b |= 0x10;
  *port_b |= 0x20;
  *port_b |= 0x40;
  *port_b |= 0x80;
  delay(1000);
  *port_b = 0x00;  
  *port_h = 0x00;
  // delay(1000);
  // *port_h = 0x60;
  // *port_b = 0xFC;

  //test stepper motor
  gearShiftStepper.step(1000);
  delay(500);
  // gearShiftStepper.step(-1000);
  // delay(500);

  //test servo
  for (pos = 0; pos <= 180; pos += 1) { 
    clockHandServo.write(pos);              
    delay(15);                       
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    clockHandServo.write(pos);              
    delay(15);                       
  }  

  //test solenoid 
  *port_e = 0x10;
  delay(1000);
  *port_e = 0x00;
  delay(1000);
}

//UART Code
void U0init(unsigned long U0baud)
{
//  Students are responsible for understanding
//  this initialization code for the ATmega2560 USART0
//  and will be expected to be able to intialize
//  the USART in differrent modes.
//
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 // Same as (FCPU / (16 * U0baud)) - 1;
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}
//
// Read USART0 RDA status bit and return non-zero true if set
//
unsigned char U0kbhit()
{
  return *myUCSR0A & RDA;
}
//
// Read input character from USART0 input buffer
//
unsigned char U0getchar()
{
  return *myUDR0;
}
//
// Wait for USART0 TBE to be set then write character to
// transmit buffer
//
void U0putchar(unsigned char U0pdata)
{
  while((*myUCSR0A & TBE)==0);
  *myUDR0 = U0pdata;
}

//ACD Code
void adc_init() //initalizes the analog to digiral converter to be ready to read analog data
{
    // setup the A register
  *my_ADCSRA |= 0b10000000; // set bit   7 to 1 to enable the ADC
  *my_ADCSRA &= 0b11011111; // clear bit 6 to 0 to disable the ADC trigger mode
  *my_ADCSRA &= 0b11110111; // clear bit 5 to 0 to disable the ADC interrupt
  *my_ADCSRA &= 0b11111000; // clear bit 0-2 to 0 to set prescaler selection to slow reading
  // setup the B register
  *my_ADCSRB &= 0b11110111; // clear bit 3 to 0 to reset the channel and gain bits
  *my_ADCSRB &= 0b11111000; // clear bit 2-0 to 0 to set free running mode
  // setup the MUX Register
  *my_ADMUX  &= 0b01111111; // clear bit 7 to 0 for AVCC analog reference
  *my_ADMUX  |= 0b01000000; // set bit   6 to 1 for AVCC analog reference
  *my_ADMUX  &= 0b11011111; // clear bit 5 to 0 for right adjust result
  *my_ADMUX  &= 0b11100000; // clear bit 4-0 to 0 to reset the channel and gain bits
}

unsigned int adc_read(unsigned char adc_channel_num) //returns the analog data read from the analog channel given as a parameter
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX  &= 0b11100000;
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0b11110111;
  // set the channel number
  if(adc_channel_num > 7)
  {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5
    *my_ADCSRB |= 0b00001000;
  }
  // set the channel selection bits
  *my_ADMUX  += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
}
//not sure if I need this yet
void print_int(unsigned int out_num)
{
  unsigned char print_flag = 0;

  if(out_num >= 1000){
    U0putchar (out_num / 1000 + '0');
    print_flag = 1;
    out_num = out_num % 1000;
    }  
  if(out_num >= 100 || print_flag){
    U0putchar (out_num / 100 + '0');
    print_flag = 1;
    out_num = out_num % 100;
    }  
  if(out_num >= 10 || print_flag){
    U0putchar (out_num / 10 + '0');
    print_flag = 1;
    out_num = out_num % 10;
    }  
  U0putchar (out_num + '0');
  U0putchar ('\n');
}

//stepper functions
void stepper_init()
{
  gearShiftStepper.setSpeed(60);
}

//servo functions
void servo_init()
{
  clockHandServo.attach(3);
}

//timer functions
void setup_timer_regs()
{
  // setup the timer control registers
  *myTCCR1A= 0x00;
  *myTCCR1B= 0X00;
  *myTCCR1C= 0x00;
  
  // reset the TOV flag
  *myTIFR1 |= 0x01;
  
  // enable the TOV interrupt
  *myTIMSK1 |= 0x01;
}

//interrupt expample 
// TIMER OVERFLOW ISR
// ISR(TIMER1_OVF_vect)
// {
//   // Stop the Timer
//   *myTCCR1B &=0xF8;
//   // Load the Count
//   *myTCNT1 =  (unsigned int) (65535 -  (unsigned long) (currentTicks));
//   // Start the Timer
//   *myTCCR1B |=   0b00000001;
//   // if it's not the STOP amount
//   if(currentTicks != 65535)
//   {
//     // XOR to toggle PB6
//     *port_b ^= 0x40;
//   }
// }
