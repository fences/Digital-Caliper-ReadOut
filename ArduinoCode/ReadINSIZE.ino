
const int LEN  = 25;
const int DATA = 20;
const int SIGN = 21;
const int MODE = 24;
int bit_array[LEN];        // For storing the data bit. bit_array[0] = data bit 1 (LSB), bit_array[23] = data bit 24 (MSB).

unsigned long time_now;   // For storing the time when the clock signal is changed from HIGH to LOW (falling edge trigger of data output).
long debouncing_time = 50; //Debouncing Time in Milliseconds
unsigned long last_micros = 0;

int DATA_PIN = 3;
int CLOCK_PIN = 4;
int SWITCH_PIN = 2;
bool read_to_end = false;

void setup()
{

  Serial.begin(115200);
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), debounceInterrupt, HIGH);
}

void loop()
{

}

void decode() {
  int sign = 1;
  int i = 0;
  float value = 0.0;
  float result = 0.0;

  bit_array[i] = !digitalRead(DATA_PIN);       // Store the 1st bit (start bit) which is always 1.
  while (digitalRead(CLOCK_PIN) == LOW) {}
  for (i = 1; i < LEN; i++)
  {
    while (digitalRead(CLOCK_PIN) == HIGH) { } // Wait until clock returns to HIGH
    bit_array[i] = !digitalRead(DATA_PIN);
    while (digitalRead(CLOCK_PIN) == LOW) {} // Wait until clock returns to LOW
  }

  for (i = 0; i < DATA; i++)
  { // Turning the value in the bit array from binary to decimal.
    value = value + (pow(2, i - 1) * bit_array[i]);
  }

  if (bit_array[SIGN] == 1) sign = -1;          // Bit SIGN is the sign bit. 0 -> +, 1 => -

  if (bit_array[MODE] == 1)
  { // Bit MODE tells the measuring unit (1 -> in, 0 -> mm)
    result = (value * sign) / 2000.00;
    Serial.print(result, 3);                  // Print result with 3 decimals
    Serial.println(" in");
  } else
  {
    result = (value * sign) / 100.00;
    Serial.print(result, 2);                  // Print result with 2 decimals
    Serial.println(" mm");

  }
}


void debounceInterrupt()
{
  if (!read_to_end)
  {
    if ((millis() - last_micros) >= debouncing_time)
    {
      Read();
      last_micros = millis();
    }
  }
}

void Read()
{
  read_to_end = true;
  while (digitalRead(CLOCK_PIN) == HIGH) {}  // If clock is LOW wait until it turns to HIGH
  time_now = micros();
  while (digitalRead(CLOCK_PIN) == LOW) {} // Wait for the end of the HIGH pulse
  if ((micros() - time_now) > 800)
  {
    decode();
  }
  read_to_end = false;
}
