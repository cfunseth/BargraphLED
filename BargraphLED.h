/**********************************************************************
 *
 * This is the library for the BL28 LED bargraph module
 * 
 * Original HT16K33 source code: https://github.com/jonpearse/ht16k33-arduino
 *
 **********************************************************************/
#ifndef BargraphLED_h
  #define BargraphLED_h
  
  // include appropriate version of Arduino code
  #if (ARDUINO >= 100)
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  
  // include Wire for I2C comms  
  #include <Wire.h>
  
  // different commands
  #define HT16K33_CMD_RAM     0x00
  #define HT16K33_CMD_KEYS    0x40
  #define HT16K33_CMD_SETUP   0x80
  #define HT16K33_CMD_ROWINT  0xA0
  #define HT16K33_CMD_DIMMING 0xE0
  
  // other options
  #define HT16K33_DISPLAY_OFF 0x00
  #define HT16K33_DISPLAY_ON  0x01
  #define HT16K33_BLINK_OFF   0x00
  #define HT16K33_BLINK_1HZ   0x02
  #define HT16K33_BLINK_2HZ   0x04
  #define HT16K33_BLINK_0HZ5  0x06
  
  // bargraph options
  #define BARGRAPH_SIZE       28
  
  // actual class
  class BargraphLED
  {
    public:
      void init(uint8_t addr);
      
      // brightness control
      void setBrightness(uint8_t brightness);
      
      // blink controls
      void setBlink(uint8_t blink);
      
      // buffer stuff
      void clear(void);
      void setPixel(uint8_t row, uint8_t col, uint8_t onff);
      
      // read/write
      void write(void);
	  
      // bargraph functions
      void setSegment(uint8_t pos, uint8_t value);
	    uint8_t size(void);
	        
    private:
      uint16_t *_buffer;
      uint8_t  _i2c_addr;
      
      void writeRow(uint8_t col);
  };
  
#endif
