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
#ifdef FABLE_RETAIL_CSCRIPTINFO_FASTCALL_ALLOC
    // Retail opt-in (wf1, row 0x004B4260, the same-TU body of CQuestManager::ActivateQuests 0x004B3CE0): the script allocator is
    // called __fastcall there (0x004B3F3A `mov edx,[world+38h] ; mov eax,[info] ; mov ecx,<master data> ; call eax`, no stack bytes).
    CScriptBase* (__fastcall *AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);
#else
    CScriptBase* (__cdecl *AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);
#endif
    CScriptDataBase* (__cdecl *MasterScriptDataAlloc)();
    CCharString Name;
    long ID;
    bool MasterScript;
#ifdef FABLE_RETAIL_CSCRIPTINFO_GETNAME
    // Retail opt-in (chunk 890, row 0x0056AD40 CTCBoastUI::Activate): FableWin.pdb ?GetName@CScriptInfo@@QBEABVCCharString@@XZ
    // (0x0239E200; no Ego_r.pdb copy, inline in retail: Activate copies the CCharString at PScriptInfo +8 in place, 0x0056AE18 /
    // 0x0056AF40). Member function only: no layout change.
    const CCharString& GetName() const { return Name; }
#endif
};
#endif
