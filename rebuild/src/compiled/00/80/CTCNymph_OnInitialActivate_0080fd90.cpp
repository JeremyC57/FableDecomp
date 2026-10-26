// CTCNymph::OnInitialActivate() -- base hook, fetch the nymph sub-def from the owner's def, reset the attack/fade state and give the flight spline five control points
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080FD90, length 108 bytes.
// Callees: CTCBase::OnInitialActivate @ 0x00686870 (non-virtual base call; catalog "$E2"), CDefClassBase::GetSubDef<CNymphDef>
// @ 0x0080FE60 (catalog <CAICreatureWillPowerIndicatorDef> is a byte twin), the def's slot-1 release, vector<C3DVector>::resize
// @ 0x0075C040 (catalog "vector<CBrainBehaviour>::resize"; as the landed CTCCreatureFlyInCircles ctor 0x007DDF90).
// Layout (retail, local model; see the CTCNymph ctor 0x004E3325): PDef +0x0C, FlightSpline.ControlPoints +0x2C, FlyingAround +0x40,
// LastTargetTick +0x54, CanAttack/ReadyToAttack/ReadyToSummon/HasBeenInLimbo/HasSpline/BeenHit +0x58..+0x5D, FadeCounter +0x60,
// TotalFadeTime +0x64; the owner's def pointer is CThing +0x70.
// Shape: the owner def is copied into a pointer local with a hand-written add-ref and release around GetSubDef(&PDef) (both
// null-tested against the shared zero in ebx); the state resets are body stores in retail order; resize(5) takes a default
// (uninitialised) C3DVector temporary.
class CDefClassBase;
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
class CNymphDef;
template<class T> class CDefPointer { public:
    T* Object;
};
class CDefClassBase { public:
    virtual void Destroy();
    virtual void Release();                     // slot 1
    long RefCount;                              // +0x04
    bool GetSubDef(CDefPointer<const CNymphDef>* out) const;   // 0x0080FE60
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
};
struct CVector3D {                              // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
};
class CCardinalSpline { public:                        // retail 0x14 bytes
    CVector3D ControlPoints;                    // +0x00
    float     Tension;                          // +0x0c
    bool      EndPointsConnected;               // +0x10
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    void OnInitialActivate();                   // 0x00686870
};
class CTCNymph : public CTCBase { public:
    CDefPointer<const CNymphDef> PDef;          // +0x0c
    unsigned char _pad10[0x2c - 0x10];          // PointToFlyAround, EffectHeightOffset, InitialPos
    CCardinalSpline FlightSpline;               // +0x2c
    bool FlyingAround;                          // +0x40
    unsigned char PEffect[8];                   // +0x41 CIntelligentPointer<CThing> (byte-aligned)
    unsigned char POldEffect[8];                // +0x49
    unsigned long LastTargetTick;               // +0x54
    bool CanAttack;                             // +0x58
    bool ReadyToAttack;                         // +0x59
    bool ReadyToSummon;                         // +0x5a
    bool HasBeenInLimbo;                        // +0x5b
    bool HasSpline;                             // +0x5c
    bool BeenHit;                               // +0x5d
    long FadeCounter;                           // +0x60
    long TotalFadeTime;                         // +0x64
    void OnInitialActivate();
};

void CTCNymph::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    CDefClassBase* def = ParentThing->PDef;
    if (def)
        ++def->RefCount;
    def->GetSubDef(&PDef);
    if (def && --def->RefCount == 0)
        def->Release();
    CanAttack = true;
    ReadyToAttack = false;
    ReadyToSummon = false;
    LastTargetTick = 0;
    HasBeenInLimbo = false;
    FlyingAround = false;
    HasSpline = false;
    BeenHit = false;
    FadeCounter = 0;
    TotalFadeTime = 0;
    FlightSpline.ControlPoints.resize(5);
}