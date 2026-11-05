// Byte-exact reconstruction of CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing @ 0x007449e0
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).

#include "engine/CTCPerceiveThing.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCPerceiveThing;

// callee at 0xa01b50: takes (this+0xc) in ecx, returns int
// callee at 0x744730: void member, this in ecx
extern void __fastcall Do_744730(CTCPerceiveThing* self);


void __fastcall CTCPerceiveThing_SetAsIndirectlyAwareOfPerceivedThing(CTCPerceiveThing* self)
{
    if (((int)(((CBaseIntelligentPointer*)((char*)self + 0xc))->GetPItem())) == 0)
        return;
    if (self->LineOfSight != 0)
        return;
    Do_744730(self);
}