#ifndef PROCESSUNIT_H
#define PROCESSUNIT_H

#include "stdint.h"
#include <cstddef>

class ProcessUnit;
typedef void (*ProcessHandler)(ProcessUnit *processUnit, void *context);

class ProcessUnit {
    private:
        int32_t _index;

        bool _register;
        bool _active;
        ProcessHandler _handler;
        uint32_t _interval;
        uint32_t _nextMillis;
        void *_context;
    public:
        int32_t getIndex() {
            return _index;
        }

        bool getRegister() {
            return _register;
        }

        void setUnregister() {
            _register = false;
        }

        void setActive(bool active) {
            _active = active;
        }

        bool getActive() {
            return _active;
        }

        void setInterval(uint32_t interval) {
            _interval = interval;
        }

        uint32_t getInterval() {
            return _interval;
        }

        ProcessUnit(int32_t index, ProcessHandler processHandler, uint32_t interval, void *context);
        void handler();
};

#endif
