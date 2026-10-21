// Grade: DIFFER(382v382) -- scheduling only: the last two stores before SetRHSet (Up.X fstp and Forward.Y int copy) are swapped; retail fstp's Up.X before the Up.Z int copy
// CTCCreatureFlyInCircles::FrameUpdate() -- while active: keep the creature at full health (100), run a queued mode change, keep the loop anim playing, check for hits, advance the swoop (mode 2) or the circling flight from the creature's position and facing, record in the flagged component whether the position is on the map, then write the new position back and orient the creature along its facing (up = (facing.Y, -facing.X, 0) x facing)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DE030, 384 bytes (`ret`).
// Callees: the creature physics component's virtual slots 70 GetFacing (+0x118, C3DVector by hidden pointer), 30 SetPosition
// (+0x78) and 75 SetRHSet (+0x12c, {up, forward}); CThing::GetPosition() const @ 0x004C73D0, ChangeMode @ 0x007DDCC0,
// PlayLoopAnim @ 0x007DDDA0, CheckForHits @ 0x007DD990, UpdateSwoop(C3DVector&, C3DVector&) @ 0x007DD540, UpdateFlight @
// 0x007DD270, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (interface 0x3a), CTCBase::GetWorldMap @
// 0x00686D30, CWorldMap::IsPosInMap(const C3DVector&) const @ 0x004FC600. Globals: 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCCreatureFlyInCircles (Creature +0x0c, Mode +0x34, ModeChangeQueued +0x39, Active +0x3a); the creature's
// physics component at +0x60 and CThingPhysical::Health at +0xb4 (retail, as the landed rows); the type-0x3a component's flag
// is bit 0 of +0x50 (as the landed CModeDragonFlying::OnDeactivate).
// Shape: the component is fetched into a local before the map test; the orientation is built from an inline cross product
// with a (Y, -X, 0) side vector (the zero product kept as `fmul 0.0f`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
    CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) { }
};
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2)
    virtual void SetPosition(const C3DVector& pos);     // slot 30 (+0x78)
    VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                      // slot 70 (+0x118)
    VF(71) VF(72) VF(73) VF(74)
    virtual void SetRHSet(const CRightHandedSet& set);  // slot 75 (+0x12c)
#undef VF10
#undef VF
};
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCFlagged {                                     // the type-0x3a component
    unsigned char _pad00[0x50];
    unsigned char InMap : 1;                            // +0x50 bit 0
};
struct CThingCreatureBase {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char _pad64[0xb4 - 0x64];
    float Health;                                       // +0xb4 (CThingPhysical)
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CWorldMap {
    bool IsPosInMap(const C3DVector& pos) const;        // 0x004FC600
};
struct CTCCreatureFlyInCircles {
    virtual void v00();
    void* ParentThing;                                  // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CThingCreatureBase* Creature;                       // +0x0c
    unsigned char _pad10[0x34 - 0x10];
    long Mode;                                          // +0x34 EFlyMode
    bool Transitioning;                                 // +0x38
    bool ModeChangeQueued;                              // +0x39
    bool Active;                                        // +0x3a
    CWorldMap* GetWorldMap();                           // 0x00686D30 (CTCBase)
    void ChangeMode();                                  // 0x007DDCC0
    void PlayLoopAnim();                                // 0x007DDDA0
    void CheckForHits();                                // 0x007DD990
    void UpdateSwoop(C3DVector& pos, C3DVector& facing);    // 0x007DD540
    void UpdateFlight(C3DVector& pos, C3DVector& facing);   // 0x007DD270
    void FrameUpdate();
};

void CTCCreatureFlyInCircles::FrameUpdate()
{
    if (!Active)
        return;
    C3DVector facing = Creature->PhysicsTC->GetFacing();
    C3DVector pos = Creature->GetPosition();
    Creature->Health = 100.0f;
    if (ModeChangeQueued)
        ChangeMode();
    PlayLoopAnim();
    CheckForHits();
    if (Mode == 2)
        UpdateSwoop(pos, facing);
    else
        UpdateFlight(pos, facing);
    if (Creature->HasTC(0x3a)) {
        CTCFlagged* flagged = (CTCFlagged*)Creature->GetTC(0x3a);
        flagged->InMap = GetWorldMap()->IsPosInMap(pos);
    }
    Creature->PhysicsTC->SetPosition(pos);
    CTCPhysicsBase* physics = Creature->PhysicsTC;
    CRightHandedSet set(CrossProduct(C3DVector(facing.Y, -facing.X, 0.0f), facing), facing);
    physics->SetRHSet(set);
}
