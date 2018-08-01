#ifndef PWM_H
#define PWM_H

#include "common.h"

#define BACKLIGHT_ROOT "/sys/class/backlight/backlight"
#define BACKLIGHT_VALUE   BACKLIGHT_ROOT "/brightness"
#define BACKLIGHT_MAX     BACKLIGHT_ROOT "/max_brightness"



#define PWM_ROOT      "/sys/class/pwm/pwmchip%d"
#define PWM_EXPORT    PWM_ROOT "/export"
#define PWM_UNEXPORT  PWM_ROOT "/unexport"
#define PWM_PERIOD    PWM_ROOT "/pwm0/period"
#define PWM_CYCLE     PWM_ROOT "/pwm0/duty_cycle"
#define PWM_ENABLE    PWM_ROOT "/pwm0/enable"

#define PWM 2

int backlight_set_value(int value);


/*** PWM functions ***/
/* PWM export */
int pwm_export(unsigned int pwm);
/* PWM unexport */
int pwm_unexport(unsigned int pwm);
/* PWM configuration */
int pwm_config(unsigned int pwm, unsigned int period, unsigned int duty_cycle);
/* PWM enable */
int pwm_enable(unsigned int pwm);
/* PWM disable */
int pwm_disable(unsigned int pwm);




#endif // PWM_H
