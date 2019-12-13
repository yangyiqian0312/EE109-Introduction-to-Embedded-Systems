# EE109-Introduction-to-Embedded-Systems
A final project using C to work with Arduino and LCD shield.

In this project, I build a device that measures how fast an object is moving as it passes two sensors (phototransistors) and then relays that speed to a remote device. 

The project includes:
 1. Two LED light sources, and two phototransistors light detectors for determining the time it takes for an object to go from the first sensor to the second sensor.
 2. An LED to indicate that a speed measurement is in progress.
 3. An LCD display for showing the measured time delay in msec and the speed in cm/sec. The display is also used for setting the speed threshold and displaying the speed received from the remote device.
 4. A buzzer for playing an alarm tone.
 5. A control knob for selecting the speed threshold.
 6. A serial interface to another speed trap unit. When the local unit measures a speed it sends it to the remote unit. When a speed is received from the remote unit it is displayed on the LCD. If the speed received from the other device is above the speed threshold, it sounds an alarm.

-----------------------------------------------------------------------------------------
To run, please compile with 
  1. make clean
  2. make flash
  
  
![speedtrapdiagram](https://user-images.githubusercontent.com/35481665/70802618-c78d0900-1d66-11ea-96e5-d39887f48aed.png)
![speed trap](https://user-images.githubusercontent.com/35481665/70801893-e7233200-1d64-11ea-970a-bc93089b9128.jpg)


GIF Demo:

![speedtrap](https://user-images.githubusercontent.com/35481665/70802286-e2ab4900-1d65-11ea-843c-3a7315dab77d.gif)
