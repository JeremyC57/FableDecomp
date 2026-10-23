// CTCDParticleEmitter::SetEmitterPosition(const C3DVector& position) -- move the live emitter (start and end position both `position`), or else reset the component's physics position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0500, length 142 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CBase::CBase @ 0x0099A2D0 / ~CBase @ 0x0099A2E0, NParticleEngine::
// CParticleEmitter::SetPosition(const C3DPositionInterpolator&) @ 0x00AEDDC0, CTCPhysicsBase::ResetPosition (slot 31, +0x7C).
// Shape: the C3DPositionInterpolator temporary is built by an inline ctor (out-of-line CBase ctor, two vector copies, vtable
// 0x0123981C; C3DVector's user copy ctor gives the register rotation) before the object expression `PEmitter.GetPItem()` is evaluated; the parent's physics is at the retail CThing +0x60.
// Layout: Ego_r.pdb CTCDParticleEmitter (0x48 bytes, retail identical; see the landed SetAlwaysUpdate 0x006E0590): PEmitter +0x14
// (CIntelligentPointer<NParticleEngine::CParticleEmitter>), PAttachedToThing +0x1C, PCreationParameters +0x3C (CCountedPointer:
// object +0x3C, count block +0x40), flag bytes +0x44 (bit 0 WaitingForParticleToLoad, bit 3 ParticleCreationDisabled) and +0x45
// (bit 0 AttachedToThing, bit 1 AlwaysUpdate, bit 2 Asleep). CParticleCreationParameters: ParticleCreationDisabled +1, RHSet +0x10.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
namespace N_0099a2d0 { struct CBase {                                  // polymorphic root of the interpolators
    virtual void v00();
    CBase();                                    // 0x0099A2D0
    ~CBase();                                   // 0x0099A2E0
}; } using N_0099a2d0::CBase;
struct C3DOrientationInterpolator : CBase {     // 0x34 bytes
    CRightHandedSet Start;                      // +0x04
    CRightHandedSet End;                        // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
};
struct C3DPositionInterpolator : CBase {        // 0x1c bytes, vtable 0x0123981C
    C3DVector Start;                            // +0x04
    C3DVector End;                              // +0x10
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end) : Start(start), End(end) {}
    virtual void v01();
};
namespace NParticleEngine {
struct CParticleEmitter {
    void SetPosition(const C3DPositionInterpolator& position);          // 0x00AEDDC0
    void SetOrientation(const C3DOrientationInterpolator& orientation); // 0x00AEDE60
    void Kill();                                                         // 0x00AEE8C0
};
}
struct CParticleCreationParameters {
    bool InterpolatingParticle;                 // +0x00
    bool ParticleCreationDisabled;              // +0x01
    bool UpdateParametersSet;                   // +0x02
    bool AlwaysUpdateFlag;                      // +0x03
    long StartParticleID;                       // +0x04
    long EndParticleID;                         // +0x08
    float Interpolate;                          // +0x0c
    CRightHandedSet RHSet;                      // +0x10
};
template<class T> struct CCountedPointer {
    T*    Object;
    long* PCount;
    T* operator->() const { return Object; }
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);     // slot 31 (+0x7c)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {                // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                // +0x04
    NParticleEngine::CParticleEmitter* GetPItem() const;   // 0x00A01B50
};
struct CTCDParticleEmitter {
    virtual void v00();
    CThing*                 ParentThing;        // +0x04 (CTCBase)
    unsigned char           _base08[0x4];
    void*                   EmitterName;        // +0x0c CCharString
    long                    ParticleTypeID;     // +0x10
    CBaseIntelligentPointer PEmitter;           // +0x14
    CBaseIntelligentPointer PAttachedToThing;   // +0x1c
    unsigned long           AttachmentFlags;    // +0x24
    void*                   AttachmentPosName;  // +0x28 CCharString
    long                    AttachmentPosIndex; // +0x2c
    float                   FloatOffset;        // +0x30
    long                    DieOffTimer;        // +0x34
    long                    FramesLeftToWaitForParticleToCreate;   // +0x38
    CCountedPointer<CParticleCreationParameters> PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;          // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;          // bit 3
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool AttachedToThing : 1;                   // +0x45 bit 0
    bool AlwaysUpdate : 1;
    bool Asleep : 1;
    void SetEmitterOrientation(const CRightHandedSet& rhs);
    void SetEmitterPosition(const C3DVector& position);
};

void CTCDParticleEmitter::SetEmitterPosition(const C3DVector& position)
{
    if (PEmitter.GetPItem())
        PEmitter.GetPItem()->SetPosition(C3DPositionInterpolator(position, position));
    else
        ParentThing->PhysicsTC->ResetPosition(position);
}