# Requirements for pv test station

## Idear of the Projekt
To achieve Germany's self-imposed climate goals, it is essential to further expand renewable energies. Photovoltaics, especially in southern Germany, plays a crucial role in this expansion. 
However, rooftop installations alone are insufficient to meet the demand; therefore, open spaces must also be utilized. At the same time, protecting important agricultural lands is vital to secure regional food production.

Agrivoltaic systems (Agri-PV) offer a solution that combines both needs. These systems generate electricity while allowing agricultural activities to continue on the same land. Moreover, by employing bifacial solar cells, the power output during midday—when energy supply often exceeds demand—is reduced. 
Yet, they maintain relatively high output in the morning and evening hours, as well as in winter, because they more effectively utilize diffuse light.

![Agri PV](https://github.com/user-attachments/assets/2e0609a2-ed12-4297-b54f-54f5730913f6 "photovoltaik.eu/landwirtschaft/next2sun-baut-agri-pv-mit-buergerbeteiligung")

Given that such a project becomes profitable only when implemented on a large scale—since the costs of installing power lines from the open-field site to the grid connection point must be covered by the developer—the investment costs are substantial. 
To accurately calculate the economic feasibility, a measurement station is to be developed to record the generated power over the span of a year. Additionally, to enable forecasts for future years, environmental factors such as ambient temperature and the temperature directly at the panels should also be documented.

## Requirements
- Meassure Power, Temperature of the Panel and the Environment
- Measurement interval every second
- Store the Data in a safe place
- The future Park will probably be built with Bifacial Cells from Huasun with 470Wp. The test station should use comparable cells

  
## Realtime definition
This is Project is a Real-time System with soft time limits. The system should measure every second. But when there is an issue and the time is not reached, it's bad but has no catastrophic impact like an Airbag system.

![image](https://github.com/user-attachments/assets/37c90a3b-ba38-4e24-bffb-13c7e6fcdb8e)

The System has many sensors for the power measurement and the temperatur. The plant is the evironment and the sun and the controller is the ATSAMD21G18 microcontroller.

## Circuit diagram

![PXL_20241002_170940363~2](https://github.com/user-attachments/assets/a554ddd2-987b-41a4-b51a-8d934b707e7e)

## Protocol

![PXL_20241002_162630208~2](https://github.com/user-attachments/assets/00d5ead2-207e-48eb-a023-3717233414ea)

## Parts

- [ ] 1x (Adafruit Feather M0 LoRa Radio - 433MHz)[https://www.mouser.de/account/orders/detail?qs=Sn2QTPuoay%252BUEfD1VQ4ZfDkVjOmp3iWeccA1x4dP3mE%3d]
- [ ] 1x (Adafruit LoRa Radio Bonnet - 433MHz)[https://www.mouser.de/ProductDetail/485-4075]
- [ ] 1x Raspberry Pi 4
- [ ] 1x Current Sensor [CT426-HSN830DR](https://www.mouser.de/ProductDetail/462-CT426-HSN830DR)
- [ ] 2x [MCP9808 Temp Sensor Breakout Adafruit](https://www.mouser.de/ProductDetail/485-1782)
- [ ] 47K Resistor
- [ ] 3,3K Resistor
- [ ] (Adafruit DS3231 Precision RTC)[https://www.mouser.de/ProductDetail/485-5188]
- [ ] I2C extender (LTC4311) [https://www.mouser.de/ProductDetail/485-4756]
- [ ] EEPROM (M24C01-WMN6P) [https://www.mouser.de/ProductDetail/511-M24C01-WMN6P]
- [ ] (433MHz Antenna db station) [https://www.mouser.de/ProductDetail/538-204287-0100]
- [ ] (433MHz Antenna dataminer)[https://www.mouser.de/ProductDetail/712-ANT-433-PW-QWUFL]
- [ ] (MPPT Controller (Off-Grid))[https://www.offgridtec.com/victron-smartsolar-mppt-75-15-15a-12v-24v-solar-laderegler.html]
- [ ] Dummyload ~500W immersion heaters
- [ ] (Relais 24V 20A) [https://www.digikey.de/de/products/detail/american-zettler/AZ576-1A-5DE/14307551]
- [ ] (Step Down DC/DC) [https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/908/DFR0571_Web.pdf]
- [ ] PCB to Wire connector panel side min 43V 14A (Phoenic Contact 277-1579-ND)[https://www.digikey.de/de/products/detail/phoenix-contact/1935187/568616]
- [ ] PCB to Wire connector load side min 24V 21A (Phoenic Contact 277-17516-ND) [https://www.digikey.de/de/products/detail/phoenix-contact/1017505/9346606]
