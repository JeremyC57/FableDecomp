#ifndef FABLE_ENGINE_CQR_ESCORTTRADER_MANAGERSCRIPT_H
#define FABLE_ENGINE_CQR_ESCORTTRADER_MANAGERSCRIPT_H
// NScript::CQR_EscortTrader_ManagerScript (NF436, new header; no includer before NF436). Base CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> (FableWin.pdb LF_BCLASS) with the
// class's own members.
// Layout: Ego_r.pdb CQR_EscortTrader_ManagerScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets = PDB - 0x14 (retail CScriptBase is 0x40; no STLport
// container in the class); retail size 0x58 (the landed Alloc 0x00E25970 news 0x58). Member names / types: Ego_r.pdb and FableWin.pdb.
// Methods (FableWin.pdb LF_CLASS field list, all public; Ego_r.pdb has no out-of-line ctor copy, so it is inline; the destructor is
// compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the virtual overrides below
// (FableWin.pdb ??_7CQR_EscortTrader_ManagerScript@NScript@@6B@ slot order; retail vftable 0x012E1D48). The class's other methods are not declared yet.
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTrader_ManagerScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum EscortTraderManagerPhase {
        ET_MANAGER_PHASE_ONE = 0,
        ET_MANAGER_PHASE_TWO = 1,
        ET_MANAGER_PHASE_THREE = 2,
        ET_MANAGER_PHASE_FOUR = 3,
        ET_MANAGER_PHASE_FIVE = 4,
        ET_MANAGER_PHASE_SIX = 5,
        ET_MANAGER_PHASE_SEVEN = 6,
        ET_MANAGER_PHASE_EIGHT = 7,
        ET_MANAGER_PHASE_MAX = 8
    };
    CQR_EscortTrader_ManagerScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00E25970
    virtual void RegisterMain();                            // slot 1  0x00E25690
    virtual void Main();                                    // slot 2  0x00E24E10
    virtual void Init();                                    // slot 3  0x00E24E00
    virtual void OnPersist(CPersistContext&);               // slot 4  0x00E25750
    long OrchardFarmTraderEscortCounter;          // +0x48 (Ego_r.pdb +0x5c; FableWin.pdb long)
    long DebugCounter;                            // +0x4c (Ego_r.pdb +0x60; FableWin.pdb long)
    bool Activated;                               // +0x50 (Ego_r.pdb +0x64; FableWin.pdb bool)
    EscortTraderManagerPhase OrchardFarmTraderEscortPhase; // +0x54 (Ego_r.pdb +0x68; FableWin.pdb NScript::CQR_EscortTrader_ManagerScript::EscortTraderManagerPhase)
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQR_EscortTrader_ManagerScript) == 0x58);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQR_ESCORTTRADER_MANAGERSCRIPT_H
