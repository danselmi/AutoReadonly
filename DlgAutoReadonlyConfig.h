/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Copyright: 2008 NanYU
 *
 */


#ifndef DLGAUTOREADONLYCONFIG_H
#define DLGAUTOREADONLYCONFIG_H

#include <configurationpanel.h>

#include "AutoReadonlyCfg.h"

class CBAutoReadonly;

class DlgAutoReadonlyConfig : public cbConfigurationPanel
{
    public:
        DlgAutoReadonlyConfig(wxWindow* parent, CBAutoReadonly* plg);
        virtual ~DlgAutoReadonlyConfig();

        virtual wxString GetTitle() const;
        virtual wxString GetBitmapBaseName() const;
        virtual void OnApply();
        virtual void OnCancel();

    protected:
        void OnAddPath(wxCommandEvent & event);
        void OnModifyPath(wxCommandEvent & event);
        void OnRemovePath(wxCommandEvent & event);
        void OnToggleCheck(wxCommandEvent & event);
        
        void OnUpdateUI(wxUpdateUIEvent& event);
    
    private:
        CBAutoReadonly* plg_;
        AutoReadonlyCfg cfg_;
        
        wxCheckBox* cbNonPrj_, *cbWhilteList_, *cbBlackList_, *cbAlert_, *cbCS_;
        wxCheckListBox* clbPathes_;
        
        void PostCfg();
        void QueryCfg();
            
        DECLARE_EVENT_TABLE()
};

#endif // DLGAUTOREADONLYCONFIG_H
