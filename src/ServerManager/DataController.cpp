#include "DataController.h"

DataController::DataController() {
    _bucketID[0] = _token[0] = 0;
    _delayBetweenTicks = _maxUpdates = _currentTick = 0;
    _status = EMPTY_STATE;
    xTaskCreate(
            taskController,
            "TickData",
            3000,
            NULL,
            10,
            &taskTickHandle
    );
}

bool DataController::setNewDataSender(const char *bucketID, const char *token, int delayBetweenTicks,
                                      int maxUpdates) {
    if (_status == RUNNING)
        return false;

    _status = RUNNING;

    strncpy(this->_bucketID, bucketID, MAX_BUCKET_ID_LENGTH);
    strncpy(this->_token, token, MAX_TOKEN_BUCKET_LENGTH);

    Serial.println("time");
    _delayBetweenTicks = delayBetweenTicks;
    _maxUpdates = maxUpdates;
    _currentTick = 0;

    Serial.println(_delayBetweenTicks);

    Serial.println("Resuming taskTickHandle");
    if (taskTickHandle)
        vTaskResume(taskTickHandle);

    return true;
}

bool DataController::stopController() {
    if (_status != RUNNING)
        return false;

    _status = END;
    _bucketID[0] = 0;
    _token[0] = 0;

    Serial.println("Stoping taskTickHandle");
    if (taskTickHandle)
        vTaskSuspend(taskTickHandle);

    return true;
}

void DataController::tick() {
    if (_status != RUNNING)
        return;

    Serial.println("Ticking");
    _currentTick++;
    Serial.println(_currentTick);

    if (_currentTick >= _maxUpdates) {
        _status = END;
        if (taskTickHandle)
            vTaskSuspend(taskTickHandle);
    }
}

void DataController::taskController(void *_) {
    vTaskSuspend(NULL);

    for (;;) {
        DataController::Instance().tick();
        vTaskDelay(DataController::Instance().getDelayBetweenTicks() / portTICK_PERIOD_MS);
    }
}
