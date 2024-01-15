#include <ProcessManager.h>
#include <Arduino.h>

int32_t _ProcessManager::getFreeIndex() {
    for (int32_t i = 0; i < _capacity; i++) {
        if (!_storage[i]) {
            return i;
        }
    }

    return -1;
}

bool _ProcessManager::begin(int32_t capacity) {
    if (_storage) {
        return false;
    }

    _storage = new ProcessUnit*[capacity];
    _capacity = capacity;
    _free = capacity;

    for (int32_t i = 0; i < capacity; i++) {
        _storage[i] = NULL;
    }

    return true;
}

bool _ProcessManager::close() {
    if (!_storage) {
        return true;
    }

    if (!unregisterAllProcess()) {
        return false;
    }

    delete _storage;
    _storage = NULL;
    _capacity = 0;
    _free = 0;

    return true;
}

bool _ProcessManager::checkProcess(ProcessUnit *processUnit) {
    if (!_storage) {
        return false;
    }

    if (!processUnit) {
        return false;
    }

    for (int32_t i = 0; i < _capacity; i++) {
        if (_storage[i] == processUnit && i == processUnit->getIndex()) {
            return true;
        }
    }

    return false;
}

ProcessUnit *_ProcessManager::registerProcess(ProcessHandler processHandler, uint32_t interval, void *context) {
    if (!_storage) {
        return NULL;
    }
    
    int32_t freeIndex = getFreeIndex();

    if (freeIndex >= 0) {
        _storage[freeIndex] = new ProcessUnit(freeIndex, processHandler, interval, context);
        _free--;
        handler();

        return _storage[freeIndex];
    }

    return NULL;
}

bool _ProcessManager::unregisterProcess(ProcessUnit *processUnit) {
    if (!checkProcess(processUnit)) {
        return false;
    }
    
    int32_t processIndex = processUnit->getIndex();

    if (_storage[processIndex] != processUnit) {
        return false;
    }

    delete _storage[processIndex];
    _storage[processIndex] = NULL;
    _free++;
    handler();

    return true;
}

bool _ProcessManager::unregisterAllProcess() {
    if (!_storage) {
        return false;
    }

    bool result = true;

    for (int32_t i = 0; i < _capacity; i++) {
        if (_storage[i]) {
            if (!unregisterProcess(_storage[i])) {
                result = false;
            }
        }
    }

    return result;
}

void _ProcessManager::handler() {
    if (!_storage) {
        _interval = 2500;
        delay(_interval);
        return;
    }

    _interval = UINT32_MAX;
    uint32_t processInterval = 0;

    for (int32_t i = 0; i < _capacity; i++) {
        if (_storage[i]) {
            _storage[i]->handler();

            if (_storage[i]) {
                if (_storage[i]->getRegister()) {
                    processInterval = _storage[i]->getInterval();
                } else {
                    delete _storage[i];
                    _storage[i] = NULL;
                    _free++;
                }
            }

            if (processInterval < _interval) {
                _interval = processInterval;
            }
        }
    }

    if (_interval > 5000) {
        _interval = 5000;
    } else if (_interval < 2) {
        _interval = 2;
    }

    _interval /= 2;

    delay(_interval);
}

_ProcessManager &ProcessManager = ProcessManager.getInstance();