#ifndef FABLE_ENGINE_NUISYSTEM_CNAVBUTTON_H
#define FABLE_ENGINE_NUISYSTEM_CNAVBUTTON_H
// NUISystem::CNavButton (new, chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent allocates it): RTTI .?AVCNavButton@NUISystem@@
// with the single direct base NUISystem::CClickable at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv) sit 0x30
// lower in retail (on the retail CClickable, sizeof 0x194), minus 4 after each STLport vector; constructor 0x00558B90 (retail
// `push 0x194 ; call operator new` in CreateComponent). Name set: Ego_r.pdb / FableWin.pdb ??0CNavButton@NUISystem@@QAE@J@Z.
// Only the constructor is declared (the factory row calls nothing else).
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CNavButton : public CClickable {
public:
    CNavButton(long);                                                       // 0x00558B90
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CNavButton) == 0x194);
#endif
