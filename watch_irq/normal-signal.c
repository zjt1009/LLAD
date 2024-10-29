#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/gpio.h>
#include <linux/cpumask.h>

#include "normal-signal.h"
#include "gpio-ops.h"
#include "stats.h"
#include "params.h"
#include "debugfs-utils.h"
 
static struct hrtimer hr_timer;

static ktime_t ktime_interval, last;
static int index = 0;
 
enum hrtimer_restart gpio_signal_callback( struct hrtimer *timer )
{
	index ^= 1;

        gpio_set_value(gpio_signal, index);

	if(index)
		make_signal_record();
 
	if(stats_should_stop()) {
		set_status(STATUS_DONE);
		return HRTIMER_NORESTART;
	}

	hrtimer_forward_now(timer, ktime_interval);

	return HRTIMER_RESTART;
}
 
int normal_signal_start(void)
{
	printk("start sending signals.\n");

	index = 0;
	
    gpio_set_value(gpio_signal, index);

	hrtimer_init(&hr_timer, CLOCK_BOOTTIME, HRTIMER_MODE_REL);
 
	ktime_interval = ns_to_ktime(stats_interval);
	
	hr_timer.function = gpio_signal_callback;

	hrtimer_start(&hr_timer, ktime_interval, HRTIMER_MODE_REL);
 
	last = ktime_get_boottime();
 
	return 0;
}

void normal_signal_stop( void )
{
	int ret;


	ret = hrtimer_cancel(&hr_timer);

	printk("stop sending signals.\n");
}


void normal_signal_init(void)
{

}

void normal_signal_release(void)
{

}

