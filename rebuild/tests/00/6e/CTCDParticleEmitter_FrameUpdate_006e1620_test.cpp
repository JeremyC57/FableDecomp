// Behaviour test for 006e1620 CTCDParticleEmitter::FrameUpdate: final.cpp body verbatim; the engine calls are stubs
// recording their arguments.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
//   (`SetPositionFrom(ParentThing->GetPosition())`): the by-value argument copy gives retail's load-ahead interleave.
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
    __forceinline void SetPositionFrom(C3DVector position)
    {
        NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
        emitter->SetPosition(C3DPositionInterpolator(position, position));
    }
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
            SetPositionFrom(ParentThing->GetPosition());
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

CBase::CBase() {}
void CBase::v00() {}
void CBaseIntelligentPointer::v00() {}
CBase::~CBase() {}
C3DPositionInterpolator::C3DPositionInterpolator(const C3DVector& s, const C3DVector& e) : Start(s), End(e) {}
static int g_setPos = 0, g_setOri = 0, g_hidden = -1, g_kill = 0, g_killArg = -1, g_create = 0, g_thingAttach = 0, g_camAttach = 0;
static C3DPositionInterpolator* g_lastPos = 0;
static C3DVector g_posStart, g_posEnd;
namespace NParticleEngine {
void CParticleEmitter::SetPosition(const C3DPositionInterpolator& p) { ++g_setPos; g_posStart = p.Start; g_posEnd = p.End; }
void CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& o) { ++g_setOri; }
void CParticleEmitter::SetHidden(bool h) { g_hidden = h ? 1 : 0; }
}
#define V(n) void CTCPhysicsBase::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29)
V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49)
V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69)
V(70) V(71)
#undef V
void CTCPhysicsBase::SetPosition(const C3DVector&) {}
static CRightHandedSet g_ori;
const CRightHandedSet& CTCPhysicsBase::GetOrientation() { return g_ori; }
static int g_getOri = 0;
void CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator* out) { ++g_getOri; out->End.Up = C3DVector(0, 0, 7); }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
static C3DVector g_parentPos(1.5f, -2.5f, 3.25f);
const C3DVector& CThing::GetPosition() const { return g_parentPos; }
void CThing::Kill(bool immediately) { ++g_kill; g_killArg = immediately ? 1 : 0; }
void* CBaseIntelligentPointer::GetPItem() const { return PData; }
void CFrameSource::v00() {} void CFrameSource::v01() {} void CFrameSource::v02() {} void CFrameSource::v03() {}
void CFrameSource::v04() {} void CFrameSource::v05() {} void CFrameSource::v06() {} void CFrameSource::v07() {}
void CFrameSource::v08() {}
static bool g_active = true;
bool CFrameSource::IsActive() { return g_active; }
static CFrameSource g_frameSource;
CFrameSource* CTCDParticleEmitter::GetFrameSource() { return &g_frameSource; }
CWorld* CTCDParticleEmitter::GetWorld() { return 0; }
void CTCDParticleEmitter::CreateParticleWaitingFor() { ++g_create; }
static bool g_creationDisabled = false;
bool CTCDParticleEmitter::IsParticleCreationDisabled() { return g_creationDisabled; }
static int g_setDisabled = -1;
void CTCDParticleEmitter::SetParticleCreationDisabled(bool d) { g_setDisabled = d ? 1 : 0; ParticleCreationDisabled = d; }
void CTCDParticleEmitter::UpdateEmitterCameraAttachment(bool) { ++g_camAttach; }
void CTCDParticleEmitter::UpdateEmitterThingAttachment(bool, float) { ++g_thingAttach; }
const CWorldMap& CTCDParticleEmitter::PeekWorldMap() const { return *(const CWorldMap*)0; }
void CTCDParticleEmitter::v00() {}

static void Reset(CTCDParticleEmitter& e)
{
    e.PEmitter.PData = 0; e.PAttachedToThing.PData = 0; e.AttachmentFlags = 0; e.DieOffTimer = -1;
    e.FramesLeftToWaitForParticleToCreate = 0; e.PCreationParameters.Object = 0; e.PCreationParameters.PCount = 0;
    e.WaitingForParticleToLoad = false; e.ForceParticleToLoad = false; e.IndependantObject = false;
    e.ParticleCreationDisabled = false; e.ResetEmitterPosition = false; e.ResetEmitterOrientation = false;
    e.AttachedToCamera = false; e.Moving = false; e.AttachedToThing = false; e.AlwaysUpdate = false; e.Asleep = false;
    g_kill = 0; g_killArg = -1; g_setPos = 0; g_setOri = 0; g_create = 0; g_thingAttach = 0; g_camAttach = 0;
    g_hidden = -1; g_setDisabled = -1; g_getOri = 0;
}

static int Check()
{
    CThing parent; parent.Flags91 = 0; parent.PhysicsTC = 0;
    CTCPhysicsBase physics; parent.PhysicsTC = &physics;
    NParticleEngine::CParticleEmitter emitter;
    CTCDParticleEmitter e; e.ParentThing = &parent;
    // 1: waiting runs out -> Kill(false)
    Reset(e); e.WaitingForParticleToLoad = true; e.FramesLeftToWaitForParticleToCreate = 1;
    e.FrameUpdate();
    if (g_kill != 1 || g_killArg != 0 || g_create != 0 || e.FramesLeftToWaitForParticleToCreate != 0) return 1;
    // 2: still waiting -> create, then the die-off timer at 0 -> Kill(true)
    Reset(e); e.WaitingForParticleToLoad = true; e.FramesLeftToWaitForParticleToCreate = 5; e.DieOffTimer = 0;
    e.FrameUpdate();
    if (g_create != 1 || g_kill != 1 || g_killArg != 1) return 2;
    // 3: inactive frame source and no emitter (not waiting) -> Kill(true)
    Reset(e); g_active = false;
    e.FrameUpdate();
    g_active = true;
    if (g_kill != 1 || g_killArg != 1) return 3;
    // 4: independent object with an emitter: position interpolator from the parent position, orientation set; timer counts
    Reset(e); e.IndependantObject = true; e.PEmitter.PData = &emitter; e.DieOffTimer = 4;
    e.FrameUpdate();
    if (g_kill != 0 || e.DieOffTimer != 3 || g_getOri != 1 || g_setPos != 1 || g_setOri != 1) return 4;
    if (g_posStart.X != 1.5f || g_posStart.Y != -2.5f || g_posStart.Z != 3.25f) return 5;
    if (g_posEnd.X != 1.5f || g_posEnd.Y != -2.5f || g_posEnd.Z != 3.25f) return 6;
    // 5: independent, no emitter, waiting -> creation parameters take the orientation end
    Reset(e); e.IndependantObject = true; e.WaitingForParticleToLoad = true; e.FramesLeftToWaitForParticleToCreate = 9;
    CParticleCreationParameters params; params.RHSet.Up = C3DVector(0, 0, 0);
    e.PCreationParameters.Object = &params;
    e.FrameUpdate();
    if (params.RHSet.Up.Z != 7.0f || g_setPos != 0) return 7;
    // 6: attached to a vanished thing without flag 0x10 -> Kill(true), then dead -> stop (no camera update)
    Reset(e); e.AttachedToThing = true; e.AttachedToCamera = true; parent.Flags91 = 0;
    e.PEmitter.PData = &emitter;
    {
        // Kill stub does not set the dead bit, so the attachment block runs: camera branch
        e.FrameUpdate();
        if (g_kill != 1 || g_camAttach != 1) return 8;
    }
    // 7: attached thing present: creation-disabled sync, hidden from bit 5, no interface 0x47 -> UpdateEmitterThingAttachment
    Reset(e);
    CThing attached; attached.Flags91 = 0x20;
    for (int i = 0; i < 9; ++i) attached.m_presentBits[i] = 0;
    attached.m_interfaces.first = attached.m_interfaces.last = 0;
    e.PAttachedToThing.PData = &attached; e.PEmitter.PData = &emitter; e.ResetEmitterPosition = true;
    g_creationDisabled = true;
    e.FrameUpdate();
    if (g_setDisabled != 1 || g_hidden != 1 || g_thingAttach != 1 || e.ResetEmitterPosition || e.ResetEmitterOrientation) return 9;
    // 8: interface 0x47 present with Field10 set -> no attachment update; flag 8 re-arms the resets
    Reset(e);
    CTCInterface47 comp; comp.Field10 = &comp;
    CInterfaceMap::SEntry entries[1]; entries[0].type = 0x47; entries[0].component = (CTCBase*)&comp;
    attached.m_presentBits[0x47 >> 5] = 1UL << (0x47 & 31);
    attached.m_interfaces.first = entries; attached.m_interfaces.last = entries + 1;
    attached.Flags91 = 0;
    e.PAttachedToThing.PData = &attached; e.ParticleCreationDisabled = true; e.AttachmentFlags = 8;
    g_creationDisabled = false;
    e.FrameUpdate();
    if (g_setDisabled != 0 || g_thingAttach != 0 || g_hidden != -1 || !e.ResetEmitterPosition || !e.ResetEmitterOrientation) return 10;
    // 9: dead parent stops before the attachment block
    Reset(e); parent.Flags91 = 1; e.AttachedToCamera = true;
    e.FrameUpdate();
    if (g_camAttach != 0) return 11;
    return 0;
}

int main()
{
    int r = Check();
    if (r == 0) printf("006e1620_TEST PASS\n"); else printf("006e1620_TEST FAIL %d\n", r);
    return r;
}