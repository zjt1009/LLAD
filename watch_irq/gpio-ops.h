#define GPIO_21_OUT	(21)
#define GPIO_25_IN	(25)

#define	INPUT_MODE	0
#define OUTPUT_MODE	1

enum gpio_status_t {
        GPIO_STATUS_NONE,
        GPIO_STATUS_INVALID,
        GPIO_STATUS_BUSY,
        GPIO_STATUS_REQUESTED,
        GPIO_STATUS_REGISTERED,
};

int gpio_init(int gpio_number, int mode, const char *name);

extern int gpio_signal;
extern int gpio_interrupt;

int gpio_pins_init(void);
int gpio_pins_release(void);

