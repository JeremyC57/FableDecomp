// CTCAssassinRush::Reset() -- reset the rush: remember the start position, forget the collided thing, clear the flags, convert the def's trail and rush delays to frames, take the dash distance for the spell level and kill both trail effects if alive
// Leaf: ?Reset@CTCAssassinRush
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A9B70, 267 bytes (+5 int3 padding), `ret`.
// Callees: CThing::GetPos() const 0x004C73D0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
//   GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), __ftol2 0x00BFEA70,
//   CThing::Kill(bool) 0x004C9B80. Global g_framesPerSecond 0x01375550.
// Layout (pack(1): Ego_r.pdb CTCAssassinRush puts C3DVectors at odd offsets): PDef +0x14, AssassinTrail +0x18,
//   AssassinDisplacementTrail +0x20, TrailDelay +0x28, StartTrailDelay +0x2C, PositionStartedFrom +0x2D, CollidedThing
//   +0x39, StartedMoving +0x41, JustReleased +0x42, DirectionOverride +0x48 (PDB = retail up to here); after the
//   12-byte retail ItemAlphaList everything sits 4 below the PDB: DashDistance +0x70, SpellLevel +0x8C, IsZTargetting
//   +0x9D, RushDelayFrameCount +0xA0. CSpecialAbilitiesAssassinRushDef at PDB - 4: TrailDelaySecs +0x6C, RushDelay
//   +0x7C, DashDistance (vector<float>) +0x80.
// Shape: the collided thing is cleared through the intelligent pointer's inline operator= from a CIntelligentPointer
//   (0) temporary (`item = other.GetPItem(); SetPItem(item);`, the temporary's out-of-line base destructor after);
//   the start position is the implicit C3DVector copy; both trails go through an inline IS_THING_ALIVE then
//   `GetPItem()->Kill(true)`.
extern long g_framesPerSecond;                  // 0x01375550
class C3DVector { public: float X, Y, Z; };
class CBaseObject;
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
    const C3DVector& GetPos() const;       // 0x004C73D0
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
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
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
class CSpecialAbilitiesAssassinRushDef { public:
    unsigned char _pad00[0x6c];
    float  TrailDelaySecs;                      // +0x6c
    float  SlowTimeLength;                      // +0x70
    long   AssassinAlpha;                       // +0x74
    long   SlowTimeMultiplier;                  // +0x78
    float  RushDelay;                           // +0x7c
    float* DashDistance;                        // +0x80 vector<float> first
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

#pragma pack(push, 1)
class CTCAssassinRush {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer<CThing> PTarget;        // +0x0c
    CDefPointer<const CSpecialAbilitiesAssassinRushDef> PDef;   // +0x14
    CIntelligentPointer<CThing> AssassinTrail;  // +0x18
    CIntelligentPointer<CThing> AssassinDisplacementTrail;      // +0x20
    long          TrailDelay;                   // +0x28
    bool          StartTrailDelay;              // +0x2c
    C3DVector     PositionStartedFrom;          // +0x2d
    CIntelligentPointer<CThing> CollidedThing;  // +0x39
    bool          StartedMoving;                // +0x41
    bool          JustReleased;                 // +0x42
    unsigned char _pad43;
    long          SlowTimeLength;               // +0x44
    bool          DirectionOverride;            // +0x48
    unsigned char _pad49[0x70 - 0x49];
    float         DashDistance;                 // +0x70
    unsigned char _pad74[0x8c - 0x74];
    long          SpellLevel;                   // +0x8c
    unsigned char _pad90[0x9d - 0x90];
    bool          IsZTargetting;                // +0x9d
    unsigned char _pad9e[2];
    long          RushDelayFrameCount;          // +0xa0
    void Reset();
};
#pragma pack(pop)

void CTCAssassinRush::Reset()
{
    StartTrailDelay = false;
    PositionStartedFrom = ParentThing->GetPos();
    CollidedThing = CIntelligentPointer<CThing>(0);
    StartedMoving = false;
    JustReleased = true;
    DirectionOverride = false;
    IsZTargetting = false;
    TrailDelay = (long)(g_framesPerSecond * PDef->TrailDelaySecs);
    RushDelayFrameCount = (long)(g_framesPerSecond * PDef->RushDelay);
    DashDistance = PDef->DashDistance[SpellLevel];
    if (IS_THING_ALIVE(AssassinTrail))
        AssassinTrail.GetPItem()->Kill(true);
    if (IS_THING_ALIVE(AssassinDisplacementTrail))
        AssassinDisplacementTrail.GetPItem()->Kill(true);
}