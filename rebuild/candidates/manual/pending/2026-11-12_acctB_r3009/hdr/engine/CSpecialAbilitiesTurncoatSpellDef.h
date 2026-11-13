#ifndef FABLE_ENGINE_CSPECIALABILITIESTURNCOATSPELLDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESTURNCOATSPELLDEF_H
// NEW shared header (w22 agent2027, row 0x007C6E50 CTCTurncoatSpell::FrameUpdate): the retail CSpecialAbilitiesTurncoatSpellDef, laid
// out as the sibling CSpecialAbilities*Def headers (CParentDefClassBase base, the first member at +0x3C): the 5 Ego_r.pdb members in
// order and spelling (ghidra_out/struct_layouts_egor.tsv, 0x90 there with 16-byte vectors and a 0x40 base) with 12-byte STLport
// vectors: StaminaCostPerSec +0x3C, ResistanceDamagePerSec +0x48, TurncoatTime +0x54, TurncoatRange +0x60, TurncoatXPUpEveryNSecs
// +0x6C (sizeof 0x78). Retail evidence: FrameUpdate reads TurncoatRange's start at +0x60 (`mov ecx,[eax+60h] ; fld dword ptr
// [ecx+edx*4]` with edx = LevelOfSpell, 0x007C6F7E / 0x007C6F9C); the landed SetPTarget 0x007C6940 reads TurncoatTime at +0x54.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesTurncoatSpellDef : public CParentDefClassBase {
public:
    std::vector<float,std::allocator<float> > StaminaCostPerSec;                 // +0x3C
    std::vector<float,std::allocator<float> > ResistanceDamagePerSec;            // +0x48
    std::vector<float,std::allocator<float> > TurncoatTime;                      // +0x54
    std::vector<float,std::allocator<float> > TurncoatRange;                     // +0x60
    std::vector<float,std::allocator<float> > TurncoatXPUpEveryNSecs;            // +0x6C
};
#endif
