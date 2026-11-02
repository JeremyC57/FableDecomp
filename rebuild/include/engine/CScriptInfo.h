#ifndef FABLE_ENGINE_CSCRIPTINFO_H
#define FABLE_ENGINE_CSCRIPTINFO_H
#include "fable_string.h"
// Layout: Ego_r CScriptInfo sizeof 20; native pair copy cb7388 stores
// AllocFunc +0, MasterScriptDataAlloc +4, Name +8, ID +12, MasterScript +16.
class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptInfo {
public:
    CScriptBase* (__cdecl *AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);
    CScriptDataBase* (__cdecl *MasterScriptDataAlloc)();
    CCharString Name;
    long ID;
    bool MasterScript;
};
#endif
