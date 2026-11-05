// CTCGossip::CTCGossip(CThing* owner) -- thing-component base, the gossipee intelligent pointer built, MyTurnToSpeak cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8C27, length 46 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Vtables: CTCGossip 0x0123EE44 (rtti-vtables.tsv), CIntelligentPointer<CThingAICreature> 0x0123E7E0.
// Called from the factory Create_CTCGossip @ 0x004DB141 (landed; operator new(0x18)).
// Layout from Ego_r.pdb (CTCGossip, 24 bytes): the CTCBase part +0x00..+0x0b, Gossipee
// (CIntelligentPointer<CThingAICreature>) +0x0c, MyTurnToSpeak (bool) +0x14.
// Shape: size-optimised module (`push dword ptr [esp+0Ch]` pass-through); the derived vtable is stored between the
// member's `lea`/`mov ecx` and its base-pointer constructor call, the member stores its own vtable, and the bool
// initialiser (declared after the member) follows in declaration order.
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCGossip
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("s", on)
class CThing;

class CTCBase { public:                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThingAICreature;

class CTCGossip : public CTCBase { public:
    CIntelligentPointer<CThingAICreature> Gossipee;      // +0x0c
    bool                                 MyTurnToSpeak; // +0x14
    virtual void v00();
    CTCGossip(CThing* owner);
};

CTCGossip::CTCGossip(CThing* owner)
    : CTCBase(owner), Gossipee(), MyTurnToSpeak(false)
{
}