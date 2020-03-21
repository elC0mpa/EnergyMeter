# EnergyMeter

Arduino library to get data from energy meters in order to monitor them.

![](button.gif)

## What it is for

This library is used to monitor energy meters. It uses **callbacks** to trigger actions each time the meter measures a certain amount of kilowatts-hour. By using this library, energy meters can be **polled** *(less efficient approach)* or can be interfaced through **external interrupts** *(most efficient approach)*.

## How to install

Install the library using one of the following options

- [Arduino Library Manager](https://www.arduino.cc/en/Guide/Libraries)
- [Clone this repository into your libraries directory](https://help.github.com/articles/cloning-a-repository/)
- [Use it locally cloning this repository into your project's directory](https://help.github.com/articles/cloning-a-repository/)

## Compatibility
This library is compatible with any energy meter which is based on ADE7755 (or similar) integrated circuit. This integrated circuit has a high frequency output (CF) wich is the easiest way to interface the energy meter to a microcontroller. The most important specification of energy meters in order to monitor them with this library is **impulses per kilowatt-hour**. 

## How to use

### **Include the library**

``` c++
#include <EnergyMeter.h>
```

### **Create the Energy Meter instance**

Create an instance of `EnergyMeter` class for each energy meter that you want to monitor, and pass the pin number and impulses per kilowatt-hour as arguments. 

```c++
// Energy meter instance
EnergyMeter meter(METER_PIN, PULSES_PER_KILOWATT_HOUR);
```

The class constructor takes the following additional arguments:

**energy** : Energy measured by the meter at the moment that the instance is created. By default, zero.

**voltage** : Line voltage of the circuit which is power is being measured by the energy meter. You should consider that this value might change.

```c++
// EnergyMeter class's constructor signature
EnergyMeter(uint8_t pulsesPin, unsigned int pulsesPerKilowattHour, float energy = 0, float voltage = 230);
```

### **Callbacks**

The library provides callbacks to trigger actions each time the meter measures a certain kind of kilowatts-hour. 
```c++
// Energy measured 
meter.onConsumedEnergy(1, energyConsumed);
```
The callback function `energyConsumed` **must receive a float parameter** which represents the **energy in kWH measured by the meter** until the callback is called.

```c++
void energyConsumed(float energy)
{
  Serial.println(String(energy) + " kilowatts-hour consumed");
}
```

```c++
// Current consumption
meter.onCurrentChanged(currentChanged);
```
The callback function `currentChanged` **will be called each time a new pulse is detected**. Each time this happens, the library will calculate the current consumption  

```c++
void currentChanged(float current)
{
  Serial.println(String(current) + " A consumed");
}
```

### **Polling meter**
In order to continuously read the status of the meter (less efficient approach), `read` function must be called inside `loop`.
```c++
void loop() {
    meter.read();
}
```
### **Interfacing meter through external interrupts**
First of all, `enableInterrupt` function must be called. If this function returns false, other pin should be used in order to interface the meter through external interrupts.
```c++
if (!meter.enableInterrupt(pulseDetected))
  {
    Serial.println("Pin " + String(METER_PIN) + " does not support interrupt in this board");
    while (true);
  }
```
When using interrupts, **`read` method shouldn't be called inside the `loop` function**. It should only be called in the callback provided to `enableInterrupt` function, as shown below:
```c++
void pulseDetected()
{
  meter.read();
}
```
As interruption service routines (ISR) should be as short as possible, callback functions will never be called inside the ISR. This is why `update` method must be continuously called inside `loop`. This method checks if any callback function should be called.
```c++
void loop() {
  //update() function must be called only when using interrupts.
  meter.update();
}
```
If there is no longer need to use external interrupts, `disableInterrupt` function should be called.
```c++
  meter.disableInterrupt();
```
  ## Changelog  
  - **v1.0.0 (2019/08/27)** - library initial release

  ## Examples

  [ConsumedEnergy](https://github.com/elC0mpa/EnergyMeter/blob/master/examples/ConsumedEnergy/ConsumedEnergy.ino)

  [Interrupt_ConsumedEnergy](https://github.com/elC0mpa/EnergyMeter/blob/master/examples/Interrupt_ConsumedEnergy/Interrupt_ConsumedEnergy.ino)

## Copyright

[MIT](../LICENSE.md) © [José Gabriel Companioni Benítez (elC0mpa)](https://github.com/elC0mpa)
