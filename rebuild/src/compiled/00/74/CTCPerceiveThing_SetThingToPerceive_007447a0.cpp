// CTCPerceiveThing::SetThingToPerceive(const CThing* thing) -- start perceiving the thing (Perceiving = true) or stop (Perceiving = false, no target)
// Leaf: ?SetThingToPerceive@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007447A0, 55 bytes (+9 int3), __fastcall `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (its result unused), CTCPerceiveThing::SetPerceivedThing @
//   0x007446B0 (landed).
// Caller: 0x00745D40 (passes the result of a three-call accessor chain on the parent thing's world).
// Label: the manifest's "DeleteData" is a propagation; Ego_r.pdb names
//   `?SetThingToPerceive@CTCPerceiveThing@@QAEXPBVCThing@@@Z`. Layout as the landed SetPerceivedThing 0x007446B0 (local model:
//   engine/CTCPerceiveThing.h flattens the CIntelligentPointer member): PerceivedThing +0x0C, Perceiving +0x2C.
// Shape: `if (thing) { GetPerceivedThing(); Perceiving = true; SetPerceivedThing(thing); } else { Perceiving = false;
//   SetPerceivedThing(0); }` -- retail keeps a GetPItem() call on PerceivedThing whose result is dropped (the remains of a
//   check on the old target that folded away); VC7.1 duplicates the call + return tail into both arms.
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {                 // 8 bytes: vptr + the pointed-to item
public:
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
};

class CTCPerceiveThing {
public:
    void*     __vftable;                        // +0x00 (CTCBase)
    CThing*   ParentThing;                      // +0x04
    bool      ToBeRemoved;                      // +0x08
    bool      IsInGlobalUpdateTCs;              // +0x09
    CIntelligentPointer PerceivedThing;         // +0x0c
    float     DistanceFromPerceivedThingSquared;// +0x14
    float     AwarenessTimer;                   // +0x18
    long      FramesSinceDirectlyPerceivedThing;// +0x1c
    float     LastPerceivedPos[3];              // +0x20
    bool      Perceiving;                       // +0x2c
    const CThing* GetPerceivedThing() const { return (const CThing*)PerceivedThing.GetPItem(); }
    void SetPerceivedThing(const CThing* thing);   // 0x007446B0
    void SetThingToPerceive(const CThing* thing);
};

void CTCPerceiveThing::SetThingToPerceive(const CThing* thing)
{
    if (thing) {
        GetPerceivedThing();
        Perceiving = true;
        SetPerceivedThing(thing);
    }
    else {
        Perceiving = false;
        SetPerceivedThing(0);
    }
}