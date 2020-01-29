/**
 * EnergyMeter.cpp
 * @created     25/08/2019 - 07:41 AM
 * @author      José Gabriel Companioni Benítez (https://github.com/elC0mpa)
 * @version     1.0.0
 * @license     MIT
 */

#include "EnergyMeter.h"


EnergyMeter::EnergyMeter(uint8_t pulsesPin, unsigned int pulsesPerKilowattHour, float energy)
{
    _pulses_pin = pulsesPin;
    _pulses_per_kilowatt_hour = pulsesPerKilowattHour;
    _energy = energy;
    _last_energy = _energy;
    _poll_read = true;
    _consumed_energy_callback_should_be_called = false; 
    _energy_increment_per_pulse = (float)1/_pulses_per_kilowatt_hour;
    pinMode(_pulses_pin, INPUT_PULLUP);
	_current_state = digitalRead(_pulses_pin);
}

void EnergyMeter::onConsumedEnergy(float energy, EnergyMeter::callback_consumed_energy_t callback)
{
    _energy_interval = energy;
    _consumed_energy_callback = callback;
}

bool EnergyMeter::read()
{
    if (_poll_read)                                  //Metro is being polled
    {
        _last_state = _current_state;
        _current_state = digitalRead(_pulses_pin);
        if (_current_state == HIGH && _last_state == LOW)
            _analizePulse();
        return _current_state;
    }
    else if(_poll_read == false)                     //Metro is being used through external interrupts;
        _analizePulse();

    return _current_state;
}

void EnergyMeter::_analizePulse()
{
    _energy += _energy_increment_per_pulse;
    if (_last_energy + _energy_interval <= _energy)
    {
        _last_energy = _energy;
        if (_consumed_energy_callback)
        {
            if (_poll_read)
                _consumed_energy_callback(_energy);
            else if (!_poll_read)
                _consumed_energy_callback_should_be_called = true;
        }
    }      
}

void EnergyMeter::update()
{
    if (_consumed_energy_callback_should_be_called)
    {
        _consumed_energy_callback(_energy);
        _consumed_energy_callback_should_be_called = false;
    }
}

EnergyMeter EnergyMeter::operator=(const EnergyMeter& meter)
{
    _pulses_pin = meter._pulses_pin;
	_energy = meter._energy;
	_pulses_per_kilowatt_hour = meter._pulses_per_kilowatt_hour;
	_energy_increment_per_pulse = meter._energy_increment_per_pulse;
    _last_energy = meter._last_energy;
    _energy_interval = meter._energy_interval;
    _last_state = meter._last_state;
    _current_state = meter._current_state;
    _poll_read = meter._poll_read;
	return *this;
}

bool EnergyMeter::_supportsInterrupt()
{
    return (digitalPinToInterrupt(_pulses_pin) != NOT_AN_INTERRUPT);
}

bool EnergyMeter::enableInterrupt(EnergyMeter::callback_t ISR)
{
    if (!_supportsInterrupt())
        return false;
    attachInterrupt(digitalPinToInterrupt(_pulses_pin), ISR, RISING);
    _poll_read = false;
    return true;
}

void EnergyMeter::disableInterrupt()
{
    detachInterrupt(digitalPinToInterrupt(_pulses_pin));
    _poll_read = true;
}

float EnergyMeter::getEnergy()
{
    return _energy;
}