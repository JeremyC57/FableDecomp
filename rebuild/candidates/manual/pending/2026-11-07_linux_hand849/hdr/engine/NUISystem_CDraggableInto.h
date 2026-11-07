#ifndef FABLE_ENGINE_NUISYSTEM_CDRAGGABLEINTO_H
#define FABLE_ENGINE_NUISYSTEM_CDRAGGABLEINTO_H
// NUISystem::CDraggableInto (new, chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent allocates it): RTTI .?AVCDraggableInto@NUISystem@@
// with the single direct base NUISystem::CHoverable at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv) sit 0x30
// lower in retail (on the retail CHoverable, sizeof 0x16c), minus 4 after each STLport vector; constructor 0x00558EC0 (retail
// `push 0x170 ; call operator new` in CreateComponent). Name set: Ego_r.pdb / FableWin.pdb ??0CDraggableInto@NUISystem@@QAE@J@Z.
// Only the constructor is declared (the factory row calls nothing else).
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CDraggableInto : public CHoverable {
public:
    CDraggableInto(long);                                                       // 0x00558EC0
protected:
    std::list<CCountedPointer<CAction> >* PActions;                 // +0x16c (Ego_r +0x19c)
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CDraggableInto) == 0x170);
#endif
