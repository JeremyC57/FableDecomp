#ifndef FABLE_ENGINE_NUISYSTEM_CFRONTENDBUTTON_H
#define FABLE_ENGINE_NUISYSTEM_CFRONTENDBUTTON_H
// NUISystem::CFrontEndButton (new, chunk 765): RTTI .?AVCFrontEndButton@NUISystem@@, single direct base NUISystem::CClickable at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01249554 (+0, slot 0 the ??_G 0x0054E1C0), 0x01249530 (+4, CObserver) and 0x01249528 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CFrontEndButton@NUISystem@@QAE@J@Z, ??0CFrontEndButton@NUISystem@@QAE@ABV01@@Z, ??1CFrontEndButton@NUISystem@@UAE@XZ.
// The destructor 0x0054DED0 stores the vptrs, destroys the UIActionParams list and the two CCountedPointer<CAction> vectors,
// then ~CClickable; its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CFrontEndButton : public CClickable {
public:
    CFrontEndButton(long);
    CFrontEndButton(const CFrontEndButton&);
    virtual ~CFrontEndButton();
    void GetDataFromDef();
};
}
#endif
