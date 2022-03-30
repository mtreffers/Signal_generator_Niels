#include  "button_state.h"

ButtonState::ButtonState(Button *_button1, Button *_button2,
                    void (*_button1_func)(), void (*_button2_func)(), void (*_double_press_func)()){
    this->button1 = _button1;
    this->button2 = _button2;

    this->button1_func = _button1_func;
    this->button2_func = _button2_func;
    this->double_press_func = _double_press_func;
}

void ButtonState::tick(){
    button1->tick();
    button2->tick();

    switch(this->current_state){
        case released:
            if(button1->last_state == true && button2->last_state == true){
                this->next_state = both_pressed;
            }
            else if(button1->last_state == true){
                this->next_state = b1_pressed;
            }
            else if(button2->last_state == true){
                this->next_state = b2_pressed;
            }
            else{
                this->next_state = this->current_state;
            }
            break;
        case b1_pressed:
            if(button1->last_state == false){
                this->button1_func();
                this->next_state = released;
            }
            else if(button2->last_state == true){
                this->next_state = both_pressed;
            }
            else{
                this->next_state = this->current_state;
            }
            break;
        case b2_pressed:
            if(button2->last_state == false){
                this->button2_func();
                this->next_state = released;
            }
            else if(button1->last_state == true){
                this->next_state = both_pressed;
            }
            else{
                this->next_state = this->current_state;
            }
            break;
        case both_pressed:
            if(button1->last_state == false && button2->last_state == false){
                this->double_press_func();
                this->next_state = released;
            }
            else{
                this->next_state = this->current_state;
            }
            break;
    }
    this->current_state = this->next_state;
}