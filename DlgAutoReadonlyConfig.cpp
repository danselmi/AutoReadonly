/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Copyright: 2008 NanYU
 *
 */


#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/checklst.h>    
#endif

#include "sdk.h"
#include "configmanager.h"

#include "DlgAutoReadonlyConfig.h"
#include "AutoReadonly.h"
#include "DlgPathEd.h"

BEGIN_EVENT_TABLE(DlgAutoReadonlyConfig,wxPanel)
    EVT_BUTTON(XRCID("ID_BUTTON_ADD"),DlgAutoReadonlyConfig::OnAddPath)
    EVT_BUTTON(XRCID("ID_BUTTON_REMOVE"),DlgAutoReadonlyConfig::OnRemovePath)
    EVT_BUTTON(XRCID("ID_BUTTON_MODIFY"),DlgAutoReadonlyConfig::OnModifyPath)
    EVT_BUTTON(XRCID("ID_BUTTON_TOGGLE"),DlgAutoReadonlyConfig::OnToggleCheck)
    
    EVT_UPDATE_UI(XRCID("ID_BUTTON_REMOVE"), DlgAutoReadonlyConfig::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("ID_BUTTON_TOGGLE"), DlgAutoReadonlyConfig::OnUpdateUI)
    EVT_UPDATE_UI(XRCID("ID_BUTTON_MODIFY"), DlgAutoReadonlyConfig::OnUpdateUI)
END_EVENT_TABLE()

DlgAutoReadonlyConfig::DlgAutoReadonlyConfig(wxWindow* parent, CBAutoReadonly* plg)
    : plg_(plg)
{
    wxXmlResource::Get()->LoadObject(this,parent,_T("AutoReadonlyConfigPanel"),_T("wxPanel"));
        
    cbNonPrj_ = (XRCCTRL(*this, "CheckboxEnabledNonPrj", wxCheckBox)); 
    cbWhilteList_ = (XRCCTRL(*this, "CheckboxWhiteList", wxCheckBox)); 
    cbBlackList_ = (XRCCTRL(*this, "CheckboxBlackList", wxCheckBox));
    cbCS_ = (XRCCTRL(*this, "CheckboxCase", wxCheckBox));
    cbAlert_ = (XRCCTRL(*this, "CheckboxEnableAlert", wxCheckBox));
        
    clbPathes_ = (XRCCTRL(*this, "ID_CHECKLISTBOX_PAHTES", wxCheckListBox));
    
    cfg_ = plg_->GetAutoReadonlyCfg();
    
    QueryCfg();
}

DlgAutoReadonlyConfig::~DlgAutoReadonlyConfig()
{
    //dtor
    //Must i try to delete the return from XRCCTRL?
}

wxString DlgAutoReadonlyConfig::GetTitle() const
{
    return wxString (_("AutoReadonly settings"));
}

wxString DlgAutoReadonlyConfig::GetBitmapBaseName() const
{
    return wxT("autoreadonly"); 
}

void DlgAutoReadonlyConfig::PostCfg()
{
    this->cfg_.Reset();
        
    if (this->cbNonPrj_)
    {
        cfg_.enbAutoSetOnFileWithoutPrj_ = this->cbNonPrj_->GetValue();
    }
    
    if (this->cbBlackList_)
    {
        cfg_.enbBlackList_ = cbBlackList_->GetValue();
    }
    
    if (this->cbWhilteList_)
    {
        cfg_.enbWhiteList_ = cbWhilteList_->GetValue();
    }
    
    if (this->cbAlert_)
    {
        cfg_.enbAlert_ = cbAlert_->GetValue();
    }
    
    if (this->cbCS_)
    {
        cfg_.caseSen_ = cbCS_->GetValue();
    }
    
    if (this->clbPathes_)
    {
        unsigned int count = clbPathes_->GetCount();
        
        for (unsigned int i=0; i<count; ++i)
        {
            wxString path = clbPathes_->GetString(i);
            bool enb = clbPathes_->IsChecked(i);
            
            this->cfg_.pathes_.Add(path);
            this->cfg_.enables_.Add(enb);
        }
    }
}

void DlgAutoReadonlyConfig::QueryCfg()
{
    if (this->cbNonPrj_)
    {
        cbNonPrj_->SetValue(cfg_.enbAutoSetOnFileWithoutPrj_);
    }
    
    if (this->cbBlackList_)
    {
        cbBlackList_->SetValue(cfg_.enbBlackList_);
    }
    
    if (this->cbWhilteList_)
    {
        cbWhilteList_->SetValue(cfg_.enbWhiteList_);
    }
    
    if (this->cbAlert_)
    {
        cbAlert_->SetValue(cfg_.enbAlert_);
    }
    
    if (this->cbCS_)
    {
        cbCS_->SetValue(cfg_.caseSen_);
    }
    
    if (this->clbPathes_)
    {
        unsigned int count = cfg_.pathes_.GetCount();
        unsigned int enb_count = cfg_.enables_.GetCount();
        
        for (unsigned int i=0; i<count; ++i)
        {
            wxString path = cfg_.pathes_[i];
            bool enb = false;
            
            if ( i < enb_count)
            {
                enb = cfg_.enables_[i];
            }
            
            clbPathes_->Append(path);
            clbPathes_->Check(i, enb);
        }
    }
}

void DlgAutoReadonlyConfig::OnApply()
{
    this->PostCfg();
    plg_->GetAutoReadonlyCfg() = cfg_;

    SaveAutoReadonlyCfg(this->cfg_);    
}

void DlgAutoReadonlyConfig::OnCancel()
{
    //do nothing ...
}

void DlgAutoReadonlyConfig::OnAddPath(wxCommandEvent & event)
{
    DlgPathEd dlg(this);
    dlg.SetPath(_T(""));
    
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString path = dlg.GetPath();
        
        if (!path.IsEmpty())
        {
            if (this->clbPathes_)
            {
                clbPathes_->Append(path);
                int count = clbPathes_->GetCount();
                clbPathes_->Check(count-1, true);
            }
        }
    }
}

void DlgAutoReadonlyConfig::OnModifyPath(wxCommandEvent & event)
{
    if (!clbPathes_)
    {
        return;
    }
    
    int sel = clbPathes_->GetSelection();
    
    if (sel == -1)
    {
        return;
    }
    
    DlgPathEd dlg(this);

    wxString path = clbPathes_->GetString((unsigned int) sel);
    dlg.SetPath(path);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString path = dlg.GetPath();
        
        if (!path.IsEmpty())
        {
            clbPathes_->SetString((unsigned int)sel, path);
        }
    }
}

void DlgAutoReadonlyConfig::OnRemovePath(wxCommandEvent & event)
{
    if (!clbPathes_)
    {
        return;
    }
    
    int sel = clbPathes_->GetSelection();
    
    if (sel == -1)
    {
        return;
    }
    
    clbPathes_->Delete(sel);
}

void DlgAutoReadonlyConfig::OnToggleCheck(wxCommandEvent & event)
{
    if (clbPathes_)
    {
        for (unsigned int i=0; i<clbPathes_->GetCount(); ++i)
        {
            clbPathes_->Check(i, !clbPathes_->IsChecked(i));
        }
    }
}

void DlgAutoReadonlyConfig::OnUpdateUI(wxUpdateUIEvent& event)
{
    int sel = -1;
    
    if (clbPathes_)
    {
        sel = clbPathes_->GetSelection();    
    }
    
    event.Enable(sel != -1);
}
