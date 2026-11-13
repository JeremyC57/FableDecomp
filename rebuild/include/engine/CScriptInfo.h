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
#ifdef FABLE_RETAIL_CSCRIPTINFO_CTOR
    // Retail opt-in (chunk 4003, row 0x00CD52D0 CScriptInfoManager::RegisterScripts): the constructor and the real types of the two
    // allocator members. FableWin.pdb ??0CScriptInfo@@QAE@ABVCCharString@@J_NP6IPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@ZP6IPAV3@XZ@Z
    // (name, id, master script, alloc, master data alloc; the COMDAT right after RegisterScripts in that build): both pointers are
    // __fastcall and the script allocator takes the interface by reference, as under FABLE_RETAIL_CSCRIPTINFO_ACCESSORS (the landed
    // Alloc rows, e.g. 0x00CDD550, are `static __fastcall`). Retail expands the constructor at all 161 sites of RegisterScripts: Name is
    // copy-constructed first (0x00CD531E), then ID, MasterScript, AllocFunc and MasterScriptDataAlloc are stored in the parameter order
    // (0x00CD5337..0x00CD5348). Same layout as the other branches.
    typedef CScriptBase* (FABLE_FASTCALL *tAllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase&);
    typedef CScriptDataBase* (FABLE_FASTCALL *tMasterScriptDataAllocFunc)();
    CScriptInfo(const CCharString& name, long id, bool master_script, tAllocFunc alloc_func, tMasterScriptDataAllocFunc master_script_data_alloc)
        : Name(name)
    {
        ID = id;
        MasterScript = master_script;
        AllocFunc = alloc_func;
        MasterScriptDataAlloc = master_script_data_alloc;
    }
    tAllocFunc AllocFunc;
    tMasterScriptDataAllocFunc MasterScriptDataAlloc;
    CCharString Name;
    long ID;
    bool MasterScript;
#ifdef FABLE_RETAIL_CSCRIPTINFO_ACCESSORS
    tAllocFunc GetAllocFunc() const { return AllocFunc; }
    tMasterScriptDataAllocFunc GetMasterScriptDataAllocFunc() const { return MasterScriptDataAlloc; }
    bool IsMasterScript() const { return MasterScript; }
#endif
#elif defined(FABLE_RETAIL_CSCRIPTINFO_ACCESSORS)
    // Retail opt-in (wf4, row 0x004B3CE0 CQuestManager::ActivateQuests): the allocator pointer types and the inline accessors as both
    // PDBs spell them -- ?GetAllocFunc@CScriptInfo@@QBEP6IPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@ZXZ,
    // ?GetMasterScriptDataAllocFunc@CScriptInfo@@QBEP6IPAVCScriptDataBase@@XZXZ, ?IsMasterScript@CScriptInfo@@QBE_NXZ (Ego_r.pdb and
    // FableWin.pdb): both pointers are __fastcall (P6I) and the script allocator takes the interface by reference. Inline in retail:
    // 0x004B3F0B `mov eax,[info+4] ; mov cl,[eax+10h]`, 0x004B3F17 `call [eax+4]`, 0x004B3F43 `mov eax,[ecx] ; mov ecx,<data> ; call eax`.
    CScriptBase* (__fastcall *AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase&);
    CScriptDataBase* (__fastcall *MasterScriptDataAlloc)();
    CCharString Name;
    long ID;
    bool MasterScript;
    CScriptBase* (__fastcall *GetAllocFunc() const)(CScriptDataBase*, CGameScriptInterfaceBase&) { return AllocFunc; }
    CScriptDataBase* (__fastcall *GetMasterScriptDataAllocFunc() const)() { return MasterScriptDataAlloc; }
    bool IsMasterScript() const { return MasterScript; }
#else
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
#endif
#ifdef FABLE_RETAIL_CSCRIPTINFO_GETNAME
    // Retail opt-in (chunk 890, row 0x0056AD40 CTCBoastUI::Activate): FableWin.pdb ?GetName@CScriptInfo@@QBEABVCCharString@@XZ
    // (0x0239E200; no Ego_r.pdb copy, inline in retail: Activate copies the CCharString at PScriptInfo +8 in place, 0x0056AE18 /
    // 0x0056AF40). Member function only: no layout change.
    const CCharString& GetName() const { return Name; }
#endif
};
#endif
