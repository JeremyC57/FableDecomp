// Behaviour test for 007df130: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCWallMount::UnattachObject(long*)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define snprintf _snprintf
#else
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static char g_log[4096];
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n  log=%s\n", __LINE__, #c, g_log); } } while (0)
// ---- source (verbatim) ----
// CTCWallMount::UnattachObject(long* boughtForAmount) -- take the trophy off the mount: hand back (and zero) the price paid; for a live trophy run its wall-mount effects' taken-off hook, make it targetable again and drop the live hero's mounted-trophy count; un-persist and un-flag the mount's thing; clear the trophy pointer and return the trophy
// Leaf: ?UnattachObject@CTCWallMount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF130, length 370 bytes (`ret 4`, no frame pointer; Ego_r.pdb
//   `?UnattachObject@CTCWallMount@@QAEPAVCThing@@PAJ@Z`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CBaseIntelligentPointer() 0x00A01B10 (then the
//   CIntelligentPointer<CThing> vtable 0x01238C6C), SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10;
//   CVectorMap LowerBound 0x0040F020 (inlined GetTC: interfaces 0xDB, 9, 4); CTCWallMountEffects::OnTakenOffMount
//   0x007DF440 (landed); CTCBase::GetWorld() 0x00686A70 (manifest "CViewBase::GetDisplayEngine" propagated);
//   CWorld::GetCurrentPlayer 0x0049D9D0 (manifest "_Cons_val<...>" propagated); CThing::SetAsGameStatePersisted(bool)
//   0x004C7880.
// Layout (Ego_r.pdb, offsets hold, as the landed OnInitialActivate 0x007DE5D0): CTCBase ParentThing +0x04;
//   CTCWallMount BoughtForAmount +0x0c, Trophy +0x10; CThing bitset +0x20 (0xDB = bit 27 of word +0x38, 9 = bit 9 of
//   word +0x20, 4 = bit 4 of word +0x20), interface map +0x44, dead bit +0x91 bit 0, the +0x92 flag byte (bit 4 cleared
//   here; descriptive); CTCTargeted::Targetable +0x10; CPlayer PlayerCharacter +0x34 (PDB); the interface-4
//   component's +0xd4 mounted-trophy count (descriptive).
// Shape: speed TU (inline IS_THING_ALIVE fetches the trophy twice); the effects and hero presence tests fetch the thing
//   once for the bitset and the lookup (HasTC + GetTC); the Targetable store goes through the out-parameter GetInterface
//   (`T* p; if (thing->GetInterface(&p, 9)) p->X = v;` -- the landed 0x007504E0 shape: the component lands in eax, the
//   HasTC/GetTC spelling colours it ecx and rotates the hero lookup's key lea); the hero is tested null / dead through
//   one fetch; `Trophy = 0` goes through the inline operator= with a converting temporary, after the result is fetched.
class CThing;
class CTCBase;
class CBaseObject;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
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

class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];           // +0x20 interface-present bitset
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad_0x4c[0x45];
    unsigned char   m_flags91;                  // +0x91 bit 0: dead
    unsigned char   m_flags92;                  // +0x92
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for T = CThing
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCWallMountEffects { public:                    // interface 0xDB
    void OnTakenOffMount();                     // 0x007DF440
};
class CTCTargeted { public:                            // interface 9
    unsigned char _pad_0x00[0x10];
    bool          Targetable;                   // +0x10
};
struct CTCHeroTrophies {                        // interface 4 (descriptive)
    unsigned char _pad_0x00[0xd4];
    long          MountedTrophies;              // +0xd4
};

class CPlayer { public:
    unsigned char                       _pad_0x00[0x34];
    CIntelligentPointer<CThing>         PlayerCharacter;    // +0x34 CIntelligentPointer<CThingPlayerCreature>
    CThing* GetPlayerCharacter() const { return PlayerCharacter.GetPItem(); }
};
class CWorld { public:
    CPlayer* GetCurrentPlayer();                // 0x0049D9D0
};

struct CTCBaseModel {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CWorld* GetWorld();                         // 0x00686A70
};
class CTCWallMount : public CTCBaseModel { public:
    long                        BoughtForAmount;   // +0x0c
    CIntelligentPointer<CThing> Trophy;            // +0x10
    unsigned __int64            TrophyID;          // +0x18
    CThing* UnattachObject(long* boughtForAmount);
};

CThing* CTCWallMount::UnattachObject(long* boughtForAmount)
{
    if (boughtForAmount)
        *boughtForAmount = BoughtForAmount;
    BoughtForAmount = 0;
    if (IS_THING_ALIVE(Trophy)) {
        CThing* trophy = Trophy.GetPItem();
        if (trophy->HasTC(0xdb))
            ((CTCWallMountEffects*)trophy->GetTC(0xdb))->OnTakenOffMount();
        CTCTargeted* targeted;
        if (Trophy.GetPItem()->GetInterface(&targeted, 9))
            targeted->Targetable = true;
        CThing* hero = GetWorld()->GetCurrentPlayer()->GetPlayerCharacter();
        if (IS_THING_ALIVE(hero) && hero->HasTC(4))
            --((CTCHeroTrophies*)hero->GetTC(4))->MountedTrophies;
    }
    ParentThing->SetAsGameStatePersisted(false);
    ParentThing->m_flags92 &= ~0x10;
    CThing* trophy = Trophy.GetPItem();
    Trophy = CIntelligentPointer<CThing>(0);
    return trophy;
}

// ---- stubs ----
static void logf(const char* fmt, long a = 0, long b = 0, long c = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b, c); strcat(g_log, t); strcat(g_log, ";"); }
void CBaseIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; logf("ctor"); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { logf("dtor"); }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { logf("SetPItem(%ld)", (long)(item != 0)); m_pItem = (CBaseObject*)item; }
template<> void CIntelligentPointer<CThing>::v00() { }
void CTCWallMountEffects::OnTakenOffMount() { logf("OnTakenOffMount(%lx)", (long)(this - (CTCWallMountEffects*)0)); }
static CPlayer* g_player; static CWorld g_world;   // CPlayer has no default constructor (its intelligent pointer takes an item): zero-filled storage
CPlayer* CWorld::GetCurrentPlayer() { logf("GetCurrentPlayer"); return g_player; }
CWorld* CTCBaseModel::GetWorld() { logf("GetWorld"); return &g_world; }
void CThing::SetAsGameStatePersisted(bool persisted) { logf("SetAsGameStatePersisted(%ld)", (long)persisted); }
void CTCBaseModel::v00() { }

int main()
{
    static unsigned char trophyBytes[0x100]; memset(trophyBytes, 0, sizeof trophyBytes); CThing* trophy = (CThing*)trophyBytes;
    static unsigned char heroBytes[0x100]; memset(heroBytes, 0, sizeof heroBytes); CThing* hero = (CThing*)heroBytes;
    static unsigned char parentBytes[0x100]; memset(parentBytes, 0, sizeof parentBytes); CThing* parent = (CThing*)parentBytes;
    static CTCWallMountEffects effects; static CTCTargeted targeted; static CTCHeroTrophies trophies;
    static CInterfaceMap::SEntry trophyEntries[4] = { {9, (CTCBase*)&targeted}, {0x40, (CTCBase*)0x22}, {0xdb, (CTCBase*)&effects}, {0, (CTCBase*)0} };
    static CInterfaceMap::SEntry heroEntries[3] = { {4, (CTCBase*)&trophies}, {0x50, (CTCBase*)0x33}, {0, (CTCBase*)0} };
    trophy->m_interfaces.first = trophyEntries; trophy->m_interfaces.last = trophyEntries + 3;
    hero->m_interfaces.first = heroEntries; hero->m_interfaces.last = heroEntries + 2;
    g_player = (CPlayer*)calloc(1, sizeof(CPlayer)); g_player->PlayerCharacter.m_pItem = (CBaseObject*)hero;
    CTCWallMount* mount = (CTCWallMount*)calloc(1, sizeof(CTCWallMount));   // zero-filled storage (the class has no default ctor for Trophy)
    mount->ParentThing = parent;
    char expect[1024];

    // 1. no trophy, no amount pointer: price zeroed, parent un-persisted and its +0x92 bit 4 cleared, null result
    mount->BoughtForAmount = 55; parent->m_flags92 = 0xff; g_log[0] = 0;
    CHECK(mount->UnattachObject(0) == 0);
    CHECK(mount->BoughtForAmount == 0 && parent->m_flags92 == 0xef);
    CHECK(strcmp(g_log, "SetAsGameStatePersisted(0);ctor;SetPItem(0);SetPItem(0);dtor;") == 0);

    // 2. amount handed back; dead trophy: skipped, but still released and returned
    long amount = -1; mount->BoughtForAmount = 77; mount->Trophy.m_pItem = (CBaseObject*)trophy; trophy->m_flags91 = 1; g_log[0] = 0;
    CHECK(mount->UnattachObject(&amount) == trophy);
    CHECK(amount == 77 && mount->BoughtForAmount == 0 && mount->Trophy.m_pItem == 0);
    CHECK(strcmp(g_log, "SetAsGameStatePersisted(0);ctor;SetPItem(0);SetPItem(0);dtor;") == 0);

    // 3. live trophy with effects + targeted, live hero with the trophies component: hook, targetable, count decremented
    mount->Trophy.m_pItem = (CBaseObject*)trophy; trophy->m_flags91 = 0;
    trophy->m_presentBits[0] = 1UL << 9; trophy->m_presentBits[6] = 1UL << 27; hero->m_presentBits[0] = 1UL << 4;
    targeted.Targetable = false; trophies.MountedTrophies = 3; g_log[0] = 0;
    CHECK(mount->UnattachObject(&amount) == trophy);
    snprintf(expect, sizeof expect, "OnTakenOffMount(%lx);GetWorld;GetCurrentPlayer;SetAsGameStatePersisted(0);ctor;SetPItem(0);SetPItem(0);dtor;", (long)(&effects - (CTCWallMountEffects*)0));
    CHECK(strcmp(g_log, expect) == 0);
    CHECK(targeted.Targetable == true && trophies.MountedTrophies == 2 && amount == 0);

    // 4. trophy without the components, dead hero: nothing touched
    mount->Trophy.m_pItem = (CBaseObject*)trophy; trophy->m_presentBits[0] = 0; trophy->m_presentBits[6] = 0;
    hero->m_flags91 = 1; targeted.Targetable = false; g_log[0] = 0;
    CHECK(mount->UnattachObject(0) == trophy);
    CHECK(strcmp(g_log, "GetWorld;GetCurrentPlayer;SetAsGameStatePersisted(0);ctor;SetPItem(0);SetPItem(0);dtor;") == 0);
    CHECK(targeted.Targetable == false && trophies.MountedTrophies == 2);

    // 5. live hero without the trophies component; null hero
    mount->Trophy.m_pItem = (CBaseObject*)trophy; hero->m_flags91 = 0; hero->m_presentBits[0] = 0; g_log[0] = 0;
    CHECK(mount->UnattachObject(0) == trophy);
    CHECK(trophies.MountedTrophies == 2);
    mount->Trophy.m_pItem = (CBaseObject*)trophy; g_player->PlayerCharacter.m_pItem = 0; g_log[0] = 0;
    CHECK(mount->UnattachObject(0) == trophy);
    CHECK(trophies.MountedTrophies == 2);

    if (fails == 0) printf("007df130_TEST PASS\n"); else printf("007df130_TEST FAIL\n");
    return fails != 0;
}