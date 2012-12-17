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