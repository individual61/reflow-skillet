#include "parameters.h"

// Modified Adafruit thermocouple library per
// https://forums.adafruit.com/viewtopic.php?f=19&t=108896

// If the screen looks stretched vertically, and skips lines, then the Adafruit
// library header file must be edited as parameters
// https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples

//////////////////////// GLOBALS

// Display globals
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Thermocouple globals
Adafruit_MAX31856 max =
	Adafruit_MAX31856(MAX31856_CS, MAX31856_MOSI, MAX31856_MISO, MAX31856_CLK);
float g_coldtemp = 0.0;
float g_thtemp = 0.0;
float g_tset = 0.0;
uint8_t g_fault = 0;
uint32_t g_previous_temp_read_time = 0;

// Button globalS
//

// Profile globals
uint8_t g_currentStep = 0;
uint32_t g_timeStepStart = 0;
uint32_t g_timeStepElapsed = 0;
uint32_t g_step_duration = 0;
double g_t_ramp_start = 0.0;
double g_t_ramp_end = 0.0;

double profile_times[] =
{
	210, // 0, to 150
	120, // 1, to 175
	100, // 2, to 217
	80, // 3, to 235
	30, // 4, to 235
	60, // 5, to 217
	180 // 6 to 15
};

double profile_temps[] =
{
	150.0, // 0
	175.0, // 1
	217.0, // 2
	235.0, // 3  // paste says 249, but APA102C says 235
	235.0,         // 4
	217.0, // 5
	15.0
};

/*
   double profile_times[] =
   {
        1.0, // 0
        2000.0, // 1
        3.0, // 2
        4.0, // 3
        5.0 // 4
   };

   double profile_temps[] =
   {
        100, // 0
        100, // 1
        100, // 2
        100, // 3
        15 // 4
   };
 */

// State Machine globals
State_enum theState = idle;
bool g_heartbeat = 0;

// PID globalS
bool g_heating = 0;

float g_fake_temp = 25.0;

uint32_t g_windowStartTime = 0;

double g_PID_setpoint = 0; // from profile
double g_PID_output = 0;   // control relay ON time
double g_PID_input = 0;    // measured temp

PID myPID(&g_PID_input, &g_PID_output, &g_PID_setpoint, KP, KI, KD, DIRECT);

///////////////////////////// END GLOBALS

void setup()
{

	Serial.begin(115200);

	theState = idle;
	g_fault = 0;

	// Thermocouple stuff
	max.begin();
	max.setThermocoupleType(MAX31856_TCTYPE_K);
	max.config();
	update_temps();

	// Profile stuff
	g_currentStep = 0;

	// OLED stuff
	display.begin(SSD1306_SWITCHCAPVCC);

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.clearDisplay();
	display.display();

	// PID stuff
	g_heating = 0;
	g_windowStartTime = millis();
	myPID.SetOutputLimits(0, PID_OUTPUTLIMIT);
	myPID.SetMode(AUTOMATIC);
	myPID.SetSampleTime(PID_SAMPLE_TIME);

	// SSR stuff
	pinMode(OUTPUT_PIN, OUTPUT);
	digitalWrite(OUTPUT_PIN, LOW);
}


void loop()
{
	g_heartbeat = !g_heartbeat;

	switch (theState) {

	case idle: ///////////////////////////// IDLE
		checkPauseButton();
		checkStartStopButton();
		update_temps();
		update_display();
		g_heating = 0;
		digitalWrite(OUTPUT_PIN, LOW);
		break;

	case running: ///////////////////////////// RUNNING
		checkPauseButton();
		checkStartStopButton();
		update_temps();
		g_timeStepElapsed = millis() - g_timeStepStart;

		calculate_tset_from_ramp(g_t_ramp_start, g_t_ramp_end, profile_times[g_currentStep], (double)(g_timeStepElapsed/1000.0)); // also sets g_PID_setpoint
		update_PID_and_set_output();
		update_display();

		//	Serial.print(F("g_t_ramp_start "));
		//	Serial.print(g_t_ramp_start);
//		Serial.print(F("\tg_t_ramp_end "));
//		Serial.print(g_t_ramp_end);
//		Serial.print(F("\tprofile_times[g_currentStep] "));
//		Serial.print(profile_times[g_currentStep]);
//		Serial.print(F("\tg_timeStepElapsed/1000.0 "));
//		Serial.print(g_timeStepElapsed/1000.0);
//		Serial.print(F("\tg_tset "));
		Serial.print(F("\t"));
		Serial.print(g_tset);
		//	Serial.print(F("\t g_thtemp"));
		Serial.print(F("\t"));
		Serial.print(g_thtemp);
		Serial.print(F("\t"));
		Serial.println(g_PID_output);

		if (g_timeStepElapsed > (1000 * profile_times[g_currentStep]))
		{
			advance_to_next_step();
		}
		break;

	case fault: ///////////////////////////// FAULT
		digitalWrite(OUTPUT_PIN, LOW);
		g_heating = 0;
		display.clearDisplay();
		display.setCursor(0, 0);
		display.println(F("Fault: "));

		display_fault();

		g_fault = max.readFault();
		Serial.println(F("In Fault state. Fault is reported as: "));
		Serial.println(g_fault);
		if (!g_fault) {
			theState = idle;
		}
		break;

	case pause: ///////////////////////////// PAUSED
		checkPauseButton();
		checkStartStopButton();
		update_temps();
		update_PID_and_set_output();
		update_display();

		break;
	}

	delay(10);
}
