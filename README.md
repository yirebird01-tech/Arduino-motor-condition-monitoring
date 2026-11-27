# Motor Condition Monitoring & Protection

I built an Arduino Uno breadboard prototype for DC motor monitoring and protection. I tested the circuit behavior separately in Tinkercad and built a Python/Streamlit dashboard to demonstrate fault handling with simulated data. The dashboard was not connected to the physical prototype.

## 1. Physical Arduino Prototype

The photo and video show the physical breadboard build with the motor running and the green run LED lit.

![Physical Arduino Uno motor prototype with the fan spinning and green run LED lit](photo_2026-11-26_19-41-13.jpg)

[Watch the physical motor demo (20 seconds)](physical-motor-demo.mp4)

The photo and video show the hardware running. The fault-test results below are from Tinkercad, not from the physical demo.

## 2. Tinkercad Circuit Tests

The Arduino circuit monitors motor current and temperature. I tested normal operation, overcurrent shutdown, overtemperature shutdown, and manual reset after a fault in Tinkercad.

[View the Tinkercad circuit schematic (PDF)](Arduino%20Motor%20Condition%20Monitoring%20and%20Protection%20Prototype.pdf)

![Arduino Uno breadboard wiring in Tinkercad](Screenshot_25-11-2026_105941_www.tinkercad.com.jpeg)

### Normal Operation

The green LED is on, and the Serial Monitor reports `RUNNING: Motor commanded ON.`

![Tinkercad normal-operation test](Screenshot_25-11-2026_141214_www.tinkercad.com.jpeg)

### Overtemperature Shutdown

The simulated temperature reached 79.0 °C, above the 50 °C trip threshold. The motor stopped, the red fault LED turned on, and the Serial Monitor reported `FAULT` and `0.0 mA`.

![Tinkercad overtemperature shutdown test](Screenshot_25-11-2026_141329_www.tinkercad.com.jpeg)

## 3. Python/Streamlit Dashboard Simulation

The dashboard screenshots below use simulated readings, not live Arduino data. **In the version shown here, temperature triggers a simulated shutdown; current is displayed for monitoring only.** The Arduino circuit's overcurrent test is separate.

### 3.1 Normal Operation

The simulated motor is running with no active alerts or previous faults.

![Dashboard showing normal operation](01_normal_operation.jpg)

### 3.2 Temperature Trip

A simulated overtemperature fault stops the motor.

![Dashboard showing a simulated temperature trip](02_temperature_fault.jpg)

### 3.3 Cooled, but Fault Still Latched

The temperature reading returns to normal, but the fault stays active and the simulated motor remains stopped. Cooling alone does not clear the fault.

![Dashboard showing a normal temperature reading while the motor remains stopped](03_cooled_but_latched.jpg)

### 3.4 Recovery After Reset

This screenshot shows the simulated motor running again after recovery. The previous fault is still visible in the history.

![Dashboard showing the motor running again with the previous fault in history](04_successful_reset.jpg)

### 3.5 Missing Temperature Data

This separate test shows how the dashboard reports an unavailable temperature reading.

![Dashboard showing missing temperature data](05_missing_temperature.jpg)
