/* Raw IR decoder sketch!
This sketch/program uses the Arduno and a PNA4602 to
decode IR received. This can be used to make a IR receiver
(by looking for a particular code)
or transmitter (by pulsing an IR LED at ~38KHz for the
durations detected
Code is public domain, check out www.ladyada.net and adafruit.com
for more tutorials!
*/

/* Modified a bit to give different output depending on how
   the codes are going to be used.  Also, a TSOP38338 from
   digi-key may be needed in place of the reciever from adafruit
   if a comcast remote using the XMP protocol is being recorded.
   The manufacturer says the one from adafruit won't work for XMP
   and so far that seems to be true.  -Josh */

// Define pin to use for raw pin reading.  digitalRead()
// is too slow.
#define IRpin_PIN PIND
#define IRpin 2

// Max pulse lengthe we'll listen for - 65 ms.
#define MAXPULSE 65000

// Timing resolution
#define RESOLUTION 20

// store up to 100 pulse pairs
uint16_t pulses[100][2];  // pair is high and low pulse
uint8_t currentpulse = 0; // pulse index

void setup()
{

  Serial.begin(9600);
  Serial.println("Ready to decode IR!");

}

void loop()
{

  uint16_t highpulse, lowpulse; // temporary storage timing
  highpulse = lowpulse = 0;  // start with length 0

  while (IRpin_PIN & (1 << IRpin)) {
	  
	  // wait a bit
	  highpulse++;
	  delayMicroseconds(RESOLUTION);

	  // If the pulse is too long, we timed out.  Either
	  // nothing was received or the code is finished.
	  // Print what we've got and then reset.
	  if ((highpulse >= MAXPULSE) && (currentpulse !=0)) {
		  printpulses3();
		  currentpulse=0;
		  return;
	  }
  }

  // we didn't time out so lets store the reading
  pulses[currentpulse][0] = highpulse;

  // same as above but measuring lows
  while (! (IRpin_PIN & _BV(IRpin))) {
	  // pin is still LOW
	  lowpulse++;
	  delayMicroseconds(RESOLUTION);
	  if ((lowpulse > MAXPULSE) && (currentpulse != 0)) {

		  // Change this to whichever version you need.  See
		  // the printpulses functions below
		  printpulses3();

		  currentpulse=0;
		  return;
	  }
  }

  pulses[currentpulse][1] = lowpulse;

  // we read one high-low pulse successfully, continue!
  currentpulse++;

}

// The original from ladyada
void printpulses() {
	Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
	for (uint8_t i=0; i < currentpulse; i++) {
		Serial.print(pulses[i][0] * RESOLUTION, DEC);
		Serial.print(" usec, ");
		Serial.print(pulses[i][1] * RESOLUTION, DEC);
		Serial.println(" usec");
	}

	// print it in an 'array' format
	Serial.println("int IRsignal[] = {");
	Serial.println("// ON, OFF (in 10's of microseconds)");
	for (uint8_t i = 0; i < currentpulse-1; i++) {
		Serial.print("\t"); // tab
		Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
		Serial.print(", ");
		Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
		Serial.println(",");
	}
	Serial.print("\t");
	Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
	Serial.print(", 0};");
}

// Print the pulses the long way
void printpulses2() {
	Serial.println("Received: Off \tON");
	for (uint8_t i=0; i < currentpulse; i++) {
		Serial.print("delayMicroseconds(");
		Serial.print(pulses[i][0] * RESOLUTION, DEC);
		Serial.println(");");
		Serial.print("pulseIR(");
		Serial.print(pulses[i][1] * RESOLUTION, DEC);
		Serial.println(");");
	}
}

/* Print the pulses in a way that can be used directly
   in irsend.sendRaw without modification.  Subtracting
   RESOLUTION from the pulse length got me more accurate
   results but depends a lot on how things are connected.
   Some testing will be needed to get results that work
   for you.  */
void printpulses3() {
	for (uint8_t i=0; i < currentpulse; i++) {
		if (i == 0) {
			Serial.println("theCode[] = {");
			Serial.print((pulses[i][1] * RESOLUTION) - RESOLUTION, DEC);
			Serial.println(", ");
		}
		else {
			Serial.print((pulses[i][0] * RESOLUTION) - RESOLUTION, DEC);
			Serial.print(", ");
			Serial.print((pulses[i][1] * RESOLUTION) - RESOLUTION, DEC);
			Serial.println(",");
		}
	}
	Serial.println("};");
}