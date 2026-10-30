// NCameraModes::CScriptModuleSpline::Update() -- while the scripted-spline camera point is alive and has its spline component: advance the spline (index, position, view direction, FOV, roll, finished flag), latch ReallyFinished for a self-terminating spline, normalise the direction, aim the owner camera at the focal-plane point and hand it the position, FOV and roll; with no live point (or no spline component) the module is finished
// Grade: DIFFER(364v364) -- GetTC key slot not shared with the roll local (frame 0x30 v 0x2c); tried (this sweep): the lookup in its own block, the key in a nested block inside the inline GetTC (both inert)
// Leaf: ?Update@CScriptModuleSpline
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854180, 364 bytes, plain `ret`; vtable 0x01275A6C slot 1 (CScriptModuleBase::Update).
//   FableWin.pdb ?Update@CScriptModuleSpline@NCameraModes@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice: the alive test, then the GetTC lookup),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const long*) @ 0x0040F020 (the inlined GetTC for type 0x3f),
//   CTCCameraPointScriptedSpline::UpdateCameraParams(float, C3DVector&, C3DVector&, float&, float&, bool&, const C3DVector*,
//   const CRightHandedSet*) @ 0x0075D580 (Ego_r.pdb ?UpdateCameraParams@CTCCameraPointScriptedSpline@@QAEMMAAVC3DVector@@0AAM1AA_NPBV2@PBVCRightHandedSet@@@Z),
//   C3DVector::Normalise @ 0x00A14440 (manifest "GetScaled" is propagated), CScriptModuleBase::GetViewPosInFocalPlane @
//   0x00853320 (landed; hidden result pointer).
// Layout: Ego_r.pdb CScriptModuleSpline (landed OnInitLeaf 0x008540F0): CoordOrigin +0x14, CoordAxis +0x20, POwner +0x38,
//   Index +0x4c, CurrentSplineFOV +0x50, CurrentSplineRollAngle +0x54, ReallyFinished +0x58, FinishedSpline +0x59; the
//   owner camera (landed CScriptModule OnInitLeaf rows): LookAt +0x10, Position +0x54, FOV +0x84, RollAngle +0x88 (PDB
//   CameraFOV / RollAngle); CTCCameraPointDefinitionBase SelfTerminate +0x14; CThing dead bit 0 of +0x91.
// Shape: the alive test is an inline taking the pointer once; the epsilon-squared length test as the landed
//   GetViewPosInFocalPlane.
extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise();                                // 0x00A14440
};
struct CRightHandedSet {
    C3DVector Up;                                    // +0x00
    C3DVector Forward;                               // +0x0c
};

struct CTCBase;
struct CInterfaceMap {                               // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_dead : 1;                        // +0x91 bit 0
    unsigned char m_91_1 : 7;
    template<class T> __forceinline bool GetTC(T** ppOut, long interfaceType)
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
inline bool IsThingAlive(const CThing* thing) { return thing && !thing->m_dead; }

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CTCCameraPointScriptedSpline {
    unsigned char _pad00[0x14];
    bool          SelfTerminate;                     // +0x14 (CTCCameraPointDefinitionBase)
    float UpdateCameraParams(float index, C3DVector& pos, C3DVector& direction, float& fov, float& roll, bool& finished,
                             const C3DVector* origin, const CRightHandedSet* axis);   // 0x0075D580
};
namespace NCameraModes {
struct CBase {                                       // the owning camera mode (retail offsets)
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x54 - 0x1c];
    C3DVector     Position;                          // +0x54 GoTo.Position
    unsigned char _pad60[0x84 - 0x60];
    float         FOV;                               // +0x84 CameraFOV
    float         RollAngle;                         // +0x88
};
struct CScriptModuleBase {                           // vtable 0x01255F74, 0x4c bytes
    virtual ~CScriptModuleBase();                    // slot 0
    virtual void Update() = 0;                       // slot 1
    CBaseIntelligentPointer PCamPoint;               // +0x04
    CBaseIntelligentPointer PThingOrigin;            // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CBase*              POwner;                      // +0x38
    void*               PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
    CThing* GetPCameraPoint() const { return (CThing*)PCamPoint.GetPItem(); }
    C3DVector GetViewPosInFocalPlane(const C3DVector& pos, const C3DVector& dir);   // 0x00853320
};
struct CScriptModuleSpline : CScriptModuleBase {    // vtable 0x01275A6C
    float Index;                                     // +0x4c
    float CurrentSplineFOV;                          // +0x50
    float CurrentSplineRollAngle;                    // +0x54
    bool  ReallyFinished;                            // +0x58
    bool  FinishedSpline;                            // +0x59
    long  NodeToStartOn;                             // +0x5c
    long  NodeToEndOn;                               // +0x60
    virtual void Update();
};

void CScriptModuleSpline::Update()
{
    CTCCameraPointScriptedSpline* spline;
    if (!IsThingAlive(GetPCameraPoint()) || !GetPCameraPoint()->GetTC(&spline, 0x3f)) {
        FinishedSpline = true;
        ReallyFinished = true;
        return;
    }
    if (!FinishedSpline) {
        C3DVector pos;
        C3DVector direction;
        float fov;
        float roll;
        Index = spline->UpdateCameraParams(Index, pos, direction, fov, roll, FinishedSpline, &CoordOrigin, &CoordAxis);
        CurrentSplineFOV = fov;
        CurrentSplineRollAngle = roll;
        if (FinishedSpline && spline->SelfTerminate)
            ReallyFinished = true;
        if (direction.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            direction.Normalise();
        C3DVector lookAt = GetViewPosInFocalPlane(pos, direction);
        POwner->Position = pos;
        POwner->LookAt = lookAt;
        POwner->FOV = fov;
        POwner->RollAngle = roll;
    }
}
}
