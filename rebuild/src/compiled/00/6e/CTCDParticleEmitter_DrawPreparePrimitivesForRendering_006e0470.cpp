// CTCDParticleEmitter::DrawPreparePrimitivesForRendering(long) -- calls the method @ 0x00A01B50 on the member at +0x1c (the argument is unused)
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0470, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  add ecx, 0x1c ; call 0xa01b50 ; ret 4
// Tiny-body shape `GetItemOfMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCDParticleEmitter { public:
    unsigned char _pad[0x1c];
    CBaseIntelligentPointer m_pointer;     // +0x1c
    CBaseObject* DrawPreparePrimitivesForRendering(long unused);
};

CBaseObject* CTCDParticleEmitter::DrawPreparePrimitivesForRendering(long unused)
{
    return m_pointer.GetPItem();
}