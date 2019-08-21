# SpeakerBIT Tutorial

SpeakerBIT is an Arduino-based device designed for basic sonification of biosignal data acquired with BITalino sensors. In this repository we illustrate its use for muscular activity sonification as measured using Electromyography (EMG). 

The main objective of this device is to have a responsive feedback to an input signal instead of just a graphical visualization and raw data recording performed by [OpenSignals](https://bitalino.com/en/software), while preserving the base functionality of the sensor. 

It is possible to use the device in standalone, but also to use it while acquiring data with software applications like [OpenSignals](https://bitalino.com/en/software). There are two versions of the SpeakerBIT, both react to the muscular activity, but have different responses.

In one mode the input signal amplitude is used to modulate the frequency of a generated tone, while on the other mode it is used to modulate the time in-between notes of a pre-defined melody.


## Device Overview

The following sub-sections summarize the main features of the SpeakerBIT prototype and a further explanation for each one. In addition to the sonification, the firmware running on the device computed the envelope of the discrete derivative of the input signal using a moving average filter (see [Real Time Feature Extraction from Biosignals](https://github.com/rotzila/Real-Time-Feature-Extraction-from-Biosignals)).

### Top
1. *UC-E6 Socket:* Standard UC-E6 connector used to interface the sensor with BITalino; it provides the power lines needed for SpeakerBIT to operate and provides an analog output with the same pinout used for data acquisition with a BITalino Core board.

2. *Volume Adjustment:* With a Philips-head screwdriver it is possible to increase or decrease the output volume of the speaker.

3. *Detection Threshold:* This potentiometer will define the muscle activation value that the user needs to surpass in order to activate the speaker output. Usually a user in a rest state (after the calibration) will have a value around 2/3 the maximum amplitude; the threshold can be set from 0 to 50. However, bellow 2 the speaker will be turned off; this is implemented to have a way to disable the speaker without turning off the device.

4. *Electrode Cables:* These cables enable the interface between the sensor and the user's body; the cables are represented by red (IN+), black (IN-) and white (REF) as a way to facilitate the placement on the body.

![SpeakerBIT Top View](https://github.com/rotzila/SpeakerBIT/blob/master/images/prototype_image_1.jpg "SpeakerBIT Top View")


### Bottom <a name="bottom"></a>
5. *Calibration Button:* Pressing this button for 2 seconds will activate the calibration mode; a LED turns on signaling that the calibration mode is activated (see Section [Calibration](#calibration) below). To exit the calibration mode, the button needs to be pressed again. 

![SpeakerBIT Bottom View](https://github.com/rotzila/SpeakerBIT/blob/master/images/prototype_image_2.jpg "SpeakerBIT Bottom View")


### Electrode Placement
For illustrative purposes the EMG sensor is used. There are various muscles that the EMG can be monitored; as a general rule of thumb, electrode placement is performed by attaching the REF lead in a bone region (electrically neutral), and IN+ & IN- 20mm apart
over the muscle belly (aligned with the muscle fibers). 

The following image ilustrates an example of the EMG's electrodes placement, where the black (IN-) and red (IN+) eletrodes are appliedd on the muscle that is being tested, the white (REF) eletrode are placed in an area were there is no eletric activity, namely the elbow. 

![Example electrode  placement for EMG](https://github.com/rotzila/SpeakerBIT/blob/master/images/emg_placement.jpg "Example electrode placement for EMG")

For detailed information about the EMG and electrodes placement guidelines, please refer to the following resources:
![SENIAM EMG Guidelines](http://www.seniam.org/ "SENIAM EMG Guidelines")


### Calibration <a name="calibration"></a>

Given that each user has slightly difference baseline and activation levels, this device has been fitted with a calibration mode. For instructions on how to activate and deactivate the calibration mode, please refer to Section [Device Overview](#bottom) above.

When in calibration mode, the device is recording the minimum and maximum amplitude values. During this period the user must be in a rest state for approximately 3 seconds, after this period the user should perform a Maximum Voluntary Contraction (MVC). 

Exiting the calibration mode will record the calibration values in the internal memory of the device and adapt the output range to the newly determined values. This range will give a range from 0 to 100% using the calibration; given that the values are stored in the onboard memory, upon turning off the device the calibration will be recovered in subsequent uses.

## Source Code Variants

Two different source code versions are available, one which uses the amplitude values to modulate the frequency of a generated tone, and the other that uses the amplitude values to control the time in-between notes of a pre-defined melody. 

### SpeakerBIT Frequency

In this version the output sound will be a frequency that will start at a low frequency, and the higher the activation "strength" of the muscle is, the higher the output frequency will be.

### SpeakerBIT Melody

In this version there is a default melody pre-programed on the device, which plays in a loop. By default the melody is repeating at a low speed, which will increase as a function of the muscule activation intensity.

## Build Your Own

### Materials Checklist

#### Tools

[  ] 3D printer

[  ] Glue

[  ] Soldering iron

[  ] Solder

#### 3D Printed Parts

[  ] Bottom part

[  ] Top part

[  ] Button

#### Hardware

[  ] [µduino Board](https://www.crowdsupply.com/uduino/uduino/updates/update-on-production-and-delivery)

[  ] [Groove Speaker](http://wiki.seeedstudio.com/Grove-Speaker/)

[  ] [Potenciometer](https://www.sparkfun.com/products/9806?_ga=2.258241545.2017274394.1565780560-1270259391.1560514473)

[  ] [BITalino LED](https://plux.info/barebone-actuators/19-light-emitting-diode-led.html?search_query=LED&results=32)

[  ] [BITalino Pushbutton](https://plux.info/barebone-sensors/15-pushbutton-btn.html?search_query=button&results=7)

[  ] [BITalino EMG Sensor](https://plux.info/barebone-sensors/10-electrocardiography-ecg-sensor.html?search_query=emg+sensor&results=155)

[  ] [3-Lead Electrode Cable](https://plux.info/cables/226-3-lead-electrode-cable.html)

[  ] [Pre-Gelled Self-Adhesive Disposable Ag/AgCl](https://plux.info/electrodes/59-pre-gelled-self-adhesive-disposable-agagcl-eletrodes.html)


### Schematic

Here is the schematic used in this prototype. Instead of a arduino UNO it was used the µduino, the smallest version of arduinos. It is the same connections. [This link](https://www.crowdsupply.com/uduino/uduino/updates/pinout-and-more) will help you know where to connect each cable in the µduino board.
The connection to the BITalino need to be made to the analog channle in the BITalino board.

![SpeakerBIT Circuit Schematics](https://github.com/rotzila/SpeakerBIT/blob/master/images/prototype_schematic_1.png "SpeakerBIT Circuit Schematics")


### Algorithms

There are two algorithms, one for the [Frequency](https://github.com/rotzila/SpeakerBIT/blob/master/music-muscle-freq/music-muscle-freq.ino) and another for the [Song](https://github.com/rotzila/SpeakerBIT/blob/master/music-muscle-song/music-muscle-song.ino) version. The following image depicts the signals obtained from the raw data output and after the algorithm implemented in the device firmware.

![Example EMG Signals](https://github.com/rotzila/SpeakerBIT/blob/master/images/raw_vs_smo.png "Example EMG Signals")


### 3D printed parts

To house the electronics, three parts were designed, namely a [top](https://github.com/rotzila/SpeakerBIT/blob/master/box/top.stl), a [bottom](https://github.com/rotzila/SpeakerBIT/blob/master/box/bottom.stl) and a [button](https://github.com/rotzila/SpeakerBIT/blob/master/box/button.CATPart), to encapsulate all the hardware used in this prototype.

## Acknowledgments

This project was developed at the IT - Instituto de Telecomunicações, Lisbon pole, during an internship under the supervision of Prof. Hugo Plácido da Silva.

## References

Pedro Miguel Russo. [Development and Testing of Algorithms for Real-Time Feature Extraction from Biosignals](https://github.com/rotzila/Real-Time-Feature-Extraction-from-Biosignals). BSc work, Escola Superior de Tecnologia de Setúbal - Politécnico de Setúbal, 2019.
