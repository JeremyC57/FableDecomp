// CTCCoopSpirit::CTCCoopSpirit(CThing&) -- CTCBase(thing), own vtable, the def pointer cleared, the particle-emitter intelligent pointer constructed
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Leaf: ??0CTCCoopSpirit@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCCoopSpirit@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00670050, length 48 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite" is
// propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer
// vtable 0x01238C6C). Vtable 0x0125B264 = CTCCoopSpirit.
// Layout = Ego_r.pdb CTCCoopSpirit (0x34 bytes): CTCBase 0xc, NoMovementTimer +0xc, Score +0x10,
// OffscreenCounter +0x14, MovementVector +0x18, Master +0x24, PCoopSpiritDef +0x28 (CDefPointer, one
// word), PParticleEmitter +0x2c (CIntelligentPointer). Shape: both members are member objects with inline
// constructors (their stores follow the vtable store); the pointer member is addressed once in edi.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CPlayer;
class CCoopSpiritDef;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CDefPointer_CoopSpiritDef {                          // CDefPointer<CCoopSpiritDef const>: one word
    const CCoopSpiritDef* Object;
    CDefPointer_CoopSpiritDef() : Object(0) { }
};

#include "engine/C3DVector.h"

class CTCCoopSpirit : public CTCBase { public:                      // vtable 0x0125B264
    long                      NoMovementTimer;              // +0x0c
    long                      Score;                        // +0x10
    long                      OffscreenCounter;             // +0x14
    C3DVector                 MovementVector;               // +0x18
    CPlayer*                  Master;                       // +0x24
    CDefPointer_CoopSpiritDef PCoopSpiritDef;               // +0x28
    CIntelligentPointer<CThing>       PParticleEmitter;             // +0x2c
    CTCCoopSpirit(CThing& thing);
};

CTCCoopSpirit::CTCCoopSpirit(CThing& thing)
    : CTCBase(thing)
    , PCoopSpiritDef()
    , PParticleEmitter()
{
}