#include <wx/app.h>
#include <wx/event.h>
#include "MainFrame.h"
#include <wx/image.h>

#include "board.h"

BoardPool boards;

// Define the MainApp
class MainApp : public wxApp
{
private:
	libusb_context *ctx;
	libusb_device **devs;
	struct libusb_device_descriptor desc;
	int nr_usb;

public:
	MainApp() {}
	virtual ~MainApp() {}
	bool InitUSB();

	virtual bool OnInit()
	{
		// Add the common image handlers
		wxImage::AddHandler( new wxPNGHandler );
		wxImage::AddHandler( new wxJPEGHandler );

		boards.LoadControllers();
		InitUSB();
	}

	virtual int OnExit()
	{
		libusb_free_device_list(devs, 1);
		libusb_exit(ctx);
		return 0;
	}
};

bool MainApp::InitUSB()
{
	int ret = libusb_init(&ctx);
	if (ret < 0){
		printf("USB initialization failure\n");
		return false;
	}

	nr_usb = libusb_get_device_list(ctx, &devs);
	for (int i = 0; i < nr_usb; i ++){
		int busnum = libusb_get_bus_number(devs[i]);
		int devnum = libusb_get_device_address(devs[i]);

		libusb_get_device_descriptor(devs[i], &desc);
		wxArrayString blist =
			boards.GetBoards(desc.idVendor, desc.idProduct);
		if (blist.GetCount() == 0)
			continue;

		libusb_device_handle *usb;
		int id = libusb_open(devs[i], &usb);
		libusb_claim_interface(usb, 0);

		MainFrame *mainFrame = new MainFrame(NULL, usb, blist);
		SetTopWindow(mainFrame);
		GetTopWindow()->Show();
	}

	return true;
}

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)
