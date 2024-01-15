#include <ProcessUnit.h>
#include <Arduino.h>

ProcessUnit::ProcessUnit(int32_t index, ProcessHandler handler, uint32_t interval, void *context) {
    _index = index;

    _register = true;
    _active = true;
    _handler = handler;
    _interval = interval;
    _nextMillis = 0;
    _context = context;
}

void ProcessUnit::handler() {
    if (!_active) {
        return;
    }

    uint32_t timestamp = millis();

    if (timestamp >= _nextMillis) {
        _nextMillis = timestamp + _interval;
        _handler(this, _context);
    }
}
