#ifndef FABLE_ENGINE_NUISYSTEM_CTEXTCONTAINER_H
#define FABLE_ENGINE_NUISYSTEM_CTEXTCONTAINER_H
// NUISystem::CTextContainer (new, chunk 765): RTTI .?AVCTextContainer@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01248064 (+0, slot 0 the ??_G 0x00546F00), 0x0124803C (+4, CObserver) and 0x01248034 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CTextContainer@NUISystem@@QAE@J@Z, ??0CTextContainer@NUISystem@@QAE@ABV01@@Z, ??1CTextContainer@NUISystem@@UAE@XZ.
// No user destructor body (the ??_G 0x00546F00 inlines the vptr stores and calls ~CChangingStateComponent); the constructor
// 0x00546D30 is the base call, the vptrs and GetDataFromDef 0x00546CD0.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
#ifdef FABLE_RETAIL_CTEXTCONTAINER_MEMBERS
#include "engine/C2DBoxF.h"
#endif
namespace NUISystem {
class CTextContainer : public CChangingStateComponent {
public:
    CTextContainer(long);
    CTextContainer(const CTextContainer&);
    virtual ~CTextContainer();
    void GetDataFromDef();                                          // 0x00546CD0
#ifdef FABLE_RETAIL_CTEXTCONTAINER_MEMBERS
    // Retail opt-in (chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent: `push 0x16c ; call operator new` before the
    // constructor 0x00546D30): the Ego_r.pdb data members (struct_layouts_egor.tsv) 0x30 lower in retail (STLport 4.6 container sizes),
    // so sizeof is retail's 0x16c. Members only: no row reads them yet.
protected:
    C2DBoxF Window;                              // +0x15c (Ego_r +396)
#endif
};
}
#ifdef FABLE_RETAIL_CTEXTCONTAINER_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NUISystem::CTextContainer) == 0x16c);
#endif
#endif
