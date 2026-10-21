#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingCreatureBase::UnsheatheDefaultWeapon() -- draw the creature's default weapon (the melee weapon when its def prefers melee, else -- unless already busy or already carrying a ranged weapon -- sheathe everything and draw the def's ranged weapon), then, when the def asks for it, reset the combat component's combat type to the def's one if it differs
// Leaf: ?UnsheatheDefaultWeapon@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664A50, length 336 bytes, plain `ret`.
//   Name from Ego_r.pdb `?UnsheatheDefaultWeapon@CThingCreatureBase@@QAEXXZ`.
// Callees: CThingCreatureBase::UnsheatheMeleeWeapon @ 0x006649C0, the creature's virtual slot 77 (+0x134, a busy test),
//   IsThingCarryingRangedWeapon(const CThing&) @ 0x006A1620 (`__fastcall`), CThingCreatureBase::SheatheAllWeapons
//   @ 0x006647F0, CThingCreatureBase::UnsheatheWeapon(long, bool) @ 0x006648D0, LowerBound @ 0x0040F020,
//   CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0 (CDefString
//   by value), the combat type def's virtual slot 10 (+0x28, its def name by value), CDefinitionManager::GetDef
//   <CCombatTypeDef>(long, CDefPointer*) @ 0x00669640, CTCCombat::SetCombatTypeDef @ 0x007355F0 (the landed
//   "SetAttackStyleDef"), the defs' virtual slot 1 on the last release.
// Layout (retail): CThing bitset +0x20 (0x30 CTCCombat = bit 16 of +0x24), map +0x44, PBaseDef +0x70;
//   CThingCreatureBase::PDef +0xe0; CThingCreatureDef RangedWeaponDef +0xe0, CombatTypeDef +0x124,
//   ResetCombatTypeOnUnsheatheDefaultWeapons +0x128, UseMeleeWeaponAsDefault bit 4 of +0x148 (PDB 8 higher);
//   CTCCombat::PCombatTypeDef +0x28.
struct CTCBase;

struct CDefString {
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
    bool operator==(const CDefString& other) const { return m_index == other.m_index; }
};

struct CDefPointeeBase {
    virtual void v000();
    virtual void DestroyDef() const;                    // slot 1
    mutable long m_refCount;                            // +4
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};

struct CCombatTypeDef : CDefPointeeBase {
    virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void v007(); virtual void v008(); virtual void v009();
    virtual CDefString GetDefName() const;              // slot 10 (+0x28)
};

struct CThingCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0xe0 - 8];
    long          RangedWeaponDef;                      // +0xe0
    unsigned char _pade4[0x124 - 0xe4];
    long          CombatTypeDef;                        // +0x124
    bool          ResetCombatTypeOnUnsheatheDefaultWeapons;   // +0x128
    unsigned char _pad129[0x148 - 0x129];
    unsigned char _bits148 : 4;
    unsigned char UseMeleeWeaponAsDefault : 1;          // +0x148 bit 4
};

struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
    void GetDef(long index, CDefPointer<const CCombatTypeDef>* out);   // 0x00669640
};

struct CTCCombat {
    unsigned char _pad00[0x28];
    CDefPointer<const CCombatTypeDef> PCombatTypeDef;   // +0x28
    void SetCombatTypeDef(const CDefPointer<const CCombatTypeDef>* def);   // 0x007355F0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual bool IsBusy() const;                        // slot 77 (+0x134)
    unsigned char  _pad04[0x20 - 4];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefPointer<const CThingCreatureDef> PBaseDef;      // +0x70
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
bool __fastcall IsThingCarryingRangedWeapon(const CThing& thing);   // 0x006A1620

struct CThingCreatureBase : CThing {
    unsigned char _pad74[0xe0 - 0x74];
    const CThingCreatureDef* PDef;                      // +0xe0 CDefPointer<const CThingCreatureDef>::Object
    const CThingCreatureDef* GetDef() const { return PDef; }
    __forceinline const CThingCreatureDef& PeekDefinition() const
    {
        CDefPointer<const CThingCreatureDef> def(PBaseDef);
        return *def.Object;
    }
    void SheatheAllWeapons();                           // 0x006647F0
    void UnsheatheWeapon(long weaponDef, bool primary); // 0x006648D0
    void UnsheatheMeleeWeapon();                        // 0x006649C0
    void UnsheatheDefaultWeapon();
};

void CThingCreatureBase::UnsheatheDefaultWeapon()
{
    if (GetDef()->UseMeleeWeaponAsDefault) {
        UnsheatheMeleeWeapon();
    } else if (!IsBusy() && !IsThingCarryingRangedWeapon(*this)) {
        SheatheAllWeapons();
        UnsheatheWeapon(GetDef()->RangedWeaponDef, true);
    }
    if (HasTC(0x30)) {
        CTCCombat* combat = (CTCCombat*)GetTC(0x30);
        CDefPointer<const CCombatTypeDef> combatType = combat->PCombatTypeDef;
        const CThingCreatureDef& def = PeekDefinition();
        if (!(combatType->GetDefName() == GetDefinitionManager()->GetDefNameFromGlobalIndex(def.CombatTypeDef))
            && def.ResetCombatTypeOnUnsheatheDefaultWeapons) {
            CDefPointer<const CCombatTypeDef> newType;
            GetDefinitionManager()->GetDef(def.CombatTypeDef, &newType);
            combat->SetCombatTypeDef(&newType);
        }
    }
}

void CThing::v000() {}
void CThing::v001() {}
void CThing::v002() {}
void CThing::v003() {}
void CThing::v004() {}
void CThing::v005() {}
void CThing::v006() {}
void CThing::v007() {}
void CThing::v008() {}
void CThing::v009() {}
void CThing::v010() {}
void CThing::v011() {}
void CThing::v012() {}
void CThing::v013() {}
void CThing::v014() {}
void CThing::v015() {}
void CThing::v016() {}
void CThing::v017() {}
void CThing::v018() {}
void CThing::v019() {}
void CThing::v020() {}
void CThing::v021() {}
void CThing::v022() {}
void CThing::v023() {}
void CThing::v024() {}
void CThing::v025() {}
void CThing::v026() {}
void CThing::v027() {}
void CThing::v028() {}
void CThing::v029() {}
void CThing::v030() {}
void CThing::v031() {}
void CThing::v032() {}
void CThing::v033() {}
void CThing::v034() {}
void CThing::v035() {}
void CThing::v036() {}
void CThing::v037() {}
void CThing::v038() {}
void CThing::v039() {}
void CThing::v040() {}
void CThing::v041() {}
void CThing::v042() {}
void CThing::v043() {}
void CThing::v044() {}
void CThing::v045() {}
void CThing::v046() {}
void CThing::v047() {}
void CThing::v048() {}
void CThing::v049() {}
void CThing::v050() {}
void CThing::v051() {}
void CThing::v052() {}
void CThing::v053() {}
void CThing::v054() {}
void CThing::v055() {}
void CThing::v056() {}
void CThing::v057() {}
void CThing::v058() {}
void CThing::v059() {}
void CThing::v060() {}
void CThing::v061() {}
void CThing::v062() {}
void CThing::v063() {}
void CThing::v064() {}
void CThing::v065() {}
void CThing::v066() {}
void CThing::v067() {}
void CThing::v068() {}
void CThing::v069() {}
void CThing::v070() {}
void CThing::v071() {}
void CThing::v072() {}
void CThing::v073() {}
void CThing::v074() {}
void CThing::v075() {}
void CThing::v076() {}
void CCombatTypeDef::v002() {}
void CCombatTypeDef::v003() {}
void CCombatTypeDef::v004() {}
void CCombatTypeDef::v005() {}
void CCombatTypeDef::v006() {}
void CCombatTypeDef::v007() {}
void CCombatTypeDef::v008() {}
void CCombatTypeDef::v009() {}
static int g_destroyed = 0;
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() const { ++g_destroyed; }
static CDefString MakeName(long i) { return *reinterpret_cast<CDefString*>(&i); }
CDefString CCombatTypeDef::GetDefName() const { return MakeName(m_refCount >= 0 ? (long)(size_t)this & 0xffff : 0); }
static CCombatTypeDef g_typeA, g_typeB;
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index) { return MakeName(index == 1 ? (long)(size_t)&g_typeA & 0xffff : (long)(size_t)&g_typeB & 0xffff); }
static int g_gets = 0;
void CDefinitionManager::GetDef(long index, CDefPointer<const CCombatTypeDef>* out) { ++g_gets; out->Object = index == 1 ? &g_typeA : &g_typeB; ++out->Object->m_refCount; }
static int g_sets = 0;
void CTCCombat::SetCombatTypeDef(const CDefPointer<const CCombatTypeDef>* def)
{ ++g_sets; if (PCombatTypeDef.Object) PCombatTypeDef.Object->Release(); PCombatTypeDef.Object = def->Object; ++PCombatTypeDef.Object->m_refCount; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static CDefinitionManager g_manager;
CDefinitionManager* CThing::GetDefinitionManager() const { return &g_manager; }
static bool g_busy = false, g_carryingRanged = false;
bool CThing::IsBusy() const { return g_busy; }
bool __fastcall IsThingCarryingRangedWeapon(const CThing& thing) { return g_carryingRanged; }
static int g_sheathes = 0, g_melee = 0, g_unsheathes = 0; static long g_weapon = 0;
void CThingCreatureBase::SheatheAllWeapons() { ++g_sheathes; }
void CThingCreatureBase::UnsheatheWeapon(long weaponDef, bool primary) { if (primary) { ++g_unsheathes; g_weapon = weaponDef; } }
void CThingCreatureBase::UnsheatheMeleeWeapon() { ++g_melee; }
#define CHECK(c) if (!(c)) { std::printf("00664a50_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static CThingCreatureDef def; static CThingCreatureBase c; static CTCCombat combat; static CInterfaceMap::SEntry e[2];
    e[0].type = 0x30; e[0].component = (CTCBase*)&combat; e[1].type = 0x7fffffff; e[1].component = 0;
    c.m_interfaces.first = e; c.m_interfaces.last = e + 1;
    c.PDef = &def; c.PBaseDef.Object = &def; def.m_refCount = 1; def.RangedWeaponDef = 31; def.CombatTypeDef = 1;
    def.UseMeleeWeaponAsDefault = 1;
    c.UnsheatheDefaultWeapon();                                      // melee, no combat component
    CHECK(g_melee == 1 && g_sheathes == 0 && g_unsheathes == 0 && def.m_refCount == 1);
    def.UseMeleeWeaponAsDefault = 0; g_busy = true;
    c.UnsheatheDefaultWeapon();
    CHECK(g_melee == 1 && g_sheathes == 0);
    g_busy = false; g_carryingRanged = true;
    c.UnsheatheDefaultWeapon();
    CHECK(g_sheathes == 0);
    g_carryingRanged = false;
    c.UnsheatheDefaultWeapon();
    CHECK(g_sheathes == 1 && g_unsheathes == 1 && g_weapon == 31);
    c.m_presentBits[1] = 0x10000; g_typeA.m_refCount = 1; g_typeB.m_refCount = 1; combat.PCombatTypeDef.Object = &g_typeB;
    def.UseMeleeWeaponAsDefault = 1; def.ResetCombatTypeOnUnsheatheDefaultWeapons = false;
    c.UnsheatheDefaultWeapon();                                      // differs but the def does not ask for a reset
    CHECK(g_sets == 0 && g_gets == 0 && g_typeB.m_refCount == 1 && def.m_refCount == 1);
    def.ResetCombatTypeOnUnsheatheDefaultWeapons = true;
    c.UnsheatheDefaultWeapon();                                      // reset to type A
    CHECK(g_sets == 1 && g_gets == 1 && combat.PCombatTypeDef.Object == &g_typeA && g_typeA.m_refCount == 2);
    CHECK(g_typeB.m_refCount == 0 && g_destroyed == 1 && def.m_refCount == 1);
    c.UnsheatheDefaultWeapon();                                      // same type: nothing
    CHECK(g_sets == 1 && g_gets == 1 && g_typeA.m_refCount == 2);
    std::printf("00664a50_TEST PASS\n");
    return 0;
}