// CTCFlourishTarget::CTCFlourishTarget(CThing* owner) -- thing-component base, the attacker intelligent pointer built, both movement-cull flags set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D966F, length 50 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Label correction: the manifest calls this a CTCGossip constructor; the vtable it stores, 0x0123EF34, is
// CTCFlourishTarget's (rtti-vtables.tsv), and its only caller is the factory @ 0x004DB49D (operator new(0x18)).
// Member vtable: CIntelligentPointer<CThing const> 0x01237F1C.
// Layout from Ego_r.pdb (CTCFlourishTarget, 24 bytes): the CTCBase part +0x00..+0x0b, PAttacker
// (CIntelligentPointer<CThing const>) +0x0c, CullNegativeXAxisMovement +0x14, CullAllYAxisMovement +0x15 (bools).
// Shape: size-optimised module; vtable store, member pointer constructor, then the two bool initialisers (= true)
// in declaration order.
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCFlourishTarget
#pragma optimize("s", on)
class CThing;

class CTCBase { public:                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

struct CBaseIntelligentPointer {                        // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                      // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst() : CBaseIntelligentPointer() { }
};

class CTCFlourishTarget : public CTCBase { public:
    CIntelligentPointer_CThingConst PAttacker;          // +0x0c
    bool CullNegativeXAxisMovement;                     // +0x14
    bool CullAllYAxisMovement;                          // +0x15
    virtual void v00();
    CTCFlourishTarget(CThing* owner);
};

CTCFlourishTarget::CTCFlourishTarget(CThing* owner)
    : CTCBase(owner), PAttacker(), CullNegativeXAxisMovement(true), CullAllYAxisMovement(true)
{
}