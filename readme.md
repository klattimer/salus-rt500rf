Salus RT500RF + Arduino + RFM12B
------------------------------

This is 90% a testbed and 10% an intermediate solution for me, the intention is to be able to turn on and off a
Salus RT500RF boiler controller using an 868Mhz RFM12B from HopeRF. The RFM12B is essentially discontinued but
is still an extremely useful piece of hardware. The replacement the RFM69CW should be easy enough to replace the RFM12B
with in the hardware. Using a little of Jeelib's compatibility library.

I based this project on top of this one

    http://the.cyclingengineer.co.uk/2013/11/23/home-automation-integrating-salus-rt500-rf-in-openhab-using-a-jeelink/

I reduced the decode complexity using GQRX and improved a few minor details along the way. Removing pretty much all
jeenode code as I'm not really interested in using Jeelib and Jeenode as those things seem to be hard to get ahold of
now.

The wiring method is as described here:

    https://lowpowerlab.com/wp-content/uploads/2012/12/rfm12B-arduino-moteino-atmega328_5V_connections.png

If you're using a 3.3V board like a mini-pro then remove the resistors and connect directly to the arduino.

I'd suggest that a RTL-SDR dongle is a required piece of hardware to decode your Salus RT500RF's signal with.

Every RFM12B will have slightly different transmission characteristics or even firmware
variations which can be accounted for by shifting the frequency center and the frequency shift.

With GQRX
 - Mode WFM (oirt)
 - Squelch/Attenuate the signal appropriately so you can see the transmission clearly
 - Record the audio of the signal
 - Find the pair of transmission peaks in the waterfall around 868.2 Mhz
 - Find the frequency in the center, and the gap in khz between the transmission peaks
 - Open the Wav file in Audacity and zoom to one of the repeated signals
   Ignore the noise this is usually reflection around your home, look for the distinct up/down
   usually early in the signal.
 - Decode the signal by aligning the frequency shifts to a grid and reading the bytes
 - Signal is in the form of
       0xAA 0xAA 0xAA 0x2D 0xD4 0x?? 0x?? 0x?? 0x5A
   Where the ?? bytes are the code to YOUR boiler, on and off use transmit different codes
 - Edit the bytes below to your appropriate settings.
