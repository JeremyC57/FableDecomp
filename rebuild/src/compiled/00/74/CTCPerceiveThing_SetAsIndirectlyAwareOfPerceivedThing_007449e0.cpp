// Byte-exact reconstruction of CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing @ 0x007449e0

#include "engine/CTCPerceiveThing.h"  // retyped onto the PDB layout; byte parity re-verified
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
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