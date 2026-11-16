// CTCParticleAttacher::CTCParticleAttacher(CThing* parent) -- CTCBase, own vtable, empty def pointer and particle list
// Leaf: ??0CTCParticleAttacher@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCParticleAttacher@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE7FD, length 38 bytes (size-optimised def/factory module: unaligned
// entry and `push dword ptr [esp+8]` -> #pragma optimize("s", on)). Vtable 0x0124176C is CTCParticleAttacher's (RTTI); the
// manifest label "CTCDHeroSuit::CTCDHeroSuit" is propagated (the HeroSuit body has the same shape).
// Callee: CTCBase::CTCBase(CThing&) @ 0x00686800.
// Shape: every zero store follows the vtable store -- PDef (+0x0c) and ParticleList (+0x10, the 12-byte retail vector,
// PDB 16) are member objects with inline zeroing constructors, sharing one zero register.
#pragma optimize("s", on)

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

struct CDefPointer {                        // CDefPointer<CParticleAttacherDef const>
    void* Object;
    CDefPointer() : Object(0) { }
};

struct CThingPointerVector {                // vector<CIntelligentPointer<CThing> > (12 bytes)
    void* First;
    void* Last;
    void* End;
    CThingPointerVector() : First(0), Last(0), End(0) { }
};

class CTCParticleAttacher : public CTCBase { public:
    CDefPointer PDef;                       // +0x0c
    CThingPointerVector ParticleList;       // +0x10
    CTCParticleAttacher(CThing& parent);
};

CTCParticleAttacher::CTCParticleAttacher(CThing& parent)
    : CTCBase(parent)
{
}