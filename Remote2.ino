/* This app will send out recognized signals recorded using 
the IRrecvDemo app from the IRremote library downloaded here:
http://www.arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
and raw signals recorded using slightly modified code from here:
http://learn.adafruit.com/ir-sensor/using-an-ir-sensor */

/* PinChangeInt is used to put an interrupt on an analog pin
This should allow sleeping between button presses to
conserve battery power.  Pgmspace is for storing variables
in flash.  */
#include <avr/sleep.h>
#include <IRremote.h>
#include "Codes.h"
#include <PinChangeInt.h>

const int buttonPin = A0;
IRsend irsend;

// Save memory by telling PinChangeInt there won't be interrupts
// used on port B or D (the digital pins).  We just need port C.
#define NO_PORTB_PINCHANGES
#define NO_PORTD_PINCHANGES

/* High/low values to use for button press detection
Buttons wired according to:
http://www.instructables.com/id/How-to-access-5-buttons-through-1-Arduino-input
Each button in my testing only had a possible range of about 3 values,
but I gave them 10 just to allow for a little circuit noise.
This means it's possible to have quite a few buttons on a single analog pin.
To get these values just wire up the buttons, run analogRead, and record the results.
I used the value read +/- 1 to leave a little room for interference.  */
const int BUTTON1LOW = 735;
const int BUTTON1HIGH = 737;
const int BUTTON2LOW = 804;
const int BUTTON2HIGH = 806;
const int BUTTON3LOW = 836;
const int BUTTON3HIGH = 838;
const int BUTTON4LOW = 929;
const int BUTTON4HIGH = 931;
const int BUTTON5LOW = 973;
const int BUTTON5HIGH = 975;
const int BUTTON6LOW = 977;
const int BUTTON6HIGH = 979;
const int BUTTON7LOW = 990;
const int BUTTON7HIGH = 992;
const int BUTTON8LOW = 1001;
const int BUTTON8HIGH = 1003;

// Analog reading of the button pin
int btnReading = 0;

// Should be self explanitory
int statusLED = 13;

// Uncomment for some extra output via serial
// This makes the app around 500 bytes bigger
#define DEBUGGING

void setup()
{
	pinMode(buttonPin, INPUT);

	// Attach an interrupt to the button pin and set it to
	// trigger when the signal rises.  Call intCode() when
	// a button press is detected.
	PCintPort::attachInterrupt(buttonPin, &intCode, RISING);

#ifdef DEBUGGING
	Serial.begin(9600);
	Serial.println("Lets do this!");
#endif
}

void loop()
{
	getButton();

	// Save some battery power and sleep between button presses.
	goToSleep();
}

void sendRawPulses(uint16_t *pulses, uint16_t pulseLength) {
#ifdef DEBUGGING
	Serial.print("Sending ");
	Serial.print(pulseLength);
	Serial.println(" pulses");
#endif
	// sendRaw(array containing pulses, size of the array, frequency)
	irsend.sendRaw(pulses, pulseLength, 39);
}

void getButton() {
	btnReading = analogRead(buttonPin);

	// Compare the reading with stored values for each button to 
	// see which one was pressed and set buttonState accordingly.
	// If the reading is < 10 assume it's just noise.
	while (btnReading > 10) {
		if (btnReading > BUTTON1LOW && btnReading < BUTTON1HIGH) {
			Serial.println(sizeof(cblPower));
			sendRawPulses(cblPower, sizeof(cblPower) / sizeof(uint16_t));
			statusFlash();
			delay(500);
		}
		else if (btnReading > BUTTON2LOW && btnReading < BUTTON2HIGH) {
			irsend.sendRaw(sonyPower, sizeof(sonyPower) / sizeof(uint16_t), 38);
			statusFlash();
			delay(500);
		}
		else if (btnReading > BUTTON3LOW && btnReading < BUTTON3HIGH) {
			irsend.sendNEC(yvup, sizeof(yvdown) * 8);
			statusFlash();
		}
		else if (btnReading > BUTTON4LOW && btnReading < BUTTON4HIGH) {
			irsend.sendNEC(yvdown, sizeof(yvup) * 8);
			statusFlash();
		}
		else if (btnReading > BUTTON5LOW && btnReading < BUTTON5HIGH) {
			irsend.sendNEC(ydvd, sizeof(ydvd) * 8);
			statusFlash();
		}
		else if (btnReading > BUTTON6LOW && btnReading < BUTTON6HIGH) {
			irsend.sendNEC(ycbl, sizeof(ycbl) * 8);
			statusFlash();
		}
		else if (btnReading > BUTTON7LOW && btnReading < BUTTON7HIGH) {
			irsend.sendNEC(yvcr, sizeof(yvcr) * 8);
			statusFlash();
		}
		else if (btnReading > BUTTON8LOW && btnReading < BUTTON8HIGH) {
			irsend.sendNEC(yaux, sizeof(yaux) * 8);
			statusFlash();
		}
		else {
			// This shouldn't happen, but lets deal with it just in case
			// there's some signal noise or other interference.
#ifdef DEBUGGING
			Serial.println("Unknown button result");
#endif
		}

		/* Delay so it doesn't repeat too fast.
		Debouncing is skipped because the interrupt
		is disabled as soon as a button press is detected,
		and debouncing was causing issues.  */
		delay(100);

		// Check if the button is being held down
		btnReading = analogRead(buttonPin);
	}
}

void intCode() {
	// This one might not be needed, though the example on arduino.cc
	// puts sleep_disable() in both the interrupt function and the
	// code that puts it to sleep.  I'll do it their way.
	sleep_disable();

	// Detach the interrupt so it doesn't trigger while
	// we're awake and doing stuff.
	PCintPort::detachInterrupt(buttonPin);
}

void goToSleep() {
	// Time to go to sleep!
	sleep_enable();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	// Attach the interrupt to the analog button pin
	// using the PinChangeInt library.
	PCintPort::attachInterrupt(buttonPin, &intCode, RISING);

	sleep_cpu();

	// *yawn* Wasn't that a nice nap?
	sleep_disable();
}

void statusFlash() {
	digitalWrite(statusLED, HIGH);
	delay(125);
	digitalWrite(statusLED, LOW);
	delay(125);
}