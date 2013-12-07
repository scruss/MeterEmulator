/*
 * MeterEmulator.ino
 * Pretends to be a power meter with a 50 ms LED pulse every watt hour
 * Uses DFRobot LCD screen for control/display:
 * http://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_%28SKU:_DFR0009%29
 * Outputs:
 * D2 - LTE 5208A IR LED through 29222A to get enough current
 * (D13 - Onboard LED flash in time with IR for visible confirmation)
 *
 * Much of the display stuff borrowed from Mark Bramwell's example code.
 *
 * Stewart C. Russell / scruss.com - 2013
 *
 */

#include <LiquidCrystal.h>

// select the pins used on the LCD panel; defaults used
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// panel and button state
int lcd_key = 0;
int adc_key_in = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// values for the flash timing / ports
int led = 13;
int irled = 2;
int backlightpin = 10;

/*
 * these values can only be positive, and should (untested!)
 * handle the millis() wrap-around gracefully.
 *
 * Given that a very expensive SCADA system I once worked with
 * didn't handle meter wrap-around at all (yay negative revenue!)
 * I am quite relaxed about this.
 *
 */

unsigned long pulsecount = 0;
unsigned long thismillis = 0;
unsigned long oldmillis = 0;
unsigned long looptime = 0;

/*
 * LED flash / loop timings
 *
 * You can change everything except FLASH_DURATION
 * - if you change that, you may not trigger any pulse counting
 * devices attached to your emulated meter
 *
 * The indicated power will not be exactly as you'd
 * expect from the timings below. Since the actual period
 * between LED flashes is equal to:
 *
 *  FLASH_DURATION + pause time + display overhead
 *
 * It's pretty close, though, and an accurate instantaneous
 * value is calculated and displayed in the main loop
 *
 */

// length of LED pulse: 50 ms - do not change this
#define FLASH_DURATION 50

//  350 ms gives about 2.5  Hz, indicating ~9.0 kW
#define PAUSE_DURATION_MIN 350

// 1300 ms gives about 0.74 Hz, indicating ~2.7 kW
#define PAUSE_DURATION_MAX 1300

// each button press add step size of 50 ms
#define PAUSE_DURATION_STEP 50

// default about 1.2 Hz, ~4.2 kW
#define PAUSE_DEFAULT 800

int pausetime = PAUSE_DEFAULT;

// read the buttons on DFRobot shield
int
read_LCD_buttons() {
  adc_key_in = analogRead(0);
  /* 
   * Shield uses resistor ladder to map buttons
   * - these (default) values worked for me, but 
   * other variants may be different;
   * check your documentation
   *
   */
  if (adc_key_in > 1000)
    return btnNONE;      // no value recognized
  if (adc_key_in < 50)
    return btnRIGHT;
  if (adc_key_in < 250)
    return btnUP;
  if (adc_key_in < 450)
    return btnDOWN;
  if (adc_key_in < 650)
    return btnLEFT;
  if (adc_key_in < 850)
    return btnSELECT;
  return btnNONE;        // no value recognized
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(irled, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(backlightpin, OUTPUT);
  analogWrite(backlightpin, 50);  // dim display slightly
  lcd.print("MeterEmulator...");
  lcd.setCursor(0, 1);
  lcd.print(" scruss - 2013");
  delay(500);
  lcd.clear();
}

void loop() {
  oldmillis = thismillis;
  thismillis = millis();

  // flash LEDs for 50 ms
  digitalWrite(irled, HIGH);    // send the pulse
  digitalWrite(led, HIGH);	// flash board LED for visibility
  delay(FLASH_DURATION);
  digitalWrite(irled, LOW);
  digitalWrite(led, LOW);	// both LEDs off now
  pulsecount++;


  delay(pausetime);		// pause for requested time

    // read the keyboard - gets a bit laggy for long pause times
  lcd_key = read_LCD_buttons();
  switch (lcd_key) {
  case btnDOWN:
    pausetime += PAUSE_DURATION_STEP;
    if (pausetime > PAUSE_DURATION_MAX)
      pausetime = PAUSE_DURATION_MAX;
    break;
  case btnUP:
    pausetime -= PAUSE_DURATION_STEP;
    if (pausetime < PAUSE_DURATION_MIN)
      pausetime = PAUSE_DURATION_MIN;
    break;
  case btnSELECT:
    pausetime = PAUSE_DEFAULT;
    break;
  }

  /*
   * now display results
   *
   * We're actually displaying the time of the previous loop
   * but since typical loop times are around 1 second,
   * steady state prevails
   *
   */
  looptime = thismillis - oldmillis;

  lcd.clear();
  // display cumulative energy since restart
  lcd.print((float) pulsecount / 1000.0);
  lcd.print(" kWh");

  lcd.setCursor(0, 1);	// new line
  /*
   * convert loop time (in milliseconds) to
   * instantaneous power. Should be fairly accurate
   *
   */
  lcd.print(3.6 / (((float) looptime) / 1000.0));
  lcd.print(" kW ");
  lcd.print(looptime);
  lcd.print(" ms");
}


