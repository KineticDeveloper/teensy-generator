
#include "sineTable.h"
#include "memory.h"
#include "mode.h"
#include "misc.h"
#include "config.h"



volatile uint32_t acc=0, m=0;
float f=0;
bool running = false;
int to=0;
int tf=0;
int repetitions=0;
float n_sinusoids = 0;
Mode current_mode = sweep;

IntervalTimer timer0;

float cor=1; // cor = measured frequency (with frequency counter) divided by desired frequency



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

  to = loadTo();
  tf = loadTf();
  repetitions=loadRepetitions();
  n_sinusoids=loadNSinusoids();
}

void clk()
{
  *(int16_t *)&(DAC0_DAT0L) = sineTable[acc>>23];
  acc+=m;
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
    default:
      Serial.print("invalid");
      break;
  }
}



void prompt()
{
  Serial.print("> ");
}

bool availableInput()
{
  if(Serial.available() == 0)
    return false;
  if(Serial.peek() == '\r') {
    Serial.read();
    return false;
  }
  if(Serial.peek() == '\n') {
    Serial.read();
    return false;
  }
  return true;
}

int waitInt(String name)
{
  int ret;
  Serial.print(name);
  Serial.print(" = ");
  while(availableInput()==false);
  ret = Serial.parseInt();
  Serial.println(ret);
  return ret;
}

int waitFloat(String name)
{
  float ret;
  Serial.print(name);
  Serial.print(" = ");
  while(availableInput()==false);
  ret = Serial.parseFloat();
  Serial.println(ret);
  return ret;
}


void console()
{
  enum State {Menu_S, Prompt_S, Input_S, Autostart_S, AutostartInput_S, Mode_S, ModeInput_S, Reset_S, Start_S, Stop_S, Save_S, SetCurrentFrequency_S, CurrentFrequencyInput_S, JumperRemoved_S, WaitForJumper_S, Shutdown_S};
  static State state = Menu_S;
  char input = 0;

  switch(state) {
    case Menu_S:
      Serial.println("### EEPROM ###");
      Serial.print("* Autostart = ");
      Serial.println(loadAutostart() ? "On" : "Off");
      Serial.print("* Mode = ");
      displayCurrentMode();
      Serial.println("");
      Serial.print("* frequency = ");
      Serial.print(loadFrequency());
      Serial.println(" Hz");
      Serial.print("* to = ");
      Serial.print(loadTo());
      Serial.println(" ms");
      Serial.print("* tf = ");
      Serial.print(loadTf());
      Serial.println(" ms");
      Serial.print("* repetitions = ");
      Serial.println(loadRepetitions());
      Serial.print("* n_sinusoids = ");
      Serial.println(loadNSinusoids());
      Serial.println("### RAM ###");
      Serial.print("f = ");
      Serial.print(f);
      Serial.println(" Hz");
      Serial.print("running = ");
      Serial.println(running ? "true" : "false");
      Serial.print("m = 0x");
      Serial.println(m, HEX);
      Serial.print("acc = 0x");
      Serial.println(acc, HEX);
      Serial.println("### Menu ###");
      Serial.println("1- Autostart");
      Serial.println("2- Mode");
      Serial.print("3- Reset saved frequency to ");
      Serial.print(f1);
      Serial.println(" Hz");
      Serial.println("4- Set current frequency");
      Serial.println("5- Start");
      Serial.println("6- Stop");
      Serial.println("7- Save current frequency");
      Serial.println("8- Shutdown");

      
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
      if(digitalRead(pausePin)==HIGH) {
        state=JumperRemoved_S;
        break;
      }
      if(availableInput()) {
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
            state=SetCurrentFrequency_S;
            break;
          case '5':
            state=Start_S;
            break;
          case '6':
            state=Stop_S;    
            break;
          case '7':
            state=Save_S;
            break;
          case '8':
            state=Shutdown_S;
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
      if(availableInput()) {
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
      Serial.println("3- Sweep with n sinusoids");
      prompt();
      state=ModeInput_S;
      break;

    case ModeInput_S:
      if(availableInput()) {
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
            saveTo(waitInt("to"));
            saveTf(waitInt("tf"));
            saveRepetitions(waitInt("repetitions"));
            current_mode = sweep;
            saveMode(current_mode);
            state=Menu_S;
            break;
          case '3':
            Serial.println("Setting sweep with n sinudoids mode");
            saveNSinusoids(waitFloat("n_sinudoids"));
            saveRepetitions(waitInt("repetitions"));
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
        stop();
        running=false;
        Serial.println("Stopping the generator");
      }
      state=Menu_S;
      break;

    case Save_S:
      Serial.println("Saving frequency.");
      saveFrequency(f);
      state=Menu_S;
      break;

    case SetCurrentFrequency_S:
      Serial.println("### Frequency ###");
      prompt();
      state=CurrentFrequencyInput_S;
      break;

    case CurrentFrequencyInput_S:
      if(availableInput()) {
        f=Serial.parseFloat();
        Serial.println(f);
        Serial.print("Setting current frequency to ");
        Serial.print(f);
        Serial.println(" Hz");
         state=Menu_S;
      }
      break;

    case JumperRemoved_S:
      Serial.println("Jumper removed");
      Serial.println("Saving frequency");
      saveFrequency(f);
      running=false;
      state=WaitForJumper_S;
      break;

    case WaitForJumper_S:
      if(digitalRead(pausePin)==LOW) {
        running=true;
        state=Menu_S;
        break;
      }
      if(availableInput()) {
        Serial.read();
        Serial.println("Please put the jumper back.");
      };
      break;

    case Shutdown_S:
      running=false;
      stop();
      Serial.println("Saving frequency");
      saveFrequency(f);
      ledOff();
      Serial.println("Generator shut down.");
      while(1);
      break;

    default:
      state=Menu_S;
      break;
      
  }
}

void loop() {

  console();

  if(running == false) {
    ledOff();
    return;
  }

  if(current_mode == single_frequency) {
    m = freq(f);
    ledOn();
  }
  else if(current_mode == sweep) {
    if(f<=f2) {
      for(int i=0;i<repetitions;i++) {
        m=freq(f);
        ledOn();
        delay(to);
        stop();
        ledOff();
        delay(tf); 
      }
      f+=0.01;
    }
  }
  else if(current_mode == sweep_n_sinusoids) {
    if(f<=f2) {
      for(int i=0;i<repetitions;i++) {
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
  


}



