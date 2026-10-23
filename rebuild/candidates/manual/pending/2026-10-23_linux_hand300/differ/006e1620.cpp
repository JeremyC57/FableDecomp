// CTCDParticleEmitter::FrameUpdate() -- count down the wait for the particle to load (dying when it runs out), die when
// Grade: DIFFER(608v608) -- 6 diff lines: only the copy of ParentThing->GetPosition() into the local is scheduled load,store,load,store in ours vs retail's load,load,store,load,store (plus the C3DOrientationInterpolator vtable reloc shown as 0)
// the emitter is gone or the die-off timer expires, follow an independent object's physics, die with a vanished attached
// thing, then refresh the attachment (hidden state, thing or camera) and re-arm the position/orientation resets
// Leaf: FrameUpdate
//
// Retail 0x006E1620, 608 bytes (plain `ret`). Model as differ/006dffb0.cpp (CTCDParticleEmitter, retail offsets).
// Extra callees: CTCDParticleEmitter::CreateParticleWaitingFor @ 0x006E14E0 / IsParticleCreationDisabled @ 0x006E0DF0 /
// SetParticleCreationDisabled @ 0x006E0690 / UpdateEmitterThingAttachment @ 0x006DFFB0 / UpdateEmitterCameraAttachment
// @ 0x006E0230, CTCPhysicsBase::GetOrientationInterpolator @ 0x006B0460, CParticleEmitter::SetHidden @ 0x006DFDA0,
// CThing::Kill(bool) @ 0x004C9B80, the CTCBase accessor @ 0x00686D60 (slot 9), C3DOrientationInterpolator vtable 0x01239824.
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
    C3DOrientationInterpolator() { }
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
    void SetHidden(bool hidden);                                         // 0x006DFDA0
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
struct C3DOrientationInterpolator;
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
    void GetOrientationInterpolator(C3DOrientationInterpolator* out);   // 0x006B0460
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
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CTCInterface47 {                                      // the attached thing's interface 0x47
    unsigned char _pad00[0x10];
    void* Field10;                                           // +0x10
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    CTCGraphicAppearance* Graphic;                           // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Flags91;                                   // +0x91 bit 0 dead, bit 5 hidden
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    void Kill(bool immediately);                             // 0x004C9B80
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
struct CFrameSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsActive();                                 // slot 9 (+0x24)
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
    CFrameSource* GetFrameSource();                          // 0x00686D60 (CTCBase accessor, [0x013B8A1C]+0x1c)
    void CreateParticleWaitingFor();                         // 0x006E14E0
    bool IsParticleCreationDisabled();                       // 0x006E0DF0
    void SetParticleCreationDisabled(bool disabled);         // 0x006E0690
    void UpdateEmitterCameraAttachment(bool interpolate);    // 0x006E0230
    void FrameUpdate();
    const CWorldMap& PeekWorldMap() const;                   // 0x00686D20 (CTCBase)
    void UpdateEmitterThingAttachment(bool interpolate, float time);
};


void CTCDParticleEmitter::FrameUpdate()
{
    if (WaitingForParticleToLoad) {
        if (--FramesLeftToWaitForParticleToCreate <= 0) {
            ParentThing->Kill(false);
            return;
        }
        CreateParticleWaitingFor();
    }
    if ((!GetFrameSource()->IsActive() && !PEmitter.GetPItem() && !WaitingForParticleToLoad) || DieOffTimer == 0) {
        ParentThing->Kill(true);
        return;
    }
    if (DieOffTimer >= 0)
        --DieOffTimer;
    if (IndependantObject) {
        C3DOrientationInterpolator orientation;
        ParentThing->PhysicsTC->GetOrientationInterpolator(&orientation);
        if (PEmitter.GetPItem()) {
            C3DVector position = ParentThing->GetPosition();
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetPosition(C3DPositionInterpolator(position, position));
            PEmitter.GetPItem()->SetOrientation(orientation);
        } else if (WaitingForParticleToLoad) {
            PCreationParameters->RHSet = orientation.End;
        }
    } else if (AttachedToThing) {
        if (!PAttachedToThing.GetPItem() && !(AttachmentFlags & 0x10))
            ParentThing->Kill(true);
    }
    if (ParentThing->IsDead())
        return;
    if (PAttachedToThing.GetPItem()) {
        if (IsParticleCreationDisabled()) {
            if (!ParticleCreationDisabled)
                SetParticleCreationDisabled(true);
        } else if (ParticleCreationDisabled)
            SetParticleCreationDisabled(false);
        if (PEmitter.GetPItem()) {
            CThing* attached = PAttachedToThing.GetPItem();
            PEmitter.GetPItem()->SetHidden((attached->Flags91 >> 5) & 1);
        }
        CTCInterface47* component;
        if (!PAttachedToThing.GetPItem()->GetInterface(&component, 0x47) || !component->Field10)
            UpdateEmitterThingAttachment(false, 1.0f);
        ResetEmitterPosition = false;
        ResetEmitterOrientation = false;
    } else if (AttachedToCamera)
        UpdateEmitterCameraAttachment(false);
    if (AttachmentFlags & 8) {
        ResetEmitterPosition = true;
        ResetEmitterOrientation = true;
    }
}
