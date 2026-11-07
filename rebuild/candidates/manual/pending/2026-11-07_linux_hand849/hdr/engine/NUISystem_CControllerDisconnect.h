#ifndef FABLE_ENGINE_NUISYSTEM_CCONTROLLERDISCONNECT_H
#define FABLE_ENGINE_NUISYSTEM_CCONTROLLERDISCONNECT_H
// NUISystem::CControllerDisconnect (new, chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent allocates it): RTTI .?AVCControllerDisconnect@NUISystem@@
// with the single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv) sit 0x30
// lower in retail (on the retail CChangingStateComponent, sizeof 0x15c), minus 4 after each STLport vector; constructor 0x0041CBE4 (retail
// `push 0x160 ; call operator new` in CreateComponent). Name set: Ego_r.pdb / FableWin.pdb ??0CControllerDisconnect@NUISystem@@QAE@J@Z.
// Only the constructor is declared (the factory row calls nothing else).
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CControllerDisconnect : public CChangingStateComponent {
public:
    CControllerDisconnect(long);                                                       // 0x0041CBE4
protected:
    CObserver* m_pPrevExclusiveObserver;                            // +0x15c (Ego_r +0x18c)
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CControllerDisconnect) == 0x160);
#endif
