// `operator*=(const float&)` (chunk 278 / 00880510 lever) spills the Newton-step reciprocal and multiplies from memory as
// retail; (2) the facing call's object through an inline accessor (`GetPItem()->GetPhysicsBody()->GetFacing()`, chunk 271
// lever) loads the physics body straight into ecx (`mov ecx,[eax+60h] ; mov edx,[ecx]`) instead of eax + `mov ecx,eax`.
// CAIStateGroup_CreatureFindCover::IsStillRunnable() -- the enemy is alive, holds a projectile weapon and faces us (dot of its facing with the normalised enemy->creature vector > 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097CB10, length 443 bytes.
// Callees: GetPItem @ 0x00A01B50, CVectorMap::LowerBound const @ 0x004365B0 (0x46 carrying, 0x48 weapon), GetThingInPrimarySlot
//          @ 0x006A1440, CTCWeapon::IsProjectileWeapon @ 0x005DAA40, CThing::GetPosition @ 0x004C73D0, the physics body's
//          GetFacing (slot 70, +0x118); globals 0x013CE558 (rsqrt mantissa table), 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC.
// Layout: PEnemy +0x14 (CIntelligentPointer<const CThing>), as the landed CreatureTakeCover::IsStillRunnable 0x00979E90 model.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
class CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer   // inline ctor stores the template's vtable after the base ctor
{ public:
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    T* Get() const { return (T*)GetPItem(); }
};

class CTCBase;

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                  // +0x00
    SEntry* last;                                   // +0x04
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
class CThing
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                      // +0x60
    __forceinline CPhysicsBody* GetPhysicsBody() const { return PhysicsBody; }
    unsigned char _pad_0x64[0x91 - 0x64];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long type)    // the entry; `->component` read at the call site
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->type <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CAIBrain
{ public:
    unsigned char _pad0[0x18];
    CThing* Creature;                               // +0x18
};

class CTCCarrying                                  // interface type 0x46 (word 2 bit 6)
{ public:
    CThing* GetThingInPrimarySlot();                // 0x006A1440
};

class CTCWeapon                                    // interface type 0x48 (word 2 bit 8)
{ public:
    bool IsProjectileWeapon();                      // 0x005DAA40
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIStateGroup_CreatureFindCover
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointer<const CThing> PEnemy;       // +0x14
    bool IsStillRunnable();
};

bool CAIStateGroup_CreatureFindCover::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy))
        return false;
    CTCCarrying* carrying;
    if (PEnemy.GetPItem()->GetInterface(0x46, &carrying)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon) {
            CTCWeapon* tcWeapon;
            if (weapon->GetInterface(0x48, &tcWeapon) && tcWeapon->IsProjectileWeapon()) {
                CThing* creature = PBrain->Creature;
                C3DVector toMe = creature->GetPos() - PEnemy.GetPItem()->GetPos();
                toMe.Normalise();
                if (Dot(toMe, PEnemy.GetPItem()->GetPhysicsBody()->GetFacing()) > 0.0f)
                    return true;
            }
        }
    }
    return false;
}