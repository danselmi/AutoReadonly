#include "sdk.h" // Code::Blocks SDK

#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/checklst.h>    
#endif

#include <wx/filefn.h>

#include "configmanager.h"
#include "cbstyledtextctrl.h"

#include "ReadonlyAbleTool.h"

namespace CBReadonlyabletool
{

cbEditor* GetActiveEditor ()
{
    return Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
}
    
bool IsEditorReadonly(cbEditor* editor)
{
    if (!editor)
    {
        return false;
    }
    
    cbStyledTextCtrl * control = editor->GetControl();
    
    if (!control)
    {
        return false;
    }
    
    return control->GetReadOnly();    
}

bool IsActiveEdtiorReadonly()
{
    return IsEditorReadonly(GetActiveEditor());
}

FileToggleReadonlyAble TestEditorReadonlyAble(cbEditor* editor, ProjectFile*& project)
{
    project = 0;
    
    if (!editor)
    {
        return EDITOR_UNTOGGLE_READONLYABLE;
    }
    
    if (ProjectFile* pf = GetProjectFile(editor)) //get pf first..
    {
        project = pf;
    }
    
    cbStyledTextCtrl * control = editor->GetControl();
    
    if (!control)
    {
        return EDITOR_UNTOGGLE_READONLYABLE;
    }
    
    bool current_is_readonly = control->GetReadOnly();

    if (!current_is_readonly && editor->GetModified()) //?? a file which is modified... 
                                    //so, don't to ask for readonly.. 
    {                           //pls undo or save it...
        return FILE_IS_MODIFIED;
    }
    
    //the file on disk is writeable...
    wxString fn = editor->GetFilename();
    
    if (!wxFileExists(fn))
    {
        return FILE_IS_MISSING;
    }
    
    if (current_is_readonly && !wxFile::Access(fn.c_str(), wxFile::write))
    {
        return FILE_IS_READONLY_ON_DISK;
    }
    
    return ((project)? EDITOR_TOGGLE_READONLYABLE_WITH_PRJFILE 
                        : EDITOR_TOGGLE_READONLYABLE_WITHOUT_PRJFILE);
}

FileToggleReadonlyAble TestActiveEditorReadonlyAble(ProjectFile* &project)
{
    return TestEditorReadonlyAble(GetActiveEditor(), project);
}

ProjectFile* GetProjectFile(cbEditor* editor)
{
    if (ProjectFile * pf = editor->GetProjectFile())
    {
        return pf;
    }
    
    return GetProjectFile(editor->GetFilename());
}

FileToggleReadonlyAble ToggleEditorReadonly(cbEditor* editor)
{
    ProjectFile* pf= 0;
    
    FileToggleReadonlyAble able = TestEditorReadonlyAble(editor, pf);
    
    if (EDITOR_UNTOGGLE_READONLYABLE == able)
    {
        return able;
    }
    
    if (able < EDITOR_UNTOGGLE_READONLYABLE)
    {
        wxString fmt, msg;
        
        switch(able)
        {
            case FILE_IS_MODIFIED :
                fmt = _("%s is modified.\nSave or cancel the modification before it can change to read-only status.");
                break;
                
            case FILE_IS_MISSING :
                fmt = _("%s is missing.\nCan't change it's read-only status.");
                break;

            case FILE_IS_READONLY_ON_DISK :
            default :
                fmt = _("%s is a readonly file.\nIt is always in read-only status.");
                break;
        }
        
        wxString fn = ((pf)? pf->relativeFilename : editor->GetFilename());
        
        msg.Printf(fmt, fn.c_str());
        cbMessageBox(msg, _("AutoReadonly message"));
        
        return able;
    }
    
    if (cbStyledTextCtrl * control = editor->GetControl())
    {
        bool ro = !control->GetReadOnly();
        
        control->SetReadOnly(ro);
        
        if (pf)
        {
            pf->SetFileState(ro? fvsReadOnly : fvsNormal);
        }    
    }
    
    return able;
}

FileToggleReadonlyAble ToggleActiveEditorReadonly()
{
    return ToggleEditorReadonly(GetActiveEditor());
}

bool SetEditorReadonly(cbEditor* editor, ProjectFile* project, bool readonly)
{
    if (!editor)
    {
        return false;
    }
    
    if (cbStyledTextCtrl * control = editor->GetControl())
    {
        bool ro = control->GetReadOnly();
        
        if (ro != readonly)
        {
            control->SetReadOnly(readonly);

            if (project)
            {
                project->SetFileState(readonly? fvsReadOnly : fvsNormal);
            }     
            
            return true;   
        }
    } 
    
    return false;
}

bool SetActiveEditorReadonly(bool readonly)
{
    cbEditor* editor = GetActiveEditor();
    ProjectFile* project = 0;
    
    if (editor)
    {
        project = GetProjectFile(editor);
    }
    
   return SetEditorReadonly(GetActiveEditor(),project, readonly);
}

ProjectFile* GetActiveProjectFile(wxString const & filename)
{
    ProjectManager* pm = Manager::Get()->GetProjectManager();
    
    if (!pm)
    {
        return 0;
    }
    
    cbProject* project = pm->GetActiveProject();
    
    if (!project || !pm->IsProjectStillOpen(project))
    {
        return 0;
    }
    
    bool isAbsolute = (filename.Length() > 1 && filename.GetChar(1) == ':')
           || filename.StartsWith(_T("/")) 
           || filename.StartsWith(_T("\\"));    

    return project->GetFileByFilename(filename, !isAbsolute);
}

ProjectFile* GetProjectFile(wxString const & filename)
{
    ProjectManager* pm = Manager::Get()->GetProjectManager();
    
    if (!pm)
    {
        return 0;
    }
    
    ProjectsArray* pa = pm->GetProjects();
    
    if (!pa)
    {
        return 0;
    }
    
    bool isAbsolute = (filename.Length() > 1 && filename.GetChar(1) == ':')
           || filename.StartsWith(_T("/")) 
           || filename.StartsWith(_T("\\"));    
    
    for (unsigned int i=0; i<pa->GetCount(); ++i)
    {
        cbProject* project = pa->Item(i);
        
        if (project && pm->IsProjectStillOpen(project))
        {
            if (ProjectFile* f = project->GetFileByFilename(filename, !isAbsolute))
            {
                return f;
            }
        }
    }
    
    return 0;
}

FileToggleReadonlyAble TestEditorReadonlyAble(cbEditor* editor)
{
    ProjectFile* pf = 0;
    return TestEditorReadonlyAble(editor, pf);
}

FileToggleReadonlyAble TestActiveEditorReadonlyAble()
{
    ProjectFile* pf = 0;
    return TestActiveEditorReadonlyAble(pf);
}
    
}
