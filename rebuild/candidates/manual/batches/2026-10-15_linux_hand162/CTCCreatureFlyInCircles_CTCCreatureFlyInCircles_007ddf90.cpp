// CTCCreatureFlyInCircles::CTCCreatureFlyInCircles(CThing* parent) -- circling-flight component: the creature is the parent, two null intelligent pointers, an empty swoop spline (tension 0.5) sized to three control points, speed 3, fly mode 1, active
// Leaf: ??0CTCCreatureFlyInCircles
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DDF90, length 156 bytes (+ int3 padding). Vtable 0x0126FB04.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (twice: then the
// CIntelligentPointer<CThing> vtable 0x01238C6C and the CIntelligentPointer<const CThingCreatureBase> vtable 0x0125AAEC),
// vector<C3DVector>::resize(size_type, const C3DVector&) @ 0x0075C040 (the manifest's "vector<CBrainBehaviour>::resize" is a
// byte twin over the same 12-byte element).
// Layout: Ego_r.pdb CTCCreatureFlyInCircles is `#pragma pack(1)`-like (PBurninationEffect at the unaligned +0x3B): Creature +0xC,
// Centre +0x10 (unset), Speed +0x1C, DeltaSpeed +0x20, SpeedChangeTicks +0x24, CurrentRadius +0x28, DeltaRadius +0x2C,
// RadiusChangeTicks +0x30, Mode +0x34, Transitioning +0x38, ModeChangeQueued +0x39, Active +0x3A, PBurninationEffect +0x3B,
// HasBeenHit +0x43, PSwoopTarget +0x44, SwoopSpline +0x4C (retail CCardinalSpline 0x14: vector, Tension +0xC,
// EndPointsConnected +0x10), SwoopTargOffset +0x60, SwoopProgress +0x6C, SwoopDelta +0x70, LastExplodeTick +0x74.
// Shape: Creature is the only initialiser read from the base (stored before the vtable store); the spline's inline ctor zeroes
// the vector, clears the flag and sets the tension; the scalars are body assignments in retail store order, the tension is set
// once more, then the control points are resized with a default C3DVector temporary (the uninitialised 12-byte frame).
struct CThing;
struct CThingCreatureBase;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* parent);                    // 0x00686800
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
struct CVector3D {                              // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    SRetailAllocProxy<C3DVector> _M_end_of_storage;
    CVector3D() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
};
struct CCardinalSpline {                        // retail 0x14 bytes
    CVector3D ControlPoints;                    // +0x00
    float     Tension;                          // +0x0c
    bool      EndPointsConnected;               // +0x10
    CCardinalSpline() : EndPointsConnected(false) { Tension = 0.5f; }
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {                 // vtable 0x01238C6C
    virtual void v01();
    CIntelligentPointerThing() {}
};
struct CIntelligentPointerConstCreature : CBaseIntelligentPointer {         // vtable 0x0125AAEC
    virtual void v01();
    CIntelligentPointerConstCreature() {}
};
struct CTCCreatureFlyInCircles : CTCBase {
    CThingCreatureBase* Creature;               // +0x0c
    C3DVector Centre;                           // +0x10
    float Speed;                                // +0x1c
    float DeltaSpeed;                           // +0x20
    long  SpeedChangeTicks;                     // +0x24
    float CurrentRadius;                        // +0x28
    float DeltaRadius;                          // +0x2c
    long  RadiusChangeTicks;                    // +0x30
    long  Mode;                                 // +0x34 EFlyMode
    bool  Transitioning;                        // +0x38
    bool  ModeChangeQueued;                     // +0x39
    bool  Active;                               // +0x3a
    CIntelligentPointerThing PBurninationEffect;            // +0x3b
    bool  HasBeenHit;                           // +0x43
    CIntelligentPointerConstCreature PSwoopTarget;          // +0x44
    CCardinalSpline SwoopSpline;                // +0x4c
    C3DVector SwoopTargOffset;                  // +0x60
    float SwoopProgress;                        // +0x6c
    float SwoopDelta;                           // +0x70
    long  LastExplodeTick;                      // +0x74
    virtual void v01();
    CTCCreatureFlyInCircles(CThing* parent);
};
#pragma pack(pop)

CTCCreatureFlyInCircles::CTCCreatureFlyInCircles(CThing* parent)
    : CTCBase(parent)
    , Creature((CThingCreatureBase*)ParentThing)
{
    Active = true;
    Mode = 1;
    Transitioning = false;
    ModeChangeQueued = false;
    HasBeenHit = false;
    CurrentRadius = 0.0f;
    DeltaRadius = 0.0f;
    RadiusChangeTicks = 0;
    Speed = 3.0f;
    DeltaSpeed = 0.0f;
    SpeedChangeTicks = 0;
    LastExplodeTick = 0;
    SwoopSpline.Tension = 0.5f;
    SwoopSpline.ControlPoints.resize(3);
}
