// CThingAICreature::UpdateAITurning(const C3DVector& movement) -- pick the creature's look target from its AI turn state (a fixed position, a live target thing's head helper / helper point / position, or its own position plus the movement when that is long enough) and hand it to the look component (TC 0x43)
// Leaf: UpdateAITurning
// Callees: GetPItem 0xA01B50, 0x661E80 (byte-identical to the landed CText::GetAlignment; here a CThingAICreature flag query), CTCGraphicAppearance 0x4C10E0 / 0x4C1060(1.0f) (by-value C3DVector),
//   CThing::GetPosition 0x4C73D0, LowerBound 0x40F020, CTCLook::LookAtPos 0x753E90.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) { Z = o.Z; Y = o.Y; X = o.X; }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
__forceinline float GetLengthSquared(C3DVector v) { return v.Z * v.Z + v.Y * v.Y + v.X * v.X; }
struct CBaseObject;
struct CBaseIntelligentPointer {
    unsigned char _pad00[0x08];
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
struct CTCGraphicAppearance {
    C3DVector GetHeadPosition() const;                      // 0x004C10E0
    C3DVector GetHelperPointOrCentre(float fraction) const; // 0x004C1060
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_tcs;                                    // +0x44
    unsigned char _pad50[0x64 - 0x50];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Dead : 1;                                 // +0x91
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_tcs;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CTCLook {
    void LookAtPos(const C3DVector& position, long speed);  // 0x00753E90
};
struct CAITurnState {
    unsigned long Flags;                                    // +0x00
    bool IsFacingPosition() const { return (unsigned char)(Flags >> 1) & 1; }
    bool IsFacingThing() const { return (unsigned char)(Flags >> 2) & 1; }
    bool IsFacingMovement() const { return (unsigned char)(Flags >> 4) & 1; }
    CThing* GetThing() const { return (CThing*)Thing.GetPItem(); }
    bool IsThingValid() const { CThing* thing = GetThing(); return thing && !thing->Dead; }
    long Speed;                                             // +0x04
    unsigned char _pad08[0x34 - 0x08];
    C3DVector Position;                                     // +0x34
    CBaseIntelligentPointer Thing;                          // +0x40
};
struct CThingAICreature : CThing {
    unsigned char _pad94[0x1a0 - 0x94];
    CAITurnState* PTurnState;                               // +0x1a0
    bool LooksAtHeadHelper() const;                         // 0x00661E80
    void UpdateAITurning(const C3DVector& movement);
};

void CThingAICreature::UpdateAITurning(const C3DVector& movement)
{
    C3DVector target;
    if (PTurnState->IsFacingPosition()) {
        target = PTurnState->Position;
    } else if (PTurnState->IsFacingThing() && PTurnState->IsThingValid()) {
        if (PTurnState->GetThing()->PGraphicAppearance) {
            if (LooksAtHeadHelper()) {
                CTCGraphicAppearance* app = PTurnState->GetThing()->PGraphicAppearance;
                target = app->GetHeadPosition();
            } else {
                CTCGraphicAppearance* app = PTurnState->GetThing()->PGraphicAppearance;
                target = app->GetHelperPointOrCentre(1.0f);
            }
        } else {
            target = PTurnState->GetThing()->GetPosition();
        }
    } else if (PTurnState->IsFacingMovement() && GetLengthSquared(movement) > 0.0001f) {
        target = GetPosition() + movement;
    } else {
        return;
    }
    CTCLook* look;
    if (HasTC(0x43))
        look = (CTCLook*)GetTC(0x43);
    look->LookAtPos(target, PTurnState->Speed);
}