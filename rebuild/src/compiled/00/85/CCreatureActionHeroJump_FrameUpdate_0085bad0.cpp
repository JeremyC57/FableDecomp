// Byte-exact reconstruction of CCreatureActionHeroJump::FrameUpdate 0x0085bad0
// Leaf: ?CCreatureActionHeroJump_FrameUpdate@@YIXPAVCCreatureActionHeroJump@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).

struct SubObj8;

// method at 0xa01b50 called with ecx = &self at +8 (subobject)
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionHeroJump { public:
    int   m00;      // +0x00
    int   m04;      // +0x04
    int   m08;      // +0x08  (subobject start)
    int   m0c;      // +0x0c
    int   m10;      // +0x10
    int   m14;      // +0x14
    int   m18;      // +0x18
    int   m1c;      // +0x1c
};

// tail-called method at 0x859d80 (ecx = self)
void __fastcall Base_FrameStep(CCreatureActionHeroJump* action);

void __fastcall CCreatureActionHeroJump_FrameUpdate(CCreatureActionHeroJump* action)
{
    int v = action->m1c + 7;
    if (v >= action->m10) {
        ((CBaseIntelligentPointer*)((SubObj8*)&action->m08))->GetPItem();
    }
    Base_FrameStep(action);
}