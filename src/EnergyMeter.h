/**
 * EnergyMeter.h
 * @created     25/08/2019 - 05:28 AM
 * @author      José Gabriel Companioni Benítez (https://github.com/elC0mpa)
 * @version     1.0.0
 * @license     MIT
 */

#include <Arduino.h>

class EnergyMeter
{

public:

    typedef void(*callback_t)();
	typedef void(*callback_consumed_energy_t)(float energy);

	//ctors

	EnergyMeter(){}
	EnergyMeter(uint8_t pulsesPin, unsigned int pulsesPerKilowattHour, float energy = 0, float voltage = 230);

    //events

	//Calls a callback function each time energy increment by the given value.
    void onConsumedEnergy(float energy, callback_consumed_energy_t callback);		

	//user functions

	//Check if there is a positive edge to analize.
    bool read();
	//Only needed when using interrupts. Check if some callback function should be called.													
	void update();													

	//interrupt functions

	//Enable interrupt if pin allow it
	bool enableInterrupt(callback_t ISR);
	//Disable interrupt
	void disableInterrupt();

	//operators override
	EnergyMeter operator=(const EnergyMeter&);

	//getters						

	float getEnergy();

private:
	//pins
	uint8_t _pulses_pin;											//Pin from wich energy meter pulses will be analized.
	
	//electrical variables
	float _energy;													//Consumed energy in kWH.
	unsigned int _pulses_per_kilowatt_hour;							//Represents amount of pulses to consume a kWH.
	float _energy_increment_per_pulse;								//Consumed energy when a new pulse arrives.
    float _last_energy;												//Last energy value when _consumed_energy_callback was called.
    float _energy_interval;											//Energy interval to call _consumed_energy_callback.
	float _voltage;													//Ideal voltage

	//Actual Serie fields
	unsigned long _prev_millis_value;
	unsigned long _actual_millis_value;

    //pin states
    bool _last_state;												//Last state of signal incoming from energy meter.
    bool _current_state;											//Current state of signal incoming from energy meter.

    bool _poll_read;												//True if energy meter is being polled, False if is being used through external interrupts.

	bool _consumed_energy_callback_should_be_called;				//Only needed when using interrupts.
    
	//functions
    void _analizePulse();											//Update private fields and check if _consumed_energy_callback should be called
	bool _supportsInterrupt();
    //callbacks
    callback_consumed_energy_t _consumed_energy_callback;
};

