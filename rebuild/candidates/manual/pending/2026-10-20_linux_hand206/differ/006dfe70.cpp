// Grade: DIFFER(178v178) -- scheduling only: the params temporary's vtable store sinks below the physics vtable load and the push (retail stores it before 'mov edx,[edi]'); same shape as 00779d20
// CTCDParticleEmitter::Construct(long particleType, const C3DVector& position, bool independent, bool forceLoad) -- place the emitter's physics at the position with the default orientation, then record the particle type and the IndependantObject / ForceParticleToLoad flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DFE70, 178 bytes (`ret 10h`, 0x28-byte frame = the physics params object).
// Callees: CBaseClassNonCopyable ctor @ 0x0099A2F0 / ~CBaseClassNonCopyable @ 0x0099A300 (the CTCParamsPhysicsBase temporary,
// vtable 0x0125C818), the physics TC's virtual slot 3 ConstructFromParams (+0x0c). Global: a default CRightHandedSet
// @ 0x01380104 (six floats; named descriptively).
// Layout: CThing PhysicsTC +0x60 (retail); CTCParamsPhysicsBase as the landed ctor 0x006A06E0 (Position +0x04, Rhs +0x10);
// Ego_r.pdb CTCDParticleEmitter: ParticleTypeID +0x10, the bool:1 group at +0x44 (WaitingForParticleToLoad bit 0,
// ForceParticleToLoad bit 1, IndependantObject bit 2, ...).
// Shape: the physics TC read into a local first; the params temporary is the two-argument inline ctor (memberwise position
// and Rhs copies through integer registers, vtable stored last); the two one-bit stores merge into one read-modify-write
// (`and 0F9h`).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
extern CRightHandedSet g_DefaultRightHandedSet;         // 0x01380104
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                            // 0x0099A2F0
    ~CBaseClassNonCopyable();                           // 0x0099A300
};
struct CTCParamsBase : CBaseClassNonCopyable { };
struct CTCParamsPhysicsBase : CTCParamsBase {           // vtable 0x0125C818
    C3DVector       Position;                           // +0x04
    CRightHandedSet Rhs;                                // +0x10
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs) : Position(position), Rhs(rhs) { }
    virtual void v00();
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void ConstructFromParams(CTCParamsBase* params);   // slot 3 (+0x0c)
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
struct CTCDParticleEmitter {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    long ParticleTypeID;                                // +0x10
    unsigned char _pad14[0x44 - 0x14];
    bool WaitingForParticleToLoad : 1;                  // +0x44 bit 0
    bool ForceParticleToLoad : 1;                       // +0x44 bit 1
    bool IndependantObject : 1;                         // +0x44 bit 2
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    void Construct(long particleType, const C3DVector& position, bool independent, bool forceLoad);
};

void CTCDParticleEmitter::Construct(long particleType, const C3DVector& position, bool independent, bool forceLoad)
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    physics->ConstructFromParams(&CTCParamsPhysicsBase(position, g_DefaultRightHandedSet));
    ParticleTypeID = particleType;
    IndependantObject = independent;
    ForceParticleToLoad = forceLoad;
}
