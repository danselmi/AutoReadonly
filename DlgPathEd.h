#ifndef DLGPATHED_H
#define DLGPATHED_H

//(*Headers(DlgPathEd)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class DlgPathEd: public wxDialog
{
	public:

		DlgPathEd(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~DlgPathEd();

		//(*Declarations(DlgPathEd)
		wxTextCtrl* TextCtrlPath;
		wxButton* ButtonOK;
		wxButton* ButtonBrowse;
		wxButton* ButtonCancel;
		//*)
		
		wxString GetPath();
        void SetPath(wxString const& path);
	protected:

		//(*Identifiers(DlgPathEd)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL_PATH;
		static const long ID_BUTTON_BROWSE;
		static const long ID_BUTTON_OK;
		static const long ID_BUTTON_CANCEL;
		//*)

	private:

		//(*Handlers(DlgPathEd)
		void OnButtonBrowseClick(wxCommandEvent& event);
		void OnButtonOKClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
