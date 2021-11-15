#ifndef _PWM_H
#define _PWM_H

void pwm_enable_init();
void pwm_enable_set_frequency(uint16_t frequency);
void pwm_enable_set_duty_cycle(float duty_cycle);

#endif /* _PWM_H */
