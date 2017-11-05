#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include "kbdlayout.h"

MainFrame::MainFrame(wxWindow* parent, libusb_device_handle *usb)
    : MainFrameBaseClass(parent)
{
	this->usb = usb;

	KBDLayoutCanvas* kbd = new KBDLayoutCanvas(m_panelKBD);
	wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);	
	boxSizer1->Add(kbd, 1, wxALL | wxEXPAND, 0);
	m_panelKBD->SetSizer(boxSizer1);
	
	KBDLayoutCanvas* keycodes = new KBDLayoutCanvas(m_panelKeycodes);
	wxBoxSizer* boxSizer2 = new wxBoxSizer(wxVERTICAL);	
	boxSizer2->Add(keycodes, 1, wxALL | wxEXPAND, 0);
	m_panelKeycodes->SetSizer(boxSizer2);
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	wxUnusedVar(event);
	Close();
	libusb_release_interface(this->usb, 0);
	libusb_close(this->usb);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxUnusedVar(event);
	wxAboutDialogInfo info;
	info.SetCopyright(_("BonKUI"));
	info.SetLicence(_("GPL v3"));
	info.SetDescription(_("Unified custom keyboard configurator"));
	::wxAboutBox(info);
}

void MainFrame::onDownload(wxCommandEvent& event)
{
}

void MainFrame::onNew(wxCommandEvent& event)
{
}

void MainFrame::onOpen(wxCommandEvent& event)
{
}

void MainFrame::onSave(wxCommandEvent& event)
{
}

void MainFrame::onUpload(wxCommandEvent& event)
{
}
