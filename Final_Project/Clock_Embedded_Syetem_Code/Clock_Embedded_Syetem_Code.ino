//Allowed Libraries
#include <LiquidCrystal.h> //display library
#include <RTClib.h>        //clock library
#include <Servo.h>         //servo library ASK IF THIS IS OKAY//Permission was given

// //interrupt Functions
// ISR(TIMER1_OVF_vect);


//BAUD Pointers
#define RDA 0x80
#define TBE 0x20  
#define DRE 0x10


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

//servo setup
Servo clockHand1Servo;
Servo clockHand2Servo;
Servo clockHand3Servo;

unsigned int pos = 0;
unsigned int pos1 = 0;
unsigned int pos2 = 0;
unsigned int pos3 = 0;


//hand Arrays
unsigned int hand1[] = {0, 30, 60, 90, 120, 10000, 180, 210, 240, 270, 300, 330};
unsigned int hand2[] = {5, 35, 65, 95, 125, 10005, 185, 21000, 245, 275, 305, 335};
unsigned int hand3[] = {10, 40, 70, 100, 130, 160, 190, 220, 250, 280, 310, 340};
unsigned int hand4[] = {1000, 45, 75, 105, 135, 165, 195, 225, 255, 285, 31000, 345};
unsigned int hand5[] = {20, 50, 80, 110, 140, 170, 200, 230, 260, 290, 320, 350};
unsigned int hand6[] = {25, 55, 85, 11000, 145, 175, 205, 235, 265, 295, 325, 355};

unsigned int* hands[] = {hand1, hand2, hand3, hand4, hand5, hand6};

unsigned int hand1Position = 0;
unsigned int hand2Position = 0;
unsigned int hand3Position = 0;
unsigned int hand4Position = 0;
unsigned int hand5Position = 0;
unsigned int hand6Position = 0;

unsigned int handpositions[] = {hand1Position, hand2Position, hand3Position, hand4Position, hand5Position, hand6Position};

unsigned int numClockPositions = 12;
unsigned int numHands = 6;

unsigned char printArmChar[46] = "Select the arm you would like to move. [1-6]: ";
unsigned char printPostionChar[52] = "Select where you would like to move it to. [1-12]: ";
unsigned char printTempChar[46] = "Select arm and position. [a-z][0-9]: ";

unsigned int data = 0; 

void setup() {
  // setup the UART
  U0init(9600);
  // U1init(9600);
  // setup the ADC
  adc_init();
  //setup peripherials
  servo_init();
  setup_timer_regs();

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
  // clockHand1Servo.write(90);   
  // clockHand2Servo.write(90);   
  // clockHand3Servo.write(90);   

  // run test function
  //test();

}

void loop() {
  // unsigned char cs1;
  // while (U0kbhit()==0){}; // wait for RDA = true
  // cs1 = U0getchar();    // read character
  // U0putchar(cs1);     // echo character
  // put your main code here, to run repeatedly:
  // printArmChoice();
  // unsigned int armChoice = getArmChoice();

  // USART0_Flush();

  // printPositionChoice();
  // unsigned int positionChoice = getPositionChoice();

  // unsigned int positionChange = getNewHandPostion(armChoice, positionChoice);
  workLight();

  printTempChoice();

  workLight();

  moveHandTemp();
  
  workLight();
}


void test(){
  // //test LED's by setting Port B High
  // *port_h |= 0x20;
  // *port_h |= 0x40;
  // //*port_h = 0x00;
  // *port_b |= 0x04;
  // *port_b |= 0x08;
  // *port_b |= 0x10;
  // *port_b |= 0x20;
  // *port_b |= 0x40;
  // *port_b |= 0x80;
  // delay(1000);
  // *port_b = 0x00;  
  // *port_h = 0x00;
  // delay(1000);
  // *port_h = 0x60;
  // *port_b = 0xFC;

  // test servo
  for (pos = 0; pos <= 180; pos += 1) { 
    clockHand1Servo.write(pos);              
    delay(1000); 
  }
  for (pos = 0; pos <= 180; pos += 1) {   
    clockHand2Servo.write(pos);              
    delay(1000);
  }
  for (pos = 0; pos <= 180; pos += 1) { 
    clockHand3Servo.write(pos);              
    delay(1000);                       
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    clockHand1Servo.write(pos);              
    delay(1000);   
  }
  for (pos = 0; pos >= 180; pos += 1) { 
    clockHand2Servo.write(pos);              
    delay(1000);  
  } 
  for (pos = 0; pos >= 180; pos += 1) { 
    clockHand3Servo.write(pos);              
    delay(1000);                       
  }  
}


//UART Code
void U0init(unsigned long U0baud)
{
  unsigned long FCPU = 16000000;
  unsigned int tbaud;
  tbaud = (FCPU / 16 / U0baud - 1);
  // Same as (FCPU / (16 * U0baud)) - 1;
  *myUCSR0A = 0xE0;
  *myUCSR0B = 0x18;
  *myUCSR0C = 0x06;
  *myUBRR0  = tbaud;
}


unsigned char U0kbhit()
{
  return *myUCSR0A & RDA;
}


unsigned char U0getchar()
{
  return *myUDR0;
}


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


//servo functions
void servo_init()
{
  clockHand1Servo.attach(3);
  clockHand2Servo.attach(4);
  clockHand3Servo.attach(5);
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


unsigned int getNewHandPostion(unsigned int armChoice, unsigned int posChoice){
  unsigned int positionChange = 0;

  for(unsigned int i = 0; i < numHands; i++){   
    if(i == armChoice){                                //find the hand      
      for(unsigned int j =0; j < numClockPositions; j++){              //find the position  
        if(j == posChoice){           
          positionChange = hands[i][j] - handpositions[i];
          print_int(positionChange);
          return positionChange;
        }
      }
    }
  }
}
// void moveHand(unsigned int armChoice, unsigned int posChange){
//   if(pos < posChange){
//     if(armChoice == 1){
//       for (pos = 0; pos <= posChange; pos += 1) { 
//         clockHand1Servo.write(pos);              
//         delay(1000);  
//       }
//     }
//     else if(armChoice == 2){
//       for (pos = 0; pos <= posChange; pos += 1) { 
//         clockHand2Servo.write(pos);              
//         delay(1000); 
//       }
//     }
//     else if(armChoice == 3){
//       for (pos = 0; pos <= posChange; pos += 1) { 
//         clockHand3Servo.write(pos);              
//         delay(1000); 
//       }
//     }
//   }
//   else if(pos > posChange){
//     if(armChoice == 1){
//       for (pos = 0; pos >= posChange; pos += 1) { 
//         clockHand1Servo.write(pos);              
//         delay(1000);  
//       }
//     }
//     else if(armChoice == 2){
//       for (pos = 0; pos >= posChange; pos += 1) { 
//         clockHand2Servo.write(pos);              
//         delay(1000); 
//       }
//     }
//     else if(armChoice == 3){
//       for (pos = 0; pos >= posChange; pos += 1) { 
//         clockHand3Servo.write(pos);              
//         delay(1000); 
//       }
//     }
//   }
// }
void moveHandTemp(){
  
  while (U0kbhit()==0){}; // wait for RDA = true
  unsigned char temp = U0getchar();
  U0putchar(temp);
  if(temp == 'a'){
      clockHand1Servo.write(hand1[0]/4);              
      delay(1000);  
  }

  else if(temp == 'b'){
      clockHand1Servo.write(hand1[1]/4);              
      delay(1000); 
  }


  else if(temp == 'c'){
      clockHand1Servo.write(hand1[2]/4);              
      delay(1000); 
  }
  


  else if(temp == 'd'){
      clockHand1Servo.write(hand1[3]/4);              
      delay(1000);  
  }


  else if(temp == 'e'){
      clockHand1Servo.write(hand1[4]/4);              
      delay(1000); 
  }


  else if(temp == 'f'){
      clockHand1Servo.write(hand1[5]/4);              
      delay(1000); 
  }


  else if(temp == 'g'){
      clockHand1Servo.write(hand1[6]/4);              
      delay(1000); 
  }


  else if(temp == 'h'){
      clockHand1Servo.write(hand1[7]/4);              
      delay(1000); 
  }


  else if(temp == 'i'){
      clockHand1Servo.write(hand1[8]/4);              
      delay(1000);  
  }

  else if(temp == 'j'){
      clockHand1Servo.write(hand1[9]/4);              
      delay(1000); 
  }

  else if(temp == 'k'){
      clockHand1Servo.write(hand1[10]/4);              
      delay(1000); 
  }

  else if(temp == 'l'){
      clockHand1Servo.write(hand1[11]/4);              
      delay(1000); 
  }

  else if(temp == 'm'){
      clockHand2Servo.write(hand2[0]/4);              
      delay(1000); 
  }

  else if(temp == 'n'){
      clockHand2Servo.write(hand2[1]/4);              
      delay(1000);  
  }


  else if(temp == 'o'){
      clockHand2Servo.write(hand2[2]/4);              
      delay(1000); 
  }

  else if(temp == 'p'){
      clockHand2Servo.write(hand2[3]/4);              
      delay(1000); 
  }


  else if(temp == 'q'){
      clockHand2Servo.write(hand2[4]/4);              
      delay(1000); 
  }
  
  else if(temp == 'r'){
      clockHand2Servo.write(hand2[5]/4);              
      delay(1000); 
  }

  else if(temp == 's'){
      clockHand2Servo.write(hand2[6]/4);              
      delay(1000);  
  }

  else if(temp == 't'){
      clockHand2Servo.write(hand2[7]/4);              
      delay(1000); 
  }

  else if(temp == 'u'){
      clockHand2Servo.write(hand2[8]/4);              
      delay(1000); 
  }

  else if(temp == 'v'){
      clockHand2Servo.write(hand2[9]/4);              
      delay(1000); 
  }

  else if(temp == 'w'){
      clockHand2Servo.write(hand2[10]/4);              
      delay(1000); 
  }


  else if(temp == 'x'){
      clockHand2Servo.write(hand2[11]/4);              
      delay(1000);  
  }


  else if(temp == 'y'){
      clockHand3Servo.write(hand3[0]/4);              
      delay(1000); 
  }

  else if(temp == 'z'){
      clockHand3Servo.write(hand3[1]/4);              
      delay(1000); 
  }


  else if(temp == '1'){
      clockHand3Servo.write(hand3[2]/4);              
      delay(1000); 
  }


  else if(temp == '2'){
      clockHand3Servo.write(hand3[3]/4);              
      delay(1000); 
  }


  else if(temp == '3'){
      clockHand3Servo.write(hand3[4]/4);              
      delay(1000);  
  }


  else if(temp == '4'){
      clockHand3Servo.write(hand3[5]/4);              
      delay(1000); 
  }

  else if(temp == '5'){
      clockHand3Servo.write(hand3[6]/4);              
      delay(1000); 
  }

  else if(temp == '6'){
      clockHand3Servo.write(hand3[7]/4);              
      delay(1000); 
  }

  else if(temp == '7'){
      clockHand3Servo.write(hand3[8]/4);              
      delay(1000); 
  }


  else if(temp == '8'){
      clockHand3Servo.write(hand3[9]/4);              
      delay(1000);  
  }

  else if(temp == '9'){
      clockHand3Servo.write(hand3[10]/4);              
      delay(1000); 
  }


  else if(temp == '0'){
      clockHand3Servo.write(hand3[11]/4);              
      delay(1000);
  }
}


//TIMER OVERFLOW ISR
ISR(TIMER1_OVF_vect)
{
  // Stop the Timer
  *myTCCR1B &=0xF8;
  // Load the Count
  *myTCNT1 =  (unsigned int) (65535 -  (unsigned long) (currentTicks));
  // Start the Timer
  *myTCCR1B |=   0b00000001;
  // if it's not the STOP amount
  if(currentTicks != 65535)
  {
    // XOR to toggle PB6
    *port_b ^= 0x40;
  }
}


void printArmChoice(){
  unsigned char strLength = 46;
  unsigned char i = 0;
  for(i; i < strLength; i++){
    U0putchar(printArmChar[i]);
  }
}


void printPositionChoice(){
  unsigned char strLength = 51;
  unsigned char i = 0;
  for(i; i < strLength; i++){
    U0putchar(printPostionChar[i]);
  }
}


void printTempChoice(){
  unsigned char strLength = 37;
  unsigned char i = 0;
  for(i; i < strLength; i++){
    U0putchar(printTempChar[i]);
  }
}


unsigned int getArmChoice(){
  while (U0kbhit()==0){}; // wait for RDA = true
  unsigned char armChoice = U0getchar();
  if(armChoice = '1'){
    U0putchar('1');
    U0putchar('\n');
    return 1;
  }
  else if(armChoice = '2'){
    U0putchar('2');
    U0putchar('\n');
    return 2;
  }
  else if(armChoice = '3'){
    U0putchar('3');
    U0putchar('\n');
    return 3;
  }
}


unsigned int getPositionChoice(){
  while (U0kbhit()==0){}; // wait for RDA = true
  unsigned char positionChoice = U0getchar();
  if(positionChoice = '1'){
    U0putchar('1');
    U0putchar('\n');
    return 1;
  }
  else if(positionChoice = '2'){
    U0putchar('2');
    U0putchar('\n');
    return 2;
  }
  else if(positionChoice = '3'){
    U0putchar('3');
    U0putchar('\n');
    return 3;
  }
  else if(positionChoice = '4'){
    U0putchar('4');
    U0putchar('\n');
    return 4;
  }
  else if(positionChoice = '5'){
    U0putchar('5');
    U0putchar('\n');
    return 5;
  }
  else if(positionChoice = '6'){
    U0putchar('6');
    U0putchar('\n');
    return 6;
  }
  else if(positionChoice = '7'){
    U0putchar('7');
    U0putchar('\n');
    return 7;
  }
  else if(positionChoice = '8'){
    U0putchar('8');
    U0putchar('\n');
    return 8;
  }
  else if(positionChoice = '9'){
    U0putchar('9');
    U0putchar('\n');
    return 9;
  }
  else if(positionChoice = '10'){
    U0putchar('10');
    U0putchar('\n');
    return 10;
  }
  else if(positionChoice = '11'){
    U0putchar('11');
    U0putchar('\n');
    return 11;
  }
  else if(positionChoice = '12'){
    U0putchar('12');
    U0putchar('\n');
    return 12;
  }
}


void USART0_Flush()
{
  unsigned char dummy;
  while (*myUCSR0A & RDA) dummy = UDR0;
}


void workLight(){
  unsigned int data = 0; 
  data = adc_read(1);
  if(data > 15){
    *port_b |= 0x80;
  }
  else{
    *port_b |= 0x00;
  }
}
