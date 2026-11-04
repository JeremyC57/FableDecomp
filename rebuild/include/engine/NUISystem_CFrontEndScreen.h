#ifndef FABLE_ENGINE_NUISYSTEM_CFRONTENDSCREEN_H
#define FABLE_ENGINE_NUISYSTEM_CFRONTENDSCREEN_H
// NUISystem::CFrontEndScreen (new, chunk 765): RTTI .?AVCFrontEndScreen@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x012497E4 (+0, slot 0 the ??_G 0x0054E580), 0x012497BC (+4, CObserver) and 0x012497B4 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CFrontEndScreen@NUISystem@@QAE@J@Z, ??0CFrontEndScreen@NUISystem@@QAE@ABV01@@Z, ??1CFrontEndScreen@NUISystem@@UAE@XZ.
// The destructor 0x0054E450 releases the left-click action member, then ~CChangingStateComponent; its user body is empty.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CFrontEndScreen : public CChangingStateComponent {
public:
    CFrontEndScreen(long);
    CFrontEndScreen(const CFrontEndScreen&);
    virtual ~CFrontEndScreen();
};
}
#endif
