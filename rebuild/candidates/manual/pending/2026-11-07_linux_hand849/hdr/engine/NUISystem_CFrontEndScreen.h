#ifndef FABLE_ENGINE_NUISYSTEM_CFRONTENDSCREEN_H
#define FABLE_ENGINE_NUISYSTEM_CFRONTENDSCREEN_H
// NUISystem::CFrontEndScreen (new, chunk 765): RTTI .?AVCFrontEndScreen@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x012497E4 (+0, slot 0 the ??_G 0x0054E580), 0x012497BC (+4, CObserver) and 0x012497B4 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CFrontEndScreen@NUISystem@@QAE@J@Z, ??0CFrontEndScreen@NUISystem@@QAE@ABV01@@Z, ??1CFrontEndScreen@NUISystem@@UAE@XZ.
// The destructor 0x0054E450 releases the left-click action member, then ~CChangingStateComponent; its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
#ifdef FABLE_RETAIL_CFRONTENDSCREEN_MEMBERS
#include "engine/CCountedPointer.h"
#endif
namespace NUISystem {
class CFrontEndScreen : public CChangingStateComponent {
public:
    CFrontEndScreen(long);
    CFrontEndScreen(const CFrontEndScreen&);
    virtual ~CFrontEndScreen();
#ifdef FABLE_RETAIL_CFRONTENDSCREEN_MEMBERS
    // Retail opt-in (chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent: `push 0x16c ; call operator new` before the
    // constructor 0x0054E3D0): the Ego_r.pdb data members (struct_layouts_egor.tsv) 0x30 lower in retail (STLport 4.6 container sizes),
    // so sizeof is retail's 0x16c. Members only: no row reads them yet.
protected:
    float m_fLastInputTime;                      // +0x15c (Ego_r +396)
    CCountedPointer<CAction> PLeftClickAction;   // +0x160 (Ego_r +400)
    CComponent* PAcceptButton;                   // +0x168 (Ego_r +408)
#endif
};
}
#ifdef FABLE_RETAIL_CFRONTENDSCREEN_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NUISystem::CFrontEndScreen) == 0x16c);
#endif
#endif
