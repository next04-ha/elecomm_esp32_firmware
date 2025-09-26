#ifndef BUZZERMANAGERTASK_H
#define BUZZERMANAGERTASK_H

#include <Arduino.h>
#include <MicroTasks.h>

#define BUZZER_PIN 33
#define BUZZER_ON_STATE HIGH

class BuzzerManagerTask : public MicroTasks::Task {
private:
    int _pin;
    bool _activeLow;

    enum State {
        IDLE,
        BEEPING,
        WAITING
    };

    State _state;
    unsigned long _startTime;

    class BeepMessage {
    private:
        BeepMessage *_next;
        int _duration;
        int _delay;
    public:
        BeepMessage(int duration, int delay);
        BeepMessage *getNext() { return _next; }
        void setNext(BeepMessage *msg) { _next = msg; }
        int getDuration() { return _duration; }
        int getDelay() { return _delay; }
    };

    BeepMessage *_head;
    BeepMessage *_tail;

    void enqueueBeep(int duration, int delay);

protected:
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;

public:
    BuzzerManagerTask(int pin = BUZZER_PIN, bool activeLow = !(bool)(BUZZER_ON_STATE));

    void begin();

    void suona(int duration = 200, int delayMs = 50);
    void suonaSequenza(const char* sequenza);

    struct Suoni {
        static constexpr const char* avvio = ". _";
        static constexpr const char* carta_accettata = ". .";
        static constexpr const char* carta_nonRiconosciuta = "_ _ .";
    };
};

extern BuzzerManagerTask buzzer; // dichiarazione esterna

#endif // BUZZERMANAGERTASK_H