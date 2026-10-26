// CTCSpecialAbilities::IsAbleToUseAnyAbility(EHeroAbility ability) const -- the hero may use an ability unless the abilities are locked, he is in bed or knocked down, or his interface-0x29 component disallows it; a carried weapon must also be usable by him (ability 8 exempt)
// Leaf: IsAbleToUseAnyAbility
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072AF30, 254 bytes, `ret 4`.
// Callees: IsKnockedDown(CThing*) @ 0x0085F010 (`__fastcall`), the const interface LowerBound @ 0x004365B0 (interfaces 0x29,
//   0x46 = CTCCarrying, 0x48 = CTCWeapon), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsUsableBy(const
//   CThing&) const @ 0x005DAB10.
// Layout: Ego_r.pdb CTCSpecialAbilities AbilityMutexLock +0x0C, UsingBed +0x0D; retail CThing interface bitset +0x20
//   (0x29 = word +0x24 bit 9, 0x46 = byte +0x28 bit 6, 0x48 = word +0x28 bit 8), map +0x44; the 0x29 component's
//   abilities-allowed flag +0x0D (named descriptively).
// Shape: the 0x29 component is a zero-preloaded pointer filled under HasTC (`xor eax,eax`) and dereferenced unconditionally;
//   the later lookups are HasTC + end/hit ternary (const LowerBound); the weapon key homes in the dead `ability` slot.
//   The weapon test goes through an inline IsWeaponUsableBy(weapon, user) whose weapon argument is the GetTC ternary:
//   the component lands in ecx before ParentThing is loaded and pushed through eax (a direct member call on the
//   FindEntry / GetTC / out-parameter component pushes the argument first or joins the component through eax).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
bool __fastcall IsKnockedDown(CThing* thing);           // 0x0085F010
struct CTCAbilityPermissions {                          // interface 0x29 (named descriptively)
    unsigned char _pad00[0x0d];
    bool          AbilitiesAllowed;                     // +0x0d
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot();                    // 0x006A1440
};
class CTCWeapon { public:
    bool IsUsableBy(const CThing& thing) const;         // 0x005DAB10
};
enum EHeroAbility { HERO_ABILITY_8 = 8 };
class CTCSpecialAbilities { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                              // +0x0c
    bool UsingBed;                                      // +0x0d
    long CurrentAbility;                                // +0x10
    bool IsAbleToUseAnyAbility(EHeroAbility ability) const;
};

__forceinline bool IsWeaponUsableBy(CTCWeapon* weapon, const CThing& user) { return weapon->IsUsableBy(user); }
bool CTCSpecialAbilities::IsAbleToUseAnyAbility(EHeroAbility ability) const
{
    if (!AbilityMutexLock && !UsingBed && !IsKnockedDown(ParentThing)) {
        const CThing* thing = ParentThing;
        CTCAbilityPermissions* permissions = 0;
        if (thing->HasTC(0x29))
            permissions = thing->GetTC<CTCAbilityPermissions>(0x29);
        if (permissions->AbilitiesAllowed) {
            if (!thing->HasTC(0x46))
                return true;
            CThing* carried = thing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
            if (!carried || ability == HERO_ABILITY_8 || !carried->HasTC(0x48)
                || IsWeaponUsableBy(carried->GetTC<CTCWeapon>(0x48), *ParentThing))
                return true;
        }
        return false;
    }
    return false;
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static bool g_knocked = false;
bool __fastcall IsKnockedDown(CThing*) { return g_knocked; }
static CThing* g_carried = 0;
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_carried; }
static bool g_usable = false; static const CThing* g_user = 0; static int g_usableCalls = 0;
bool CTCWeapon::IsUsableBy(const CThing& thing) const { ++g_usableCalls; g_user = &thing; return g_usable; }
void CTCSpecialAbilities::v00() { }
static void SetBit(CThing& t, long k, bool on)
{
    if (on) t.m_presentBits[k >> 5] |= 1UL << (k & 31);
    else t.m_presentBits[k >> 5] &= ~(1UL << (k & 31));
}
static CThing g_hero, g_sword;
static CTCAbilityPermissions g_perm;
static CTCCarrying g_carrying;
static CTCWeapon g_weapon;
static CInterfaceMap::SEntry g_heroEntries[2], g_swordEntries[1];
int main()
{
    g_heroEntries[0].type = 0x29; g_heroEntries[0].component = (CTCBase*)&g_perm;
    g_heroEntries[1].type = 0x46; g_heroEntries[1].component = (CTCBase*)&g_carrying;
    g_hero.m_interfaces.first = g_heroEntries; g_hero.m_interfaces.last = g_heroEntries + 2;
    g_swordEntries[0].type = 0x48; g_swordEntries[0].component = (CTCBase*)&g_weapon;
    g_sword.m_interfaces.first = g_swordEntries; g_sword.m_interfaces.last = g_swordEntries + 1;
    SetBit(g_hero, 0x29, true); SetBit(g_hero, 0x46, true); SetBit(g_sword, 0x48, true);
    g_perm.AbilitiesAllowed = true;
    CTCSpecialAbilities a;
    a.ParentThing = &g_hero; a.AbilityMutexLock = false; a.UsingBed = false; a.CurrentAbility = 0;
    g_carried = 0;
    CHECK(a.IsAbleToUseAnyAbility((EHeroAbility)1));             // nothing carried
    g_carried = &g_sword; g_usable = false;
    CHECK(!a.IsAbleToUseAnyAbility((EHeroAbility)1));            // unusable weapon
    CHECK(g_usableCalls == 1 && g_user == &g_hero);
    CHECK(a.IsAbleToUseAnyAbility(HERO_ABILITY_8));              // ability 8 exempt
    CHECK(g_usableCalls == 1);
    g_usable = true;
    CHECK(a.IsAbleToUseAnyAbility((EHeroAbility)2));             // usable weapon
    g_usable = false; SetBit(g_sword, 0x48, false);
    CHECK(a.IsAbleToUseAnyAbility((EHeroAbility)2));             // carried thing is not a weapon
    SetBit(g_hero, 0x46, false);
    CHECK(a.IsAbleToUseAnyAbility((EHeroAbility)2));             // carries nothing at all
    g_perm.AbilitiesAllowed = false;
    CHECK(!a.IsAbleToUseAnyAbility((EHeroAbility)2));            // abilities disallowed
    g_perm.AbilitiesAllowed = true;
    a.UsingBed = true;  CHECK(!a.IsAbleToUseAnyAbility((EHeroAbility)2)); a.UsingBed = false;
    a.AbilityMutexLock = true; CHECK(!a.IsAbleToUseAnyAbility((EHeroAbility)2)); a.AbilityMutexLock = false;
    g_knocked = true; CHECK(!a.IsAbleToUseAnyAbility((EHeroAbility)2)); g_knocked = false;
    CHECK(a.IsAbleToUseAnyAbility((EHeroAbility)2));
    if (fails == 0) printf("0072af30_TEST PASS\n");
    return fails;
}