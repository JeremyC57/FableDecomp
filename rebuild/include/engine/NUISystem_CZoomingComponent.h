#ifndef FABLE_ENGINE_NUISYSTEM_CZOOMINGCOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CZOOMINGCOMPONENT_H
// NUISystem::CZoomingComponent (new, chunk 765): RTTI .?AVCZoomingComponent@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01247DC4 (+0, slot 0 the ??_G 0x00546C20), 0x01247DA0 (+4, CObserver) and 0x01247D98 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CZoomingComponent@NUISystem@@QAE@J@Z, ??0CZoomingComponent@NUISystem@@QAE@ABV01@@Z, ??1CZoomingComponent@NUISystem@@UAE@XZ.
// The destructor 0x00546660 removes the observer from CManager, then ~CChangingStateComponent.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CZoomingComponent : public CChangingStateComponent {
public:
    CZoomingComponent(long);
    CZoomingComponent(const CZoomingComponent&);
    virtual ~CZoomingComponent();
    void GetDataFromDef();
};
}
#endif
