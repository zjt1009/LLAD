#include "debugfs-utils.h"
#include "main-test.h"

static struct dentry *root_dir;

static ssize_t cmd_read(struct file *f, char *buffer, size_t len, loff_t *offset)
{
	return simple_read_from_buffer(buffer, len, offset, cmd, strlen(cmd));
}

static ssize_t cmd_write(struct file *f, const char *buffer, size_t len, loff_t *offset)
{
	ssize_t ret;

	ret = simple_write_to_buffer(cmd, PAGE_SIZE, offset, buffer, len);

	if(ret < 0)
		return ret;
	else {

		if(!strncmp(buffer, "start", strlen("start"))){
			printk(KERN_INFO "start command\n");
			set_status(STATUS_RUNNING);
			start_test();
		}

		if(!strncmp(buffer, "stop", strlen("stop"))){
			printk(KERN_INFO "stop command\n");
			set_status(STATUS_DONE);
			stop_test();
		}

		if(!strncmp(buffer, "print", strlen("print"))){
			printk(KERN_INFO "print command\n");
			print_test();
		}

		return ret;
	}
}

const struct file_operations cmd_fops = {
    .owner = THIS_MODULE,
    .read = cmd_read,
    .write = cmd_write,
};

int debugfs_init(void)
{
	root_dir = debugfs_create_dir(DEBUGFS_ROOT_DIR, NULL);

	debugfs_create_u64("begin", 0644, root_dir, &stats_begin);
	debugfs_create_u64("end", 0644, root_dir, &stats_end);
	debugfs_create_u64("slice", 0644, root_dir, &stats_slice);
	debugfs_create_u64("interval", 0644, root_dir, &stats_interval);
	debugfs_create_u64("sleep", 0644, root_dir, &stats_sleep);
	debugfs_create_u64("repeat", 0644, root_dir, &stats_repeat);
	debugfs_create_u8("debug", 0644, root_dir, &stats_debug);
	debugfs_create_u8("status", 0644, root_dir, &status);

	debugfs_create_file("command", 0644, root_dir, NULL, &cmd_fops);

	strcpy(cmd, "nop\n");

	return 0;
}

int debugfs_delete(void)
{
	debugfs_remove_recursive(root_dir);
	return 0;
}

int set_status(u8 s)
{
	status = s;
	return 0;
}
