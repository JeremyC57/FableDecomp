// CTCBattleCharge::Reset() -- reset the charge: remember the start position, forget the collided thing and the target, clear the flags, take the dash distance and radius of effect for the spell level and convert the def's pose and charge delays to frames
// Leaf: ?Reset@CTCBattleCharge
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A6F00, 241 bytes (+15 int3 padding), `ret`.
// Callees: CThing::GetPosition() const 0x004C73D0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
//   GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), __ftol2 0x00BFEA70.
//   Global g_framesPerSecond 0x01375550.
// Layout: Ego_r.pdb CTCBattleCharge (retail = PDB up to the 12-byte PeopleHit vector at +0x70, then 4 below):
//   PTarget +0x0C, PDef +0x14, PositionStartedFrom +0x18, CollidedThing +0x24, StartedMoving +0x2C, JustReleased +0x2D,
//   DirectionOverride +0x2E, DashDistance +0x3C, SpellLevel +0x40, RadiusOfEffect +0x4C, PoseDelay +0x68,
//   StartPoseDelay +0x6C, IsZTargetting +0x88, ChargeDelayFrameCount +0x8C. CSpecialAbilitiesBattleChargeDef with 12-byte
//   retail vectors: DashDistance +0x40, RadiusOfEffect +0x4C, PoseDelaySecs +0x88, ChargeDelay +0x8C.
// Shape: the CTCAssassinRush::Reset sibling (0x007A9B70): each intelligent pointer cleared through the inline operator=
//   from a CIntelligentPointer(0) temporary; the start position is the implicit C3DVector copy.
extern long g_framesPerSecond;                  // 0x01375550
struct C3DVector { float X, Y, Z; };
class CBaseObject;
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool immediately);                // 0x004C9B80
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
#pragma pack(pop)
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_stateFlags & 1))
            return true;
    }
    return false;
}
struct CSpecialAbilitiesBattleChargeDef {
    unsigned char _pad00[0x40];
    float* DashDistance;                        // +0x40 vector<float> first
    float* DashDistanceLast;                    // +0x44
    float* DashDistanceEnd;                     // +0x48
    float* RadiusOfEffect;                      // +0x4c vector<float> first
    unsigned char _pad50[0x88 - 0x50];
    float  PoseDelaySecs;                       // +0x88
    float  ChargeDelay;                         // +0x8c
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};

class CTCBattleCharge {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer<CThing> PTarget;        // +0x0c
    CDefPointer<const CSpecialAbilitiesBattleChargeDef> PDef;   // +0x14
    C3DVector     PositionStartedFrom;          // +0x18
    CIntelligentPointer<CThing> CollidedThing;  // +0x24
    bool          StartedMoving;                // +0x2c
    bool          JustReleased;                 // +0x2d
    bool          DirectionOverride;            // +0x2e
    unsigned char _pad2f[0x3c - 0x2f];
    float         DashDistance;                 // +0x3c
    long          SpellLevel;                   // +0x40
    CIntelligentPointer<CThing> PItemRemoved;   // +0x44
    float         RadiusOfEffect;               // +0x4c
    unsigned char _pad50[0x68 - 0x50];
    long          PoseDelay;                    // +0x68
    bool          StartPoseDelay;               // +0x6c
    bool          HitSomething;                 // +0x6d
    unsigned char _pad6e[0x88 - 0x6e];
    bool          IsZTargetting;                // +0x88
    unsigned char _pad89[3];
    long          ChargeDelayFrameCount;        // +0x8c
    void Reset();
};

void CTCBattleCharge::Reset()
{
    PositionStartedFrom = ParentThing->GetPosition();
    CollidedThing = CIntelligentPointer<CThing>(0);
    StartedMoving = false;
    JustReleased = true;
    DirectionOverride = false;
    DashDistance = PDef->DashDistance[SpellLevel];
    RadiusOfEffect = PDef->RadiusOfEffect[SpellLevel];
    PoseDelay = (long)(g_framesPerSecond * PDef->PoseDelaySecs);
    ChargeDelayFrameCount = (long)(g_framesPerSecond * PDef->ChargeDelay);
    StartPoseDelay = false;
    PTarget = CIntelligentPointer<CThing>(0);
    IsZTargetting = false;
}
