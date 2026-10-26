// Behaviour test for 004c7f40: body copied from src/004c7f40.cpp (CThing::InitFromThingInfo), the def manager and string compare stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const { return strcmp(PData, text) == 0; }
};
class CCharStringData { public: CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public: CCharStringData* PStringData; };
class CThingBaseDef;
struct CDefPointer { const CThingBaseDef* Object; };
static int g_nameCalls = 0, g_defCalls = 0; static long g_lastIndex = -1; static CDefPointer* g_lastOut = 0; static long g_nameResult = 0;
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) { ++g_nameCalls; return g_nameResult; }
    bool GetDef(long index, CDefPointer* out) { ++g_defCalls; g_lastIndex = index; g_lastOut = out; out->Object = (const CThingBaseDef*)0x5150; return true; }
};
class CThingManager { public: unsigned char _pad00[0x20]; CDefinitionManager* DefinitionManager; };
CThingManager* g_pThingManager;
class CThingInfo { public: unsigned __int64 UniqueID; CCharString DefName; signed char Player; };
class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;
    unsigned char    _pad20[0x70 - 0x20];
    CDefPointer      PBaseDef;
    unsigned char    _pad74[0x8c - 0x74];
    unsigned short   DefGlobalIndex;
    short            CurrentMapNumber;
    signed char      PlayerNumber;
    unsigned char    Dead : 1;
    unsigned char    CreatedByOwnerThingCreation : 1;
    unsigned char    IncludeInThingScans : 1;
    unsigned char    Activated : 1;
    unsigned char    GloballyLoaded : 1;
    unsigned char    InLimbo : 1;
    unsigned char    KillLocked : 1;
    unsigned char    CreatedBySerialisation : 1;
    bool InitFromThingInfo(const CThingInfo& info);
};
bool CThing::InitFromThingInfo(const CThingInfo& info)
{
    UniqueID = info.UniqueID;
    PlayerNumber = info.Player;
    if (info.DefName.PStringData == 0) {              // CCharString::operator==(const char*) hand-split: the two tests join the same arm
        if (strcmp("NULL", "") == 0)
            goto null_name;
    } else if (info.DefName.PStringData->Data == "NULL") {
        goto null_name;
    }
    DefGlobalIndex = (unsigned short)g_pThingManager->DefinitionManager->GetDefGlobalIndexFromName(info.DefName);
    if (DefGlobalIndex == 0) {
        Dead = true;
        return false;
    }
    goto lookup;
null_name:
    DefGlobalIndex = 0;
lookup:
    if (DefGlobalIndex > 0)
        g_pThingManager->DefinitionManager->GetDef(DefGlobalIndex, &PBaseDef);
    return true;
}
static CCharStringData* MakeName(const char* text) { CCharStringData* d = new CCharStringData; d->Data.PData = (char*)text; d->Data.StringLength = (unsigned long)strlen(text); d->NoRefs = 1; return d; }
int main()
{
    CDefinitionManager dm; CThingManager tm; memset(&tm, 0, sizeof(tm)); tm.DefinitionManager = &dm; g_pThingManager = &tm;
    CThing t; memset(&t, 0, sizeof(t));
    CThingInfo info; info.UniqueID = 0x1122334455667788ULL; info.Player = 3;
    // 1. a real name: lookup, index stored, def fetched
    info.DefName.PStringData = MakeName("CREATURE_HERO"); g_nameResult = 0x1234;
    CHECK(t.InitFromThingInfo(info) == true);
    CHECK(t.UniqueID == 0x1122334455667788ULL && t.PlayerNumber == 3 && t.DefGlobalIndex == 0x1234 && t.Dead == 0);
    CHECK(g_nameCalls == 1 && g_defCalls == 1 && g_lastIndex == 0x1234 && g_lastOut == &t.PBaseDef && t.PBaseDef.Object == (const CThingBaseDef*)0x5150);
    // 2. the "NULL" name: no lookup, index 0, no def
    memset(&t, 0, sizeof(t)); info.DefName.PStringData = MakeName("NULL");
    CHECK(t.InitFromThingInfo(info) == true);
    CHECK(t.DefGlobalIndex == 0 && g_nameCalls == 1 && g_defCalls == 1 && t.PBaseDef.Object == 0 && t.Dead == 0);
    // 3. a null string block: strcmp("NULL", "") != 0, so the lookup runs
    memset(&t, 0, sizeof(t)); info.DefName.PStringData = 0; g_nameResult = 7;
    CHECK(t.InitFromThingInfo(info) == true && t.DefGlobalIndex == 7 && g_nameCalls == 2 && g_defCalls == 2 && g_lastIndex == 7);
    // 4. an unknown name: index 0 -> dead, false, no def
    memset(&t, 0, sizeof(t)); info.DefName.PStringData = MakeName("BOGUS"); g_nameResult = 0;
    CHECK(t.InitFromThingInfo(info) == false && t.DefGlobalIndex == 0 && t.Dead == 1 && g_nameCalls == 3 && g_defCalls == 2);
    // 5. a name whose index truncates to 16 bits
    memset(&t, 0, sizeof(t)); g_nameResult = 0x10003;
    CHECK(t.InitFromThingInfo(info) == true && t.DefGlobalIndex == 3 && g_lastIndex == 3);
    if (fails == 0) printf("004c7f40_TEST PASS\n"); else printf("004c7f40_TEST FAIL\n");
    return fails;
}