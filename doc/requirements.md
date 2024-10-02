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

## Realtime definition
This is Project is a Real-time System with soft time limits. The system should measure every second. But when there is an issue and the time is not reached, it's bad but has no catastrophic impact like an Airbag system.

![image](https://github.com/user-attachments/assets/37c90a3b-ba38-4e24-bffb-13c7e6fcdb8e)

The System has many sensors for the power measurement and the temperatur. The plant is the evironment and the sun and the controller is the ATSAMD21G18 microcontroller.

## Circuit diagram



## Protocol

![PXL_20241002_162630208~2](https://github.com/user-attachments/assets/00d5ead2-207e-48eb-a023-3717233414ea)
