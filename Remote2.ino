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

// Number the buttons so we can work with them
const int BUTTON1 = 1;
const int BUTTON2 = 2;
const int BUTTON3 = 3;

/* High/low values to use for button press detection
Buttons wired according to:
http://www.instructables.com/id/How-to-access-5-buttons-through-1-Arduino-input
Each button in my testing only had a possible range of about 3 values,
but I gave them 10 just to allow for a little circuit noise.
This means it's possible to have quite a few buttons on a single analog pin. */
const int BUTTON1LOW = 925;
const int BUTTON1HIGH = 935;
const int BUTTON2LOW = 885;
const int BUTTON2HIGH = 895;
const int BUTTON3LOW = 835;
const int BUTTON3HIGH = 845;

// This is where we store which button has been pressed
int buttonState;
// Analog reading of the button pin
int btnReading = 0;

int ledpin = 13;
int a = ledpin;

// Uncomment for some extra output via serial
// This makes the app almost 2 kb bigger
// #define DEBUGGING

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
	irsend.sendRaw(pulses, pulseLength, 38);
}

void getButton() {
	btnReading = analogRead(buttonPin);

	// Compare the reading with stored values for each button to 
	// see which one was pressed and set buttonState accordingly.
	// If the reading is < 10 assume it's just noise.
	while (btnReading > 10) {
		if (btnReading > BUTTON3LOW && btnReading < BUTTON3HIGH) {
			buttonState = BUTTON3;
		}
		else if (btnReading > BUTTON2LOW && btnReading < BUTTON2HIGH) {
			buttonState = BUTTON2;
		}
		else if (btnReading > BUTTON1LOW && btnReading < BUTTON1HIGH) {
			buttonState = BUTTON1;
		}
		else {
			buttonState = LOW;
		}

		// Do stuff with that state
		switch (buttonState) {
		case BUTTON1:
			// Note that the size of raw pulses are calculated as
			// sizeof(arrayname) / sizeof(datatype)
			sendRawPulses(yamahaPwr, sizeof(yamahaPwr) / sizeof(uint16_t));
			break;
		case BUTTON2:
			// Yamaha Volume Up using NEC code from the IRremote lib
			// size here is just code size (in bytes) * 8 bits per byte
			irsend.sendNEC(yvup, sizeof(yvup) * 8);
			break;
		case BUTTON3:
			// Yamaha Volume Down
			irsend.sendNEC(yvdown, sizeof(yvdown) * 8);
			break;
		}

		/* Delay so it doesn't repeat too fast.
		Debouncing is skipped because the interrupt
		is disabled as soon as a button press is detected,
		and debouncing was causing issues.  */
		delay(250);

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