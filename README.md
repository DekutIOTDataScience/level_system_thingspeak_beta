# level_system_thingspeak_beta
A level  system using ultrasonic range finder. The reading is sent to thingspeak via ESP8286 for storage and a backup SD Card incase the data fails to be sent to Thingspeak Server. A DS3231 timer is used to attach time to the value recorded on the SD card.

Being a Beta version it has the following characteristics which we will improve in future.

The Nucleo F411RE tries to send data via hotspot using ESP8286 even though no wifi hotspot is available.
Inconsistency of data storage.Thingspeak and SD card might store data in intervals if wifi is not strong enough.

Future Improvements:
Consistent recording on sd card for a while before trying to check wifi availability.Hence consistent storage.

Overcoming the Inconsistency
Since there is timestamp attached to data in the sd card and data stored in ThingSpeak Servers a sorting software can be used to inject data recorded on the sd card to csv file downloaded from thingspeak when doing analysis.


![img_20180119_120945](https://user-images.githubusercontent.com/30165974/35184232-ee12a9f6-fe03-11e7-939a-200c75688d71.jpg)
![img_20180119_144032](https://user-images.githubusercontent.com/30165974/35185032-8c256ba6-fe0e-11e7-857b-f27d51b8b1eb.jpg)
![img_20180119_144003](https://user-images.githubusercontent.com/30165974/35185034-991e92e2-fe0e-11e7-9bd3-44a0d03ca673.jpg)

