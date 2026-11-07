#ifndef FABLE_ENGINE_NUISYSTEM_COK_H
#define FABLE_ENGINE_NUISYSTEM_COK_H
// NUISystem::COk (new, chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent allocates it): RTTI .?AVCOk@NUISystem@@
// with the single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv) sit 0x30
// lower in retail (on the retail CChangingStateComponent, sizeof 0x15c), minus 4 after each STLport vector; constructor 0x0041CB70 (retail
// `push 0x160 ; call operator new` in CreateComponent). Name set: Ego_r.pdb / FableWin.pdb ??0COk@NUISystem@@QAE@J@Z.
// Only the constructor is declared (the factory row calls nothing else).
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class COk : public CChangingStateComponent {
public:
    COk(long);                                                       // 0x0041CB70
protected:
    void (*OkCallback)();                                           // +0x15c (Ego_r +0x18c, a __cdecl void function pointer)
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::COk) == 0x160);
#endif
