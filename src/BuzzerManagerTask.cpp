#include "BuzzerManagerTask.h"
#include <Arduino.h>

BuzzerManagerTask buzzer(BUZZER_PIN, true);

BuzzerManagerTask::BuzzerManagerTask(int pin, bool activeLow)
    : _pin(pin), _activeLow(activeLow), _head(nullptr), _tail(nullptr), _startTime(0), _state(State::IDLE) {}

void BuzzerManagerTask::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, _activeLow ? HIGH : LOW);
}

void BuzzerManagerTask::setup() {
    begin();
}

unsigned long BuzzerManagerTask::loop(MicroTasks::WakeReason reason) {
    unsigned long currentMillis = millis();

    if (_head) {
        BeepMessage *msg = _head;

        switch (_state) {
            case State::IDLE:
                if (msg->getDuration() > 0) {
                    //Serial.printf("Suono buzzer per %d ms, delay %d ms\n", msg->getDuration(), msg->getDelay());
                    digitalWrite(_pin, _activeLow ? LOW : HIGH);
                    _startTime = currentMillis;
                    _state = State::BEEPING;
                } else {
                    //Serial.printf("Silenzio per %d ms\n", msg->getDelay());
                    _startTime = currentMillis;
                    _state = State::WAITING;
                }
                break;

            case State::BEEPING:
                if (currentMillis - _startTime >= (unsigned long)msg->getDuration()) {
                    digitalWrite(_pin, _activeLow ? HIGH : LOW);
                    _startTime = currentMillis;
                    _state = State::WAITING;
                }
                break;

            case State::WAITING:
                if (currentMillis - _startTime >= (unsigned long)msg->getDelay()) {
                    _head = _head->getNext();
                    delete msg;
                    _state = State::IDLE;
                }
                break;
        }
    }

    return 0;
}

void BuzzerManagerTask::enqueueBeep(int duration, int delayMs) {
    BeepMessage *msg = new BeepMessage(duration, delayMs);
    if (!_head) {
        _head = _tail = msg;
    } else {
        _tail->setNext(msg);
        _tail = msg;
    }
}

void BuzzerManagerTask::suona(int duration, int delayMs) {
    enqueueBeep(duration, delayMs);
}
/* Funzione per suonare sequenze di punti (.) e trattini (_)
    il punto    '.' corrisponde a un suono di 150 ms
    il trattino '_' corrisponde a un suono di 400 ms
    lo spazio   ' ' corrisponde a un silenzio di 150 ms
*/
void BuzzerManagerTask::suonaSequenza(const char* sequenza) {
    for (const char* p = sequenza; *p != '\0'; ++p) {
        switch (*p) {
            case '.': enqueueBeep(150, 50); break;
            case '_': enqueueBeep(400, 50); break;
            case ' ': enqueueBeep(0, 150); break;
        }
    }
    //stampa seriale suono inviato
    if(strcasecmp(sequenza, BuzzerManagerTask::Suoni::avvio) == 0){
        Serial.printf("Buzzer: suonaSequenza 'avvio'\n");
    }else if(strcasecmp(sequenza, BuzzerManagerTask::Suoni::carta_accettata) == 0){
        Serial.printf("Buzzer: suonaSequenza 'carta accettata'\n");
    }else if(strcasecmp(sequenza, BuzzerManagerTask::Suoni::carta_nonRiconosciuta) == 0){
        Serial.printf("Buzzer: suonaSequenza 'carta non riconosciuta'\n");
    }else{
        Serial.printf("Buzzer: suonaSequenza personalizzata \"%s\"\n", sequenza);
    }
}

// --- BeepMessage implementation ---
BuzzerManagerTask::BeepMessage::BeepMessage(int duration, int delay)
    : _next(nullptr), _duration(duration), _delay(delay) {}