/**********************************************************************
 *
 * This is the library for the BL28 LED bargraph module
 * 
 * Original HT16K33 source code: https://github.com/jonpearse/ht16k33-arduino
 *
 **********************************************************************/
#include <Wire.h>
#include "BargraphLED.h"

// Constructor
void BargraphLED::init(uint8_t addr)
{  
  // set the I2C address
  _i2c_addr = addr;
  
  // _buffer is an array of (8) COM registers, each register is a 16 bit value that represents (16) ROW values
  // see page 16 of HT16K33 datasheet for reference
  _buffer = (uint16_t*)calloc(8, sizeof(uint16_t));
  
  // start everything
  Wire.begin();
  Wire.beginTransmission(_i2c_addr);
  Wire.write(0x21); // turn on oscillator
  Wire.write(HT16K33_CMD_SETUP | HT16K33_DISPLAY_ON); // turn on display
  Wire.endTransmission();
  
  // set blink off + brightness all the way up
  setBlink(HT16K33_BLINK_OFF);
  setBrightness(15);
  
  // write the matrix, just in case
  write();
}

/**
 * Sets the brightness of the display.
 */
void BargraphLED::setBrightness(uint8_t brightness)
{
  // constrain the brightness to a 4-bit number (0–15)
  brightness = brightness & 0x0F;
  
  // send the command
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_DIMMING | brightness);
  Wire.endTransmission();
}

/**
 * Set the blink rate.
 */
void BargraphLED::setBlink(uint8_t blink)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_SETUP | HT16K33_DISPLAY_ON | blink);
  Wire.endTransmission();
}

/**
 * Clears the display buffer. Note that this doesn’t clear the display—you’ll need to call write() to do this.
 */
void BargraphLED::clear(void)
{  
  for (uint8_t i = 0; i < 8; i++)
  {
    _buffer[i] = 0;
  }  
}

/**
 * Sets the value of a particular pixel.
 */
void BargraphLED::setPixel(uint8_t col, uint8_t row, uint8_t val)
{
  // bounds checking
  col = col & 0x07;
  row = row & 0x0F;
  val = val & 0x01;
  
  // write the buffer
  if (val == 1)
  {
    _buffer[col] |= 1 << row;
  }
  else
  {
    _buffer[col] &= ~(1 << row);
  }

}

/**
 * Write the RAM buffer to the matrix.
 */
void BargraphLED::write(void)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_RAM);
  
  for (uint8_t col = 0; col < 8; col++)
  {
    writeRow(col);
  }
  
  Wire.endTransmission();
}

/**
 * Write a row to the chip.
 */
void BargraphLED::writeRow(uint8_t col)
{  
  Wire.write(_buffer[col] & 0xFF); // first byte
  Wire.write(_buffer[col] >> 8); // second byte
}

/**
* Sets the bargraph segment (0 through 27) to a value (0 or 1)
*/
void BargraphLED::setSegment(uint8_t pos, uint8_t val)
{
  uint8_t row, col;
  
  val &= 0x01;	//constrain val to be either 0 or 1
  
  // verify the position isn't greater than the bargraph size
  if(pos > (BARGRAPH_SIZE - 1) ) pos = BARGRAPH_SIZE - 1;
  
  row = pos / 4;
  col = pos % 4;
	
  setPixel(col, row, val);
}

/**
* Get the quantity of segments in the bargraph
*/
uint8_t BargraphLED::size(void)
{
  return BARGRAPH_SIZE;
}
