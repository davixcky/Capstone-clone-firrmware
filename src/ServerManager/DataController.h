#ifndef DATACONTROLLER__H
#define DATACONTROLLER__H

#include "globals.h"

#include <Arduino.h>

#include "Display/Display.h"

class DataController {
public:

    DataController();

    enum controllerStatus {
        EMPTY_STATE,
        RUNNING,
        END
    };

    static DataController &Instance() {
        static DataController instance;
        return instance;
    }

    bool setNewDataSender(const char *bucketID, const char *token, int delayBetweenTicks, int maxUpdates);
    void tick();

    int getDelayBetweenTicks() const {
        return _delayBetweenTicks;
    }

    void increaseTick() {
        _currentTick++;
    }

    int getMaxUpdate() const {
        return _maxUpdates;
    }

    controllerStatus getStatus() const {
        return _status;
    }

    static void taskController(void *_);

    bool stopController();

private:

    int _delayBetweenTicks;
    int _maxUpdates;
    int _currentTick;
    char _bucketID[MAX_BUCKET_ID_LENGTH];
    char _token[MAX_TOKEN_BUCKET_LENGTH];
    controllerStatus _status;
    TaskHandle_t taskTickHandle;

};

#endif //DATACONTROLLER__H
