#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <ProcessUnit.h>

class _ProcessManager {
    private:
        ProcessUnit **_storage;
        int32_t _capacity;
        int32_t _free;
        uint32_t _interval;

        int32_t getFreeIndex();

        _ProcessManager() = default;
        _ProcessManager(const _ProcessManager& root) = delete;
        _ProcessManager& operator=(const _ProcessManager&) = delete;
    public:
        static _ProcessManager& getInstance() {
            static _ProcessManager instance;
            return instance;
        }

        int32_t getCapacity() {
            return _capacity;
        }

        int32_t getFree() {
            return _free;
        }

        uint32_t getInterval() {
            return _interval;
        }

        bool begin(int32_t capacity = 8);
        bool close();
        bool checkProcess(ProcessUnit *processUnit);
        ProcessUnit *registerProcess(ProcessHandler handler, uint32_t interval, void *context);
        bool unregisterProcess(ProcessUnit *processUnit);
        bool unregisterAllProcess();
        void handler();
};

extern _ProcessManager &ProcessManager;
#endif
