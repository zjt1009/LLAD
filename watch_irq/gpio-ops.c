#include <linux/gpio.h>

#include "gpio-ops.h"
#include "debugfs-utils.h"

int gpio_signal;
int gpio_interrupt;

int gpio_init(int gpio_number, int mode, const char *name)
{
        //Output GPIO configuration
        //Checking the GPIO is valid or not
        if(gpio_is_valid(gpio_number) == false){
                pr_err("GPIO %d is not valid\n", gpio_number);
		return -EINVAL;
        }

        //Requesting the GPIO
        if(gpio_request(gpio_number, name) < 0){
		pr_err("ERROR: GPIO %d request\n", gpio_number);
		return -EBUSY;
        }

        //configure the GPIO as output
	switch(mode){
		case INPUT_MODE:
			gpio_direction_input(gpio_number);
			break;
		case OUTPUT_MODE:
			gpio_direction_output(gpio_number, 0);
			break;
		default:
			break;
	}

	return 0;
}

int gpio_init_signal(int gpio_number)
{
        int ret;

        ret = gpio_init(gpio_number, OUTPUT_MODE, "signal");

        return ret < 0 ? ret : gpio_number;
}

int gpio_init_interrupt(int gpio_number)
{
        int ret;

        ret = gpio_init(gpio_number, INPUT_MODE, "interrupt");

        return ret < 0 ? ret : gpio_number;
}

int gpio_pins_init(void)
{

	gpio_signal = gpio_init_signal(GPIO_21_OUT);

	gpio_interrupt = gpio_init_interrupt(GPIO_25_IN);

	if(stats_debug >= 2)
		printk(KERN_INFO "init gpio signal is %d, gpio interrupt is %d\n", gpio_signal, gpio_interrupt);

        if(gpio_signal < 0 || gpio_interrupt < 0){
		if(stats_debug >= 2)
			printk(KERN_INFO "gpio pins init failed\n");
                return -1;
	}

        return 0;
}

int gpio_pins_release(void)
{
	if(gpio_signal > 0)
		gpio_free(gpio_signal);

	if(gpio_interrupt > 0)
		gpio_free(gpio_interrupt);

        return 0;
}
