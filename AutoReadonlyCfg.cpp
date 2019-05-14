#include "sdk.h" // Code::Blocks SDK
#include "configmanager.h"

#include "AutoReadonlyCfg.h"

void AutoReadonlyCfg::Reset()
{
    enbAutoSetOnFileWithoutPrj_ = false;
    enbBlackList_ = true;
    enbAlert_ = false;
    caseSen_ = false;
    
    pathes_.Clear();
    enables_.Clear();
}

#define CFG_AUTOREADONLY_ENABLE_NONPRJ_ENTRY _T("/AutoReadonly/EnableNonPrj")
#define CFG_AUTOREADONLY_ENABLE_BLACKLIST_ENTRY _T("/AutoReadonly/EnableBlackList")
#define CFG_AUTOREADONLY_ENABLE_WHITELIST_ENTRY _T("/AutoReadonly/EnableWhiteList")
#define CFG_AUTOREADONLY_ENABLE_ALERT_ENTRY _T("/AutoReadonly/EnableAlert")
#define CFG_AUTOREADONLY_ENABLE_CASE_ENTRY _T("/AutoReadonly/EnableCaseSensitive")
#define CFG_AUTOREADONLY_PATHES_ENBRY _T("/AutoReadonly/Pathes")

void LoadAutoReadonlyCfg(AutoReadonlyCfg & ar)
{
    ar.Reset();
    
    if (ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor")))
    {
        ar.enbAutoSetOnFileWithoutPrj_ = cfg->ReadBool(CFG_AUTOREADONLY_ENABLE_NONPRJ_ENTRY, false);
        ar.enbBlackList_ = cfg->ReadBool(CFG_AUTOREADONLY_ENABLE_BLACKLIST_ENTRY, false);
        ar.enbWhiteList_ = cfg->ReadBool(CFG_AUTOREADONLY_ENABLE_WHITELIST_ENTRY, false);
        ar.enbAlert_ = cfg->ReadBool(CFG_AUTOREADONLY_ENABLE_ALERT_ENTRY, false);
        ar.caseSen_ = cfg->ReadBool(CFG_AUTOREADONLY_ENABLE_CASE_ENTRY, false);
        
        wxArrayString tmppathes;
        
        cfg->Read(CFG_AUTOREADONLY_PATHES_ENBRY, &tmppathes);
        
        #define PATH_ENABLE_PRE_FLAG _T("-Y-")
        #define PATH_ENABLE_PRE_FLAG_SIZE 3
        
        for (int i=0; i<tmppathes.GetCount(); ++i)
        {
            wxString tmp = tmppathes[i];
            bool enb = false;
            
            if (!tmp.IsEmpty())
            {
                unsigned int len = tmp.Length();
                
                if (len > PATH_ENABLE_PRE_FLAG_SIZE 
                    && tmp.Mid(0, PATH_ENABLE_PRE_FLAG_SIZE) == PATH_ENABLE_PRE_FLAG)
                {
                    enb = true;
                    tmp = tmp.Mid(PATH_ENABLE_PRE_FLAG_SIZE, len-PATH_ENABLE_PRE_FLAG_SIZE);
                }
            }
            
            ar.enables_.Add(enb);
            ar.pathes_.Add(tmp);
        }
    }
    
}

void SaveAutoReadonlyCfg(AutoReadonlyCfg const & ar)
{
    if(ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor")))
    {
        cfg->Write(CFG_AUTOREADONLY_ENABLE_NONPRJ_ENTRY, ar.enbAutoSetOnFileWithoutPrj_);
        cfg->Write(CFG_AUTOREADONLY_ENABLE_BLACKLIST_ENTRY, ar.enbBlackList_);
        cfg->Write(CFG_AUTOREADONLY_ENABLE_WHITELIST_ENTRY, ar.enbWhiteList_);
        cfg->Write(CFG_AUTOREADONLY_ENABLE_ALERT_ENTRY, ar.enbAlert_);
        cfg->Write(CFG_AUTOREADONLY_ENABLE_CASE_ENTRY, ar.caseSen_);
        
        wxArrayString tmppathes;
        
        int enb_count = ar.enables_.GetCount();
        
        for (int i=0; i<ar.pathes_.GetCount(); ++i)
        {
            wxString tmp = ar.pathes_[i];
            
            if (i < enb_count && ar.enables_[i])
            {
                tmp = wxString(PATH_ENABLE_PRE_FLAG) + tmp;
            }
            
            tmppathes.Add(tmp);
        }
        
        #undef PATH_ENABLE_PRE_FLAG_SIZE
        #undef PATH_ENABLE_PRE_FLAG

        cfg->Write(CFG_AUTOREADONLY_PATHES_ENBRY, tmppathes);
    }
}

#undef CFG_AUTOREADONLY_PATHES_ENBRY
#undef CFG_AUTOREADONLY_ENABLE_CASE_ENTRY
#undef CFG_AUTOREADONLY_ENABLE_ALERT_ENTRY
#undef CFG_AUTOREADONLY_ENABLE_BLACKLIST_ENTRY
#undef CFG_AUTOREADONLY_ENABLE_WHITELIST_ENTRY
#undef CFG_AUTOREADONLY_ENABLE_NONPRJ_ENTRY

namespace
{
    enum BWFlag
    {
        bwfWhiteList,bwfBlackList
    };
}

bool FileIsInCfgList(AutoReadonlyCfg const & cfg, wxString const & fn, BWFlag black_white)
{
    unsigned int c1 = cfg.pathes_.GetCount();
    unsigned int c2 = cfg.enables_.GetCount();
    
    wxString tmp = fn;
    
    if (!cfg.caseSen_)
        tmp.MakeUpper();
        
    wxFileName wxFN(tmp);
    
    wxString path_of_fn = wxFN.GetPath();
    
    if (path_of_fn.Length() > 3 
        && path_of_fn[1] == wxChar(':') && path_of_fn[2] == wxChar('\\'))
    {
        if (!path_of_fn.EndsWith(_T("\\")))
        {
            path_of_fn += _T("\\");
        }
    }
    else if (path_of_fn.Length() > 2 //NOTE: code about path for linux but no test.
        && path_of_fn[0] == wxChar('/') && path_of_fn[1] == wxChar('/'))
    {
        if (!path_of_fn.EndsWith(_T("/")))
        {
            path_of_fn += _T("/");
        }
    }
            
    for (unsigned int i=0; i<c1; ++i)
    {
        if (i<c2 && (cfg.enables_[i] == black_white))
        {
            wxString path = cfg.pathes_[i];
            
            if (!cfg.caseSen_)
            {
                path.MakeUpper();
            }
            
            unsigned int len = path.Length();

            if (path_of_fn.Mid(0, len) == path)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool FileIsInCfgBlackList(AutoReadonlyCfg const & cfg, wxString const & fn)
{
    return FileIsInCfgList(cfg, fn, bwfBlackList);
}

bool FileIsInCfgWhiteList(AutoReadonlyCfg const & cfg, wxString const & fn)
{
    return FileIsInCfgList(cfg, fn, bwfWhiteList);
}
