#ifndef FABLE_ENGINE_CEXPRESSION_FISHSCRIPT_H
#define FABLE_ENGINE_CEXPRESSION_FISHSCRIPT_H
// Retail opt-in (chunk 790): NScript::CExpression_FishScript, new header (no includer before chunk 790).
// PDB scope: namespace NScript, class key V (Ego_r.pdb / FableWin.pdb
// ??0CExpression_FishScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z, ?Main@CExpression_FishScript@NScript@@UAEXXZ,
// ?OnPersist@..., ?Alloc@...@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z).
// A global expression script: the ctor takes only the interface, so the base is CScriptBase itself with its own Game pointer
// (no master data). Layout: Ego_r.pdb CExpression_FishScript (struct_layouts_egor.tsv, 92 bytes) at retail offsets = PDB - 0x14
// (retail CScriptBase is 0x40, Ego_r 0x54): Game +0x40 (the landed ctor 0x00EE94A0 stores it), LastFishingLevel +0x44 (the landed
// OnPersist 0x00EE9590 transfers "LastFishingLevel" at +0x44), sizeof 0x48.
// Retail methods: the script vtable 0x012F54E4 (stored by the ctor 0x00EE94A0 / Alloc 0x00EE9F00): slot 1 RegisterMain 0x00EE94D0
// (landed), slot 2 Main 0x00EE95C0 (its only reference, .rdata 0x012F54EC), slot 4 OnPersist 0x00EE9590 (landed as a stand-in).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CExpression_FishScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
class CPersistContext;
namespace NScript {
class CExpression_FishScript : public CScriptBase {
public:
    CExpression_FishScript(CGameScriptInterfaceBase&);                   // 0x00EE94A0
    virtual void RegisterMain();                                         // 0x00EE94D0 (slot 1)
    virtual void Main();                                                 // 0x00EE95C0 (slot 2)
    virtual void OnPersist(CPersistContext&);                            // 0x00EE9590 (slot 4)
    CGameScriptInterfaceBase* Game;                                      // +0x40
    long LastFishingLevel;                                               // +0x44
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CExpression_FishScript) == 0x48);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CEXPRESSION_FISHSCRIPT_H
