#ifndef FABLE_ENGINE_NUISYSTEM_CSPELLCONTAINER_H
#define FABLE_ENGINE_NUISYSTEM_CSPELLCONTAINER_H
// NUISystem::CSpellContainer (new, chunk 765): RTTI .?AVCSpellContainer@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124786C (+0, slot 0 the ??_G 0x005460A0), 0x01247848 (+4, CObserver) and 0x01247840 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CSpellContainer@NUISystem@@QAE@J@Z, ??0CSpellContainer@NUISystem@@QAE@ABV01@@Z, ??1CSpellContainer@NUISystem@@UAE@XZ.
// The destructor 0x00545840 removes the observer from CManager, then the compiler destroys the BaseSpells vector.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CSpellContainer : public CChangingStateComponent {
public:
    CSpellContainer(long);
    CSpellContainer(const CSpellContainer&);
    virtual ~CSpellContainer();
    void GetDataFromDef();
};
}
#endif
