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
