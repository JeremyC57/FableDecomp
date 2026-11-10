#ifndef FABLE_ENGINE_NUISYSTEM_CYESNO_H
#define FABLE_ENGINE_NUISYSTEM_CYESNO_H
#ifndef FABLE_RETAIL_CYESNO
#error "NUISystem_CYesNo.h: define FABLE_RETAIL_CYESNO (retail model only)"
#endif
// NUISystem::CYesNo (chunk 735, row 0x00577AC0): RTTI .?AVCYesNo@NUISystem@@ (rtti_bases.tsv: primary base
// NUISystem::CChangingStateComponent at 0, CObserver at +4, CActionParam at +24). Only the base relation is modelled (the
// yes/no dialog component CTCInventoryExperience::pYesNo points at); its own members are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CYesNo : public CChangingStateComponent {
#ifdef FABLE_RETAIL_CYESNO_CALLBACKS
    // Retail opt-in (chunk 1005, rows 0x00576C4F UndoBuyAbility / 0x00576ED1 UndoBuyStat of CTCInventoryExperience): FableWin.pdb
    // ?SetYesCallBack@CYesNo@NUISystem@@UAEXP6IXXZ@Z / ?SetNoCallBack@CYesNo@NUISystem@@UAEXP6IXXZ@Z, the class's first two new
    // virtuals after CChangingStateComponent's 0..141 (retail calls them through +0x238 / +0x23C of vtable 0x0122F884 with the
    // __fastcall callbacks EnsureUndoAbility 0x00570009 / CancelUndoAbility 0x0056E866).
public:
    virtual void SetYesCallBack(void (__fastcall* callback)());     // slot 142
    virtual void SetNoCallBack(void (__fastcall* callback)());      // slot 143
#endif
#ifdef FABLE_RETAIL_CYESNO_CTOR
    // Retail opt-in (chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent: `push 164h ; call operator new` then the
    // constructor 0x0041CADC): Ego_r.pdb / FableWin.pdb ??0CYesNo@NUISystem@@QAE@J@Z and the two Ego_r members (YesCallback +396,
    // NoCallback +400, __cdecl void function pointers) 0x30 lower in retail; sizeof 0x164.
public:
    CYesNo(long);                                                   // 0x0041CADC
protected:
    void (*YesCallback)();                                          // +0x15c
    void (*NoCallback)();                                           // +0x160
#endif
};
}
#ifdef FABLE_RETAIL_CYESNO_CTOR
FABLE_STATIC_ASSERT(sizeof(NUISystem::CYesNo) == 0x164);
#endif
#endif
