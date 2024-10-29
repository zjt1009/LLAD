#include <linux/slab.h>
#include "stats.h"
#include "params.h"
#include "debugfs-utils.h"

struct result_record record_stats;
unsigned long long out_range;
unsigned long long max_elapse;
unsigned long long timeouts;
unsigned long long stats_buffer[MAX_SLICE];
unsigned long long lost = 0;
static struct result_record record_buffer;

int stats_init(void)
{
	out_range = 0;
	max_elapse = 0;
	timeouts = (stats_interval << 1) + stats_interval;
	if(stats_slice <= 0)
		stats_slice = DEFAULT_SLICE;

	if(stats_slice > MAX_SLICE)
		stats_slice = MAX_SLICE;

	if(stats_end < stats_begin)
		stats_end = stats_begin + DEFAULT_SLICE;

        if(stats_interval <= 0)
                stats_interval = DEFAULT_INTERVAL;

        if(stats_repeat <= 0)
                stats_repeat = DEFAULT_REPEAT;

        if(stats_debug < 0)
                stats_debug = DEFAULT_DEBUG;

        memset(stats_buffer, 0, MAX_SLICE * sizeof(unsigned long long));

        memset(&record_buffer, 0, sizeof(struct result_record));

	return 0;
}

int stats_delete(void)
{
	return 0;
}

int analyse_stats(void)
{
        long index;

        if(record_buffer.elapsed <= 0)// no elapsed time means some kind of error
                return -1;

        index = (long)(((long)record_buffer.elapsed - stats_begin) * stats_slice / (stats_end - stats_begin) / NSEC_PER_USEC);

		if(index > max_elapse)
			max_elapse = index;

        if(stats_debug >= 2){
            printk(KERN_INFO "elapsed is %lld, index is %ld\n", record_buffer.elapsed, index);
        }

        if(index >= 0 && index < stats_slice)
                stats_buffer[index]++;
        else {
				out_range++;
                //printk(KERN_INFO "index is out of range\n");
	}

        return 0;
}


int print_statistics(void)
{
	int i;
	unsigned long long sum;

	for(i = 0; i < stats_slice; i++){
		printk(KERN_INFO "sum of %3d occurrence is %llu\n", i, stats_buffer[i]);
	}

	sum = 0;
	for(i = 0; i < stats_slice; i++){
		sum += stats_buffer[i];
	}
	printk(KERN_INFO "send %llu signals\n", record_buffer.s_seq);
	printk(KERN_INFO "received %llu signals\n", record_buffer.r_seq);
	printk(KERN_INFO "%llu is in range, %llu is out of range, total results is %llu\n", sum, out_range, sum + out_range);
	printk(KERN_INFO "max is %llu\n", max_elapse);
	printk(KERN_INFO "lost results is %llu\n", record_buffer.s_seq - record_buffer.r_seq);

	return 0;

}

static int set_send_time(void)
{
	record_buffer.s_seq++;

	record_buffer.s_time = ktime_get_boottime();

	return 0;
}

static int set_receive_time(void)
{
	record_buffer.r_seq++;
	record_buffer.r_time = ktime_get_boottime();
	return 0;
}

static int set_elapsed_time(void)
{

	record_buffer.elapsed = ktime_sub(record_buffer.r_time, record_buffer.s_time);

	return 0;
}

// static int varify_times(void)
// {
// 	if(ktime_to_ns(record_buffer.elapsed) >= timeouts) {
// 		printk(KERN_INFO "lost signal\n");
		
// 	}

// 	return 0;
// }

static int debug_times(void)
{
	if(stats_debug >= 2){
		printk(KERN_INFO "%llu | %llu = %llu\n", record_buffer.s_time, record_buffer.r_time, record_buffer.elapsed);
	}

	return 0;
}

int make_signal_record(void)
{
	set_send_time();

	return 0;
}


int make_signaled_record(void)
{

	set_receive_time();
	set_elapsed_time();

	//varify_times();

	debug_times();

    analyse_stats();

	return 0;
}

bool stats_should_stop(void)
{
        if(record_buffer.s_seq >= stats_repeat)
                return true;
	else
        	return false;
}
