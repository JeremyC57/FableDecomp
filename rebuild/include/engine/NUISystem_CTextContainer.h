#ifndef FABLE_ENGINE_NUISYSTEM_CTEXTCONTAINER_H
#define FABLE_ENGINE_NUISYSTEM_CTEXTCONTAINER_H
// NUISystem::CTextContainer (new, chunk 765): RTTI .?AVCTextContainer@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01248064 (+0, slot 0 the ??_G 0x00546F00), 0x0124803C (+4, CObserver) and 0x01248034 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CTextContainer@NUISystem@@QAE@J@Z, ??0CTextContainer@NUISystem@@QAE@ABV01@@Z, ??1CTextContainer@NUISystem@@UAE@XZ.
// No user destructor body (the ??_G 0x00546F00 inlines the vptr stores and calls ~CChangingStateComponent); the constructor
// 0x00546D30 is the base call, the vptrs and GetDataFromDef 0x00546CD0.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CTextContainer : public CChangingStateComponent {
public:
    CTextContainer(long);
    CTextContainer(const CTextContainer&);
    virtual ~CTextContainer();
    void GetDataFromDef();                                          // 0x00546CD0
};
}
#endif
