# EnviroVitals
Capstone Project to collect water and air quality data in real-time displayed on a webserver

Air Quality Sensors:
- Ozone (O<sub>3</sub>)
- Nitrogen Dioxide (NO<sub>2</sub>)
- Fine Particulate Matter (PM 2.5)
- Volatile Organic Compounds (VOCs)
- Temperature
- Humidity
- Carbon Dioxide (CO)

Water Quality Sensors:
- Temperature
- pH
- Dissolved Oxygen
- Oxidation Reduction Potential
- Total Dissolved Solids
- Turbidity

Project consisted of 3 nodes (home, water and air) each using an ESP32C6. The water and air nodes collect raw data, filter and send data to home node via ESP-NOW. Data tranmission is done using common Wi-Fi channel and MAC address. Sensor nodes are setup as stations and home node is setup as access point and station to recieve data from peer ESP32 devices and transmit data to server. The home node connects to local internet connection containing JavaScript, HTML and CSS files. Home node sets up web socket to handle communication between webserver and incoming data from sensor nodes. 

![image](https://github.com/user-attachments/assets/169edc87-d6cc-4c56-8996-81274b228f1f)
