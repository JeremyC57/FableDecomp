#ifndef FABLE_ENGINE_NUISYSTEM_CSCROLLINGCOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CSCROLLINGCOMPONENT_H
// NUISystem::CScrollingComponent (new, chunk 765): RTTI .?AVCScrollingComponent@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x012482CC (+0, slot 0 the ??_G 0x00547260), 0x012482A8 (+4, CObserver) and 0x012482A0 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CScrollingComponent@NUISystem@@QAE@J@Z, ??0CScrollingComponent@NUISystem@@QAE@ABV01@@Z, ??1CScrollingComponent@NUISystem@@UAE@XZ.
// The constructor 0x00546F40 is the base call and the three vptrs; the ??_G 0x00547260 inlines an empty destructor.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CScrollingComponent : public CChangingStateComponent {
public:
    CScrollingComponent(long);
    CScrollingComponent(const CScrollingComponent&);
    virtual ~CScrollingComponent();
};
}
#endif
