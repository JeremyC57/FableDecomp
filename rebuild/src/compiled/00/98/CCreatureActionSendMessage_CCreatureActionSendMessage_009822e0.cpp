// CCreatureAction_SendMessage::~CCreatureAction_SendMessage() -- implicit destructor: PTarget, then the PDef counted def handle, then ~CCreatureActionBase by tail jump
// Leaf: ??1CCreatureAction_SendMessage@@UAE@XZ
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

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer() { }
};

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class C3DVector {                                           // Ego_r.pdb, 12 bytes
public:
    float X;
    float Y;
    float Z;
};

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