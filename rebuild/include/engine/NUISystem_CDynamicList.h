#ifndef FABLE_ENGINE_NUISYSTEM_CDYNAMICLIST_H
#define FABLE_ENGINE_NUISYSTEM_CDYNAMICLIST_H
// NUISystem::CDynamicList (new, chunk 765): RTTI .?AVCDynamicList@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01246E44 (+0, slot 0 the ??_G 0x00541870), 0x01246E20 (+4, CObserver) and 0x01246E18 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CDynamicList@NUISystem@@QAE@J@Z, ??0CDynamicList@NUISystem@@QAE@ABV01@@Z, ??1CDynamicList@NUISystem@@UAE@XZ.
// The destructor 0x00541350 stores the three vptrs, frees the 12-byte STLport vector at +0x174 and calls
// ~CChangingStateComponent 0x0052CCF0; its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CDynamicList : public CChangingStateComponent {
public:
    CDynamicList(long);
    CDynamicList(const CDynamicList&);
    virtual ~CDynamicList();
protected:
    void GetDataFromDef();                                          // ?GetDataFromDef@CDynamicList@NUISystem@@IAEXXZ
};
}
#endif
