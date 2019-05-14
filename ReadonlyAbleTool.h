#ifndef READONLYABLETOOL_H_INCLUDED
#define READONLYABLETOOL_H_INCLUDED

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

namespace CBReadonlyabletool
{

enum FileToggleReadonlyAble
{
    FILE_IS_READONLY_ON_DISK = -3, //-3 : the file is readonly on disk
    FILE_IS_MISSING = -2, //-2 : the file is miss.
    FILE_IS_MODIFIED = -1, //-1 : pls undo or save this file bef you set it to readonly

    EDITOR_UNTOGGLE_READONLYABLE = 0,
    EDITOR_TOGGLE_READONLYABLE_WITHOUT_PRJFILE = 1, //1 : can toggle readonly
    EDITOR_TOGGLE_READONLYABLE_WITH_PRJFILE= 2, //2 : can toggel readonly and this file in project
};

cbEditor* GetActiveEditor();

bool IsEditorReadonly(cbEditor* editor);
bool IsActiveEdtiorReadonly();

FileToggleReadonlyAble TestEditorReadonlyAble(cbEditor* editor, ProjectFile* &project);
FileToggleReadonlyAble TestActiveEditorReadonlyAble(ProjectFile* &project);

FileToggleReadonlyAble TestEditorReadonlyAble(cbEditor* editor);
FileToggleReadonlyAble TestActiveEditorReadonlyAble();

FileToggleReadonlyAble ToggleEditorReadonly(cbEditor* editor);
FileToggleReadonlyAble ToggleActiveEditorReadonly();

bool SetEditorReadonly(cbEditor* editor, ProjectFile* project, bool readonly = true);
bool SetActiveEditorReadonly(bool readonly = true);

ProjectFile* GetProjectFile(wxString const & filename);
ProjectFile* GetProjectFile(cbEditor* editor);
ProjectFile* GetActiveProjectFile(wxString const & filename);
}

#endif // READONLYABLETOOL_H_INCLUDED
