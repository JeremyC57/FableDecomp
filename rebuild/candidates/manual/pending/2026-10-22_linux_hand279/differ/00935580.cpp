// Grade: DIFFER(615v617) -- first inline 2D normalise keeps r on the x87 stack (fst; fmul st(2)) where retail stores it (fstp; fxch; fmul [r]); dot Z term operand order
// CAIStateGroup_MinionFaceHero::IsRunnable() -- runnable when the living hero is in the minion's extended line of sight, the last facing is more than 35 seconds ago (or never), the hero is within 2.2 units and faces the minion (2D facing dot the hero-to-minion direction above 0.4); then remembers the hero as the thing to face
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00935580, length 617 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: the player-manager accessor @ 0x008FCF80, GetPMainPlayer @ 0x00449960, CPlayer::GetControlledCreature
// @ 0x00487DD0, CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of
// +0x24), CTCPerceiveThing::IsPlayerExtendedLineOfSight @ 0x00744830, CWorld::GetWorldFrame @ 0x0049D870,
// CThing::GetPosition @ 0x004C73D0, GFWorld3DDistanceSmallerThan @ 0x006614F0 (__fastcall), CThingCreatureBase::
// GetPosition @ 0x006A4D00, the hero physics body's virtual GetFacing (slot 70, +0x118, by hidden pointer),
// CIntelligentPointer<const CThing>(const CThing*) @ 0x006892A0, CBaseIntelligentPointer GetPItem @ 0x00A01B50 /
// SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10.
// Data: g_framesPerSecond @ 0x01375550, the reciprocal-square-root mantissa table @ 0x013CE558, 3.0f / 0.5f / 0.0f
// @ 0x0122DED4 / 0x0122F59C / 0x0122DEDC, 0.4f @ 0x0125A318.
// Layout: engine/CAIStateGroup_MinionFaceHero.h flattens the base, so the model is local (PBrain +4, PHero +0x14,
// LastFacedFrame +0x1c; retail CThing bitset +0x20 / map +0x44 / physics body +0x60 / dead bit +0x91).
// Shape: two fast inline 2D normalisations (the C3DVector inline Normalise of the landed 0x007A6E20 with Z = 0).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern long g_framesPerSecond;                       // 0x01375550

struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPhysicsBody {                           // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                  // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CTCPerceiveThing {                       // interface 0x34
    bool IsPlayerExtendedLineOfSight();         // 0x00744830
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(const CThing* thing);    // 0x006892A0
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerConstThing& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct CPlayer {
    CThingCreatureBase* GetControlledCreature();    // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetPMainPlayer();                  // 0x00449960
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF80
};
struct CAIStateGroup_MinionFaceHero : CAIStateGroupBase {
    CIntelligentPointerThing PHero;             // +0x14
    unsigned long LastFacedFrame;               // +0x1c
    virtual bool IsRunnable();
};

__forceinline bool HasTimePassed(const unsigned long& lastFrame, long seconds)
{
    long fps = g_framesPerSecond;
    return CWorld::GetWorldFrame() - lastFrame > (unsigned long)(seconds * fps);
}

bool CAIStateGroup_MinionFaceHero::IsRunnable()
{
    CThingCreatureBase* hero = GetPPlayerManager()->GetPMainPlayer()->GetControlledCreature();
    if (hero != 0 && !(hero->m_flags91 & 1) && GetCreature()->HasTC(0x34)) {
        CTCPerceiveThing* perceive = GetCreature()->GetTC<CTCPerceiveThing>(0x34);
        if (perceive != 0 && perceive->IsPlayerExtendedLineOfSight()) {
            if (LastFacedFrame == 0 || HasTimePassed(LastFacedFrame, 35)) {
                CThingCreatureBase* creature = GetCreature();
                if (GFWorld3DDistanceSmallerThan(static_cast<const CThing*>(creature)->GetPosition(),
                        static_cast<const CThing*>(hero)->GetPosition(), 2.2f)) {
                    CThingCreatureBase* me = GetCreature();
                    const C3DVector& heroPos = hero->GetPosition();
                    const C3DVector& myPos = me->GetPosition();
                    C3DVector toMe(myPos.X - heroPos.X, myPos.Y - heroPos.Y, 0.0f);
                    toMe.Normalise();
                    C3DVector facing = hero->PhysicsBody->GetFacing();
                    facing.Z = 0.0f;
                    facing.Normalise();
                    if (Dot(facing, toMe) > 0.4f) {
                        PHero = hero;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
