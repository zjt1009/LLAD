#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/gpio.h>

#include "gpio-ops.h"
#include "stats.h"
#include "interrupt-handler.h"

static unsigned int irq_interrupt;

ktime_t signaled_time;

static irqreturn_t gpio_irq_handler(int irq,void *dev_id)
{
	static unsigned long flags = 0;

	local_irq_save(flags);

	make_signaled_record();

	local_irq_restore(flags);

	return IRQ_HANDLED;

}

static int register_interrupt(int gpio_number)
{

	//Get the IRQ number for our GPIO
	irq_interrupt = gpio_to_irq(gpio_interrupt);

	pr_info("irq number = %d\n", irq_interrupt);

	if (request_irq(irq_interrupt,             //IRQ number
			(void *)gpio_irq_handler,   //IRQ handler
			IRQF_TRIGGER_RISING,        //Handler will be called in raising edge
			"gpio_test",               //used to identify the device name using this IRQ
			NULL)) {                    //device id for shared IRQ
		pr_err("my_device: cannot register IRQ ");
		goto r_gpio_in;
	}

	pr_info("Device Driver Insert...Done!!!\n");

	return 0;
r_gpio_in:

	return -1;
}

static void release_interrupt(int gpio_number)
{
	if(irq_interrupt > 0)
		free_irq(irq_interrupt, NULL);

	pr_info("Device Driver Remove...Done!!\n");
}

int gpio_interrupt_register(void)
{
	return register_interrupt(GPIO_25_IN);
}

void gpio_interrupt_release(void)
{
	release_interrupt(GPIO_25_IN);
}
