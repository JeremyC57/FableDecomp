// CTCDParticleEmitter::UpdateEmitterCameraAttachment(bool interpolate) -- for a live (or loading) emitter attached to the
// Grade: DIFFER -- frame 0x7c vs retail 0x98 (210 diff lines): the orientation copy is store-sunk and the camera-location temps are laid out differently
// camera: place it 4 units along the camera's forward vector (optionally lifted by the float offset), facing the camera's
// orientation (or keeping its own), and push the pending position / orientation resets to the emitter (interpolating from
// its current state unless asked to jump) or to the parent's physics / the creation parameters
// Leaf: UpdateEmitterCameraAttachment
//
// Retail 0x006E0230, 576 bytes (`ret 4`).
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
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
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
    NParticleEngine::CParticleEmitter* GetPItem() const;     // 0x00A01B50
};
struct CTCDParticleEmitter {
    virtual void v00();
    CThing*                 ParentThing;                     // +0x04
    unsigned char           _base08[0x4];
    void*                   EmitterName;                     // +0x0c
    long                    ParticleTypeID;                  // +0x10
    CBaseIntelligentPointer PEmitter;                        // +0x14
    CBaseIntelligentPointer PAttachedToThing;                // +0x1c
    unsigned long           AttachmentFlags;                 // +0x24
    void*                   AttachmentPosName;               // +0x28
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
    void UpdateEmitterCameraAttachment(bool interpolate);
};

void CTCDParticleEmitter::UpdateEmitterCameraAttachment(bool interpolate)
{
    if (!PEmitter.GetPItem() && !WaitingForParticleToLoad)
        return;
    CCameraLocation camera = GetWorld()->GetCameraLocation(1.0f);
    C3DVector position = camera.Position + camera.Orientation.Forward * 4.0f;
    CRightHandedSet orientation = (AttachmentFlags & 0x20) ? camera.Orientation
        : (PEmitter.GetPItem() ? PEmitter.GetPItem()->GetCurrentOrientation() : PCreationParameters->RHSet);
    if (!ResetEmitterPosition && !ResetEmitterOrientation)
        return;
    if (AttachmentFlags & 0x80)
        position.Z += FloatOffset;
    if (ResetEmitterPosition) {
        ParentThing->PhysicsTC->SetPosition(position);
        if (PEmitter.GetPItem()) {
            const C3DVector& start = interpolate ? position : PEmitter.GetPItem()->CurrentPosition;
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetPosition(C3DPositionInterpolator(start, position));
        }
        ResetEmitterPosition = false;
    }
    if (ResetEmitterOrientation) {
        if (PEmitter.GetPItem()) {
            const CRightHandedSet& start = interpolate ? orientation : PEmitter.GetPItem()->CurrentOrientation;
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetOrientation(C3DOrientationInterpolator(start, orientation));
        } else {
            PCreationParameters->RHSet = orientation;
        }
        ResetEmitterOrientation = false;
    }
}
