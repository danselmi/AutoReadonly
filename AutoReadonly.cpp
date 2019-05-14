/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Copyright: 2008 NanYu (Xiamen Fujian China)
 *
 */

#include <sdk.h> // Code::Blocks SDK

#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/checklst.h>
#endif

#include "configmanager.h"
#include "configurationpanel.h"

#include "AutoReadonly.h"
#include "DlgAutoReadonlyConfig.h"
#include "ReadonlyAbleTool.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<CBAutoReadonly> reg(_T("AutoReadonly"));

    long idToggleActiveEditorReadonly = wxNewId();
    long idToggleOpenedEditorReadonly = wxNewId();
}

// events handling
BEGIN_EVENT_TABLE(CBAutoReadonly, cbPlugin)
    // add any events you want to handle here
    EVT_MENU(idToggleActiveEditorReadonly, CBAutoReadonly::OnToggleActiveEditorReadonly)
   // EVT_MENU(idToggleOpenedEditorReadonly, CBAutoReadonly::OnToggleOpenedEditorReadonly)
END_EVENT_TABLE()

using namespace CBReadonlyabletool;

// constructor
CBAutoReadonly::CBAutoReadonly()
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("AutoReadonly.zip")))
    {
        NotifyMissingFile(_T("AutoReadonly.zip"));
    }
}

// destructor
CBAutoReadonly::~CBAutoReadonly()
{
}

void CBAutoReadonly::LoadCfg()
{
    LoadAutoReadonlyCfg(this->cfg_);
}

void CBAutoReadonly::SaveCfg()
{
    SaveAutoReadonlyCfg(this->cfg_);
}

void CBAutoReadonly::OnAttach()
{
    // do whatever initialization you need for your plugin
    // after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...

    this->LoadCfg();

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_ACTIVATED
        , new cbEventFunctor<CBAutoReadonly, CodeBlocksEvent>(this, &CBAutoReadonly::OnEditorEvent));

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_DEACTIVATED
        , new cbEventFunctor<CBAutoReadonly, CodeBlocksEvent>(this, &CBAutoReadonly::OnEditorEvent));

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_OPEN
        , new cbEventFunctor<CBAutoReadonly, CodeBlocksEvent>(this, &CBAutoReadonly::OnEditorEvent));

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_CLOSE
        , new cbEventFunctor<CBAutoReadonly, CodeBlocksEvent>(this, &CBAutoReadonly::OnEditorEvent));
}

void CBAutoReadonly::OnRelease(bool appShutDown)
{
    // do de-initialization for your plugin
    // if appShutDown is true, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // after this function, the inherited member variable
    // m_IsAttached will be FALSE...

    this->SaveCfg();
}

cbConfigurationPanel* CBAutoReadonly::GetConfigurationPanel(wxWindow* parent)
{
    if ( !IsAttached() )
        return NULL;

    return (new DlgAutoReadonlyConfig(parent, this));
}

void CBAutoReadonly::BuildMenu(wxMenuBar* menuBar)
{
    //The application is offering its menubar for your plugin,
    //to add any menu items you want...
    //Append any items you need in the menu...
    //Be careful in here... The application's menubar is at your disposal.

    if (!m_IsAttached || !menuBar)
    {
        return;
    }

    int idx = menuBar->FindMenu(_("&Edit"));

    if (idx != wxNOT_FOUND)
    {
        if (wxMenu* menu = menuBar->GetMenu(idx))
        {
            DoBuildMenu(menu, idToggleActiveEditorReadonly, true);
        }
    }
}

void CBAutoReadonly::DoBuildMenu(wxMenu* menu, long cmdId, bool hotkey)
{
    wxString title = (hotkey)? _("&Readonly\tCtrl-Alt-R") : _("Readonly");

    wxMenuItemList& items = menu->GetMenuItems();

    wxMenuItem* itemTmp = new wxMenuItem(menu,
                            cmdId
                            , title
                            , _("Toggle current active file's read-only status")
                            , wxITEM_CHECK);

    size_t i = items.GetCount();

    //just append with seperator
    menu->InsertSeparator(i++);
    menu->Insert(i, itemTmp);

    bool eb = (EDITOR_UNTOGGLE_READONLYABLE != TestActiveEditorReadonlyAble());
    bool ck = (eb? (IsActiveEdtiorReadonly()) : false);

    menu->Enable(idToggleActiveEditorReadonly, eb);
    menu->Check(idToggleActiveEditorReadonly, ck);
}

void CBAutoReadonly::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    //Some library module is ready to display a pop-up menu.
    //Check the parameter \"type\" and see which module it is
    //and append any items you need in the menu...
    //for consistency, add a separator as the first item...

    if (!IsAttached() || !menu)
    {
        return;
    }

    switch(type)
    {
//        case mtOpenFilesList :
//            BuildOpenFileLstMenu(menu,data);
//            break;

        case mtEditorManager:
            BuildEditorMenu(menu, data);
            break;

        default :
            break;
    }
}

void CBAutoReadonly::BuildEditorMenu(wxMenu* menu, const FileTreeData* /*data*/)
{
    if (menu)
    {
        DoBuildMenu(menu, idToggleActiveEditorReadonly);
    }
}

void CBAutoReadonly::BuildOpenFileLstMenu(wxMenu* menu, const FileTreeData* data)
{
    if (menu)
    {
        if (data)
        {
            selectedOpenedFileName_ = data->GetFolder();
        }

        DoBuildMenu(menu, idToggleOpenedEditorReadonly);
    }
}

void CBAutoReadonly::OnEditorEvent(CodeBlocksEvent& event)
{
    struct EventSkiper
    {
        EventSkiper(wxEvent* evt) : evt_(evt) {}
        ~EventSkiper() {evt_->Skip(true);}
        wxEvent* evt_;
    } event_skiper(&event);

    EditorBase* baseEdtior = event.GetEditor();

    wxString filename = event.GetString();
    wxEventType type = event.GetEventType();

    if (cbEVT_EDITOR_CLOSE == type)
    {
        if (ProjectFile* file_in_prj = GetProjectFile(filename))
        {
            file_in_prj->SetFileState(fvsNormal);
        }

        return;
    }

    if (!baseEdtior->IsBuiltinEditor())
    {
        return;
    }

    cbEditor* editor = dynamic_cast<cbEditor*>(baseEdtior);
    if (!editor)
    {
        return;
    }

    if (filename.IsEmpty())
    {
        filename = editor->GetFilename();
    }

    ProjectFile* file_in_prj = 0;
    FileToggleReadonlyAble able = TestEditorReadonlyAble(editor, file_in_prj);

    bool need_alert = false;

    if (cbEVT_EDITOR_OPEN == type)
    {
        if (!(cfg_.enbWhiteList_ && FileIsInCfgWhiteList(cfg_, filename)))
        {
            if (cfg_.enbAutoSetOnFileWithoutPrj_
                && able == EDITOR_TOGGLE_READONLYABLE_WITHOUT_PRJFILE) //file no in project
            {
                SetEditorReadonly(editor, file_in_prj, true);
                need_alert = true;
            }
            else if (cfg_.enbBlackList_)
            {
                if (FileIsInCfgBlackList(cfg_, filename))
                {
                    SetEditorReadonly(editor, file_in_prj, true);
                    need_alert = true;
                }
            }
        }
    }

    UpdateReadonlyMenuItem(able);

    if (need_alert && cfg_.enbAlert_)
    {
        wxString fmt = _("%s\nis changed to read-only status.");
        wxString msg;
        msg.Printf(fmt, filename.c_str());

        cbMessageBox(msg, _("AutoReadonly message"));
    }

    event.Skip(true);
}

void CBAutoReadonly::UpdateReadonlyMenuItem(int able)
{
    if (wxMenuBar* mbar = Manager::Get()->GetAppFrame()->GetMenuBar())
    {
        bool eb = (able != EDITOR_UNTOGGLE_READONLYABLE);
        bool ck = (eb? IsActiveEdtiorReadonly() : false);

        mbar->Enable(idToggleActiveEditorReadonly, eb);
        mbar->Check(idToggleActiveEditorReadonly, ck);
    }
}

void CBAutoReadonly::OnToggleActiveEditorReadonly(wxCommandEvent& event)
{
    FileToggleReadonlyAble able = ToggleActiveEditorReadonly();
    UpdateReadonlyMenuItem(able);
}

void CBAutoReadonly::OnToggleOpenedEditorReadonly(wxCommandEvent& event)
{
    if (selectedOpenedFileName_.IsEmpty())
    {
        return;
    }

    EditorManager * em = Manager::Get()->GetEditorManager();

    if (!em)
    {
        return;
    }

    EditorBase* eb = em->GetEditor(selectedOpenedFileName_);

    if(!eb || !eb->IsBuiltinEditor())
    {
        return;
    }

    cbEditor* editor = dynamic_cast<cbEditor*> (eb);

    if (!editor)
    {
        return;
    }

    FileToggleReadonlyAble able = ToggleEditorReadonly(editor);
    UpdateReadonlyMenuItem(able);

    selectedOpenedFileName_.Clear();
}
