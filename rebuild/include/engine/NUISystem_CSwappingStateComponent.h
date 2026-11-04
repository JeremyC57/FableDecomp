#ifndef FABLE_ENGINE_NUISYSTEM_CSWAPPINGSTATECOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CSWAPPINGSTATECOMPONENT_H
// NUISystem::CSwappingStateComponent (new, chunk 765): RTTI .?AVCSwappingStateComponent@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x012485AC (+0, slot 0 the ??_G 0x00547AB0), 0x01248584 (+4, CObserver) and 0x0124857C (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CSwappingStateComponent@NUISystem@@QAE@J@Z, ??0CSwappingStateComponent@NUISystem@@QAE@ABV01@@Z, ??1CSwappingStateComponent@NUISystem@@UAE@XZ.
// The destructor 0x005474C0 stores the vptrs, frees the SwappingStates vector storage (+0x15C) and calls ~CChangingStateComponent;
// its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CSwappingStateComponent : public CChangingStateComponent {
public:
    CSwappingStateComponent(long);
    CSwappingStateComponent(const CSwappingStateComponent&);
    virtual ~CSwappingStateComponent();
    void GetDataFromDef();
};
}
#endif
