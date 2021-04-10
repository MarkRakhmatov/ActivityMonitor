#include "MainFrame.h"

MainFrame::MainFrame(OnExitCallbackType onExit)
    : wxFrame(NULL, wxID_ANY, "Activity Monitor")
    , mMenuFile(new wxMenu)
    , mMenuHelp(new wxMenu)
    , mMenuBar(new wxMenuBar)
    , mOnExit(onExit)
{
    mMenuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    mMenuFile->AppendSeparator();
    mMenuFile->Append(wxID_EXIT);
    mMenuHelp->Append(wxID_ABOUT);
    mMenuBar->Append(mMenuFile, "&File");
    mMenuBar->Append(mMenuHelp, "&Help");
    SetMenuBar(mMenuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    mOnExit();
    Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}
void MainFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}