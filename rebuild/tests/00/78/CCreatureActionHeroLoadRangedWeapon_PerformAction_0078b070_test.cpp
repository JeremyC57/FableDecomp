#include <stdio.h>
#include <string.h>
// CCreatureAction_HeroLoadRangedWeapon::PerformAction() -- when the ranged weapon lives and is analogue-loading, create one
// shot of the weapon's ammo (its created-shot def) at the hero for the hero's player, keep it in PAmmo, and put a live
// carryable (0x47) ammo thing into the hero's carry slot for the weapon's category
// Leaf: ?PerformAction@CCreatureAction_HeroLoadRangedWeapon
// Layout: retail CCreatureActionBase is 0xa8 (Ego_r.pdb +0xc), so PRangedWeapon +0xa8 and PAmmo +0xb0 (PDB +0xb4/+0xbc);
//   the shot def's object def index at +0x7c; CThing interface bitset +0x20, map +0x44, player +0x90.
// Shape: IS_THING_ALIVE<> are out-of-line template instances (0x004CC340, 0x0078B2A0); the object is created through an
//   inline wrapper taking the position thing and the owner (both GetPItem calls before the pushes); CIntelligentPointer
//   <CThingObject>(CThing*) is out of line (0x0065E133) and its operator= inline, reading the new item into a named local
//   before SetPItem (so &PAmmo is formed after the GetPItem call as retail); GetInterface stores the component as ONE ternary
//   `*ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;` (fixes the esi/ebx/edi permutation).
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef() const;                  // slot 1
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
    mutable long m_refCount;                          // +4
};
struct CShotDef : CDefPointeeBase {
    unsigned char _pad08[0x7c - 8];
    long          ObjectDefIndex;                     // +0x7c
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<const CShotDef>* out);   // 0x0078B230
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x90 - 0x4c];
    char          PlayerNumber;                       // +0x90
    const C3DVector& GetPosition() const;             // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing {};
class CRightHandedSet;
CThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                           unsigned long flags, const CCharString& name);   // 0x00703210
__forceinline CThingObject* CreateThingObject(long defIndex, const CThing* owner, const CThing* at, const CCharString& name)
{
    return CreateThingObject(defIndex, &at->GetPosition(), owner->PlayerNumber, 0, 0, name);
}
class CTCWeapon { public:                                    // interface 0x48
    bool IsAnalogueLoading_005da8f0();                         // 0x005DA8F0
    long GetCategoryIdentifier();                     // 0x005DA920
    long GetCreatedShotDefGlobalIndex();              // 0x005DA750
};
class CTCCarrying { public:                                  // interface 0x46
    void PutThingInCarrySlot(CThing* thing, long slot, bool show);   // 0x006A1F80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                     // out of line (CThingObject: 0x0065E133)
    __forceinline CIntelligentPointer& operator=(T* item)
    {
        CIntelligentPointer pointer(item);
        CThing* p = pointer.GetPItem();
        SetPItem(p);
        return *this;
    }
};
template<class T> bool __fastcall IS_THING_ALIVE(const T& pointer);   // 0x004CC340 / 0x0078B2A0
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;            // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase { public:
    CIntelligentPointer<CThing>       PRangedWeapon;  // +0xa8
    CIntelligentPointer<CThingObject> PAmmo;          // +0xb0
    void PerformAction();
};

void CCreatureAction_HeroLoadRangedWeapon::PerformAction()
{
    CTCWeapon* weapon;
    if (IS_THING_ALIVE(PRangedWeapon) && PRangedWeapon.GetPItem()->GetInterface(&weapon, 0x48)) {
        CTCCarrying* carrying;
        if (GetCreature()->GetInterface(&carrying, 0x46) && weapon->IsAnalogueLoading_005da8f0()) {
            long category = weapon->GetCategoryIdentifier();
            long shot = weapon->GetCreatedShotDefGlobalIndex();
            if (category >= 0 && shot >= 0) {
                CDefPointer<const CShotDef> def;
                GetPDefinitionManager()->GetDef(shot, &def);
                long objectDef = def->ObjectDefIndex;
                PAmmo = CreateThingObject(objectDef, GetCreature(), GetCreature(), "");
                if (IS_THING_ALIVE(PAmmo) && PAmmo.GetPItem()->HasTC(0x47))
                    carrying->PutThingInCarrySlot(PAmmo.GetPItem(), category, true);
            }
        }
    }
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static int g_strings, g_stringDtors, g_destroyed, g_creates, g_puts, g_ipDtors;
static long g_createDef, g_createPlayer, g_createFlags; static const C3DVector* g_createPos; static const void* g_createSet; static const char* g_createName;
static CThing* g_putThing; static long g_putSlot; static int g_putShow;
static bool g_analogue; static long g_category, g_shot; static CShotDef* g_shotDef; static long g_getDefIndex;
static CThingObject* g_newObject; static CThing* g_dead;
CCharString::CCharString(const char* text, long length) : PStringData((void*)text) { g_strings++; (void)length; }
CCharString::~CCharString() { g_stringDtors++; }
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() const { g_destroyed++; }
bool CDefinitionManager::GetDef(long index, CDefPointer<const CShotDef>* out) { g_getDefIndex = index; out->Object = g_shotDef; ++g_shotDef->m_refCount; return true; }
static C3DVector g_heroPos = { 4.0f, 5.0f, 6.0f };
const C3DVector& CThing::GetPosition() const { return g_heroPos; }
CThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set, unsigned long flags, const CCharString& name)
{ g_creates++; g_createDef = defIndex; g_createPos = position; g_createPlayer = player; g_createSet = set; g_createFlags = (long)flags; g_createName = (const char*)name.PStringData; return g_newObject; }
bool CTCWeapon::IsAnalogueLoading_005da8f0() { return g_analogue; }
long CTCWeapon::GetCategoryIdentifier() { return g_category; }
long CTCWeapon::GetCreatedShotDefGlobalIndex() { return g_shot; }
void CTCCarrying::PutThingInCarrySlot(CThing* thing, long slot, bool show) { g_puts++; g_putThing = thing; g_putSlot = slot; g_putShow = show; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { g_ipDtors++; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
template<class T> CIntelligentPointer<T>::CIntelligentPointer(T* item) { m_pItem = item; }
template<class T> bool __fastcall IS_THING_ALIVE(const T& pointer) { return pointer.GetPItem() != 0 && pointer.GetPItem() != g_dead; }
void CCreatureActionBase::v00() {}
static CDefinitionManager g_defMgr;
CDefinitionManager* CCreatureActionBase::GetPDefinitionManager() const { return &g_defMgr; }
static void Reset() { g_strings = g_stringDtors = g_destroyed = g_creates = g_puts = g_ipDtors = 0; g_putThing = 0; g_createName = 0; g_getDefIndex = -1; }
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
static unsigned char g_heroBuf[0x100], g_bowBuf[0x100], g_ammoBuf[0x100], g_actionBuf[sizeof(CCreatureAction_HeroLoadRangedWeapon)];
int main()
{
    CThing* hero = (CThing*)g_heroBuf; CThing* bow = (CThing*)g_bowBuf; CThingObject* ammo = (CThingObject*)g_ammoBuf;
    static CTCWeapon weapon; static CTCCarrying carrying;
    CInterfaceMap::SEntry he[] = { { 0x46, (CTCBase*)&carrying } }; SetInterfaces(hero, he, 1); hero->PlayerNumber = 2;
    CInterfaceMap::SEntry be[] = { { 0x10, 0 }, { 0x48, (CTCBase*)&weapon } }; SetInterfaces(bow, be, 2);
    CInterfaceMap::SEntry ae[] = { { 0x47, 0 } }; SetInterfaces(ammo, ae, 1);
    static CShotDef shot; shot.m_refCount = 1; shot.ObjectDefIndex = 321; g_shotDef = &shot;
    CCreatureAction_HeroLoadRangedWeapon* a = (CCreatureAction_HeroLoadRangedWeapon*)g_actionBuf;
    a->PCreature.m_pItem = hero; a->PRangedWeapon.m_pItem = bow; a->PAmmo.m_pItem = 0;
    g_analogue = true; g_category = 5; g_shot = 77; g_newObject = ammo; Reset();
    a->PerformAction();
    CHECK(g_getDefIndex == 77 && g_creates == 1 && g_createDef == 321 && g_createPos == &g_heroPos && g_createPlayer == 2 && g_createSet == 0 && g_createFlags == 0);
    CHECK(g_createName && g_createName[0] == 0 && a->PAmmo.m_pItem == ammo);
    CHECK(g_puts == 1 && g_putThing == ammo && g_putSlot == 5 && g_putShow == 1);
    CHECK(shot.m_refCount == 1 && g_destroyed == 0 && g_strings == g_stringDtors && g_ipDtors == 1);
    // created ammo is not carryable: kept in PAmmo but not carried
    SetInterfaces(ammo, ae, 0); a->PAmmo.m_pItem = 0; Reset();
    a->PerformAction();
    CHECK(g_creates == 1 && a->PAmmo.m_pItem == ammo && g_puts == 0);
    // creation failed
    g_newObject = 0; Reset(); a->PerformAction();
    CHECK(g_creates == 1 && a->PAmmo.m_pItem == 0 && g_puts == 0);
    g_newObject = ammo; SetInterfaces(ammo, ae, 1);
    // negative category or shot: nothing created
    g_category = -1; Reset(); a->PerformAction(); CHECK(g_creates == 0 && g_getDefIndex == -1);
    g_category = 5; g_shot = -1; Reset(); a->PerformAction(); CHECK(g_creates == 0);
    g_shot = 77;
    // not analogue-loading; hero without carrying; dead weapon; weapon without the weapon component
    g_analogue = false; Reset(); a->PerformAction(); CHECK(g_creates == 0);
    g_analogue = true; SetInterfaces(hero, he, 0); Reset(); a->PerformAction(); CHECK(g_creates == 0);
    SetInterfaces(hero, he, 1); g_dead = bow; Reset(); a->PerformAction(); CHECK(g_creates == 0);
    g_dead = 0; SetInterfaces(bow, be, 1); Reset(); a->PerformAction(); CHECK(g_creates == 0);
    // all good again
    SetInterfaces(bow, be, 2); Reset(); a->PerformAction(); CHECK(g_creates == 1 && g_puts == 1);
    if (fails == 0) printf("0078b070_TEST PASS\n"); else printf("0078b070_TEST FAIL\n");
    return fails;
}