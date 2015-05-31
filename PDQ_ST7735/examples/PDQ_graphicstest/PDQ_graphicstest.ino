// This is part of the PDQ re-mixed version of Adafruit's GFX library
// and associated chipset drivers.
// Here is the libraries original copyright notice:

/***************************************************
  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>				// must include this here (or else IDE can't find it)

#include <PDQ_GFX.h>			// PDQ: Core graphics library
#include "PDQ_ST7735_config.h"	// PDQ: ST7735 pins and other setup for this sketch
#include <PDQ_ST7735.h>			// PDQ: Hardware-specific driver library
PDQ_ST7735 tft;					// PDQ: create LCD object (using pins in "PDQ_ST7735_config.h")

// NOTE: Changes to test with Adafruit libraries (comment out PDQ lines above and un-comment the AF: ones below)

// AF: #include <Adafruit_GFX.h>	// AF: Core graphics library
// AF: #include <Adafruit_ST7735.h>	// AF: Hardware-specific library
// AF: Adafruit_ST7735 tft = Adafruit_ST7735(10,  9, 0); // AF: create LCD object (HW SPI, CS=pin 10, D/C=pin 9, reset=Arduino reset)
// AF: #define color565 Color565	// AF: use consistent naming (same as other Adafruit drivers) needed for Adafruit libs

// These are used to get information about static SRAM and flash memory sizes
extern "C" char __data_start[];		// start of SRAM data
extern "C" char _end[];				// end of SRAM data (used to check amount of SRAM this program's variables use)
extern "C" char __data_load_end[];	// end of FLASH (used to check amount of Flash this program's code and data uses)
void setup()
{
	Serial.begin(115200);
	while (!Serial);

// AF:	tft.initR(INITR_18BLACKTAB);	// AF: initialize SPI/LCD

#if defined(_PDQ_ST7735H_)
	Serial.println(F("PDQ ST7735 1.8\" SPI TFT Test!     ")); 
#else
 	Serial.println(F("Adafruit ST7735 1.8\" SPI TFT Test!")); 
#endif
 
#if defined(ST7735_RST_PIN)				// reset like Adafruit does
	FastPin<ST7735_RST_PIN>::setOutput();
	FastPin<ST7735_RST_PIN>::hi();
	FastPin<ST7735_RST_PIN>::lo();
	delay(1);
	FastPin<ST7735_RST_PIN>::hi();
#endif

	tft.begin();						// initialize LCD
}

// NOTE: This demo should run, but may look odd on 128x128 LCD (vs 128x160)

void loop(void)
{
	Serial.print(F(__DATE__ " " __TIME__ " - Flash=0x"));
	Serial.print((uint16_t)__data_load_end, HEX);
	Serial.print(F(" RAM=0x"));
	Serial.println((uint16_t)_end - (uint16_t)__data_start, HEX);
	Serial.println(F("Benchmark                Time (microseconds)"));

	uint32_t usecHaD = testHaD();
	Serial.print(F("HaD pushColor             "));
	Serial.println(usecHaD);
	delay(100);

	uint32_t usecFillScreen = testFillScreen();
	Serial.print(F("Screen fill              "));
	Serial.println(usecFillScreen);
	delay(100);

	uint32_t usecText = testText();
	Serial.print(F("Text                     "));
	Serial.println(usecText);
	delay(100);

	uint32_t usecPixels = testPixels();
	Serial.print(F("Pixels                   "));
	Serial.println(usecPixels);
	delay(100);

	uint32_t usecLines = testLines(ST7735_BLUE);
	Serial.print(F("Lines                    "));
	Serial.println(usecLines);
	delay(100);

	uint32_t usecFastLines = testFastLines(ST7735_RED, ST7735_BLUE);
	Serial.print(F("Horiz/Vert Lines         "));
	Serial.println(usecFastLines);
	delay(100);

	uint32_t usecRects = testRects(ST7735_GREEN);
	Serial.print(F("Rectangles (outline)     "));
	Serial.println(usecRects);
	delay(100);

	uint32_t usecFilledRects = testFilledRects(ST7735_YELLOW, ST7735_MAGENTA);
	Serial.print(F("Rectangles (filled)      "));
	Serial.println(usecFilledRects);
	delay(100);

	uint32_t usecFilledCircles = testFilledCircles(10, ST7735_MAGENTA);
	Serial.print(F("Circles (filled)         "));
	Serial.println(usecFilledCircles);
	delay(100);

	uint32_t usecCircles = testCircles(10, ST7735_WHITE);
	Serial.print(F("Circles (outline)        "));
	Serial.println(usecCircles);
	delay(100);

	uint32_t usecTriangles = testTriangles();
	Serial.print(F("Triangles (outline)      "));
	Serial.println(usecTriangles);
	delay(100);

	uint32_t usecFilledTrangles = testFilledTriangles();
	Serial.print(F("Triangles (filled)       "));
	Serial.println(usecFilledTrangles);
	delay(100);

	uint32_t usecRoundRects = testRoundRects();
	Serial.print(F("Rounded rects (outline)  "));
	Serial.println(usecRoundRects);
	delay(100);

	uint32_t usedFilledRoundRects = testFilledRoundRects();
	Serial.print(F("Rounded rects (filled)   "));
	Serial.println(usedFilledRoundRects);
	delay(100);

	Serial.println(F("Done!"));
	
	uint16_t c = 4;
	int8_t d = 1;
	for (int32_t i = 0; i < tft.height(); i++)
	{
		tft.drawFastHLine(0, i, tft.width(), c);
		c += d;
		if (c <= 4 || c >= 11)
			d = -d;
	}
	
	tft.setCursor(0, 0);
	tft.setTextColor(ST7735_MAGENTA);
#if defined(_PDQ_ST7735H_)
	tft.println(F(" PDQ GFX+PDQ ST7735 "));
#else
 	tft.println(F(" Adafruit GFX+ST7735"));
#endif
	tft.setTextSize(1);
	tft.setTextColor(ST7735_WHITE);
	tft.print(F("SPI LCD on 328p"));
	tft.setTextColor(ST7735_YELLOW);
	tft.print(F("@"));
	tft.setTextColor(ST7735_WHITE);
	tft.print(F("16MHz"));
	tft.setTextSize(1);
	tft.setTextColor(tft.color565(0x80, 0x80, 0x80));
	tft.print(F("Flash=0x"));
	tft.print((uint16_t)__data_load_end, HEX);
	tft.print(F(" RAM=0x"));
	tft.print((uint16_t)_end - (uint16_t)__data_start, HEX);

	tft.setTextColor(ST7735_GREEN);
	tft.print(F("Benchmark        usec"));

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("HaD logo   "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecHaD);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Clear      "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecFillScreen);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Text       "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecText);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Pixels     "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecPixels);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Lines      "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecLines);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("H/V Lines  "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecFastLines);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Rectangles "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecRects);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Rects-Fill "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecFilledRects);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Circles    "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecCircles);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("CircleFill "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecFilledCircles);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Triangles  "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecTriangles);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Tris-Fill  "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecFilledTrangles);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("Rnd-Rects  "));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usecRoundRects);

	tft.setTextColor(ST7735_CYAN);
	tft.print(F("RRects-Fill"));
	tft.setTextColor(ST7735_YELLOW);
	printnice(usedFilledRoundRects);

	tft.setTextSize(1);
	tft.println(F(""));
	tft.setTextColor(ST7735_GREEN);
	tft.print(F("Benchmark Completed!"));

	delay(15 * 1000L);
}

void printnice(int32_t v)
{
	char	str[32] = { 0 };
	sprintf(str, "%lu", v);
	for (char *p = (str+strlen(str))-3; p > str; p -= 3)
	{
		memmove(p+1, p, strlen(p)+1);
		*p = ',';
		
	}
	while (strlen(str) < 10)
	{
		memmove(str+1, str, strlen(str)+1);
		*str = ' ';
	}
	tft.print(str);
}

static inline uint32_t micros_start() __attribute__ ((always_inline));
static inline uint32_t micros_start()
{
	uint8_t oms = millis();
	while ((uint8_t)millis() == oms)
		;
	return micros();
}

uint32_t testHaD()
{
	// pseudo-code for cheesy RLE
	// start with color1
	// while more input data remaining
	// 	count =  0nnnnnnn = 1 byte or 1nnnnnnn nnnnnnnn 2 bytes (0 - 32767)
	// 	repeat color count times
	// 	toggle color1/color2
	static const uint8_t HaD_128x160[] PROGMEM =
	{
		0x85, 0x91, 0x09, 0x4b, 0x09, 0x24, 0x0a, 0x47, 0x09, 0x27, 0x0a, 0x44, 0x0a, 0x29, 0x0a, 0x42, 
		0x0a, 0x2b, 0x0a, 0x41, 0x0a, 0x2c, 0x0a, 0x3e, 0x0b, 0x2f, 0x09, 0x3d, 0x09, 0x32, 0x08, 0x3c, 
		0x09, 0x33, 0x09, 0x3b, 0x08, 0x33, 0x0a, 0x3a, 0x0a, 0x31, 0x0b, 0x3a, 0x0c, 0x1d, 0x01, 0x10, 
		0x0d, 0x39, 0x0c, 0x1d, 0x01, 0x10, 0x0d, 0x39, 0x0d, 0x0f, 0x01, 0x0c, 0x03, 0x0d, 0x0e, 0x39, 
		0x0e, 0x0c, 0x03, 0x0c, 0x04, 0x0b, 0x0f, 0x39, 0x0f, 0x0a, 0x04, 0x0c, 0x05, 0x09, 0x10, 0x39, 
		0x10, 0x08, 0x05, 0x0c, 0x06, 0x07, 0x11, 0x39, 0x11, 0x06, 0x06, 0x0d, 0x07, 0x04, 0x13, 0x37, 
		0x12, 0x05, 0x07, 0x0d, 0x08, 0x02, 0x15, 0x34, 0x15, 0x03, 0x08, 0x0d, 0x20, 0x32, 0x20, 0x0e, 
		0x21, 0x31, 0x20, 0x0f, 0x21, 0x2e, 0x22, 0x10, 0x22, 0x2b, 0x22, 0x12, 0x22, 0x12, 0x05, 0x12, 
		0x22, 0x14, 0x22, 0x0c, 0x0f, 0x0c, 0x22, 0x16, 0x22, 0x08, 0x15, 0x08, 0x22, 0x18, 0x22, 0x05, 
		0x19, 0x05, 0x21, 0x1c, 0x1f, 0x04, 0x1c, 0x05, 0x1f, 0x1f, 0x1c, 0x04, 0x1e, 0x04, 0x1d, 0x2b, 
		0x11, 0x04, 0x21, 0x03, 0x12, 0x36, 0x0f, 0x03, 0x24, 0x03, 0x10, 0x38, 0x0d, 0x03, 0x26, 0x03, 
		0x0d, 0x3b, 0x0b, 0x03, 0x28, 0x03, 0x0b, 0x3d, 0x0a, 0x03, 0x29, 0x03, 0x09, 0x40, 0x07, 0x03, 
		0x2b, 0x03, 0x07, 0x42, 0x05, 0x03, 0x2c, 0x04, 0x04, 0x45, 0x04, 0x03, 0x2d, 0x03, 0x04, 0x46, 
		0x02, 0x03, 0x2e, 0x03, 0x03, 0x48, 0x01, 0x03, 0x2f, 0x03, 0x01, 0x4c, 0x31, 0x4e, 0x32, 0x4e, 
		0x33, 0x4c, 0x34, 0x4c, 0x34, 0x4c, 0x35, 0x4b, 0x35, 0x4a, 0x0e, 0x03, 0x14, 0x04, 0x0d, 0x4a, 
		0x0b, 0x09, 0x0e, 0x0a, 0x0a, 0x4a, 0x0a, 0x0b, 0x0c, 0x0c, 0x09, 0x4a, 0x09, 0x0d, 0x0a, 0x0e, 
		0x09, 0x49, 0x08, 0x0f, 0x09, 0x0e, 0x09, 0x49, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x49, 0x08, 0x0f, 
		0x09, 0x0f, 0x08, 0x49, 0x07, 0x0f, 0x0a, 0x0f, 0x08, 0x49, 0x07, 0x0f, 0x0b, 0x0e, 0x08, 0x49, 
		0x07, 0x0d, 0x0e, 0x0d, 0x08, 0x49, 0x07, 0x0b, 0x13, 0x0a, 0x08, 0x49, 0x08, 0x07, 0x18, 0x08, 
		0x08, 0x49, 0x08, 0x06, 0x1b, 0x06, 0x08, 0x49, 0x09, 0x04, 0x1c, 0x05, 0x08, 0x4a, 0x09, 0x04, 
		0x1d, 0x04, 0x08, 0x4a, 0x0a, 0x03, 0x1d, 0x03, 0x09, 0x4b, 0x19, 0x02, 0x1a, 0x4b, 0x19, 0x03, 
		0x19, 0x4b, 0x18, 0x04, 0x18, 0x4d, 0x17, 0x05, 0x17, 0x4a, 0x01, 0x02, 0x17, 0x05, 0x16, 0x4a, 
		0x02, 0x02, 0x17, 0x05, 0x16, 0x02, 0x03, 0x44, 0x03, 0x03, 0x16, 0x02, 0x01, 0x02, 0x16, 0x02, 
		0x03, 0x43, 0x05, 0x03, 0x15, 0x01, 0x03, 0x01, 0x15, 0x03, 0x04, 0x41, 0x06, 0x03, 0x15, 0x01, 
		0x03, 0x01, 0x14, 0x03, 0x07, 0x3d, 0x09, 0x03, 0x2d, 0x03, 0x08, 0x3b, 0x0a, 0x04, 0x2b, 0x03, 
		0x0a, 0x39, 0x0c, 0x03, 0x2a, 0x04, 0x0b, 0x37, 0x0e, 0x03, 0x28, 0x03, 0x0e, 0x2e, 0x04, 0x03, 
		0x10, 0x03, 0x27, 0x03, 0x10, 0x03, 0x03, 0x24, 0x19, 0x03, 0x26, 0x03, 0x1a, 0x1e, 0x1d, 0x03, 
		0x24, 0x03, 0x1e, 0x19, 0x20, 0x04, 0x21, 0x03, 0x20, 0x17, 0x22, 0x04, 0x1f, 0x03, 0x22, 0x15, 
		0x22, 0x04, 0x21, 0x04, 0x21, 0x13, 0x22, 0x05, 0x15, 0x01, 0x0b, 0x05, 0x21, 0x12, 0x21, 0x06, 
		0x15, 0x01, 0x0b, 0x06, 0x21, 0x10, 0x21, 0x07, 0x0a, 0x01, 0x0a, 0x01, 0x0b, 0x07, 0x21, 0x0e, 
		0x20, 0x0a, 0x09, 0x02, 0x09, 0x02, 0x09, 0x09, 0x20, 0x0e, 0x08, 0x02, 0x15, 0x0b, 0x08, 0x03, 
		0x08, 0x03, 0x08, 0x0b, 0x15, 0x03, 0x08, 0x0d, 0x07, 0x04, 0x13, 0x0d, 0x06, 0x05, 0x06, 0x06, 
		0x05, 0x0d, 0x14, 0x04, 0x07, 0x0c, 0x07, 0x06, 0x11, 0x38, 0x12, 0x06, 0x06, 0x0c, 0x06, 0x08, 
		0x10, 0x39, 0x10, 0x08, 0x05, 0x0c, 0x04, 0x0b, 0x0f, 0x39, 0x0f, 0x0a, 0x04, 0x0c, 0x03, 0x0d, 
		0x0e, 0x39, 0x0e, 0x0c, 0x03, 0x0c, 0x02, 0x0e, 0x0e, 0x39, 0x0d, 0x0f, 0x01, 0x0c, 0x01, 0x10, 
		0x0d, 0x39, 0x0d, 0x0f, 0x01, 0x1e, 0x0c, 0x39, 0x0c, 0x30, 0x0a, 0x3a, 0x0a, 0x33, 0x09, 0x3b, 
		0x08, 0x34, 0x09, 0x3b, 0x09, 0x32, 0x09, 0x3c, 0x0a, 0x2f, 0x0a, 0x3e, 0x0a, 0x2d, 0x0b, 0x3f, 
		0x0a, 0x2b, 0x0b, 0x41, 0x0a, 0x29, 0x0b, 0x43, 0x0a, 0x27, 0x0a, 0x46, 0x0a, 0x25, 0x0a, 0x49, 
		0x09, 0x23, 0x08, 0x4e, 0x08, 0x96, 0x12 
	};
	
	tft.fillScreen(ST7735_BLACK);

	uint32_t start = micros_start();
	
	for (int i = 0; i < 0x10; i++)
	{
		tft.setAddrWindow(0, 0, tft.width()-1, tft.height()-1);

		uint16_t cnt = 0;
		uint16_t color = tft.color565((i << 4) | i, (i << 4) | i, (i << 4) | i);
		uint16_t curcolor = 0;

		const uint8_t *cmp = &HaD_128x160[0];

		while (cmp < &HaD_128x160[sizeof(HaD_128x160)])
		{
			cnt = pgm_read_byte(cmp++);
			if (cnt & 0x80)
				cnt = ((cnt & 0x7f) << 8) | pgm_read_byte(cmp++);

#if defined(_PDQ_ST7735H_)
			tft.pushColor(curcolor, cnt);	// PDQ_GFX has count
#else
			while (cnt--)
				tft.pushColor(curcolor);	// Adafruit needs a loop
#endif
			curcolor ^= color;
		}
	}

	uint32_t t = micros() - start;

	tft.setTextColor(ST7735_YELLOW);
	tft.setCursor(0, 145);
	tft.print(F(" http://hackaday.io/"));
	tft.print(F("         Xark"));

	delay(3 * 1000L);
	
	return t;
}

uint32_t testFillScreen()
{
	uint32_t start = micros_start();

	for (uint8_t i = 0; i < 12; i++)
	{
		tft.fillScreen(ST7735_WHITE);
		tft.fillScreen(ST7735_RED);
		tft.fillScreen(ST7735_GREEN);
		tft.fillScreen(ST7735_BLUE);
		tft.fillScreen(ST7735_BLACK);
	}

	return micros() - start;
}

uint32_t testText() {
	tft.fillScreen(ST7735_BLACK);
	uint32_t start = micros_start();
	tft.setCursor(0, 0);
	tft.setTextColor(ST7735_WHITE);	tft.setTextSize(1);
	tft.println(F("Hello World!"));
	tft.setTextColor(tft.color565(0xff, 0x00, 0x00));
	tft.print(F("RED "));
	tft.setTextColor(tft.color565(0x00, 0xff, 0x00));
	tft.print(F("GREEN "));
	tft.setTextColor(tft.color565(0x00, 0x00, 0xff));
	tft.println(F("BLUE"));
	tft.setTextColor(ST7735_YELLOW);
	tft.println(1234.56);
	tft.setTextColor(ST7735_RED);
	tft.println(0xDEADBEEF, HEX);
	tft.setTextColor(ST7735_GREEN);
	tft.setTextSize(2);
	tft.println(F("Groop"));
	tft.println(F("I implore thee,"));
	tft.setTextSize(1);
	tft.println(F("my foonting turlingdromes."));
	tft.println(F("And hooptiously drangle me"));
	tft.println(F("with crinkly bindlewurdles,"));
	tft.println(F("Or I will rend thee"));
	tft.println(F("in the gobberwarts"));
	tft.println(F("with my blurglecruncheon,"));
	tft.println(F("see if I don't!"));
	tft.println(F(""));
	tft.setTextColor(ST7735_MAGENTA);
	tft.println(F("Woot!"));
	uint32_t t = micros() - start;
	delay(1000);
	return t;
}

uint32_t testPixels()
{
	int32_t	w = tft.width();
	int32_t	h = tft.height();

	uint32_t start = micros_start();

	for (uint16_t y = 0; y < h; y++)
	{
		for (uint16_t x = 0; x < w; x++)
		{
			tft.drawPixel(x, y, tft.color565(x<<3, y<<3, x*y));
		}
	}
	
	return micros() - start;
}


uint32_t testLines(uint16_t color)
{
	uint32_t start, t;
	int32_t	x1, y1, x2, y2;
	int32_t	w = tft.width();
	int32_t	h = tft.height();

	tft.fillScreen(ST7735_BLACK);

	x1 = y1 = 0;
	y2 = h - 1;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = w - 1;

	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t = micros() - start; // fillScreen doesn't count against timing

	tft.fillScreen(ST7735_BLACK);

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t += micros() - start;

	tft.fillScreen(ST7735_BLACK);

	x1 = 0;
	y1 = h - 1;
	y2 = 0;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}
	t += micros() - start;

	tft.fillScreen(ST7735_BLACK);

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t += micros() - start;

	return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
	uint32_t start;
	int32_t x, y;
	int32_t w = tft.width();
	int32_t h = tft.height();

	tft.fillScreen(ST7735_BLACK);

	start = micros_start();

	for (y = 0; y < h; y += 5)
		tft.drawFastHLine(0, y, w, color1);
	for (x = 0; x < w; x += 5)
		tft.drawFastVLine(x, 0, h, color2);

	return micros() - start;
}

uint32_t testRects(uint16_t color)
{
	uint32_t start;
	int32_t n, i, i2;
	int32_t cx = tft.width() / 2;
	int32_t cy = tft.height() / 2;

	tft.fillScreen(ST7735_BLACK);
	n = min(tft.width(), tft.height());
	start = micros_start();
	for (i = 2; i < n; i += 6)
	{
		i2 = i / 2;
		tft.drawRect(cx-i2, cy-i2, i, i, color);
	}

	return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
	uint32_t start, t = 0;
	int32_t n, i, i2;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(ST7735_BLACK);
	n = min(tft.width(), tft.height());
	for (i = n; i > 0; i -= 6)
	{
		i2 = i / 2;

		start = micros_start();

		tft.fillRect(cx-i2, cy-i2, i, i, color1);

		t += micros() - start;

		// Outlines are not included in timing results
		tft.drawRect(cx-i2, cy-i2, i, i, color2);
	}

	return t;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
	uint32_t start;
	int32_t x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

	tft.fillScreen(ST7735_BLACK);

	start = micros_start();

	for (x = radius; x < w; x += r2)
	{
		for (y = radius; y < h; y += r2)
		{
			tft.fillCircle(x, y, radius, color);
		}
	}

	return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
	uint32_t start;
	int32_t x, y, r2 = radius * 2;
	int32_t w = tft.width() + radius;
	int32_t h = tft.height() + radius;

	// Screen is not cleared for this one -- this is
	// intentional and does not affect the reported time.
	start = micros_start();

	for (x = 0; x < w; x += r2)
	{
		for (y = 0; y < h; y += r2)
		{
			tft.drawCircle(x, y, radius, color);
		}
	}

	return micros() - start;
}

uint32_t testTriangles()
{
	uint32_t start;
	int32_t n, i;
	int32_t cx = tft.width()/ 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(ST7735_BLACK);
	n = min(cx, cy);

	start = micros_start();

	for (i = 0; i < n; i += 5)
	{
		tft.drawTriangle(
			cx		, cy - i, // peak
			cx - i, cy + i, // bottom left
			cx + i, cy + i, // bottom right
			tft.color565(0, 0, i));
	}

	return micros() - start;
}

uint32_t testFilledTriangles()
{
	uint32_t start, t = 0;
	int32_t i;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(ST7735_BLACK);

	start = micros_start();

	for (i = min(cx,cy); i > 10; i -= 5) {
		start = micros_start();
		tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
			tft.color565(0, i, i));
		t += micros() - start;
		tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
			tft.color565(i, i, 0));
	}

	return t;
}

uint32_t testRoundRects()
 {
	uint32_t start;
	int32_t w, i, i2;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(ST7735_BLACK);
	
	w = min(tft.width(), tft.height());
	
	start = micros_start();

	for (i = 0; i < w; i += 6)
	{
		i2 = i / 2;
		tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
	}

	return micros() - start;
}

uint32_t testFilledRoundRects()
{
	uint32_t start;
	int32_t i, i2;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(ST7735_BLACK);

	start = micros_start();

	for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
	{
		i2 = i / 2;
		tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
	}

	return micros() - start;
}
