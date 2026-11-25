#ifndef FABLE_ENGINE_CQ_ORCHARDFARMRAIDEVILSCRIPT_H
#define FABLE_ENGINE_CQ_ORCHARDFARMRAIDEVILSCRIPT_H
// NScript::CQ_OrchardFarmRaidEvilScript (NF434, new header; no includer before NF434). The quest-script default shape: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> with no
// members of its own.
// Layout: Ego_r.pdb CQ_OrchardFarmRaidEvilScript (struct_layouts_egor.tsv, 92 bytes) at retail size = PDB - 0x14 = 0x48 (retail CScriptBase is
// 0x40); the landed Alloc 0x00DD2300 news 0x48.
// Methods (FableWin.pdb LF_CLASS field list; the ctor is user-declared and Ego_r.pdb has no out-of-line copy, so it is inline; the
// destructor is compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the public virtual overrides
// below, in the FableWin.pdb ??_7CQ_OrchardFarmRaidEvilScript@NScript@@6B@ slot order (retail vftable 0x012DB7B0).
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_OrchardFarmRaidEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x00DD2300
    virtual void RegisterMain();                            // slot 1  0x00DD2240
    virtual void Main();                                    // slot 2  0x00DD2010
    virtual void Init();                                    // slot 3  0x00DD2020
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OrchardFarmRaidEvilScript) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_ORCHARDFARMRAIDEVILSCRIPT_H
