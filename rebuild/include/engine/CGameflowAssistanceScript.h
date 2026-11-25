#ifndef FABLE_ENGINE_CGAMEFLOWASSISTANCESCRIPT_H
#define FABLE_ENGINE_CGAMEFLOWASSISTANCESCRIPT_H
// NScript::CGameflowAssistanceScript (NF436, new header; no includer before NF436). Base CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> (FableWin.pdb LF_BCLASS) with the
// class's own members.
// Layout: Ego_r.pdb CGameflowAssistanceScript (struct_layouts_egor.tsv, 124 bytes) at retail offsets = PDB - 0x14 (retail CScriptBase is 0x40; no STLport
// container in the class); retail size 0x68 (the landed Alloc 0x00CF0640 news 0x68). Member names / types: Ego_r.pdb and FableWin.pdb.
// Methods (FableWin.pdb LF_CLASS field list, all public; Ego_r.pdb has no out-of-line ctor copy, so it is inline; the destructor is
// compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the virtual overrides below
// (FableWin.pdb ??_7CGameflowAssistanceScript@NScript@@6B@ slot order; retail vftable 0x012C5DE0). The class's other methods are not declared yet.
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGameflowAssistanceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CGameflowAssistanceScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00CF0640
    virtual void RegisterMain();                            // slot 1  0x00CEFA20
    virtual void Main();                                    // slot 2  0x00CEFAC0
    virtual void Init();                                    // slot 3  0x00CEFA00
    virtual void OnPersist(CPersistContext&);               // slot 4  0x00CF0560
    long FlourishesOffered;                       // +0x48 (Ego_r.pdb +0x5c; FableWin.pdb long)
    long FlourishesPerformed;                     // +0x4c (Ego_r.pdb +0x60; FableWin.pdb long)
    long CombatMultHighestLevel;                  // +0x50 (Ego_r.pdb +0x64; FableWin.pdb long)
    long CombatMultFramesSinceChange;             // +0x54 (Ego_r.pdb +0x68; FableWin.pdb long)
    long HealthWarningsGiven;                     // +0x58 (Ego_r.pdb +0x6c; FableWin.pdb long)
    long WillEnergyWarningsGiven;                 // +0x5c (Ego_r.pdb +0x70; FableWin.pdb long)
    long SavedRenownLevel;                        // +0x60 (Ego_r.pdb +0x74; FableWin.pdb long)
    long SaveWarningsGiven;                       // +0x64 (Ego_r.pdb +0x78; FableWin.pdb long)
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CGameflowAssistanceScript) == 0x68);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CGAMEFLOWASSISTANCESCRIPT_H
