// CThingGameObject::UpdateStandingOnThing() -- wakes the object's physics (SetAsMoving(true)) when it is resting on nothing, or on a thing whose own physics is moving
// Leaf: UpdateStandingOnThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00838AA0, length 128 bytes.
// Callees: CTCPhysicsBase vtable 0x0125D2C4 slots 52/55/56 (+0xd0 0x006B1180, +0xdc 0x006B11B0, +0xe0 0x006B11C0; unnamed state
// queries), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130.
// Layout (retail, local model): CThing flag byte +0x91 (bit 0x20 skips the update), PhysicsTC +0x60; CThingGameObject
// PThingStandingOn (CIntelligentPointer<CThing>) at retail +0xc0 (PDB +0xd8: retail CThing is 24 bytes smaller);
// CTCPhysicsBase Moving bit field at +0x3c bit 0 (PDB).
// Shape: early returns; the component is a named local; GetPItem is called again for each use (the intelligent pointer
// is not cached); the final call re-reads PhysicsTC from the thing.
class CThing;
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual bool IsStateA();                    // slot 52 (+0xd0) 0x006B1180
    virtual void v053(); virtual void v054();
    virtual bool IsStateC();                    // slot 55 (+0xdc) 0x006B11B0
    virtual bool IsStateB();                    // slot 56 (+0xe0) 0x006B11C0
    unsigned char _pad04[0x3c - 0x04];
    bool Moving : 1;                            // +0x3c bit 0
    bool OldMoving : 1;
    void SetAsMoving(bool moving);              // 0x006B0130
};
struct CThingPhysicalBase {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
struct CBaseIntelligentPointer {
    void* m_vfptr;
    void* m_pItem;
    CThingPhysicalBase* GetPItem() const;       // 0x00A01B50
};
class CThingGameObject : public CThingPhysicalBase { public:
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91
    unsigned char _pad92[0xc0 - 0x92];
    CBaseIntelligentPointer PThingStandingOn;   // +0xc0 (retail)
    void UpdateStandingOnThing();
};

void CThingGameObject::UpdateStandingOnThing()
{
    if (m_flags91 & 0x20)
        return;
    CTCPhysicsBase* physics = PhysicsTC;
    if (!physics)
        return;
    if (physics->IsStateA())
        return;
    if (physics->IsStateB())
        return;
    if (physics->Moving)
        return;
    if (!physics->IsStateC())
        return;
    if (PThingStandingOn.GetPItem()) {
        if (!PThingStandingOn.GetPItem()->PhysicsTC)
            return;
        if (!PThingStandingOn.GetPItem()->PhysicsTC->Moving)
            return;
    }
    PhysicsTC->SetAsMoving(true);
}