// CTCHeroSpecialMovement::CTCHeroSpecialMovement(CThing& owner) -- the base constructed with the owner, two cleared flag bytes at +0xc/+0xd, cleared words at +0x14 and +0x18, this class's vtable
// Leaf: ??0CTCHeroSpecialMovement@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model (CTCBase_00686800, which bound a made-up constructor name to
//   0x00686800) is gone; the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as both PDBs spell it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E08E0, length 37 bytes.
// Callees: the base constructor @ 0x00686800 (the owner; the manifest label "CEngineSubPrimitive2DClockSprite" is propagated). Vtable 0x0126FE3C = CTCHeroSpecialMovement
// (rtti-vtables.tsv). Shape: the zero register serves the two bytes and two words, the vtable store is scheduled between the +0x14 and +0x18 stores. The chunk
// label "CEnginePrimitiveManagerParticleGroup" is propagated.

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
struct SZeroWord_007e08e0 { long m_value; SZeroWord_007e08e0() : m_value(0) { } };

class CTCHeroSpecialMovement : public CTCBase { public: // vtable 0x0126FE3C
    bool  m_active;                                // +0x0c
    bool  m_pending;                               // +0x0d
    unsigned char _pad0e[6];
    long  m_target;                                // +0x14
    SZeroWord_007e08e0 m_timer;                    // +0x18 (a member object: its inline constructor runs after the vtable store)
    CTCHeroSpecialMovement(CThing& owner);
};

CTCHeroSpecialMovement::CTCHeroSpecialMovement(CThing& owner)
    : CTCBase(owner)
    , m_active(false), m_pending(false), m_target(0), m_timer()
{
}