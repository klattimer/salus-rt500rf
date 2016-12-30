/* Copyright 2016 Karl Lattimer <karl@qdh.org.uk>
 *
 * License: MIT
 * Derrived from: https://github.com/cyclingengineer/jeelink-ha
 *
 * This is a simplified arduino program to use an RFM12B to control a salus 500RF boiler controller
 *
 * Every RFM12B will have slightly different transmission characteristics or even firmware
 * variations which can be accounted for by shifting the frequency center and the frequency shift
 * With GQRX
 *  - Mode WFM (oirt)
 *  - Squelch/Attenuate the signal appropriately so you can see the transmission clearly
 *  - Record the audio of the signal
 *  - Find the pair of transmission peaks in the waterfall around 868.2 Mhz
 *  - Find the frequency in the center, and the gap in khz between the transmission peaks
 *  - Open the Wav file in Audacity and zoom to one of the repeated signals
 *    Ignore the noise this is usually reflection around your home, look for the distinct up/down
 *    usually early in the signal.
 *  - Decode the signal by aligning the frequency shifts to a grid and reading the bytes
 *  - Signal is in the form of
 *        0xAA 0xAA 0xAA 0x2D 0xD4 0x?? 0x?? 0x?? 0x5A
 *    Where the ?? bytes are the code to YOUR boiler, on and off use transmit different codes
 *  - Edit the bytes below to your appropriate settings.
 *
 */

#include <rfm12b.h>

#define SALUS_CMD_LEN_BYTES         4
#define SALUS_CMD_REPEAT_COUNT      30
#define SALUS_CMD_INTER_PKT_GAP_MS  20

static uint8_t salus_boiler_on_buf[SALUS_CMD_LEN_BYTES]   = { 0x00, 0x01, 0x01, 0x5a };
static uint8_t salus_boiler_off_buf[SALUS_CMD_LEN_BYTES]  = { 0x00, 0x02, 0x02, 0x5a };

void set_boiler(uint8_t enable) {
  rf12_initialize(1, RF12_868MHZ, 0xD4);  // D4 appears to be end of the preamble/the header
  rf12_control(RF12_DATA_RATE_2);         // 2.4kbps (doesn't seem to change)

  // The control code is prefixed with A6, somewhere between 70 and 90 should be good
  rf12_control(0xA685);                   // 868.260 Mhz (ish)

  // The frequency shift will vary between controllers, values between 20 and 50 should work
  // this is the gap in khz between the two transmission peaks 40 == 75Khz, 30 == 125Khz
  // RFM12B data sheet might help
  rf12_control(0x9830);                   // 125khz freq shift

  uint8_t *data;
  if (enable) {
    data = (uint8_t *)&salus_addr123_boiler_on_buf;
  } else {
    data = (uint8_t *)&salus_addr123_boiler_on_buf;
  }
  for (int n=0; n < SALUS_CMD_REPEAT_COUNT; n++) {
    rf12_sendNow(data, SALUS_CMD_LEN_BYTES);
    delay(SALUS_CMD_INTER_PKT_GAP_MS);
  }
}

void setup() {
}

void loop() {
}
