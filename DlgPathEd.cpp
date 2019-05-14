#include "DlgPathEd.h"

//(*InternalHeaders(DlgPathEd)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "globals.h" //cb sdk

//(*IdInit(DlgPathEd)
const long DlgPathEd::ID_STATICTEXT1 = wxNewId();
const long DlgPathEd::ID_TEXTCTRL_PATH = wxNewId();
const long DlgPathEd::ID_BUTTON_BROWSE = wxNewId();
const long DlgPathEd::ID_BUTTON_OK = wxNewId();
const long DlgPathEd::ID_BUTTON_CANCEL = wxNewId();
//*)

BEGIN_EVENT_TABLE(DlgPathEd,wxDialog)
	//(*EventTable(DlgPathEd)
	//*)
END_EVENT_TABLE()

DlgPathEd::DlgPathEd(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(DlgPathEd)
	wxStaticText* StaticText1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	
	Create(parent, id, _("Autoreadonly path"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(400,152));
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Input a auto-read-only path:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	TextCtrlPath = new wxTextCtrl(this, ID_TEXTCTRL_PATH, wxEmptyString, wxDefaultPosition, wxSize(353,22), 0, wxDefaultValidator, _T("ID_TEXTCTRL_PATH"));
	BoxSizer2->Add(TextCtrlPath, 1, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonBrowse = new wxButton(this, ID_BUTTON_BROWSE, _("..."), wxDefaultPosition, wxSize(30,24), 0, wxDefaultValidator, _T("ID_BUTTON_BROWSE"));
	BoxSizer2->Add(ButtonBrowse, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonOK = new wxButton(this, ID_BUTTON_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_OK"));
	ButtonOK->SetDefault();
	BoxSizer3->Add(ButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonCancel = new wxButton(this, ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_CANCEL"));
	BoxSizer3->Add(ButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_BUTTON_BROWSE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DlgPathEd::OnButtonBrowseClick);
	Connect(ID_BUTTON_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DlgPathEd::OnButtonOKClick);
	Connect(ID_BUTTON_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DlgPathEd::OnButtonCancelClick);
	//*)
}

DlgPathEd::~DlgPathEd()
{
	//(*Destroy(DlgPathEd)
	//*)
}

wxString DlgPathEd::GetPath()
{
    wxString path = TextCtrlPath->GetValue().Strip();        
    return path;
}

void DlgPathEd::SetPath(wxString const& path)
{
    TextCtrlPath->SetValue(path.Strip());
}

void DlgPathEd::OnButtonBrowseClick(wxCommandEvent& event)
{
    wxString org = this->GetPath();
    
    wxString reu = ChooseDirectory(this, _("Select a AutoReadonly path:")
        , org, _T(""), false, false);
    
    if (!reu.IsEmpty())
    {
        this->TextCtrlPath->SetValue(reu);
    }
}

void DlgPathEd::OnButtonOKClick(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

void DlgPathEd::OnButtonCancelClick(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}
