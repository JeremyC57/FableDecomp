// Behaviour test for 00892cf0: the model below is a copy of src/00892cf0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const { return strcmp(PData, text) == 0; }
};
struct CCharStringData {                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    CCharString(const char* text, long length) { PStringData = new CCharStringData; PStringData->Data.PData = (char*)text; PStringData->Data.StringLength = (unsigned long)strlen(text); }
    ~CCharString() { delete PStringData; }
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
static int g_peekCalls = 0, g_getCalls = 0, g_lookups = 0, g_entryCalls = 0; static long g_lastEntry = -1; static long g_regionFor = 0;
struct CRegion_Retail {                             // 0x58-byte world-map region record
    unsigned char _pad00[0x55];
    bool          m_creatureGenerationEnabled;      // +0x55
};
struct CWorldMap_Retail {
    CRegion_Retail regions[4];
    CRegion_Retail& GetRegionEntry(long region) { ++g_entryCalls; g_lastEntry = region; return regions[region]; }
    long GetRegionNumberFromName(const CCharString& name) { ++g_lookups; return g_regionFor; }
};
struct CWorld {
    virtual void w000() {} virtual void w001() {} virtual void w002() {} virtual void w003() {} virtual void w004() {} virtual void w005() {} virtual void w006() {} virtual void w007() {}
    virtual void w008() {} virtual void w009() {} virtual void w010() {} virtual void w011() {}
    virtual CWorldMap_Retail* GetWorldMap() { ++g_getCalls; return &map; }   // slot 12 (+0x30)
    virtual CWorldMap_Retail* PeekWorldMap() { ++g_peekCalls; return &map; }   // slot 13 (+0x34)
    unsigned char                _pad04[0xd0];
    bool                         m_creatureGeneratorsEnabled;   // +0xd4 (global switch)
    CWorldMap_Retail map;
};
struct CGameScriptInterface {
    virtual void v000() {}
    CWorld*                    World;               // +0x04
    void SetCreatureGeneratorsEnabled(const CCharString& regionName, bool enabled) const;
};

void CGameScriptInterface::SetCreatureGeneratorsEnabled(const CCharString& regionName, bool enabled) const
{
    if (regionName.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (regionName.PStringData->Data == "")
        goto empty;
    {
        long region = World->PeekWorldMap()->GetRegionNumberFromName(regionName);
        if (region > 0)
            World->GetWorldMap()->GetRegionEntry(region).m_creatureGenerationEnabled = enabled;
    }
    return;
empty:
    World->m_creatureGeneratorsEnabled = enabled;
}

// ---- test doubles and driver ----
static bool Run(const CCharString& name, long regionFor, bool enabled, bool expectGlobal, int expectLookups, int expectEntries, long expectEntry)
{
    CWorld world; std::memset(&world._pad04, 0, sizeof(world) - sizeof(void*)); 
    CGameScriptInterface gsi; gsi.World = &world;
    g_peekCalls = g_getCalls = g_lookups = g_entryCalls = 0; g_lastEntry = -1; g_regionFor = regionFor;
    gsi.SetCreatureGeneratorsEnabled(name, enabled);
    if (world.m_creatureGeneratorsEnabled != expectGlobal) return false;
    if (g_lookups != expectLookups || g_peekCalls != expectLookups || g_entryCalls != expectEntries || g_getCalls != expectEntries) return false;
    if (expectEntries && (g_lastEntry != expectEntry || world.map.regions[expectEntry].m_creatureGenerationEnabled != enabled)) return false;
    return true;
}
int main()
{
    bool ok = true;
    { CCharString n("", -1); ok = ok && Run(n, 2, true, true, 0, 0, 0); }                       // empty block: the global switch
    { CCharString n("", -1); delete n.PStringData; n.PStringData = 0; ok = ok && Run(n, 2, true, true, 0, 0, 0); }   // null block: the global switch
    { CCharString n("Oakvale", -1); ok = ok && Run(n, 2, true, false, 1, 1, 2); }               // region 2 found and set
    { CCharString n("Oakvale", -1); ok = ok && Run(n, 0, true, false, 1, 0, 0); }               // region 0: nothing
    { CCharString n("Nowhere", -1); ok = ok && Run(n, -1, false, false, 1, 0, 0); }             // not found: nothing
    if (!ok) { std::printf("00892cf0_TEST FAIL\n"); return 1; }
    std::printf("00892cf0_TEST PASS\n");
    return 0;
}