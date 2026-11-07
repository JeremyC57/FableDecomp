#ifndef FABLE_ENGINE_NUISYSTEM_CSWAPPINGSTATECOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CSWAPPINGSTATECOMPONENT_H
// NUISystem::CSwappingStateComponent (new, chunk 765): RTTI .?AVCSwappingStateComponent@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x012485AC (+0, slot 0 the ??_G 0x00547AB0), 0x01248584 (+4, CObserver) and 0x0124857C (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CSwappingStateComponent@NUISystem@@QAE@J@Z, ??0CSwappingStateComponent@NUISystem@@QAE@ABV01@@Z, ??1CSwappingStateComponent@NUISystem@@UAE@XZ.
// The destructor 0x005474C0 stores the vptrs, frees the SwappingStates vector storage (+0x15C) and calls ~CChangingStateComponent;
// its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
#ifdef FABLE_RETAIL_CSWAPPINGSTATECOMPONENT_MEMBERS
#include "stlport/fable_containers.h"
#endif
namespace NUISystem {
class CSwappingStateComponent : public CChangingStateComponent {
public:
    CSwappingStateComponent(long);
    CSwappingStateComponent(const CSwappingStateComponent&);
    virtual ~CSwappingStateComponent();
    void GetDataFromDef();
#ifdef FABLE_RETAIL_CSWAPPINGSTATECOMPONENT_MEMBERS
    // Retail opt-in (chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent: `push 0x170 ; call operator new` before the
    // constructor 0x00547600): the Ego_r.pdb data members (struct_layouts_egor.tsv) 0x30 lower in retail (STLport 4.6 container sizes),
    // so sizeof is retail's 0x170. Members only: no row reads them yet.
protected:
    std::vector<std::pair<unsigned long,float> > SwappingStates; // +0x15c (STLport 12 bytes; Ego_r +396)
    float LastSwapTime;                          // +0x168 (Ego_r +412)
    unsigned long Seed;                          // +0x16c (Ego_r +416)
#endif
};
}
#ifdef FABLE_RETAIL_CSWAPPINGSTATECOMPONENT_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NUISystem::CSwappingStateComponent) == 0x170);
#endif
#endif
