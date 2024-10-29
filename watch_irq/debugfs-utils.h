#include <linux/debugfs.h>
#include "params.h"

extern u64 stats_begin;
extern u64 stats_end;
extern u64 stats_slice;
extern u64 stats_interval;
extern u64 stats_sleep;
extern u64 stats_repeat;
extern u8 stats_debug;
extern u8 status;
extern char cmd[MAX_CMD_LINE_LEN];

int debugfs_init(void);
int debugfs_delete(void);
int set_status(u8 s);
