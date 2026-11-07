#ifndef FABLE_ENGINE_NUISYSTEM_CFRONTENDBUTTON_H
#define FABLE_ENGINE_NUISYSTEM_CFRONTENDBUTTON_H
// NUISystem::CFrontEndButton (new, chunk 765): RTTI .?AVCFrontEndButton@NUISystem@@, single direct base NUISystem::CClickable at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01249554 (+0, slot 0 the ??_G 0x0054E1C0), 0x01249530 (+4, CObserver) and 0x01249528 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CFrontEndButton@NUISystem@@QAE@J@Z, ??0CFrontEndButton@NUISystem@@QAE@ABV01@@Z, ??1CFrontEndButton@NUISystem@@UAE@XZ.
// The destructor 0x0054DED0 stores the vptrs, destroys the UIActionParams list and the two CCountedPointer<CAction> vectors,
// then ~CClickable; its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CClickable.h"
#ifdef FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#endif
namespace NUISystem {
class CFrontEndButton : public CClickable {
public:
    CFrontEndButton(long);
    CFrontEndButton(const CFrontEndButton&);
    virtual ~CFrontEndButton();
    void GetDataFromDef();
#ifdef FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS
    // Retail opt-in (chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent: `push 0x1b4 ; call operator new` before the
    // constructor 0x0054E0B0): the Ego_r.pdb data members (struct_layouts_egor.tsv) 0x30 lower in retail (STLport 4.6 container sizes),
    // so sizeof is retail's 0x1b4. Members only: no row reads them yet.
protected:
    float m_fLastInputTime;                      // +0x194 (Ego_r +452)
    std::vector<CCountedPointer<CAction> > Actions; // +0x198 (STLport 12 bytes; Ego_r +456)
    std::vector<CCountedPointer<CAction> > ActionsOnDelete; // +0x1a4 (Ego_r +472)
    std::list<CActionParam*> UIActionParams;     // +0x1b0 (STLport 4 bytes; Ego_r +488)
#endif
};
}
#ifdef FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NUISystem::CFrontEndButton) == 0x1b4);
#endif
#endif
