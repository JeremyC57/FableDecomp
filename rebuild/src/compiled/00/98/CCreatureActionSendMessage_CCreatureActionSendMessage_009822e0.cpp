// CCreatureAction_SendMessage::~CCreatureAction_SendMessage() -- implicit destructor: PTarget, then the PDef counted def handle, then ~CCreatureActionBase by tail jump
// Leaf: ??1CCreatureAction_SendMessage@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009822E0, 52 bytes, thiscall, tail `jmp ~CCreatureActionBase`; no caller (an
//   unreferenced COMDAT the linker kept).
// Identity (proven): it sits right after CCreatureAction_SendMessage's constructor 0x00982210 (stores vtable 0x0129947C, RTTI
//   .?AVCCreatureAction_SendMessage@@; PDef +0xA8, Pos +0xAC..+0xB4, PTarget +0xB8 with the CIntelligentPointer<CThingCreatureBase>
//   vtable 0x0123E7D8, ExtraData +0xC0), and the class's scalar deleting destructor 0x00982550 (slot 0 of 0x0129947C) inlines
//   exactly this sequence (+0xB8, the +0xA8 def release, 0x00693EF0). FableWin.pdb spells ??1CCreatureAction_SendMessage@@UAE@XZ
//   (donor 0x028940D0, after the ctor / ??_E / copy-ctor run); Ego_r.pdb CCreatureAction_SendMessage : CCreatureActionBase has
//   PDef (CDefPointer<const CMessageEventDef>) +0xB4, Pos (C3DVector) +0xB8, PTarget (CIntelligentPointer<CThingCreatureBase>)
//   +0xC4, ExtraData +0xCC (retail -0xC) -- the only Ego_r class with a CDefPointer at +0xB4 and an intelligent pointer at +0xC4.
//   No 0x0129947C store: the implicit destructor. The catalog label (~CCreatureAction_PerformExpressionExtended) is propagated
//   from the byte twin 0x0084ADC0's family (this copy differs in the pointer offset +0xB8).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCreatureActionBase::~CCreatureActionBase 0x00693EF0;
//   the def release is CIVCountedPointeeBase's inline Release plus the virtual Delete (slot 1).
// Shape: PTarget's implicit dtor (the out-of-line base dtor), engine/CDefPointer.h's inline ~CDefPointer, the base dtor; the trivial
//   C3DVector Pos emits nothing. DestroyOne (ungraded emission helper, chunk 656/657 pattern) emits the implicit `??1`.
// Layout: CCreatureActionBase modelled locally at its retail size 0xA8 (as the landed 0x007ED750): engine/CCreatureActionBase.h
//   has the retail size but flattens the vptr into a data member, so it cannot be the polymorphic base whose out-of-line
//   virtual destructor 0x00693EF0 this body calls (header debt). No engine header exists for CCreatureAction_SendMessage or
//   C3DVector (C3DVector as Ego_r: X, Y, Z floats).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingCreatureBase;
class CMessageEventDef;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

#include "engine/C3DVector.h"

class CCreatureAction_SendMessage : public CCreatureActionBase {   // vtable 0x0129947C, retail 0xC4 bytes
public:
    CCreatureAction_SendMessage(CThingCreatureBase& creature, const CDefPointer<const CMessageEventDef>& def,
        CThingCreatureBase* target, const C3DVector& pos, long extraData);   // 0x00982210
    CDefPointer<const CMessageEventDef>     PDef;           // +0xA8
    C3DVector                               Pos;            // +0xAC
    CIntelligentPointer<CThingCreatureBase> PTarget;        // +0xB8
    long                                    ExtraData;      // +0xC0
};

void DestroyOne(CCreatureAction_SendMessage* action)
{
    action->CCreatureAction_SendMessage::~CCreatureAction_SendMessage();
}