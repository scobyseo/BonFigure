#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <getopt.h>

#include <libusb.h>

libusb_device_handle *dev_handle;

/* controller options encoded in report length */
#define CMD_INFO		100
#define CMD_MAPPER		101
#define CMD_RGB_OPTION		133	/* uses OPTION_XXX in report */

#define CMD_KEYMAP		120	/* BEGIN - L1 - L2 - L3 - L4 */
#define CMD_DUALACTION		150
#define	CMD_MACRO_BEGIN		168	/* 12 macro keys */
#define	CMD_QMACRO_BEGIN	180	/* 12 quick macro keys */

/* command encoded in report index */
#define INDEX_BOOTLOADER	0x00
#define INDEX_MAPPER		0x01
#define INDEX_READY		0x90
#define INDEX_ACTION		0x91

/* command encoded in report */
#define CMD_BOOTMAPER_STOP	0x00
#define CMD_BOOTMAPER_START	0x01

/* --------------- LED options --------------- */
#define OPTION_RGB_RAINBOW	0x10
#define OPTION_RGB_COLOR1	0x11
#define OPTION_RGB_COLOR2	0x12
#define OPTION_RGB_COLOR3	0x13
#define OPTION_RGB_RAINBOW_INIT	0x1F

#define OPTION_RGB_MODE		0x20
#define OPTION_RGB_KEY_BEGIN	0x21
#define OPTION_RGB_KEY_MODE	0x22
#define OPTION_RGB_RAINBOW_MODE	0x23

#define OPTION_RGB_COUNT	0x30
#define OPTION_RGB_BRIGHTNESS	0x40

#define OPTION_SWLED_MODE	0x80
#define OPTION_SWLED_BRIGHTNESS	0x81

#define OPTION_ESC_TILDE	0x82
#define OPTION_FN_LED		0x83
#define OPTION_RGB_DELAY	0x84
#define OPTION_INDICATOR_NL	0x85
#define OPTION_INDICATOR_CL	0x86
#define OPTION_INDICATOR_SL	0x87
#define OPTION_NUMLOCK_LAYER	0x88
#define OPTION_DEBOUNCE_COUNT	0x89

#define OPTION_RGB_KEY_SET	0xA0
#define OPTION_RGB_KEY_SET1	0xA1

#define OPTION_SWLED_ENABLE	0xE0
#define OPTION_SWLED_SAVE	0xF0

#define	INDICATOR_DEFAULT	0
#define	INDICATOR_ON		1
#define	INDICATOR_OFF		2
#define	INDICATOR_INVERSE	3
#define	INDICATOR_FN2		4
#define	INDICATOR_FN3		5
#define	INDICATOR_FN23		6

#pragma pack(push)
#pragma pack(1)

typedef struct rgb_s {
	uint8_t  g, r, b;
} rgb_t;

typedef enum {
	RGB_MODE_OFF, RGB_MODE_RAINBOW,
	RGB_MODE_COLOR1, RGB_MODE_COLOR2, RGB_MODE_COLOR3,
} rgb_mode_t;

typedef struct option_s {
	uint8_t   size;

	uint8_t   rgb_cnt;
	uint8_t   rgb_mode;
	uint8_t   rgb_brightness;
	rgb_t	  rgb_color[3];
	rgb_t	  rgb_rainbow[7];
	uint8_t   rgb_key_mode;
	rgb_t	  rgb_key_color;
	uint8_t   rgb_rainbow_mode;
	uint8_t   rgb_delay;		/* Added in v1.1 */

	uint8_t   swled_mode;
	uint8_t   swled_brightness;

	uint8_t   esc_tilde;
	uint8_t   fn_led;

	uint8_t   version[3];		/* Added in v1.2 */
	uint8_t   firmware;
	uint8_t   indicator_nl;
	uint8_t   indicator_cl;
	uint8_t   indicator_sl;
	uint8_t   numlock_layer;	/* Added in v1.2.4 */
	uint8_t   debounce;		/* Added in v1.3.0 */
} option_t;

#pragma pack(pop)

#define KBD_CMD_SET	  (LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_ENDPOINT_OUT)
#define KBD_CMD_GET	  (LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_ENDPOINT_IN)

#define REQUEST_GET		0x01
#define REQUEST_SET		0x09

#define USBHID_CMD_KEYBOARD	0x200
#define USBHID_CMD_INFO	0x301
#define USBHID_CMD_BOOT	0x302
#define USBHID_CMD_SYSTEM	0x303
#define USBHID_CMD_CONSUMER	0x304
#define USBHID_CMD_MOUSE	0x305

int set_bootloader(int is_bootloader)
{
	char buf[8] = {0, INDEX_BOOTLOADER, 0, 0, 0, 0, 0, 0};
	libusb_control_transfer(dev_handle, KBD_CMD_SET,
				REQUEST_SET, USBHID_CMD_INFO, 0,
				buf, 8, 0);
}

int set_bootmapper(int mode)	/* DEPRECATED */
{
	char buf[8] = {0, INDEX_MAPPER, mode, 0, 0, 0, 0, 0};
	libusb_control_transfer(dev_handle, KBD_CMD_SET,
				REQUEST_SET, USBHID_CMD_INFO, 0,
				buf, 8, 0);
}

int toggle_bootmapper()		/* Since 1.3? */
{
	char buf[CMD_MAPPER];
	libusb_control_transfer(dev_handle, KBD_CMD_SET,
				REQUEST_GET, USBHID_CMD_INFO, 0,
				buf, CMD_MAPPER, 0);
	//buf[1] --> current mode
}

int set_led_state(int state)
{
	char buf[8] = {0, state, 0, 0, 0, 0, 0, 0};
	libusb_control_transfer(dev_handle, KBD_CMD_SET,
				REQUEST_SET, USBHID_CMD_KEYBOARD, 0,
				buf, 8, 0);
}

int set_led_option(int option, int value)
{
	char buf[8] = {0, option, value, 0, 0, 0, 0, 0};
	libusb_control_transfer(dev_handle, KBD_CMD_SET,
				REQUEST_SET, USBHID_CMD_INFO, 0, buf, 8, 0);
//	libusb_bulk_transfer(dev_handle, 81, buf, 8, &len, 0);
}

int set_rgb_mode(rgb_mode_t mode)
{
	/* RGB mode: off - rainbow - c1 - c2 - c3 - ... */
	set_led_option(OPTION_RGB_MODE, mode);
}

int set_rgb_count(int n)
{
	set_led_option(OPTION_RGB_COUNT, n);
}

int set_rgb_brightness(int value)	/* 0 < brightness <= 250 */
{
	set_led_option(OPTION_RGB_BRIGHTNESS, value);
}

int set_rgb_delay(int delay)	/* 0 < delay <= 250*/
{
	set_led_option(OPTION_RGB_DELAY, delay);
}

int set_rgb_rainbow_color()
{
	char buf[133];
	char p[] = {
		0, OPTION_RGB_RAINBOW, 0  , 0  , 0  , 0  , 0  , 0,
		1, OPTION_RGB_RAINBOW, 255, 0  , 0  , 0  , 255, 0,
		2, OPTION_RGB_RAINBOW, 0  , 0  , 255, 0  , 255, 255,
		3, OPTION_RGB_RAINBOW, 255, 0  , 255, 255, 255, 0,
		4, OPTION_RGB_RAINBOW, 0  , 0  , 128, 0  , 0  , 0
	};
	/* begin color setting mode */
	libusb_control_transfer(dev_handle, KBD_CMD_SET,
				REQUEST_GET, USBHID_CMD_BOOT, 0, buf, 133, 0);
	/* transfer color values */
	libusb_control_transfer(dev_handle, KBD_CMD_SET,
				REQUEST_SET, USBHID_CMD_BOOT, 0, p, 40, 0);
}

int set_esc_tilde(int enable)
{
	set_led_option(OPTION_ESC_TILDE, enable);
}

int set_numlock_layer(int value)
{
	set_led_option(OPTION_NUMLOCK_LAYER, value);
}

int set_debounce(int value)
{
	set_led_option(OPTION_DEBOUNCE_COUNT, value);
}

char *rgb_color(char *buf, rgb_t rgb)
{
	sprintf(buf, "%02x%02x%02x", rgb.r, rgb.g, rgb.b);
	buf[6] = '\0';
	return buf;
}

int get_options(option_t *option)
{
	unsigned char buf[CMD_INFO];
	int i;
	libusb_control_transfer(dev_handle, KBD_CMD_GET,
				REQUEST_GET, USBHID_CMD_INFO, 0,
				buf, CMD_INFO, 100);
	memcpy(option, buf, sizeof(option_t));
	printf("-- RGB settings ---------------- \n"
	       "  count: %d  mode: %d  type: %d  brightness: %d  event: %d\n",
	       option->rgb_cnt, option->rgb_mode, option->rgb_rainbow_mode,
	       option->rgb_brightness, option->rgb_key_mode);
	printf("  color: %s - %s - %s  key color: %s\n",
	       rgb_color(buf, option->rgb_color[0]),
	       rgb_color(buf+10, option->rgb_color[0]),
	       rgb_color(buf+20, option->rgb_color[0]),
	       rgb_color(buf, option->rgb_key_color));
	printf("  rainbow: %s - %s - %s - %s - %s - %s - %s\n\n",
	       rgb_color(buf, option->rgb_rainbow[0]),
	       rgb_color(buf+10, option->rgb_rainbow[1]),
	       rgb_color(buf+20, option->rgb_rainbow[2]),
	       rgb_color(buf+30, option->rgb_rainbow[3]),
	       rgb_color(buf+40, option->rgb_rainbow[4]),
	       rgb_color(buf+50, option->rgb_rainbow[5]),
	       rgb_color(buf+60, option->rgb_rainbow[6]));
	printf("-- LED settings ---------------- \n"
	       "  mode: %d   brightness: %d\n\n"
	       "-- Keyboard settings ----------- \n"
	       "  esc-tilde: %d  fnled: %x  numlock layer: %d  debounce: %d\n"
	       "  numlock: %x  capslock: %x  scrlock: %x\n"
	       "  version: %d.%d.%d (firmware: %d)\n",
	       option->swled_mode, option->swled_brightness,
	       option->esc_tilde, option->fn_led,
	       option->numlock_layer, option->debounce,
	       option->indicator_nl, option->indicator_cl, option->indicator_sl,
	       option->version[0], option->version[1], option->version[2],
	       option->firmware);
}

int main(int argc, char *argv[])
{
	libusb_context *ctx;
	libusb_device *dev, **devs;
	libusb_device *bf_devs[10];
	struct libusb_device_descriptor desc;
	char buf[256];
	int num_devs, i, busnum, devnum, nr_bf_dev = 0;
	int len;

	num_devs = libusb_init(&ctx);
	if (num_devs < 0)
		printf(" inniti fail\n");

	num_devs = libusb_get_device_list(ctx, &devs);
	for (i = 0; i < num_devs; i ++){
		uint8_t bnum, dnum;
		dev = devs[i];
		busnum = libusb_get_bus_number(dev);
		devnum = libusb_get_device_address(dev);

		if ((busnum == -1) || (devnum == -1)){
			printf(" ?? \n");
		}

		libusb_get_device_descriptor(dev, &desc);
		if ((desc.idVendor == 0x20a0) && (desc.idProduct == 0x422d)){
			printf("Found bus %d dev %d\n", busnum, devnum);
			bf_devs[nr_bf_dev ++] = dev;
		}
	}

	printf("Select device: ");
	scanf("%d", &num_devs);
	if (num_devs < nr_bf_dev + 1){
		devnum = libusb_open(bf_devs[nr_bf_dev - 1], &dev_handle);
	}

	libusb_claim_interface(dev_handle, 0);

	/* BEGIN */
	option_t opt;
	get_options(&opt);
	/* E N D */

//	libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data, 4, &actual, 0);
	libusb_release_interface(dev_handle, 0);
	libusb_close(dev_handle);
	libusb_free_device_list(devs, 1);
	libusb_exit(ctx);

	return 0;
}
