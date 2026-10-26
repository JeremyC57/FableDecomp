// Behaviour test for 005cd33f: src/005cd33f.cpp copied verbatim (including its real GetSubDef body); the GUI manager, the
// definition manager, the sub-def lookup, the bitset test, LowerBound, SetPDef and the base OnCreate are stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryStats::OnCreate() -- register the stats screen with the hero-information screen manager, point the inventory at the owner's CCreatureDef StatsScreenInventory def, run the base OnCreate and cache the owner's hero-experience (TC 0x68) and hero-stats (TC 4) components
// Grade: RELOCATION_MATCH -- the real GetSubDef body (size 246 -> 244) plus the GetInterface entry pick written as the
// ternary `((it == map->last || typeId < it->type) ? map->last : it)->component` (retry 10b20 lever): the two reads of
// map->last outweigh `this` in the esi/edi tie, giving retail this=edi (edi pushed at entry) and thingDef/owner=esi.
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CD33F, length 244 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) +
// ("s",on).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 on g_pPlayerGui (0x013B8790), NHeroInformationScreens::
// CManager::RegisterScreen(const CBase*) @ 0x0066EB11, CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*)
// @ 0x00485815, CTCInventoryBase::SetPDef(long) @ 0x005BDB4D, CTCInventoryBase::OnCreate() @ 0x005C0121, std::bitset::test
// @ 0x00410DE0 on the owner's UsingComponents (+0x20), the const component-map LowerBound @ 0x004365B0, the defs' slot 1 on
// release.
// Shape: the landed CTCInventoryExperience::OnCreate (0x0057713F) / CTCInventoryTrade::OnCreate (0x0059C8F2) def dance (the
// null-safe conversion to the CBase secondary base, the owner def add-ref'd and released by hand around GetSubDef, the sub-def
// released without a null test at the end); each component is the landed GetInterface bitset-test + const LowerBound shape
// with the owner re-read, the size-optimised `push 68h ; pop ebx` constant serving the test and the key.
// Layout: CThing PDef +0x70; CCreatureDef StatsScreenInventory retail +0x90 (PDB +0xA0); retail CTCInventoryBase 0x130 bytes
// with CBase at +0x130; CTCInventoryStats CreaturesKilled +0x134 (12-byte map), pMannequin +0x140, UseMannequin +0x144,
// lastCategoryIdx +0x148, pTCHeroExperience +0x14c, pTCHeroStats +0x150 (PDB minus 0x18).
#pragma optimize("y", off)
#pragma optimize("s", on)
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (DecRefToZero(m_refCount)) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x90 - 0x08];
    long StatsScreenInventory;                  // +0x90 (PDB +0xA0)
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefSubRef { long DefIndex; };
class CDefinitionManager { public:
    CDefPointer<CDefPointeeBase> GetDef(long index);    // 0x009ADA10 (hidden result)
};
class CDefClassBase : public CDefPointeeBase { public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual const CDefSubRef* FindSubDef(const CCharString& typeName);   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* DefManager;                     // +0x1c
    bool GetSubDef(CDefPointer<CCreatureDef>* out);     // 0x00485815
};
// 0x00485815 CDefClassBase::GetSubDef<CCreatureDef>, the retail body: look the sub def up by its class name, fetch it from
// the definition manager and assign it into *out (addref/release); only *out is written, so the caller's def word does
// not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    const CDefSubRef* sub = FindSubDef(CCharString("CCreatureDef"));
    if (!sub)
        return false;
    CCreatureDef* def;
    {
        CDefPointer<CDefPointeeBase> fetched = DefManager->GetDef(sub->DefIndex);
        def = (CCreatureDef*)fetched.Object;
        if (def)
            ++def->m_refCount;
        if (fetched.Object) {
            fetched.Object->Release();
            fetched.Object = 0;
        }
    }
    if (out->Object != def) {
        if (out->Object) {
            out->Object->Release();
            out->Object = 0;
        }
        out->Object = def;
        if (def)
            ++def->m_refCount;
    }
    if (def)
        def->Release();
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CPresenceBits {
    bool test(long bit) const;                  // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits UsingComponents;              // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    template <class T> __forceinline void GetInterface(long typeId, T** out) const
    {
        if (UsingComponents.test(typeId)) {
            long type = typeId;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || typeId < it->type) ? map->last : it)->component;
        }
    }
};
namespace NHeroInformationScreens {
class CBase { public:                                  // vtable 0x0123A3E0 (its vptr is the +0x130 word)
    virtual void v00();
};
class CManager { public:
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
class CTCHeroExperience;
class CTCHeroStats;
struct CTCInventoryBase_Stats {                 // retail 0x130 bytes
    virtual void OnCreate();                    // slot 4
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
class CTCInventoryStats : public CTCInventoryBase_Stats, public NHeroInformationScreens::CBase {
public:
    unsigned char      CreaturesKilled[0xc];    // +0x134 map<unsigned long, long>
    CThing*            pMannequin;              // +0x140
    bool               UseMannequin;            // +0x144
    long               lastCategoryIdx;         // +0x148
    CTCHeroExperience* pTCHeroExperience;       // +0x14c
    CTCHeroStats*      pTCHeroStats;            // +0x150
    virtual void OnCreate();
};

void CTCInventoryStats::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CCreatureDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    SetPDef(def->StatsScreenInventory);
    CTCInventoryBase_Stats::OnCreate();
    ParentThing->GetInterface(0x68, &pTCHeroExperience);
    ParentThing->GetInterface(4, &pTCHeroStats);
    def.Object->Release();
}

#pragma optimize("", on)
// ---- stubs ----
static int gDestroyed = 0; static CDefPointeeBase* gDestroyedObj = 0;
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() { ++gDestroyed; gDestroyedObj = this; }
static char gTypeName[32];
CCharString::CCharString(const char* text, long length) { strcpy(gTypeName, text); PStringData = 0; CHECK(length == -1); }
CCharString::~CCharString() { }
static CDefPointeeBase* gManagerDef = 0; static long gFetchedIndex = -1;
CDefPointer<CDefPointeeBase> CDefinitionManager::GetDef(long index)
{
    static struct { CDefPointeeBase* Object; } holder;
    gFetchedIndex = index; holder.Object = gManagerDef; if (holder.Object) ++holder.Object->m_refCount;
    return *(CDefPointer<CDefPointeeBase>*)&holder;
}
void CDefClassBase::v02() { } void CDefClassBase::v03() { } void CDefClassBase::v04() { } void CDefClassBase::v05() { }
void CDefClassBase::v06() { } void CDefClassBase::v07() { } void CDefClassBase::v08() { } void CDefClassBase::v09() { }
void CDefClassBase::v10() { } void CDefClassBase::v11() { } void CDefClassBase::v12() { } void CDefClassBase::v13() { }
static const CDefSubRef* gSubRef = 0;
const CDefSubRef* CDefClassBase::FindSubDef(const CCharString&) { return gSubRef; }
static unsigned long gBits[8];
bool CPresenceBits::test(long bit) const { CHECK(this != 0); return (gBits[bit >> 5] >> (bit & 31)) & 1; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void NHeroInformationScreens::CBase::v00() { }
static int gRegister = 0; static const NHeroInformationScreens::CBase* gRegistered = 0;
void NHeroInformationScreens::CManager::RegisterScreen(const CBase* screen) { ++gRegister; gRegistered = screen; }
static NHeroInformationScreens::CManager gManager;
NHeroInformationScreens::CManager* CPlayerGui::GetPHeroInfoScreensManager() const { return &gManager; }
static CPlayerGui gGui;
CPlayerGui* g_pPlayerGui = &gGui;
static int gSetPDef = 0; static long gSetPDefValue = 0;
void CTCInventoryBase_Stats::SetPDef(long index) { ++gSetPDef; gSetPDefValue = index; }
static int gBaseCreate = 0;
void CTCInventoryBase_Stats::OnCreate() { ++gBaseCreate; }

static void Reset() { gDestroyed = 0; gDestroyedObj = 0; gTypeName[0] = 0; gFetchedIndex = -1; gRegister = 0; gRegistered = 0; gSetPDef = 0; gSetPDefValue = 0; gBaseCreate = 0; memset(gBits, 0, sizeof gBits); }

int main()
{
    static unsigned char thingMem[0x80]; memset(thingMem, 0, sizeof thingMem);
    CThing& thing = *(CThing*)thingMem;
    static CDefClassBase ownerDefObject;
    CDefClassBase* ownerDef = &ownerDefObject;
    CDefinitionManager manager; ownerDef->DefManager = &manager;
    thing.PDef.Object = ownerDef;
    CCreatureDef creatureDef; creatureDef.StatsScreenInventory = 0x777;
    CDefSubRef sub = { 31 };
    CTCHeroExperience* expMarker = (CTCHeroExperience*)0x6868; CTCHeroStats* statsMarker = (CTCHeroStats*)0x0404;
    CInterfaceMap::SEntry entries[3] = { { 4, (CTCBase*)statsMarker }, { 0x68, (CTCBase*)expMarker }, { 0x7fffffff, (CTCBase*)0xEEEE } };
    thing.Components.first = entries; thing.Components.last = entries + 2;   // entries[2] = the end() sentinel's component

    CTCInventoryStats stats; stats.ParentThing = &thing; stats.pTCHeroExperience = 0; stats.pTCHeroStats = 0;
    // 1: both components present; owner def held elsewhere (count 1), creature def held by the manager (count 1)
    Reset(); ownerDef->m_refCount = 1; creatureDef.m_refCount = 1; gSubRef = &sub; gManagerDef = &creatureDef;
    gBits[0x68 >> 5] |= 1UL << (0x68 & 31); gBits[0] |= 1UL << 4;
    stats.OnCreate();
    CHECK(gRegister == 1 && gRegistered == (NHeroInformationScreens::CBase*)&stats);
    CHECK(strcmp(gTypeName, "CCreatureDef") == 0 && gFetchedIndex == 31);
    CHECK(gSetPDef == 1 && gSetPDefValue == 0x777 && gBaseCreate == 1);
    CHECK(stats.pTCHeroExperience == expMarker && stats.pTCHeroStats == statsMarker);
    CHECK(ownerDef->m_refCount == 1 && creatureDef.m_refCount == 1 && gDestroyed == 0);
    // 2: no components flagged -> pointers untouched; the owner def's only reference was ours (destroyed), and the creature
    //    def's last reference is dropped at the end (destroyed)
    Reset(); ownerDef->m_refCount = 0; creatureDef.m_refCount = 0;
    stats.pTCHeroExperience = (CTCHeroExperience*)1; stats.pTCHeroStats = (CTCHeroStats*)2;
    stats.OnCreate();
    CHECK(stats.pTCHeroExperience == (CTCHeroExperience*)1 && stats.pTCHeroStats == (CTCHeroStats*)2);
    CHECK(gDestroyed == 2 && ownerDef->m_refCount == 0 && creatureDef.m_refCount == 0);
    // 3: components flagged but missing from the map -> the end() entry's component
    Reset(); ownerDef->m_refCount = 1; creatureDef.m_refCount = 1;
    gBits[0x68 >> 5] |= 1UL << (0x68 & 31); gBits[0] |= 1UL << 4;
    thing.Components.first = entries + 2; thing.Components.last = entries + 2;
    stats.OnCreate();
    CHECK(stats.pTCHeroExperience == (CTCHeroExperience*)0xEEEE && stats.pTCHeroStats == (CTCHeroStats*)0xEEEE);
    // 4: a map whose first key is above the wanted one (typeId < it->type) -> end() as well
    Reset(); gBits[0x68 >> 5] |= 1UL << (0x68 & 31);
    CInterfaceMap::SEntry high[2] = { { 0x70, (CTCBase*)0x1234 }, { 0, (CTCBase*)0xDDDD } };
    thing.Components.first = high; thing.Components.last = high + 1;
    stats.OnCreate();
    CHECK(stats.pTCHeroExperience == (CTCHeroExperience*)0xDDDD);
    if (fails == 0) printf("005cd33f_TEST PASS\n");
    return fails;
}