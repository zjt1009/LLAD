#ifdef CONFIG_OPT_POLL_IRQ

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>


#include "normal-poll.h"
#include "debugfs-utils.h"
#include "gpio-ops.h"
#include "stats.h"
#include "params.h"
#include "normal-signal.h"

static bool terminated = false;

static bool last_condition = true;

static struct hrtimer gpio_poll_timer;


static int normal_poll_handler(void *arg){
	static unsigned long flags = 0;
	local_irq_save(flags);
	make_signaled_record();
	local_irq_restore(flags);
	
	return IRQ_HANDLED;
}

static enum hrtimer_restart normal_poll_callback(struct hrtimer *timer){

	bool condition;

    for(;;){
		if((condition = (gpio_get_value(gpio_interrupt) == 1)) && !last_condition){
			
			normal_poll_handler(NULL);
			last_condition = condition;
			break;
			
		}
		last_condition = condition;
		if (terminated || (stats_should_stop())) {
			break;
		}

	}
	
	
	if (terminated) {
		pr_info(KERN_INFO "stopped\n");
		return HRTIMER_NORESTART;
	}
	
	if(stats_should_stop()) {
		// set_status(STATUS_DONE);
		return HRTIMER_NORESTART;
	}

	hrtimer_forward_now(timer, ns_to_ktime(stats_sleep));

	return HRTIMER_RESTART;
}


void normal_poll_start(void){
	
	terminated = false; 

	hrtimer_init(&gpio_poll_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_PINNED);
	gpio_poll_timer.function = &normal_poll_callback;
	
	hrtimer_start(&gpio_poll_timer, ns_to_ktime(stats_sleep), HRTIMER_MODE_REL_PINNED);
	
}


void normal_poll_stop(void){

	terminated = true;

	last_condition = true;

	hrtimer_cancel(&gpio_poll_timer);

	if(stats_debug >= 2 )
		pr_info(KERN_INFO "normal poll stopped\n");
}


int normal_poll_register(void){

	pr_info("Device Driver Insert...Done!!!\n");

	return 0;
}

void normal_poll_release(void){
    pr_info("Device Driver Remove...Done!!\n");
}

#endif