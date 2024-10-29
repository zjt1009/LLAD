#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>

#if defined(CONFIG_OPT_POLL_IRQ)
#include "normal-signal.h"
#include "normal-poll.h"
#else
#include "normal-signal.h"
#include "interrupt-handler.h"
#endif

#include "stats.h"
#include "params.h"
#include "main-test.h"
#include "debugfs-utils.h"
#include "gpio-ops.h"

MODULE_LICENSE("GPL");

u64	stats_begin = DEFAULT_BEGIN;
u64	stats_end = DEFAULT_END;
u64	stats_slice = DEFAULT_SLICE;
u64	stats_sleep = DEFAULT_SLEEP;
u64	stats_interval = DEFAULT_INTERVAL;
u64	stats_repeat = DEFAULT_REPEAT;
u8	stats_debug = DEFAULT_DEBUG;


char	cmd[MAX_CMD_LINE_LEN];

u8	status;


static void start_signal(void)
{
	normal_signal_start();
}

static void stop_signal(void)
{
	normal_signal_stop();
}

static void signal_init(void)
{
	normal_signal_init();
}

static void signal_release(void)
{
	normal_signal_release();
}

static void handlers_init(void)
{

#if	defined(CONFIG_OPT_POLL_IRQ)
	printk(KERN_INFO "normal poll used\n");
	normal_poll_register();
#else
	printk(KERN_INFO "normal interrupt used\n");
	gpio_interrupt_register();
#endif
}

static void handlers_release(void)
{

#if	defined(CONFIG_OPT_POLL_IRQ)
	normal_poll_release();
#else
    gpio_interrupt_release();
#endif
}

int start_test(void)
{
	stats_init();


	#if defined(CONFIG_OPT_POLL_IRQ)
		normal_poll_start();
	#endif

	start_signal();

	set_status(STATUS_RUNNING);

	return 0;
}

int stop_test(void)
{
	stop_signal();


	#if defined(CONFIG_OPT_POLL_IRQ)
		normal_poll_stop();
	#endif

	print_statistics();

	stats_delete();

	set_status(STATUS_DONE);

	return 0;
}

int print_test(void)
{
	print_statistics();

	return 0;
}

static int __init watch_irq_init( void )
{


	printk("Watch irq test installed.\n");

	debugfs_init();

	gpio_pins_init();

	handlers_init();

	signal_init();

	set_status(STATUS_NONE);

	return 0;
}

static void __exit watch_irq_exit( void )
{

	printk("Watch irq test uninstalled.\n");

	set_status(STATUS_NONE);

	handlers_release();

	signal_release();

	gpio_pins_release();

	debugfs_delete();

}

module_init(watch_irq_init);
module_exit(watch_irq_exit);  
