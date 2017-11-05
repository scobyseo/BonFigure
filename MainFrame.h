#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

class MainFrame : public MainFrameBaseClass
{
private:
	libusb_device_handle *usb;

public:
	MainFrame(wxWindow* parent, libusb_device_handle *usb);
	virtual ~MainFrame();

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

protected:
	virtual void onDownload(wxCommandEvent& event);
	virtual void onNew(wxCommandEvent& event);
	virtual void onOpen(wxCommandEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void onUpload(wxCommandEvent& event);
};
#endif // MAINFRAME_H
