#ifndef AUTOREADONLYCFG_H
#define AUTOREADONLYCFG_H

#include <wx/dynarray.h>
#include <wx/arrstr.h>

WX_DEFINE_ARRAY_INT(bool, ArrayOfPathEnable);

struct AutoReadonlyCfg
{
    AutoReadonlyCfg()
        : enbAutoSetOnFileWithoutPrj_(false)
         , enbBlackList_(true)
         , enbWhiteList_(false)
         , caseSen_(false)
         , enbAlert_(false)
    {}
    
    void Reset();        
    
    bool enbAutoSetOnFileWithoutPrj_;
    bool enbBlackList_;
    bool enbWhiteList_;
    
    wxArrayString pathes_;
    
    ArrayOfPathEnable enables_;
    
    bool caseSen_;
    bool enbAlert_;
};

void LoadAutoReadonlyCfg(AutoReadonlyCfg & ar);
void SaveAutoReadonlyCfg(AutoReadonlyCfg const & ar);

bool FileIsInCfgBlackList(AutoReadonlyCfg const & cfg, wxString const & fn);
bool FileIsInCfgWhiteList(AutoReadonlyCfg const & cfg, wxString const & fn);

#endif // AUTOREADONLYCFG_H
