#ifndef _BUTTON_STATE_H
#define _BUTTON_STATE_H

#include "Button.h"

class ButtonState {
    public:
        enum State{
            b1_pressed,
            b2_pressed,
            both_pressed,
            released
        };

        State current_state = released;

        ButtonState(Button *_button1, Button *_button2,
                    void (*_button1_func)(), void (*_button2_func)(), void (*_double_press_func)());
        void tick();

    private:
        Button *button1;
        Button *button2;

        State next_state = released;

        void (*button1_func)();
        void (*button2_func)();
        void (*double_press_func)();
};

#endif /* _BUTTON_STATE_H */