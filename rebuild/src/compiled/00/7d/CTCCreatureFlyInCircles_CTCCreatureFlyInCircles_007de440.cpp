// CTCCreatureFlyInCircles::~CTCCreatureFlyInCircles() -- free the swoop spline's control points, release both intelligent pointers, then ~CTCBase
// Retail 0x007DE440, 43 bytes. Called only from the deleting destructor 0x007DE420 (vtable 0x0126FB04
// slot 0); the manifest label "CTCInventoryItem::OnDie" is propagated.
// Members die in reverse order (Ego_r.pdb offsets): SwoopSpline (CCardinalSpline, +0x4C) frees its
// ControlPoints CArray block with `free` 0x00BFEA14 under a null test (`add esp,4`: this module is
// speed-optimised); PSwoopTarget (+0x44) and the unaligned PBurninationEffect (+0x3B, after three
// bools) run ~CBaseIntelligentPointer 0x00A01C10; then CTCBase::~CTCBase 0x00686830 is tail-jumped.
// No vtable store: `__declspec(novtable)` over CTCBase. `#pragma pack(1)` reproduces the PDB's
// unaligned intelligent pointer at +0x3B.
// Leaf: ??1CTCCreatureFlyInCircles

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CThing;
class CThingCreatureBase;
class CBaseObject;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    ~CTCBase();                                    // 0x00686830
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x4
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // no destructor of its own
    virtual void v00();
};
#pragma pack(pop)

class C3DVector { public:
    float x, y, z;
};

class CCardinalSpline { public:
    C3DVector* m_pControlPoints;                   // +0x00 CArray<C3DVector> ControlPoints
    long m_count;                                  // +0x04
    long m_capacity;                               // +0x08
    long m_reserved;                               // +0x0C
    float Tension;                                 // +0x10
    bool EndPointsConnected;                       // +0x14
    ~CCardinalSpline() { if (m_pControlPoints) free(m_pControlPoints); }
};

#pragma pack(push, 1)
struct __declspec(novtable) CTCCreatureFlyInCircles : CTCBase {
    CThingCreatureBase* Creature;                  // +0x0C
    C3DVector Centre;                              // +0x10
    float Speed;                                   // +0x1C
    float DeltaSpeed;                              // +0x20
    long SpeedChangeTicks;                         // +0x24
    float CurrentRadius;                           // +0x28
    float DeltaRadius;                             // +0x2C
    long RadiusChangeTicks;                        // +0x30
    long Mode;                                     // +0x34 EFlyMode
    bool Transitioning;                            // +0x38
    bool ModeChangeQueued;                         // +0x39
    bool Active;                                   // +0x3A
    CIntelligentPointer PBurninationEffect;        // +0x3B
    bool HasBeenHit;                               // +0x43
    CIntelligentPointer PSwoopTarget;              // +0x44
    CCardinalSpline SwoopSpline;                   // +0x4C
    C3DVector SwoopTargOffset;                     // +0x64
    float SwoopProgress;                           // +0x70
    float SwoopDelta;                              // +0x74
    long LastExplodeTick;                          // +0x78
    virtual void v00();
    ~CTCCreatureFlyInCircles();
};
#pragma pack(pop)

CTCCreatureFlyInCircles::~CTCCreatureFlyInCircles()
{
}