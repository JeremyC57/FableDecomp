// CCreatureAction_FireMissileWeapon::Initialise() -- unless the player auto-faces or is first-person targeting, turn a creature
// Grade: DIFFER(486v486) -- register swap only: retail keeps &PTarget in edi (pushed at entry) and &PCreature / the creature position in esi, ours the reverse (edi pushed lazily, so the two interface-key slots sit 4 bytes lower); flow, epsilon global squared at run time, inline RHS ctor, void Normalise all match
// with a live target to face it across the ground plane (when the target is not on top of it); unload the projectile weapon
// in the creature's primary slot; take DelayTimer from the animation entry's transition-in time; base Initialise
// Leaf: ?Initialise@CCreatureAction_FireMissileWeapon
// Layout: retail CCreatureActionBase is 0xa8 (Ego_r.pdb +0xc), so PTarget +0xa8 and DelayTimer +0xc0 (PDB +0xb4/+0xcc);
//   PAnimEntry +0x74; CPlayer auto-face flag +0x20a; CThing interface bitset +0x20, map +0x44, physics +0x60, flags +0x91.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline bool IsNonZeroXY(float epsilon) const { return X * X + Y * Y > epsilon * epsilon; }
    void Normalise();                                 // 0x00A14440
};
extern const float g_fEpsilon;                        // 0x0129BA3C (0.0001)
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    __forceinline CRightHandedSet(const C3DVector& forward) : Up(0.0f, 0.0f, 1.0f), Forward(forward) {}
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CPhysicsBody {                                 // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF(70) VF(71) VF(72) VF(73) VF(74)
    virtual void SetOrientation(const CRightHandedSet& set);   // slot 75 (+0x12c)
};
struct CPlayer {
    unsigned char _pad[0x20a];
    unsigned char m_autoFaceTarget;                   // +0x20a
    bool IsFirstPersonTargeting();                    // 0x004874D0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                        // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CPlayer* GetPlayer();                             // 0x004C7A10
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCarrying {                                  // interface 0x46
    CThing* GetThingInPrimarySlot();                  // 0x006A1440
};
struct CTCWeapon {                                    // interface 0x48
    void SetProjectileWeaponLoaded(bool loaded, CThing* projectile);   // 0x005DAC50
};
struct CAnimationEntry {
    long GetAnimationTransitionInTime();              // 0x005DC1D0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x74 - 0x10];
    CAnimationEntry* PAnimEntry;                      // +0x74 CCountedPointer<CAnimationEntry>
    unsigned char _pad78[0xa8 - 0x78];
    void Initialise();                                // 0x00694D10
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;                  // +0xa8
    float ShotAccuracyPercentage;                     // +0xb0
    float ShotDamageMultiplier;                       // +0xb4
    float ShotBuildupTime;                            // +0xb8
    bool  Decapitate;                                 // +0xbc
    long  DelayTimer;                                 // +0xc0
    void Initialise();
};

void CCreatureAction_FireMissileWeapon::Initialise()
{
    if (IS_THING_ALIVE(PTarget) && !GetCreature()->GetPlayer()->m_autoFaceTarget &&
        !GetCreature()->GetPlayer()->IsFirstPersonTargeting()) {
        CThing* target = PTarget.GetPItem();
        const C3DVector& from = GetCreature()->GetPosition();
        const C3DVector& to = target->GetPosition();
        C3DVector direction(to.X - from.X, to.Y - from.Y, 0.0f);
        if (direction.IsNonZeroXY(g_fEpsilon)) {
            direction.Normalise();
            GetCreature()->GetPhysics()->SetOrientation(CRightHandedSet(direction));
        }
    }
    CTCCarrying* carrying;
    if (IS_THING_ALIVE(PCreature) && GetCreature()->GetInterface(&carrying, 0x46)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        CTCWeapon* weaponTC;
        if (weapon && !(weapon->m_flags91 & 1) && weapon->GetInterface(&weaponTC, 0x48))
            weaponTC->SetProjectileWeaponLoaded(false, 0);
    }
    if (PAnimEntry)
        DelayTimer = PAnimEntry->GetAnimationTransitionInTime();
    CCreatureActionBase::Initialise();
}
