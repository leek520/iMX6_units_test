#ifndef ADC_H
#define ADC_H


#include "common.h"

#define ADC_ROOT "/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0"
#define ADC_VALUE     ADC_ROOT "/in_voltage%d_raw"
#define ADC_SET_FREQ  ADC_ROOT "/in_voltage_sampling_frequency"
#define ADC_FREQ      ADC_ROOT "/sampling_frequency_available"
#define ADC_SCALE     ADC_ROOT "/in_voltage_scale"

#define SCALE 0.805664062       // scale=3.3v/4096*1000

#define ADC_PORT 3

int adc_get_value(unsigned int adc_in);

#endif // ADC_H
