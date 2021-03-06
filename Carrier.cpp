/*
   Carrier
   Version 0.04 January, 2015
   Copyright 2011 Bob Fisch
   Version 0.1 Juli, 2018 - Michiel Roding

   Library to generate IR codes for a Carrier air conditionner.

   For details, see http://arduino.fisch.lu
*/

#include "Carrier.h"

/**
   set the given temperature and updates the codes
   @param  _value  the new value [17..32]
*/
void Carrier::setTemperature(uint8_t _value )
{
  temperature = _value;

  if (temperature & 32) {
    codes[39] = CODE_high;
  } else {
    codes[39] = CODE_low;
  }

  if (temperature & 16) {
    codes[41] = CODE_high;
  } else {
    codes[41] = CODE_low;
  }

  if (temperature & 8) {
    codes[43] = CODE_high;
  } else {
    codes[43] = CODE_low;
  }

  if (temperature & 4) {
    codes[45] = CODE_high;
  } else {
    codes[45] = CODE_low;
  }

  if (temperature & 2) {
    codes[47] = CODE_high;
  } else {
    codes[47] = CODE_low;
  }

  if (temperature & 1) {
    codes[49] = CODE_high;
  } else {
    codes[49] = CODE_low;
  }

  setChecksum();
}

void Carrier::restoreTemperatureFromCodes()
{
  temperature = 0;

  if (codes[39] == CODE_high) {
    temperature += 32;
  }

  if (codes[41] == CODE_high) {
    temperature += 16;
  }

  if (codes[43] == CODE_high) {
    temperature += 8;
  }

  if (codes[45] == CODE_high) {
    temperature += 4;
  }

  if (codes[47] == CODE_high) {
    temperature += 2;
  }

  if (codes[49] == CODE_high) {
    temperature += 1;
  }
}

bool Carrier::verifyCodes(int* _codes )
{
  return true;
}

/**
   set the given mode and updates the codes
   @param _value  the new mode {MODE_auto,MODE_cold,MODE_warm,MODE_wind,MODE_rain}
*/
void Carrier::setMode(uint8_t _value )
{
  mode = _value;

  if (mode & 4) {
    codes[19] = CODE_high;
  } else {
    codes[19] = CODE_low;
  }

  if (mode & 2) {
    codes[21] = CODE_high;
  } else {
    codes[21] = CODE_low;
  }

  if (mode & 1) {
    codes[23] = CODE_high;
  } else {
    codes[23] = CODE_low;
  }

  setChecksum();
}

void Carrier::restoreModeFromCodes()
{
  mode = 0;

  if (codes[19] == CODE_high) {
    mode += 4;
  }

  if (codes[21] == CODE_high) {
    mode += 2;
  }

  if (codes[23] == CODE_high) {
    mode += 1;
  }
}

/**
   set the given fan and updates the codes
   @param _value  the new fan speed {FAN_1,FAN_2,FAN_3,FAN_4}
*/
void Carrier::setFan(uint8_t _value )
{
  fan = _value;

  if (fan & 2) {
    codes[25] = CODE_high;
  } else {
    codes[25] = CODE_low;
  }

  if (fan & 1) {
    codes[27] = CODE_high;
  } else {
    codes[27] = CODE_low;
  }

  setChecksum();
}

void Carrier::restoreFanFromCodes()
{
  fan = 1;

  if (codes[25] == CODE_high) {
    fan += 2;
  }

  if (codes[27] == CODE_high) {
    fan += 1;
  }
}

/**
   set the given air flow and updates the codes
   @param _value  the new air flow direction {AIRFLOW_dir_1,AIRFLOW_dir_2,AIRFLOW_dir_3,AIRFLOW_dir_4,AIRFLOW_dir_5,AIRFLOW_dir_6,AIRFLOW_change,AIRFLOW_open}
*/
void Carrier::setAirFlow(uint8_t _value )
{
  airFlow = _value;

  if (airFlow & 4) {
    codes[29] = CODE_high;
  } else {
    codes[29] = CODE_low;
  }

  if (airFlow & 2) {
    codes[31] = CODE_high;
  } else {
    codes[31] = CODE_low;
  }

  if (airFlow & 1) {
    codes[33] = CODE_high;
  } else {
    codes[33] = CODE_low;
  }

  setChecksum();
}

void Carrier::restoreAirFlowFromCodes()
{
  airFlow = 1;

  if (codes[29] == CODE_high) {
    airFlow += 4;
  }

  if (codes[31] == CODE_high) {
    airFlow += 2;
  }

  if (codes[33] == CODE_high) {
    airFlow += 1;
  }
}

/**
   set the given state and updates the codes
   @param _value  the new state {STATE_on,STATE_off}
*/
void Carrier::setState( uint8_t _value )
{
  state = _value;

  if (state & 1) {
    codes[51] = CODE_high;
  } else {
    codes[51] = CODE_low;
  }

  setChecksum();
}

void Carrier::restoreStateFromCodes()
{
  state = 0;

  if (codes[51] == CODE_high) {
    state += 1;
  }
}

void Carrier::restoreCounterFromCodes()
{
  counter = 0;

  if (codes[7] == CODE_high) {
    counter += 2;
  }

  if (codes[9] == CODE_high) {
    counter += 1;
  }

  counter--;
}

void Carrier::restoreFromCodes()
{
  restoreTemperatureFromCodes();
  restoreModeFromCodes();
  restoreFanFromCodes();
  restoreAirFlowFromCodes();
  restoreStateFromCodes();
  restoreCounterFromCodes();

  setChecksum();
}

uint8_t Carrier::getTemperature()
{
  return temperature;
}

uint8_t Carrier::getMode()
{
  return mode;
}

uint8_t Carrier::getAirFlow()
{
  return airFlow;
}

uint8_t Carrier::getState()
{
  return state;
}

uint8_t Carrier::getFan()
{
  return fan;
}

/**
   increments the counter, calculates the checksum and updates the codes
*/
void Carrier::setChecksum()
{
  // first do the counter part
  counter =(counter + 1)% 4;

  if (counter & 2) {
    codes[7] = CODE_high;
  } else {
    codes[7] = CODE_low;
  }

  if (counter & 1) {
    codes[9] = CODE_high;
  } else {
    codes[9] = CODE_low;
  }

  // next generate the checksum
  uint16_t crc = 0;

  for(uint8_t b = 0; b < 4; b++) {
    uint8_t block = 0;

    for(uint8_t i = 3 + b * 16; i <= 17 + b * 16; i = i + 2) {
      if (codes[i] > CODE_threshold) {
        block++;
      }

      if (i <= 15 + b * 16) {
        block <<= 1;
      }
    }

    crc =(crc + block)% 256;
  }

  if (crc & 128) {
    codes[67] = CODE_high;
  } else {
    codes[67] = CODE_low;
  }

  if (crc & 64) {
    codes[69] = CODE_high;
  } else {
    codes[69] = CODE_low;
  }

  if (crc & 32) {
    codes[71] = CODE_high;
  } else {
    codes[71] = CODE_low;
  }

  if (crc & 16) {
    codes[73] = CODE_high;
  } else {
    codes[73] = CODE_low;
  }

  if (crc & 8) {
    codes[75] = CODE_high;
  } else {
    codes[75] = CODE_low;
  }

  if (crc & 4) {
    codes[77] = CODE_high;
  } else {
    codes[77] = CODE_low;
  }

  if (crc & 2) {
    codes[79] = CODE_high;
  } else {
    codes[79] = CODE_low;
  }

  if (crc & 1) {
    codes[81] = CODE_high;
  } else {
    codes[81] = CODE_low;
  }
}

void Carrier::restoreFillers()
{
  // init the codes with the fillers and default values
  for (uint8_t i = 0; i < CARRIER_BUFFER_SIZE; i++) {
    if (i % 2 == 0) {
      codes[i] = CODE_filler;
    }
  }

  // set the two first ones
  codes[0] = CODE_first;
  codes[1] = CODE_second;
  // set the "always high" fields
  codes[3] = CODE_high;
  codes[5] = CODE_high;
  codes[53] = CODE_high;
}

/**
   initialiases a new code object
*/
Carrier::Carrier(uint8_t _mode,
                   uint8_t _fan,
                   uint8_t _airFlow,
                   uint8_t _temperature,
                   uint8_t _state )
{
  // init the codes with the fillers and default values
  for (uint8_t i = 0; i < CARRIER_BUFFER_SIZE; i++) {
    codes[i] = CODE_low;
  }

  restoreFillers();

  // init the counter
  counter = 0;

  // set fields
  setMode(_mode );
  setFan(_fan );
  setAirFlow(_airFlow );
  setTemperature(_temperature );
  setState(_state );
}

/**
   prints the actul state of the object on the serial line
*/
void Carrier::print()
{
  Serial.print("State = " );

  if (state == STATE_on) {
    Serial.println("on" );
  } else {
    Serial.println("off" );
  }

  Serial.print("Mode = " );

  if (mode == MODE_auto) {
    Serial.println("auto" );
  } else if (mode == MODE_rain) {
    Serial.println("rain" );
  } else if (mode == MODE_cold) {
    Serial.println("cold" );
  } else if (mode == MODE_wind) {
    Serial.println("wind" );
  } else {
    Serial.println("warm" );
  }

  Serial.print("Temperature = " );
  Serial.println(temperature, DEC );

  Serial.print("Fan = " );
  Serial.println(fan, DEC );

  Serial.print("Air-flow = " );
  Serial.println(airFlow, DEC );
}
