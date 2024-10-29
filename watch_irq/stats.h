#include <linux/list.h>
#include <linux/ktime.h>


#define	STATIC_ARRAY_STATS

struct result_record {
	unsigned long long s_seq;
	unsigned long long r_seq;
	ktime_t elapsed;
	ktime_t s_time;
	ktime_t r_time;

};

extern struct result_record record_stats;

int stats_init(void);
int stats_delete(void);

int make_signal_record(void);
int make_signaled_record(void);

int print_statistics(void);

bool stats_should_stop(void);
