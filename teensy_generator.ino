#include <EEPROM.h>

uint16_t sineTable[512] = {
  0x800,0x819,0x832,0x84b,0x864,0x87d,0x896,0x8af,
  0x8c8,0x8e1,0x8fa,0x913,0x92c,0x945,0x95e,0x976,
  0x98f,0x9a8,0x9c0,0x9d9,0x9f1,0xa09,0xa22,0xa3a,
  0xa52,0xa6a,0xa82,0xa9a,0xab1,0xac9,0xae0,0xaf8,
  0xb0f,0xb26,0xb3d,0xb54,0xb6b,0xb82,0xb98,0xbae,
  0xbc5,0xbdb,0xbf1,0xc06,0xc1c,0xc32,0xc47,0xc5c,
  0xc71,0xc86,0xc9a,0xcaf,0xcc3,0xcd7,0xceb,0xcff,
  0xd12,0xd26,0xd39,0xd4c,0xd5f,0xd71,0xd83,0xd95,
  0xda7,0xdb9,0xdca,0xddc,0xded,0xdfd,0xe0e,0xe1e,
  0xe2e,0xe3e,0xe4e,0xe5d,0xe6c,0xe7b,0xe8a,0xe98,
  0xea6,0xeb4,0xec1,0xecf,0xedc,0xee8,0xef5,0xf01,
  0xf0d,0xf19,0xf24,0xf30,0xf3a,0xf45,0xf4f,0xf59,
  0xf63,0xf6d,0xf76,0xf7f,0xf87,0xf90,0xf98,0xf9f,
  0xfa7,0xfae,0xfb5,0xfbb,0xfc2,0xfc8,0xfcd,0xfd3,
  0xfd8,0xfdc,0xfe1,0xfe5,0xfe9,0xfec,0xff0,0xff3,
  0xff5,0xff7,0xff9,0xffb,0xffd,0xffe,0xffe,0xfff,
  0xfff,0xfff,0xffe,0xffe,0xffd,0xffb,0xff9,0xff7,
  0xff5,0xff3,0xff0,0xfec,0xfe9,0xfe5,0xfe1,0xfdc,
  0xfd8,0xfd3,0xfcd,0xfc8,0xfc2,0xfbb,0xfb5,0xfae,
  0xfa7,0xf9f,0xf98,0xf90,0xf87,0xf7f,0xf76,0xf6d,
  0xf63,0xf59,0xf4f,0xf45,0xf3a,0xf30,0xf24,0xf19,
  0xf0d,0xf01,0xef5,0xee8,0xedc,0xecf,0xec1,0xeb4,
  0xea6,0xe98,0xe8a,0xe7b,0xe6c,0xe5d,0xe4e,0xe3e,
  0xe2e,0xe1e,0xe0e,0xdfd,0xded,0xddc,0xdca,0xdb9,
  0xda7,0xd95,0xd83,0xd71,0xd5f,0xd4c,0xd39,0xd26,
  0xd12,0xcff,0xceb,0xcd7,0xcc3,0xcaf,0xc9a,0xc86,
  0xc71,0xc5c,0xc47,0xc32,0xc1c,0xc06,0xbf1,0xbdb,
  0xbc5,0xbae,0xb98,0xb82,0xb6b,0xb54,0xb3d,0xb26,
  0xb0f,0xaf8,0xae0,0xac9,0xab1,0xa9a,0xa82,0xa6a,
  0xa52,0xa3a,0xa22,0xa09,0x9f1,0x9d9,0x9c0,0x9a8,
  0x98f,0x976,0x95e,0x945,0x92c,0x913,0x8fa,0x8e1,
  0x8c8,0x8af,0x896,0x87d,0x864,0x84b,0x832,0x819,
  0x800,0x7e6,0x7cd,0x7b4,0x79b,0x782,0x769,0x750,
  0x737,0x71e,0x705,0x6ec,0x6d3,0x6ba,0x6a1,0x689,
  0x670,0x657,0x63f,0x626,0x60e,0x5f6,0x5dd,0x5c5,
  0x5ad,0x595,0x57d,0x565,0x54e,0x536,0x51f,0x507,
  0x4f0,0x4d9,0x4c2,0x4ab,0x494,0x47d,0x467,0x451,
  0x43a,0x424,0x40e,0x3f9,0x3e3,0x3cd,0x3b8,0x3a3,
  0x38e,0x379,0x365,0x350,0x33c,0x328,0x314,0x300,
  0x2ed,0x2d9,0x2c6,0x2b3,0x2a0,0x28e,0x27c,0x26a,
  0x258,0x246,0x235,0x223,0x212,0x202,0x1f1,0x1e1,
  0x1d1,0x1c1,0x1b1,0x1a2,0x193,0x184,0x175,0x167,
  0x159,0x14b,0x13e,0x130,0x123,0x117,0x10a,0xfe,
  0xf2,0xe6,0xdb,0xcf,0xc5,0xba,0xb0,0xa6,
  0x9c,0x92,0x89,0x80,0x78,0x6f,0x67,0x60,
  0x58,0x51,0x4a,0x44,0x3d,0x37,0x32,0x2c,
  0x27,0x23,0x1e,0x1a,0x16,0x13,0xf,0xc,
  0xa,0x8,0x6,0x4,0x2,0x1,0x1,0x0,
  0x0,0x0,0x1,0x1,0x2,0x4,0x6,0x8,
  0xa,0xc,0xf,0x13,0x16,0x1a,0x1e,0x23,
  0x27,0x2c,0x32,0x37,0x3d,0x44,0x4a,0x51,
  0x58,0x60,0x67,0x6f,0x78,0x80,0x89,0x92,
  0x9c,0xa6,0xb0,0xba,0xc5,0xcf,0xdb,0xe6,
  0xf2,0xfe,0x10a,0x117,0x123,0x130,0x13e,0x14b,
  0x159,0x167,0x175,0x184,0x193,0x1a2,0x1b1,0x1c1,
  0x1d1,0x1e1,0x1f1,0x202,0x212,0x223,0x235,0x246,
  0x258,0x26a,0x27c,0x28e,0x2a0,0x2b3,0x2c6,0x2d9,
  0x2ed,0x300,0x314,0x328,0x33c,0x350,0x365,0x379,
  0x38e,0x3a3,0x3b8,0x3cd,0x3e3,0x3f9,0x40e,0x424,
  0x43a,0x451,0x467,0x47d,0x494,0x4ab,0x4c2,0x4d9,
  0x4f0,0x507,0x51f,0x536,0x54e,0x565,0x57d,0x595,
  0x5ad,0x5c5,0x5dd,0x5f6,0x60e,0x626,0x63f,0x657,
  0x670,0x689,0x6a1,0x6ba,0x6d3,0x6ec,0x705,0x71e,
  0x737,0x750,0x769,0x782,0x79b,0x7b4,0x7cd,0x7e6,
};

volatile uint32_t acc=0, m=0;
float f=0;
bool running = false;

enum Mode {single_frequency, sweep, sweep_n_sinusoids, none};


// ### Configuration ###
Mode current_mode = sweep;
float n_sinusoids = 10;
float f1 = 0.5;
float f2 = 121;
// ### End of configuration ###

// ### Addresses for variables stored in the eeprom
int freqAddr = 0;
int autostartAddr = freqAddr + sizeof(float);
int modeAddr = autostartAddr + 1;

IntervalTimer timer0;

float cor=1; // cor = measured frequency (with frequency counter) divided by desired frequency

const int pausePin = 0;
const int ledPin = 13;
const int resetPin = 1;

uint32_t freq(float f)
{
  return f * pow(2, 32) / (250000 * cor);
}

void stop()
{
  acc=0;
  m=0;
}

void setup() {
  Serial.begin(9600);
  SIM_SCGC6 |= SIM_SCGC6_DAC0;
  DAC0_C0 = DAC_C0_DACEN | DAC_C0_DACRFS;

  timer0.begin(clk, 4); // 4 usec -> f = 250 kHz
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  f = loadFrequency();
  if(loadAutostart())
    running = true;
  current_mode = loadMode();
}

void clk()
{
  *(int16_t *)&(DAC0_DAT0L) = sineTable[acc>>23];
  acc+=m;
}

void checkPauseButton(float currentFrenquency)
{
  if(digitalRead(pausePin) == HIGH) // HIGH means that the jumper is removed so we want to pause
    saveFrequency(currentFrenquency);
  while(digitalRead(pausePin) == HIGH)
    delay(100);
}

void ledOn()
{
  digitalWrite(ledPin, HIGH);
}

void ledOff()
{
  digitalWrite(ledPin, LOW);
}

void saveFrequency(float freq)
{
  uint8_t *ptr;
  ptr = (uint8_t *)&freq;
  for(unsigned int i=0;i<sizeof(float);i++)
    EEPROM.write(i, ptr[i]);
}

float loadFrequency()
{
  float ret=0;
  uint8_t *ptr = (uint8_t *)&ret;
  for(unsigned int i=0;i<sizeof(float);i++)
    ptr[i] = EEPROM.read(i);
  return ret;
}

void resetFrequency()
{
    saveFrequency(f1);
    f=f1;
    ledOn();
    delay(1000);
    ledOff();
    delay(1000);
    ledOn();
    delay(1000);
    ledOff();
}

bool loadAutostart()
{
  return EEPROM.read(autostartAddr) == 1 ? true : false;
}

void saveAutostart(bool val)
{
  if(val==true)
    EEPROM.write(autostartAddr, 1);
  else
    EEPROM.write(autostartAddr, 0);
}

Mode loadMode()
{
  return (Mode)EEPROM.read(modeAddr);
}

void saveMode(Mode ms)
{
  EEPROM.write(modeAddr, (byte)ms);
}

void displayCurrentMode()
{
  switch(current_mode) {
    case single_frequency:
      Serial.print("single_frequency");
      break;
    case sweep:
      Serial.print("sweep");
      break;
    case sweep_n_sinusoids:
      Serial.print("sweep_n_sinusoids");
      break;
    case none:
      Serial.print("none");
      break;
    default:
      Serial.print("invalid");
      break;
  }
}

void prompt()
{
  Serial.print("> ");
}

void console()
{
  enum State {Menu_S, Prompt_S, Input_S, Autostart_S, AutostartInput_S, Mode_S, ModeInput_S, Reset_S, Start_S, Stop_S};
  static State state = Menu_S;
  char input = 0;

  switch(state) {
    case Menu_S:
      Serial.println("### Menu ###");
      Serial.print("1- Autostart : ");
      Serial.println(loadAutostart() ? "On" : "Off");
      Serial.print("2- Mode (");
      displayCurrentMode();
      Serial.println(")");
      Serial.print("3- Reset saved frequency (f=");
      Serial.print(loadFrequency());
      Serial.println(" Hz)");
      Serial.println("4- Start");
      Serial.println("5- Stop");
      Serial.print("Current frequency : ");
      Serial.print(f);
      Serial.println(" Hz");
      state=Prompt_S;
      break;

    case Prompt_S:
      prompt();
      state=Input_S;
      break;
      
    case Input_S:
      if(digitalRead(resetPin)==LOW) {
        state=Reset_S;
        break;
      }
      if(Serial.available()>0) {
        input = Serial.read();
        Serial.println(input);
        switch(input) {
          case '1':
            state=Autostart_S;
            break;
          case '2':
            state=Mode_S;
            break;
          case '3':
            state=Reset_S;
            break;
          case '4':
            state=Start_S;
            break;
          case '5':
            state=Stop_S;    
            break;
          case 'm':
            state=Menu_S;
            break;
          default:
            Serial.println("Invalid choice");
            state=Menu_S;
            break;
        }
          
      }
      break;
      
    case Autostart_S:
      Serial.println("### Autostart ###");
      Serial.println("1- On");
      Serial.println("2- Off");
      prompt();
      state=AutostartInput_S;
      break;

    case AutostartInput_S:
      if(Serial.available()>0) {
        input = Serial.read();
        Serial.println(input);
        switch(input) {
          case '1':
            Serial.println("Setting autostart to On");
            saveAutostart(true);
            state=Menu_S;
            break;
          case '2':
            Serial.println("Setting autostart to off");
            saveAutostart(false);
            state=Menu_S;
            break;
          default:
            Serial.println("Invalid choice");
            state=Autostart_S;
            break;
        }          
      }
      break;

    case Mode_S:
      Serial.println("### Mode ###");
      Serial.println("1- Single frequency");
      Serial.println("2- Sweep");
      Serial.print("3- Sweep with ");
      Serial.print(n_sinusoids);
      Serial.println(" sinusoids");
      prompt();
      state=ModeInput_S;
      break;

    case ModeInput_S:
      if(Serial.available()>0) {
        input = Serial.read();
        Serial.println(input);
        switch(input) {
          case '1':
            Serial.println("Setting single frequency mode");
            current_mode = single_frequency;
            saveMode(current_mode);
            state=Menu_S;
            break;
          case '2':
            Serial.println("Setting sweep mode");
            current_mode = sweep;
            saveMode(current_mode);
            state=Menu_S;
            break;
          case '3':
            Serial.println("Setting sweep with n sinudoids mode");
            current_mode = sweep_n_sinusoids;
            saveMode(current_mode);
            state=Menu_S;
            break;
          default:
            Serial.println("Invalid choice");
            state=Mode_S;
            break;
        }          
      }
      break;
      
    case Reset_S:
      Serial.println("Resetting frequency");
      resetFrequency();
      state=Menu_S;
      break;

    case Start_S:
      if(running ==true)
        Serial.println("Generator is already running.");
      else {
        Serial.println("Starting the generator.");
        running=true;
      }
      state=Menu_S;
      break;

    case Stop_S:
      if(running == false)
        Serial.println("Generator is already stopped.");
      else {
        Serial.println("Saving frequency.");
        saveFrequency(f);
        stop();
        Serial.println("Stopping the generator");
        running=false;
      }
      state=Menu_S;
      break;

    default:
      state=Menu_S;
      break;
      
  }
}

void loop() {

  console();

  if(running == false)
    return;

  if(current_mode == none) {
    ledOn();
    stop();
    return;
  }

  if(current_mode == single_frequency) {
    m = freq(121);
    ledOn();
  }
  else if(current_mode == sweep) {
    for(int i=0;i<5;i++) {
      if(f>f2)
        current_mode = none;
      m=freq(f);
      ledOn();
      delay(400);
      stop();
      ledOff();
      delay(600); 
    }
    f+=0.01;
  }
  else if(current_mode == sweep_n_sinusoids) {
    for(int i=0;i<5;i++) {
      if(f>f2)
        current_mode = none;
      m=freq(f);
      ledOn();
      delay(n_sinusoids*1000/f);
      stop();
      ledOff();
      delay(n_sinusoids*1000/f);
    }
    f+=0.01;
  }
  


}



