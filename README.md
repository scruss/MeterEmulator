MeterEmulator
=============

An Arduino sketch to emulate the IR flash of a utility power meter. Controllable through attached LCD keypad shield.

If you wish to change the timing range, the included spreadsheet helps a little.

Wiring
------

* D2 - LTE 5208A IR LED through 29222A transistor to get enough current. IR LEDs are doubly fiddly, in that:
 1. you can't see their output (a digital camera can help)
 2. they draw more current than an Arduino likes to supply.

To Do
-----

* Being able to have a true indicated power of zero and still have the
  keyboard polled would be nice.

Author
------

Stewart C. Russell - http://scruss.com/blog/

References
----------

* [Reading pulses from meters with pulse outputs](http://openenergymonitor.org/emon/buildingblocks/introduction-to-pulse-counting "Reading pulses from meters with pulse outputs") - for 50 ms flash timing.
* [Lite-On Semiconductor Corporation LTE-5208A Series Datasheet](http://www.datasheet.net/view/12509933/) - the 940 nm IR LED I used.
* [Max Power IR LED Kit](https://www.sparkfun.com/products/10732 "Max Power IR LED Kit") - typical IR LED driver circuit.
* [Arduino LCD KeyPad Shield](http://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_%28SKU:_DFR0009%29#Sample_Code "Arduino LCD KeyPad Shield") - description and code.

Licence
-------

WTFPL. (Srsly; see COPYING.)
