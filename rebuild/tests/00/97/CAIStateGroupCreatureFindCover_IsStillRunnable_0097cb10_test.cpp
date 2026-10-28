// Behaviour test for CAIStateGroup_CreatureFindCover::IsStillRunnable (0x0097CB10): the model and body verbatim from
// final.cpp (the physics body's virtual slots inline-defined so the vtable links, GetFacing returning a stored vector),
// the out-of-line callees stubbed: GetPItem / SetPItem / the intelligent-pointer ctor and dtor, the interface map's
// LowerBound (linear walk), GetThingInPrimarySlot, IsProjectileWeapon and GetPosition. Checks: dead or missing enemy ->
// false; no carrying interface -> false; empty primary slot -> false; a non-projectile weapon -> false; a projectile
// weapon with the enemy facing us -> true, facing away -> false.
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558 (filled in main)
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
#define VF(n) virtual void v##n() {}   // test: inline-defined so the vtable links
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() { return Facing; }   // slot 70 (+0x118); test: returns the stored facing
    C3DVector Facing;
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
    const C3DVector* GetPosition() const;           // 0x004C73D0

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
                C3DVector toMe = *creature->GetPosition() - *PEnemy.GetPItem()->GetPosition();
                toMe.Normalise();
                if (Dot(toMe, PEnemy.GetPItem()->GetPhysicsBody()->GetFacing()) > 0.0f)
                    return true;
            }
        }
    }
    return false;
}

// ---- stubs ----
void CBaseIntelligentPointer::v00() {}
void CAIStateGroup_CreatureFindCover::v00() {}
template <> void CIntelligentPointer<const CThing>::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static CThing* g_primarySlot = 0;
static bool g_projectile = false;
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_primarySlot; }
bool CTCWeapon::IsProjectileWeapon() { return g_projectile; }
static C3DVector g_creaturePos(0.0f, 0.0f, 0.0f);
static C3DVector g_enemyPos(3.0f, 4.0f, 0.0f);
static CThing* g_creatureThing = 0;
const C3DVector* CThing::GetPosition() const { return this == g_creatureThing ? &g_creaturePos : &g_enemyPos; }

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

int main()
{
    for (int i = 0; i < 128; ++i) {
        double m = 1.0 + ((i & 63) + 0.5) / 64.0;
        double x = m * ((i >> 6) ? 1.0 : 2.0);     // bit 6 set = odd exponent (x in [1,2)), clear = even (x in [2,4))
        double r = 1.0 / sqrt(x);
        long entry = (long)((2.0 * r - 1.0) * 256.0);
        if (entry < 0) entry = 0;
        if (entry > 255) entry = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)entry;
    }
    CTCCarrying carrying; CTCWeapon tcWeapon; CPhysicsBody body;
    CThing enemy; CThing weapon; CThing creature;
    for (int w = 0; w < 9; ++w) { enemy.m_presentBits[w] = 0; weapon.m_presentBits[w] = 0; creature.m_presentBits[w] = 0; }
    CInterfaceMap::SEntry enemyEntries[1]; enemyEntries[0].type = 0x46; enemyEntries[0].component = (CTCBase*)&carrying;
    CInterfaceMap::SEntry weaponEntries[1]; weaponEntries[0].type = 0x48; weaponEntries[0].component = (CTCBase*)&tcWeapon;
    enemy.m_interfaces.first = enemyEntries; enemy.m_interfaces.last = enemyEntries + 1;
    weapon.m_interfaces.first = weaponEntries; weapon.m_interfaces.last = weaponEntries + 1;
    enemy.m_flags91 = 0; weapon.m_flags91 = 0;
    enemy.PhysicsBody = &body;
    g_creatureThing = &creature;
    CAIBrain brain; brain.Creature = &creature;
    CAIStateGroup_CreatureFindCover group; group.PBrain = &brain;

    // no enemy
    CHECK(group.IsStillRunnable() == false);
    group.PEnemy.SetPItem(&enemy);
    // dead enemy
    enemy.m_flags91 = 1;
    CHECK(group.IsStillRunnable() == false);
    enemy.m_flags91 = 0;
    // alive, no carrying interface bit
    CHECK(group.IsStillRunnable() == false);
    enemy.m_presentBits[0x46 >> 5] |= 1UL << (0x46 & 31);
    // carrying, empty primary slot
    g_primarySlot = 0;
    CHECK(group.IsStillRunnable() == false);
    // a weapon without the weapon interface bit
    g_primarySlot = &weapon;
    CHECK(group.IsStillRunnable() == false);
    weapon.m_presentBits[0x48 >> 5] |= 1UL << (0x48 & 31);
    // melee weapon
    g_projectile = false;
    CHECK(group.IsStillRunnable() == false);
    // projectile weapon, enemy at (3,4,0) facing towards the creature at the origin: toMe = (-3,-4,0)/5
    g_projectile = true;
    body.Facing = C3DVector(-1.0f, -1.0f, 0.0f);
    CHECK(group.IsStillRunnable() == true);
    // facing away
    body.Facing = C3DVector(1.0f, 1.0f, 0.0f);
    CHECK(group.IsStillRunnable() == false);
    // facing exactly sideways: dot 0 is not > 0
    body.Facing = C3DVector(4.0f, -3.0f, 0.0f);
    CHECK(group.IsStillRunnable() == false);

    printf("0097cb10_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail ? 1 : 0;
}