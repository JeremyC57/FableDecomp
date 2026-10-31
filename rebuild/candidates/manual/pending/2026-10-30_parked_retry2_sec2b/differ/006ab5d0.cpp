// CThingPlayerCreature::GetCurrentMaxMovementSpeed() const -- the speed limit for the current movement band: 0 while blocking, the sneak component's speed when sneaking (always under the sneak-speed option, else in the slow/walking bands), else walking/jogging/running by band, overridden by sprinting when the 0xDF component asks for it
// Grade: DIFFER(412v412) 21 difflines (was 24): x87 band speed now kept on the stack (fst at the join) via speed = GetMaxSprintingSpeed + single return; residue: 0xDF component pointer edx/al vs retail eax/cl (retry wave 2 sec2b)
// Was: DIFFER(412v412) -- the band speed: retail keeps it on the x87 stack (case 0 `fld 0.0f`, `fst` at the join, `fstp st(0)` before the sprint call), ours homes it in memory (`mov [esp+8],0`, `fstp`); the 0xDF component lands in edx/al (retail eax/cl)
// Leaf: GetCurrentMaxMovementSpeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB5D0, length 416 bytes including the 5-entry band jump table at 0x006AB758.
// Mangled (Ego_r.pdb): ?GetCurrentMaxMovementSpeed@CThingPlayerCreature@@IBEMXZ.
// Callees: the physics TC's virtual GetFacing() (slot 70, +0x118, by-value C3DVector), CThing::GetPosition 0x004C73D0,
//   CThingCreatureBase::IsBlocking 0x00664D00, the sneak component's GetMaxSneakingSpeed 0x00747020, CVectorMap LowerBound
//   0x004365B0 (const), virtual GetMaxWalkingSpeed/GetMaxJoggingSpeed/GetMaxRunningSpeed/GetMaxSprintingSpeed (slots 79-82).
// Data: float pool 0.0f 0x0122DEDC; the sneak-speed option byte 0x01375749 (descriptive name).
// Layout (retail, local model; the GetMaxSneakingSpeed 0x00664CB0 model): component bitset +0x20, component map +0x44, physics
//   TC +0x60 (landed CCreatureAction_SnapDragon), MovementBandType +0x200 (Ego_r.pdb +0x22C minus 0x10 CThing and 0x1C of
//   container shrink); component 0x36 flag +0x0F, component 0xDF flag +0x0C.
// Shape: the band switch has a jump table with case 4 folded into the default; the band speed is a float local reused as the
//   0x36 lookup key's frame word.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CTCBase;
struct CPhysicsTC {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;        // slot 70 (+0x118)
};
struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase* Find(long kind) const
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return (it == m_pLast || kind < it->m_kind ? m_pLast : it)->m_pTC;
    }
};
struct CTCSneakMovement {                       // component 0x36
    unsigned char _pad00[0x0f];
    bool          m_bSneaking;                  // +0x0f
    float GetMaxSneakingSpeed() const;          // 0x00747020
};
struct CTCMovementModifier {                    // component 0xDF
    unsigned char _pad00[0x0c];
    bool          m_bSprinting;                 // +0x0c
    bool          m_bJogOnly;                   // +0x0d
};
struct CThing {
    virtual void v000();
    unsigned char  _pad04[0x1c];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned char  _pad50[0x60 - 0x50];
    CPhysicsTC*    PhysicsTC;                   // +0x60
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingPlayerCreature : CThing {
    unsigned char _pad64[0x200 - 0x64];
    long MovementBandType;                      // +0x200 EMovementBandType
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078();
    virtual float GetMaxWalkingSpeed(const C3DVector& direction) const;     // slot 79 (+0x13c)
    virtual float GetMaxJoggingSpeed(const C3DVector& direction) const;     // slot 80 (+0x140)
    virtual float GetMaxRunningSpeed(const C3DVector& direction) const;     // slot 81 (+0x144)
    virtual float GetMaxSprintingSpeed(const C3DVector& direction) const;   // slot 82 (+0x148)
    bool IsBlocking() const;                    // 0x00664D00
    float GetCurrentMaxMovementSpeed() const;
    __forceinline float GetBandSpeed(const C3DVector& direction) const
    {
        switch (MovementBandType)
        {
        case 0:
            return 0.0f;
        case 1:
        case 2:
            if (HasTC(0x36))
            {
                CTCSneakMovement* sneak = (CTCSneakMovement*)GetTC(0x36);
                if (sneak->m_bSneaking)
                    return sneak->GetMaxSneakingSpeed();
            }
            return GetMaxWalkingSpeed(direction);
        case 3:
            return GetMaxJoggingSpeed(direction);
        case 4:
            return GetMaxRunningSpeed(direction);
        default:
            return GetMaxRunningSpeed(direction);
        }
    }
};
extern bool g_bSneakSpeedOption;                // 0x01375749

float CThingPlayerCreature::GetCurrentMaxMovementSpeed() const
{
    C3DVector direction = GetPosition() + PhysicsTC->GetFacing();
    if (IsBlocking())
        return 0.0f;
    if (g_bSneakSpeedOption && HasTC(0x36))
    {
        CTCSneakMovement* sneak = (CTCSneakMovement*)GetTC(0x36);
        if (sneak->m_bSneaking)
            return sneak->GetMaxSneakingSpeed();
    }
    float speed = GetBandSpeed(direction);
    if (HasTC(0xdf) && ((CTCMovementModifier*)GetTC(0xdf))->m_bSprinting)
        speed = GetMaxSprintingSpeed(direction);
    return speed;
}
