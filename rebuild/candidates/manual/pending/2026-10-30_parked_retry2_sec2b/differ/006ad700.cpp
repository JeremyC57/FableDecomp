// Grade: DIFFER(615v615) 11 difflines (was 49): frame 0x2c and the radius/key slot sharing now match (out-parameter __forceinline GetConstInterface(&hitLocations, 0x7f) replaces HasTC+GetConstTC), push scale as the literal 0.1f (component-first fmul), epsilon test X*X+Y*Y; residue: acceleration dot loads Z before Y (VC canonicalises every grouping and a member Dot), and away*along Y product fld st(1);fmul [Y] vs retail fld [Y];fmul st(2) (retry wave 2 sec2b)
// Was: DIFFER(615v615) -- x87 operand order: the 0.1 push vector loads the constant before each component (retail loads the component, then fmul by the constant); the acceleration dot product starts with Z where retail starts with Y; the projection keeps different stack slots; radius takes its own frame word instead of reusing the 0x7f key slot (+4 frame)
// CThingPlayerCreature::ControlledMove() -- compute the movement acceleration; while a combat target is locked and the hero has hit locations, push the hero out of the target's hit sphere (unless in mode 0x19 inside it) and drop the acceleration component towards it; then apply the acceleration to the physics (or the impulse velocity) unless the current action overrides it, and clear it when the action says so
// Leaf: ?ControlledMove@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AD700, length 615 bytes (`ret`).
// Callees: ComputeAcceleration @ 0x006AD260, CThing::GetWorld-style accessor @ 0x004C7AA0 (+0x34 object, virtual +0x28 lock
//   target out), LowerBound const @ 0x004365B0, CTCHitLocations::GetBoundingSphere @ 0x00815420 /
//   GetPCombatFirstHitLocationPosInside @ 0x00815870, the target's type getter @ 0x00697AB0, CThing::GetPlayer @ 0x004C7A30,
//   CPlayer::GetCurrentMode @ 0x00633BE0, C3DVector::Normalise-style @ 0x00A14440, the physics TC's virtual +0x144.
// Constants: 0.1f @ 0x0122EE58, 1e-4f @ 0x0129BA3C.
// Layout (retail; Ego_r.pdb in brackets): CThing PhysicsTC +0x60, presence bitset +0x20 (0x7f = sign bit of +0x2c), map +0x44;
//   CThingCreatureBase current-action pointer +0x100; CThingPlayerCreature MovementAcceleration +0x180 [+0x1a8],
//   ImpulseVelocity +0x1f0 [+0x21c].
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
    C3DVector& operator+=(const C3DVector& o) { X = o.X + X; Y = o.Y + Y; Z = o.Z + Z; return *this; }
    void Normalise();                                   // 0x00A14440
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CTCHitLocations {
    void GetBoundingSphere(C3DVector* centre, float* radius, bool a) const;     // 0x00815420
    void* GetPCombatFirstHitLocationPosInside(const C3DVector* position) const; // 0x00815870
};
struct CLockTarget {
    long Unknown00;
    C3DVector Position;                                 // +0x04
    long GetType() const;                               // 0x00697AB0
};
struct CTargeting {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool GetLockTarget(CLockTarget** out);      // slot 10 (+0x28)
};
struct CWorldLike {
    unsigned char _pad00[0x34];
    CTargeting* Targeting;                              // +0x34
    CTargeting* GetTargeting() const { return Targeting; }
};
extern const float g_fEpsilon;                          // 0x0129BA3C (1e-4f)
extern const float g_fLockPushScale;                    // 0x0122EE58 (0.1f)
__forceinline float DistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return dz * dz + dy * dy + dx * dx;
}
struct CPlayer {
    long GetCurrentMode() const;                        // 0x00633BE0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50();
};
struct CTCPhysicsBaseV : CTCPhysicsBase {
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void ApplyAcceleration(const C3DVector& acceleration);   // slot 81 (+0x144)
};
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual bool ClearsMovementAcceleration();          // slot 35 (+0x8c)
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48();
    virtual bool OverridesMovement();                   // slot 49 (+0xc4)
    unsigned char _pad04[0x61 - 0x04];
    bool Finished;                                      // +0x61
};
class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CPlayer* GetPlayer() const;                         // 0x004C7A30
    CWorldLike* GetWorld() const;                       // 0x004C7AA0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetConstInterface(const T** out, long type) const
    {
        if (!HasTC(type))
            return false;
        *out = (const T*)GetConstTC(type);
        return true;
    }
    __forceinline const CTCBase* GetConstTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
class CThingPlayerCreature : public CThing {
public:
    unsigned char _pad64[0x100 - 0x64];
    CCreatureActionBase* PCurrentAction;                // +0x100
    unsigned char _pad104[0x180 - 0x104];
    C3DVector MovementAcceleration;                     // +0x180
    unsigned char _pad18c[0x1f0 - 0x18c];
    C3DVector ImpulseVelocity;                          // +0x1f0
    void ComputeAcceleration();                         // 0x006AD260
    void ControlledMove();
};

void CThingPlayerCreature::ControlledMove()
{
    ComputeAcceleration();
    CLockTarget* target;
    const CTCHitLocations* hitLocations;
    if (GetWorld()->GetTargeting()->GetLockTarget(&target) && GetConstInterface(&hitLocations, 0x7f)) {
        const C3DVector* position = &target->Position;
        C3DVector centre;
        float radius;
        hitLocations->GetBoundingSphere(&centre, &radius, true);
        if (target->GetType() != 8) {
            if (GetPlayer()->GetCurrentMode() != 0x19 || !(DistanceSquared(*position, centre) < radius * radius)) {
                if (!hitLocations->GetPCombatFirstHitLocationPosInside(position))
                    goto apply;
            }
            {
                C3DVector away(centre.X - position->X, centre.Y - position->Y, 0.0f);
                if (away.X * away.X + away.Y * away.Y > g_fEpsilon * g_fEpsilon)
                    away.Normalise();
                ((CTCPhysicsBaseV*)PhysicsTC)->ApplyAcceleration(C3DVector(away.X * 0.1f, away.Y * 0.1f, away.Z * 0.1f));
                float along = away.X * MovementAcceleration.X + (away.Z * MovementAcceleration.Z + away.Y * MovementAcceleration.Y);
                MovementAcceleration -= away * along;
            }
        }
    }
apply:
    CCreatureActionBase* action = PCurrentAction;
    if (!action || action->Finished || !action->OverridesMovement()) {
        const C3DVector& acceleration = MovementAcceleration;
        if (PhysicsTC)
            ((CTCPhysicsBaseV*)PhysicsTC)->ApplyAcceleration(acceleration);
        else
            ImpulseVelocity += acceleration;
    }
    if (action && action->ClearsMovementAcceleration())
        MovementAcceleration.X = MovementAcceleration.Y = MovementAcceleration.Z = 0.0f;
}
