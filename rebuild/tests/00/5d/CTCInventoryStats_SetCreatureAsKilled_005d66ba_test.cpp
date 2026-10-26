// Behaviour test for 005d66ba: src/005d66ba.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryStats::SetCreatureAsKilled(CThing* creature)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryStats::SetCreatureAsKilled(CThing* creature) -- counts a kill of the creature's kind in the stats screen's kill table (first kill inserts 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D66BA, length 140 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Ego_r.pdb: ?SetCreatureAsKilled@CTCInventoryStats@@QAEXPAVCThing@@@Z.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (`__fastcall`, reads 0x013B879C), CDefinitionManager::GetDef<CDefClassBase>
// (long, CDefPointer*) @ 0x0042AF3C (manifest names a propagated instantiation), CDefClassBase::GetSubDef<T>(CDefPointer*)
// @ 0x00485815, the kill table's find @ 0x005D83B3 (manifest "CMemoryAllocatorVariableSize_FindNode", propagated) and
// operator[] @ 0x005D8933, the defs' slot 1 on release.
// Shape: the thing's def is a zeroed CDefPointer local ([ebp-4]); the sub-def pointer is a second zeroed local that VC homes in
// the dead `creature` slot ([ebp+8]); both release through a null-tested dtor. The find result is compared with the header
// word directly; the hit increments the node's value (+0x14).
// Layout: CThing def index (unsigned short) +0x8C; retail CTCInventoryStats kill table (STLport map<long,long>, header word
// first) at +0x134; the creature sub-def's kill-category key at +0xAC (retail offsets).
#pragma optimize("y", off)
#pragma optimize("s", on)
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CSubDefPointer {    // the GetSubDef out pointer, released by hand
    T* Object;
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CCreatureKillDef : CDefPointeeBase {
    unsigned char _pad08[0xac - 0x08];
    long KillCategory;                          // +0xac
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CSubDefPointer<CCreatureKillDef>* out);   // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CSubDefPointer<CCreatureKillDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                    // +0x8c
};
struct CKillMapNode {
    long  Colour;
    void* Parent;
    void* Left;
    void* Right;
    long  Key;                                  // +0x10
    long  Count;                                // +0x14
};
struct CKillMap {                               // STLport map<long,long>
    CKillMapNode* Header;                       // +0x00
    long          NodeCount;
    CKillMapNode* find(const long& key);        // 0x005D83B3
    long& operator[](const long& key);          // 0x005D8933
};

class CTCInventoryStats { public:
    unsigned char _pad00[0x134];
    CKillMap KilledCreatures;                   // +0x134
    void SetCreatureAsKilled(CThing* creature);
};

void CTCInventoryStats::SetCreatureAsKilled(CThing* creature)
{
    CDefPointer<CDefClassBase> def;
    CGameDefinitionManager::Get().GetDef(creature->DefIndex, &def);
    CSubDefPointer<CCreatureKillDef> killDef;
    killDef.Object = 0;
    def->GetSubDef(&killDef);
    CCreatureKillDef* creatureDef = killDef.Object;
    const long& category = creatureDef->KillCategory;
    CKillMapNode* node = KilledCreatures.find(category);
    if (node == KilledCreatures.Header)
        KilledCreatures[category] = 1;
    else
        ++node->Count;
    if (creatureDef && DecRefToZero(creatureDef->m_refCount))
        creatureDef->Destroy();
}

// ---- stubs and driver ----
static void* g_def = 0; static void* g_sub = 0; static int g_destroyed = 0;
static CDefPointeeBase* g_lastDestroyed = 0;
void* FindDef_CDefClassBase_GetSubDef_1() { return g_sub; }
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; g_lastDestroyed = this; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
static CKillMapNode g_header; static CKillMapNode g_node; static long g_findKey = -1; static bool g_present = false;
static long g_indexed = -1; static long g_slot = 0;
CKillMapNode* CKillMap::find(const long& key) { g_findKey = key; return g_present ? &g_node : Header; }
long& CKillMap::operator[](const long& key) { g_indexed = key; return g_slot; }
int main()
{
    CDefClassBase def; def.m_refCount = 2;
    CCreatureKillDef kill; kill.m_refCount = 2; kill.KillCategory = 7;
    g_def = &def; g_sub = &kill;
    CThing creature; memset(&creature, 0, sizeof(creature)); creature.DefIndex = 0x55;
    CTCInventoryStats stats; memset(&stats, 0, sizeof(stats)); stats.KilledCreatures.Header = &g_header;
    // a category not yet in the map is added with count 1
    g_present = false; stats.SetCreatureAsKilled(&creature);
    CHECK(g_findKey == 7); CHECK(g_indexed == 7); CHECK(g_slot == 1);
    CHECK(kill.m_refCount == 1); CHECK(def.m_refCount == 1); CHECK(g_destroyed == 0);
    // an existing category is counted up
    g_present = true; g_node.Count = 4; g_indexed = -1; kill.KillCategory = 9;
    stats.SetCreatureAsKilled(&creature);
    CHECK(g_findKey == 9); CHECK(g_indexed == -1); CHECK(g_node.Count == 5);
    CHECK(kill.m_refCount == 0); CHECK(def.m_refCount == 0); CHECK(g_destroyed == 2);
    if (fails == 0) printf("005d66ba_TEST PASS\n"); else printf("005d66ba_TEST FAIL\n");
    return fails;
}