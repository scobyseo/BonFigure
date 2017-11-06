#include "board.h"
#include "ps2avrGb.h"

/* controller options encoded in report length */
#define CMD_INFO                100
#define CMD_MAPPER              101
#define CMD_RGB_OPTION          133     /* uses OPTION_XXX in report */

#define CMD_KEYMAP              120     /* BEGIN - L1 - L2 - L3 - L4 */
#define CMD_DUALACTION          150
#define CMD_MACRO_BEGIN         168     /* 12 macro keys */
#define CMD_QMACRO_BEGIN        180     /* 12 quick macro keys */

/* command encoded in report index */
#define INDEX_BOOTLOADER        0x00
#define INDEX_MAPPER            0x01
#define INDEX_READY             0x90
#define INDEX_ACTION            0x91

/* command encoded in report */
#define CMD_BOOTMAPER_STOP      0x00
#define CMD_BOOTMAPER_START     0x01

/* --------------- LED options --------------- */
#define OPTION_RGB_RAINBOW      0x10
#define OPTION_RGB_COLOR1       0x11
#define OPTION_RGB_COLOR2       0x12
#define OPTION_RGB_COLOR3       0x13
#define OPTION_RGB_RAINBOW_INIT 0x1F

#define OPTION_RGB_MODE         0x20
#define OPTION_RGB_KEY_BEGIN    0x21
#define OPTION_RGB_KEY_MODE     0x22
#define OPTION_RGB_RAINBOW_MODE 0x23

#define OPTION_RGB_COUNT        0x30
#define OPTION_RGB_LEVEL	0x40

#define OPTION_SWLED_MODE       0x80
#define OPTION_SWLED_LEVEL	0x81

#define OPTION_ESC_TILDE        0x82
#define OPTION_FN_LED           0x83
#define OPTION_RGB_DELAY        0x84
#define OPTION_INDICATOR_NL     0x85
#define OPTION_INDICATOR_CL     0x86
#define OPTION_INDICATOR_SL     0x87
#define OPTION_NUMLOCK_LAYER    0x88
#define OPTION_DEBOUNCE_COUNT   0x89

#define OPTION_RGB_KEY_SET      0xA0
#define OPTION_RGB_KEY_SET1     0xA1

#define OPTION_SWLED_ENABLE     0xE0
#define OPTION_SWLED_SAVE       0xF0

#define INDICATOR_DEFAULT       0
#define INDICATOR_ON            (1 << 1)
#define INDICATOR_OFF           (1 << 2)
#define INDICATOR_INVERSE       (1 << 3)
#define INDICATOR_FN2           (1 << 4)
#define INDICATOR_FN3           (1 << 5)
#define INDICATOR_FN23          (1 << 6)

#define KEYMAP_LENGTH		120
#define KEYMAP_LAYER1		121
#define KEYMAP_LAYER2		122
#define KEYMAP_LAYER3		123
#define KEYMAP_LAYER4		124

#pragma pack(push)
#pragma pack(1)

typedef struct rgb_s {
	uint8_t  g, r, b;
} rgb_t;

typedef struct rainbow_s {
	uint8_t  idx;
	uint8_t  cmd;
	rgb_t    color[2];
} rainbow_t;

typedef enum {
	RGB_MODE_OFF, RGB_MODE_RAINBOW,
	RGB_MODE_COLOR1, RGB_MODE_COLOR2, RGB_MODE_COLOR3,
} rgb_mode_t;

typedef struct option_s {
	uint8_t   size;

	uint8_t   rgb_cnt;
	uint8_t   rgb_mode;
	uint8_t   rgb_level;
	rgb_t     rgb_color[3];
	rgb_t     rgb_rainbow[7];
	uint8_t   rgb_key_mode;
	rgb_t     rgb_key_color;
	uint8_t   rgb_rainbow_mode;
	uint8_t   rgb_delay;            /* Added in v1.1 */

	uint8_t   swled_mode;
	uint8_t   swled_level;

	uint8_t   esc_tilde;
	uint8_t   fn_led;

	uint8_t   version[3];           /* Added in v1.2 */
	uint8_t   firmware;
	uint8_t   indicator_nl;
	uint8_t   indicator_cl;
	uint8_t   indicator_sl;
	uint8_t   numlock_layer;        /* Added in v1.2.4 */
	uint8_t   debounce;             /* Added in v1.3.0 */
} option_t;

#pragma pack(pop)

#define KBD_CMD_SET       (LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_ENDPOINT_OUT)
#define KBD_CMD_GET       (LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_ENDPOINT_IN)

#define REQUEST_GET             0x01
#define REQUEST_SET             0x09

#define CMDTYPE_KEYBOARD	0x200
#define CMDTYPE_INFO		0x301
#define CMDTYPE_BOOT		0x302
#define CMDTYPE_SYSTEM		0x303
#define CMDTYPE_CONSUMER	0x304
#define CMDTYPE_MOUSE		0x305

class ps2avrGB: public Controller {
private:
	char ***ll_keymap;
	libusb_device_handle *usb;

	int SetLedOption(int option, int value);

public:
	ps2avrGB(){
		ll_keymap = NULL;
	}

	~ps2avrGB(){
		if (ll_keymap == NULL) return;
		for (int i = 0; i < nr_layers; i ++)
			delete(ll_keymap[i]);
		delete(ll_keymap);
	}

	bool LoadFile() { LoadFile("layouts/ps2avrGB.ctl"); }
	void SetUSB(libusb_device_handle *usb) { this->usb = usb; }

	int  SetKbdState(KbdState_t &state);
	int  LoadKeyMap(wxString fname);
	int  SaveKeyMap(wxString fname);
	int  RetrieveKeyMap();
	int  ApplyKeyMap();
	int  RetriveConfig();
	int  UploadConfig();
};

int ps2avrGB::SetKbdState(KbdState_t &state)
{
	char buf[CMD_MAPPER];
	int ret;

	if (state == KBD_STATE_LOADER){
		buf[1] = INDEX_BOOTLOADER;
		return libusb_control_transfer(dev, KBD_CMD_SET, REQUEST_SET,
					       CMDTYPE_INFO, 0, buf, 8, 0);
	}

	if (this->state == state)
		return 0;

	if ((state == KBD_STATE_NORMAL) || (state == KBD_STATE_NORMAL)){
		ret = libusb_control_transfer(dev, KBD_CMD_SET, REQUEST_GET,
				       CMDTYPE_INFO, 0, buf, CMD_MAPPER, 0);
		if (ret < 0)
			return -1;

		// current state: buf[0] or buf[2]? 
	}

	return 0;
}

int ps2avrGB::LoadKeyMap(wxString fname)
{
}

int ps2avrGB::SaveKeyMap(wxString fname)
{
}

int ps2avrGB::RetrieveKeyMap()
{
	unsigned char buf[KEYMAP_LENGTH];
	libusb_control_transfer(usb, KBD_CMD_GET, REQUEST_GET,
			CMDTYPE_INFO, 0, buf, KEYMAP_LAYER1, 0);
}

int ps2avrGB::ApplyKeyMap()
{
	// SetKbdState(KBD_STATE_LOADER);
	// sleep();
	// bootloadHID();
}

int ps2avrGB::RetriveConfig()
{
	unsigned char buf[CMD_INFO];
	int i;
	libusb_control_transfer(usb, KBD_CMD_GET, REQUEST_GET,
			CMDTYPE_INFO, 0, buf, CMD_INFO, 100);
	memcpy(option, buf, sizeof(option_t));

	SetConfig("RGB color1",	option.rgb_color[0]);
	SetConfig("RGB color2",	option.rgb_color[0]);
	SetConfig("RGB color3",	option.rgb_color[0]);
	SetConfig("RGB mode",	option.rgb_mode);
	SetConfig("RGB key mode", option.rgb_key_mode);
}

int ps2avrGB::SetRainbowColor(rgb_t *colors)
{
	char buf[CMD_RGB_OPTION];
	rainbow_t *rainbow;

	/* begin color setting mode */
	libusb_control_transfer(usb, KBD_CMD_SET, REQUEST_GET,
			CMDTYPE_BOOT, 0, buf, CMD_RGB_OPTION, 0);

	rainbow = buf;
	for (int i = 0; i < 5; i ++){
		rainbow[i].idx = i;
		rainbow[i].cmd = OPTION_RGB_RAINBOW;
	}
	memcpy(rainbow[1].color, colors,   6);
	memcpy(rainbow[2].color, colors+2, 6);
	memcpy(rainbow[3].color, colors+4, 6);
	memcpy(rainbow[4].color, colors+6, 3);

	/* transfer color values */
	libusb_control_transfer(usb, KBD_CMD_SET, REQUEST_SET,
			CMDTYPE_BOOT, 0, p, 40, 0);
}

int ps2avrGB::SetLedColor(int option, rgb_t color)
{
	char buf[8] = {0, option, color.g, color.r, color.b, 0, 0, 0};
	libusb_control_transfer(usb, KBD_CMD_SET, REQUEST_SET,
			CMDTYPE_INFO, 0, buf, 8, 0);
}

int ps2avrGB::SetLedOption(int option, int value)
{
	char buf[8] = {0, option, value, 0, 0, 0, 0, 0};
	libusb_control_transfer(usb, KBD_CMD_SET, REQUEST_SET,
			CMDTYPE_INFO, 0, buf, 8, 0);
}

int ps2avrGB::ApplyConfig()
{
	SetRainbowColor(option.rgb_rainbow);
	SetLedColor(OPTION_RGB_COLOR1,   option.rgb_color[0]);
	SetLedColor(OPTION_RGB_COLOR2,   option.rgb_color[1]);
	SetLedColor(OPTION_RGB_COLOR3,   option.rgb_color[2]);
	SetLedColor(OPTION_RGB_KEY_SET1, option.rgb_key_color);

	SetLedOption(OPTION_RGB_MODE,         option.rgb_mode);
	SetLedOption(OPTION_RGB_KEY_MODE,     option.rgb_key_mode);
	SetLedOption(OPTION_RGB_RAINBOW_MODE, option.rgb_rainbow_mode);
	SetLedOption(OPTION_RGB_COUNT,        option.rgb_cnt);
	SetLedOption(OPTION_RGB_LEVEL,        option.rgb_level);

	SetLedOption(OPTION_SWLED_MODE,       option.swled_mode);
	SetLedOption(OPTION_SWLED_LEVEL,      option.swled_level);

	SetLedOption(OPTION_ESC_TILDE,        option.esc_tilde);
	SetLedOption(OPTION_FN_LED,           option.fn_led);
	SetLedOption(OPTION_INDICATOR_NL,     option.indicator_nl);
	SetLedOption(OPTION_INDICATOR_CL,     option.indicator_cl);
	SetLedOption(OPTION_INDICATOR_SL,     option.indicator_sl);
	SetLedOption(OPTION_NUMLOCK_LAYER,    option.numlock_layer);
	SetLedOption(OPTION_DEBOUNCE_COUNT,   option.debounce);

	return 0;
}
