// CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator(CThing&) -- CTCBase(thing), own vtable, no regeneration frame, not generated, the generator intelligent pointer constructed, generator UID cleared
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Leaf: ??0CTCCreatureGeneratorCreator@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCCreatureGeneratorCreator@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C9720, length 57 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite"
// is propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the
// CIntelligentPointer vtable 0x01238C6C). Vtable 0x0126EB3C = CTCCreatureGeneratorCreator.
// Layout = Ego_r.pdb CTCCreatureGeneratorCreator (0x28 bytes): FrameToRegenerate +0xc, Generated +0x10,
// PGenerator +0x11 (CIntelligentPointer has alignment 1: declared under pack(1)), GeneratorUID +0x20
// (an 8-aligned unsigned __int64).
// Shape: plain initialisers in declaration order sharing the ebx zero register; the pointer member
// addressed once in edi; the UID halves stored after its constructor.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

struct CTCCreatureGeneratorCreator : CTCBase {        // vtable 0x0126EB3C
    long                FrameToRegenerate;                  // +0x0c
    bool                Generated;                          // +0x10
    CIntelligentPointer<CThing> PGenerator;                         // +0x11
    unsigned __int64    GeneratorUID;                       // +0x20
    CTCCreatureGeneratorCreator(CThing& thing);
};

CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator(CThing& thing)
    : CTCBase(thing)
    , FrameToRegenerate(0)
    , Generated(false)
    , PGenerator()
    , GeneratorUID(0)
{
}