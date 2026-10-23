// CTCDParticleEmitter::UpdateEmitterThingAttachment(bool interpolate, float time) -- for a live (or loading) emitter
// Grade: DIFFER(635v633) -- one hunk: the second thing-orientation vcall needs the wrapper form with the vtable in eax, and the wrapper in both arms gets cross-jumped (merged)
// attached to a thing: take the thing's position and (thing's physics or emitter's own) orientation, refine them from the
// thing's dummy object or helper point, clamp to the water surface and lift by the float offset when asked, and push the
// pending position / orientation resets to the emitter (interpolating unless asked to jump) or the creation parameters
// Leaf: UpdateEmitterThingAttachment
//
// Retail 0x006DFFB0, 631 bytes (`ret 8`). Extra callees: CThing::GetPosition @ 0x004C73D0, the thing physics' slot 72
// (+0x120, the orientation), CTCGraphicAppearance::GetDummyObject @ 0x004C08C0 / GetHelperPoint @ 0x004C0560,
// CTCBase::PeekWorldMap @ 0x00686D20, CWorldMap::GetWaterSizeZAt @ 0x0063A420.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCBase::GetWorld @ 0x00686A70 (camera manager +0x34, virtual
// slot 1 fills the camera position and orientation at time 1.0f), CParticleEmitter::GetCurrentOrientation @ 0x006E1B40,
// the physics TC's slot 30 (+0x78), C3DPositionInterpolator ctor @ 0x006AF570, C3DOrientationInterpolator ctor
// @ 0x005508D0, CParticleEmitter::SetPosition @ 0x00AEDDC0 / SetOrientation @ 0x00AEDE60, ~CBase @ 0x0099A2E0; 4.0f
// @ 0x0125FED8.
// Layout: as the landed SetEmitterPosition (0x006E0500): PEmitter +0x14, AttachmentFlags +0x24 (bit 5 use the camera
// orientation, bit 7 add the float offset), FloatOffset +0x30, PCreationParameters +0x3c (RHSet +0x10), flag byte +0x44
// (bit 0 WaitingForParticleToLoad, bit 4 ResetEmitterPosition, bit 5 ResetEmitterOrientation); CParticleEmitter current
// orientation +0x7c, current position +0xa4.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CRightHandedSet {
    C3DVector Up;                                            // +0x00
    C3DVector Forward;                                       // +0x0c
};
struct CBase {
    virtual void v00();
    CBase();                                                 // 0x0099A2D0
    ~CBase();                                                // 0x0099A2E0
};
struct C3DOrientationInterpolator : CBase {
    CRightHandedSet Start;
    CRightHandedSet End;
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
};
struct C3DPositionInterpolator : CBase {
    C3DVector Start;
    C3DVector End;
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end);                  // 0x006AF570
};
namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char   _pad00[0x7c];
    CRightHandedSet CurrentOrientation;                      // +0x7c
    unsigned char   _pad94[0xa4 - 0x94];
    C3DVector       CurrentPosition;                         // +0xa4
    CRightHandedSet GetCurrentOrientation() const;           // 0x006E1B40
    void SetPosition(const C3DPositionInterpolator& position);          // 0x00AEDDC0
    void SetOrientation(const C3DOrientationInterpolator& orientation); // 0x00AEDE60
};
}
struct CParticleCreationParameters {
    unsigned char   _pad00[0x10];
    CRightHandedSet RHSet;                                   // +0x10
};
template<class T> struct CCountedPointer {
    T*    Object;
    long* PCount;
    T* operator->() const { return Object; }
};
struct CTCPhysicsBase {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29)
#undef V
    virtual void SetPosition(const C3DVector& position);     // slot 30 (+0x78)
#define V(n) virtual void v##n();
    V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49)
    V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69)
    V(70) V(71)
#undef V
    virtual const CRightHandedSet& GetOrientation();         // slot 72 (+0x120)
};
struct CCharString { void* PStringData; };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float time) const;     // 0x004C08C0
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        float time) const;                                   // 0x004C0560
};
struct CWorldMap {
    float GetWaterSizeZAt(const C3DVector& pos) const;       // 0x0063A420
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    CTCGraphicAppearance* Graphic;                           // +0x64
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    const CRightHandedSet& GetOrientation() const { return PhysicsTC->GetOrientation(); }
};
struct CCameraLocation {
    C3DVector       Position;
    CRightHandedSet Orientation;
};
struct CCameraManager {
    virtual void v00();
    virtual CCameraLocation GetCameraLocation(float time);   // slot 1
};
struct CWorld {
    unsigned char   _pad00[0x34];
    CCameraManager* CameraManager;                           // +0x34
    CCameraLocation GetCameraLocation(float time) { return CameraManager->GetCameraLocation(time); }
};
extern const float g_CameraEmitterDistance;                  // 0x0125FED8 (4.0f)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;
    void* GetPItem() const;                                  // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CTCDParticleEmitter {
    virtual void v00();
    CThing*                 ParentThing;                     // +0x04
    unsigned char           _base08[0x4];
    void*                   EmitterName;                     // +0x0c
    long                    ParticleTypeID;                  // +0x10
    CIntelligentPointer<NParticleEngine::CParticleEmitter> PEmitter;   // +0x14
    CIntelligentPointer<CThing> PAttachedToThing;            // +0x1c
    unsigned long           AttachmentFlags;                 // +0x24
    CCharString             AttachmentPosName;               // +0x28
    long                    AttachmentPosIndex;              // +0x2c
    float                   FloatOffset;                     // +0x30
    long                    DieOffTimer;                     // +0x34
    long                    FramesLeftToWaitForParticleToCreate;   // +0x38
    CCountedPointer<CParticleCreationParameters> PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;                       // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;                           // bit 4
    bool ResetEmitterOrientation : 1;                        // bit 5
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool AttachedToThing : 1;                                // +0x45 bit 0
    bool AlwaysUpdate : 1;
    bool Asleep : 1;
    CWorld* GetWorld();                                      // 0x00686A70 (CTCBase)
    const CWorldMap& PeekWorldMap() const;                   // 0x00686D20 (CTCBase)
    void UpdateEmitterThingAttachment(bool interpolate, float time);
};

void CTCDParticleEmitter::UpdateEmitterThingAttachment(bool interpolate, float time)
{
    if (!PEmitter.GetPItem() && !WaitingForParticleToLoad)
        return;
    C3DVector position = PAttachedToThing.GetPItem()->GetPosition();
    CRightHandedSet orientation = PEmitter.GetPItem()
        ? ((AttachmentFlags & 0x20) ? PAttachedToThing.GetPItem()->GetOrientation()
                                    : PEmitter.GetPItem()->GetCurrentOrientation())
        : ((AttachmentFlags & 0x20) ? PAttachedToThing.GetPItem()->PhysicsTC->GetOrientation()
                                    : PCreationParameters->RHSet);
    if (!ResetEmitterPosition && !ResetEmitterOrientation)
        return;
    if (PAttachedToThing.GetPItem()->Graphic) {
        if (AttachmentFlags & 2)
            PAttachedToThing.GetPItem()->Graphic->GetDummyObject(AttachmentPosName, AttachmentPosIndex, 0, &position, &orientation, time);
        else if (AttachmentFlags & 4)
            PAttachedToThing.GetPItem()->Graphic->GetHelperPoint(AttachmentPosName, AttachmentPosIndex, 0, &position, time);
    }
    if (AttachmentFlags & 0x40) {
        float water = PeekWorldMap().GetWaterSizeZAt(position);
        if (water > 0.0f)
            position.Z = water;
    }
    if (AttachmentFlags & 0x80)
        position.Z += FloatOffset;
    if (ResetEmitterPosition) {
        ParentThing->PhysicsTC->SetPosition(position);
        if (PEmitter.GetPItem()) {
            const C3DVector& start = interpolate ? position : PEmitter.GetPItem()->CurrentPosition;
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetPosition(C3DPositionInterpolator(start, position));
        }
    }
    if (ResetEmitterOrientation) {
        if (PEmitter.GetPItem()) {
            const CRightHandedSet& start = interpolate ? orientation : PEmitter.GetPItem()->CurrentOrientation;
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetOrientation(C3DOrientationInterpolator(start, orientation));
        } else {
            PCreationParameters->RHSet = orientation;
        }
    }
}
