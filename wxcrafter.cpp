//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"


// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;


MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer37 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer37);
    
    m_notebook131 = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxBK_DEFAULT);
    
    boxSizer37->Add(m_notebook131, 1, wxALL|wxEXPAND, 0);
    
    m_panel152 = new wxPanel(m_notebook131, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    m_notebook131->AddPage(m_panel152, _("Keymap"), false);
    
    wxBoxSizer* boxSizer213 = new wxBoxSizer(wxVERTICAL);
    m_panel152->SetSizer(boxSizer213);
    
    wxBoxSizer* boxSizer234 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer213->Add(boxSizer234, 0, wxALL|wxEXPAND, 5);
    
    wxArrayString m_choiceLayerArr;
    m_choiceLayer = new wxChoice(m_panel152, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_choiceLayerArr, 0);
    
    boxSizer234->Add(m_choiceLayer, 0, wxALL, 5);
    
    m_buttonDefault = new wxButton(m_panel152, wxID_ANY, _("Default"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer234->Add(m_buttonDefault, 0, wxALL, 5);
    
    m_buttonReset = new wxButton(m_panel152, wxID_ANY, _("Reset"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer234->Add(m_buttonReset, 0, wxALL, 5);
    
    boxSizer234->Add(0, 0, 1, wxALL, 5);
    
    m_buttonCopyLayer = new wxButton(m_panel152, wxID_ANY, _("Copy to :"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer234->Add(m_buttonCopyLayer, 0, wxALL, 5);
    
    wxArrayString m_choiceCopyLayerArr;
    m_choiceCopyLayer = new wxChoice(m_panel152, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_choiceCopyLayerArr, 0);
    
    boxSizer234->Add(m_choiceCopyLayer, 0, wxALL, 5);
    
    wxBoxSizer* boxSizer51 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer213->Add(boxSizer51, 1, wxALL|wxEXPAND, 5);
    
    wxBoxSizer* boxSizer53 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer213->Add(boxSizer53, 1, wxALL|wxEXPAND, 5);
    
    m_panel179 = new wxPanel(m_notebook131, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    m_notebook131->AddPage(m_panel179, _("Macro"), false);
    
    m_panel1910 = new wxPanel(m_notebook131, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    m_notebook131->AddPage(m_panel1910, _("Configurations"), false);
    
    m_menuBar = new wxMenuBar(0);
    this->SetMenuBar(m_menuBar);
    
    m_name6 = new wxMenu();
    m_menuBar->Append(m_name6, _("File"));
    
    m_menuItem81 = new wxMenuItem(m_name6, wxID_ANY, _("New\tCtrl-N"), wxT(""), wxITEM_NORMAL);
    m_name6->Append(m_menuItem81);
    
    m_menuItem83 = new wxMenuItem(m_name6, wxID_ANY, _("Open\tCtrl-O"), wxT(""), wxITEM_NORMAL);
    m_name6->Append(m_menuItem83);
    
    m_menuItem85 = new wxMenuItem(m_name6, wxID_ANY, _("Save\tCtrl-S"), wxT(""), wxITEM_NORMAL);
    m_name6->Append(m_menuItem85);
    
    m_name6->AppendSeparator();
    
    m_menuItem91 = new wxMenuItem(m_name6, wxID_ANY, _("Download"), wxT(""), wxITEM_NORMAL);
    m_name6->Append(m_menuItem91);
    
    m_menuItem93 = new wxMenuItem(m_name6, wxID_ANY, _("Upload"), wxT(""), wxITEM_NORMAL);
    m_name6->Append(m_menuItem93);
    
    m_name6->AppendSeparator();
    
    m_menuItem7 = new wxMenuItem(m_name6, wxID_EXIT, _("Exit\tCtrl-Q"), _("Quit"), wxITEM_NORMAL);
    m_name6->Append(m_menuItem7);
    
    m_name8 = new wxMenu();
    m_menuBar->Append(m_name8, _("Help"));
    
    m_menuItem9 = new wxMenuItem(m_name8, wxID_ABOUT, _("About..."), wxT(""), wxITEM_NORMAL);
    m_name8->Append(m_menuItem9);
    
    m_mainToolbar = this->CreateToolBar(wxTB_FLAT, wxID_ANY);
    m_mainToolbar->SetToolBitmapSize(wxSize(16,16));
    
    SetSizeHints(800,600);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
    // Connect events
    this->Connect(m_menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Connect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(m_menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    
}