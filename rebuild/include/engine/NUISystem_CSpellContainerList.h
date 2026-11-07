#ifndef FABLE_ENGINE_NUISYSTEM_CSPELLCONTAINERLIST_H
#define FABLE_ENGINE_NUISYSTEM_CSPELLCONTAINERLIST_H
// NUISystem::CSpellContainerList (new, chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent allocates it): RTTI .?AVCSpellContainerList@NUISystem@@
// with the single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv) sit 0x30
// lower in retail (on the retail CChangingStateComponent, sizeof 0x15c), minus 4 after each STLport vector; constructor 0x00544B70 (retail
// `push 0x1a8 ; call operator new` in CreateComponent). Name set: Ego_r.pdb / FableWin.pdb ??0CSpellContainerList@NUISystem@@QAE@J@Z.
// Only the constructor is declared (the factory row calls nothing else).
#include "engine/NUISystem_CChangingStateComponent.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
namespace NUISystem {
class CSpellContainerList : public CChangingStateComponent {
public:
    CSpellContainerList(long);                                                       // 0x00544B70
protected:
    float LastInputTime;                                            // +0x15c (Ego_r +0x18c)
    unsigned long SpellIndex;                                       // +0x160
    unsigned long SelectedChild;                                    // +0x164
    CComponent* SpellSelectionChild;                                // +0x168
    CComponent* SpellIndicationChild;                               // +0x16c
    std::vector<CComponent*> SpellContainers;                       // +0x170 (STLport 12 bytes; Ego_r +0x1a0, 16)
    long SpellToAssign;                                             // +0x17c (Ego_r +0x1b0)
    C2DVector PositionOffset;                                       // +0x180
    float AlphaOffset;                                              // +0x188
    float NextSpellContainerAlpha;                                  // +0x18c
    C2DVector NextSpellContainerPosition;                           // +0x190
    C2DVector PositionOffsetToUse;                                  // +0x198
    float AlphaOffsetToUse;                                         // +0x1a0
    unsigned char _pad_0x1a4[0x4];                                  // +0x1a4 (Ego_r 476 bytes: tail padding / unnamed)
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CSpellContainerList) == 0x1a8);
#endif
