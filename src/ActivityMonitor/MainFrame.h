#include "wx/wx.h"
#include <functional>

using OnExitCallbackType = std::function<void()>;

class MainFrame : public wxFrame
{
public:
    virtual ~MainFrame()
    {
        mOnExit();
    }
    MainFrame(OnExitCallbackType onExit);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
private:
    wxMenu* mMenuFile;
    wxMenu* mMenuHelp;
    wxMenuBar* mMenuBar;
    OnExitCallbackType mOnExit;
};

enum
{
    ID_Hello = 1
};