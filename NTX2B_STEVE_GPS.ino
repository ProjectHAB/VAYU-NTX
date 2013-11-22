//Example String: $$GPGGA,192651.00,5127.17084,N,00010.57874,E,1,06,1.44,11.8,M,45.M,45.5,M,,*50

#define RADIOPIN 3

#include <string.h>
#include <util/crc16.h>

char datastring[256];
char gpsstring[80];
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
unsigned int ptr = 0;
unsigned int count = 0; //sentence counter
bool decode = false;
unsigned long chars;
unsigned short sentences,failed_checksum;

void setup()
{
	pinMode(RADIOPIN,OUTPUT);
	setPwmFrequency(RADIOPIN, 1);   
	Serial.begin(9600);
}
 
void loop()
{
	while(Serial.available())// check for serial data
          inChar = Serial.read();// Read a character
          if (inChar='$')// Check for $ character (ASCII Code 36)
           {
           gpsstring[index] = inChar; // Store it
           index++;// Increment counter
           while(index < 70) // One less than the size of the array
              {
                inChar = Serial.read(); // Read a character
                if (inChar !=-1)
                {
                gpsstring[index++] = inChar; // Store it
                }
              }                   
            }
    gpsstring[index] = '\0'; // Null terminate the string
    index=0;//Reset counter
    rtty_txstring (gpsstring);   //Transmit GPS String 
{


}
}
   
void rtty_txstring (char * string)
{
 
/* Simple function to sent a char at a time to
 ** rtty_txbyte function.
 ** NB Each char is one byte (8 Bits)
 */
 
char c;
 
c = *string++;
 
while ( c != '\0')
 {
 rtty_txbyte (c);
 c = *string++;
 }
}
void rtty_txbyte (char c)
{
 /* Simple function to sent each bit of a char to
 ** rtty_txbit function.
 ** NB The bits are sent Least Significant Bit first
 **
 ** All chars should be preceded with a 0 and
 ** proceded with a 1. 0 = Start bit; 1 = Stop bit
 **
 */
 
int i;
 
rtty_txbit (0); // Start bit
 
// Send bits for for char LSB first
 
for (i=0;i<7;i++) // Change this here 7 or 8 for ASCII-7 / ASCII-8
 {
 if (c & 1) rtty_txbit(1);
 
else rtty_txbit(0);
 
c = c >> 1;
 
}
 
rtty_txbit (1); // Stop bit
 rtty_txbit (1); // Stop bit
}
 
void rtty_txbit (int bit)
{
 if (bit)
 {
 // high
 analogWrite(RADIOPIN,166);
 }
 else
 {
 // low
 analogWrite(RADIOPIN,150);
 
}
 
// delayMicroseconds(3370); // 300 baud
 delayMicroseconds(10000); // For 50 Baud uncomment this and the line below.
 delayMicroseconds(10150); // You can't do 20150 it just doesn't work as the
 // largest value that will produce an accurate delay is 16383
 // See : http://arduino.cc/en/Reference/DelayMicroseconds
 
}
 
uint16_t gps_CRC16_checksum (char *string)
{
 size_t i;
 uint16_t crc;
 uint8_t c;
 
crc = 0xFFFF;
 
// Calculate checksum ignoring the first two $s
 for (i = 2; i < strlen(string); i++)
 {
 c = string[i];
 crc = _crc_xmodem_update (crc, c);
 }
 
return crc;
}
 
void setPwmFrequency(int pin, int divisor) {
 byte mode;
 if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
 switch(divisor) {
 case 1:
 mode = 0x01;
 break;
 case 8:
 mode = 0x02;
 break;
 case 64:
 mode = 0x03;
 break;
 case 256:
 mode = 0x04;
 break;
 case 1024:
 mode = 0x05;
 break;
 default:
 return;
 }
 if(pin == 5 || pin == 6) {
 TCCR0B = TCCR0B & 0b11111000 | mode;
 }
 else {
 TCCR1B = TCCR1B & 0b11111000 | mode;
 }
 }
 else if(pin == 3 || pin == 11) {
 switch(divisor) {
 case 1:
 mode = 0x01;
 break;
 case 8:
 mode = 0x02;
 break;
 case 32:
 mode = 0x03;
 break;
 case 64:
 mode = 0x04;
 break;
 case 128:
 mode = 0x05;
 break;
 case 256:
 mode = 0x06;
 break;
 case 1024:
 mode = 0x7;
 break;
 default:
 return;
 }
 TCCR2B = TCCR2B & 0b11111000 | mode;
 }
}
