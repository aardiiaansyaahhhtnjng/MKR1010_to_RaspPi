# MKR1010 to RaspPi

This project uses an Arduino-type MKR1010 as a temperature and humidity detector and a Raspberry Pi as a server for storing and processing measurement data.
The workflow of the system itself, starting from Arduino which has input the algorithm code will start detecting temperature and humidity, and then the detection results will be sent to the Raspberry Pi server via the MQTT broker, the results received are then processed on the node-red server to be stored in storage and displayed on the dashboard. All of these communications run on the internet network.
