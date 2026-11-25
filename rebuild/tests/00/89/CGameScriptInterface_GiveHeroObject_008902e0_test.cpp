/* Behavioral test for CGameScriptInterface::GiveHeroObject @ 008902e0 (NF413: linked).
 * Links the row object (the function is declared here, not defined) with recording definitions of its callees under the row's
 * spellings, drives every control-flow path, and checks the item name the row constructs: retail's pooled "" (0x0122D70E).
 * Prints exactly:
 *     FSE2_008902e0_TEST PASS
 * Returns 0 on success, nonzero on the first failed check.
 */
#include <cstdio>

/* ------- recording state for the stubs ------- */
static int   g_countRet;          /* value CountObjects returns */
static void* g_heroRet;           /* value GetPControlledCreature returns */
static unsigned char g_giveRet;   /* value Give returns */

static int   g_lastObjDefId;
static void* g_lastContainer;
static void* g_lastWorld;
static void* g_lastPlayer;
static int   g_ctorCalls, g_dtorCalls, g_giveCalls;
static int   g_lastGiveCount, g_lastGiveQty, g_lastGiveSuppress;
static void* g_lastGiveHero;
static void* g_lastGiveName;
static void* g_lastCtorThis;
static const char* g_lastCtorStr;
static int   g_lastCtorLen;
static int   g_vfnCalls;
static void* g_lastVfnThis;

/* ------- the row's callees, under the row's spellings ------- */
class CSuspendableProcessMicrothread;
CSuspendableProcessMicrothread* g_pScriptRunner;   /* the post-give UI refresh gate */

class CCharString { public: void* rep; };

struct ObjContainer { int CountObjects(int objDefId); };
int ObjContainer::CountObjects(int objDefId) { g_lastContainer = this; g_lastObjDefId = objDefId; return g_countRet; }

class CPlayer;
class CPlayerManager { public: CPlayer& GetMainPlayer(); };
static char g_playerBuf[16];
CPlayer& CPlayerManager::GetMainPlayer() { g_lastWorld = this; return *(CPlayer*)g_playerBuf; }

struct Hero { unsigned char Give(int count, int quantity, int suppressFlag, CCharString* name); };
unsigned char Hero::Give(int count, int quantity, int suppressFlag, CCharString* name) {
    g_giveCalls++; g_lastGiveHero = this;
    g_lastGiveCount = count; g_lastGiveQty = quantity; g_lastGiveSuppress = suppressFlag;
    g_lastGiveName = name; return g_giveRet;
}
class CThingPlayerCreature : public Hero { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { g_lastPlayer = (void*)this; return (CThingPlayerCreature*)g_heroRet; }

struct Str { void ctor(const char* cstr, int len); void dtor(); };
void Str::ctor(const char* cstr, int len) { g_ctorCalls++; g_lastCtorThis = this; g_lastCtorStr = cstr; g_lastCtorLen = len; }
void Str::dtor() { g_dtorCalls++; }

/* ---- the function under test: definition linked from the row object ---- */
void __fastcall CGameScriptInterface_GiveHeroObject(void* thisptr, unsigned long /*edx*/,
                                                    int objDefId, int quantity, int suppressFlag);

/* ---- a fake "this" object with a vtable whose slot 0x1c records calls ---- */
static void __fastcall VfnRecord(void* thisp) { g_vfnCalls++; g_lastVfnThis = thisp; }

/* vtable large enough to hold slot 0x1c/4 = index 7 */
static void* g_vtbl[16];

struct FakeGSI {
    void*  vptr;      /* +0x00 : vtable pointer */
    char   pad[0x0c]; /* +0x04..0x0f */
    void*  container; /* +0x10 */
    void*  world;     /* +0x14 */
};

static ObjContainer g_container;
static char         g_world[16];
static char         g_heroBuf[0x100];   /* hero object; +0x91 is the block flag */
static char         g_runner[4];

static void resetHero(unsigned char flag91) {
    for (int i = 0; i < (int)sizeof(g_heroBuf); ++i) g_heroBuf[i] = 0;
    g_heroBuf[0x91] = (char)flag91;
}
static void resetCounters() {
    g_ctorCalls = g_dtorCalls = g_giveCalls = g_vfnCalls = 0;
    g_lastObjDefId = g_lastGiveCount = g_lastGiveQty = g_lastGiveSuppress = 0;
    g_lastCtorStr = 0; g_lastCtorLen = 0; g_lastCtorThis = 0; g_lastGiveName = 0;
    g_lastWorld = 0; g_lastPlayer = 0; g_lastContainer = 0;
}

#define CHECK(c) do { if(!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); return 1; } } while(0)

int main(void)
{
    for (int i = 0; i < 16; ++i) g_vtbl[i] = (void*)&VfnRecord;

    FakeGSI gsi;
    gsi.vptr = g_vtbl;
    gsi.container = &g_container;
    gsi.world = g_world;

    /* Path 1: count <= 0 -> early return, nothing else happens. */
    resetCounters(); resetHero(0);
    g_countRet = 0; g_heroRet = g_heroBuf; g_giveRet = 0; g_pScriptRunner = (CSuspendableProcessMicrothread*)g_runner;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 4242, 3, 0);
    CHECK(g_lastObjDefId == 4242);
    CHECK(g_lastContainer == &g_container);
    CHECK(g_lastWorld == 0);
    CHECK(g_giveCalls == 0 && g_ctorCalls == 0 && g_vfnCalls == 0);

    /* Path 2: hero == 0 -> return after world lookup. */
    resetCounters(); resetHero(0);
    g_countRet = 5; g_heroRet = 0;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 7, 2, 0);
    CHECK(g_lastWorld == (void*)g_world && g_lastPlayer == (void*)g_playerBuf);
    CHECK(g_giveCalls == 0 && g_ctorCalls == 0 && g_vfnCalls == 0);

    /* Path 3: hero blocked (flag +0x91 bit0 set) -> return before give. */
    resetCounters(); resetHero(1);
    g_countRet = 5; g_heroRet = g_heroBuf;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 7, 2, 0);
    CHECK(g_giveCalls == 0 && g_ctorCalls == 0 && g_vfnCalls == 0);

    /* Path 4: full give, suppressFlag != 0 -> give happens, NO ui refresh. */
    resetCounters(); resetHero(0);
    g_countRet = 9; g_heroRet = g_heroBuf; g_giveRet = 0; g_pScriptRunner = (CSuspendableProcessMicrothread*)g_runner;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 111, 22, 1 /*suppress*/);
    CHECK(g_ctorCalls == 1 && g_dtorCalls == 1 && g_giveCalls == 1);
    CHECK(g_lastGiveCount == 9 && g_lastGiveQty == 22 && g_lastGiveSuppress == 1);
    CHECK(g_lastGiveHero == (void*)g_heroBuf);
    CHECK(g_lastGiveName != 0 && g_lastGiveName == g_lastCtorThis);   /* the name it built is the one it gives */
    CHECK(g_lastCtorLen == -1 && g_lastCtorStr != 0 && g_lastCtorStr[0] == 0);   /* retail's pooled "" (NF413) */
    CHECK(g_vfnCalls == 0);            /* suppressed */

    /* Path 5: full give, suppress == 0, gate != 0 -> two vtable calls. */
    resetCounters(); resetHero(0);
    g_countRet = 3; g_heroRet = g_heroBuf; g_giveRet = 0; g_pScriptRunner = (CSuspendableProcessMicrothread*)g_runner;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 5, 6, 0);
    CHECK(g_giveCalls == 1 && g_lastGiveSuppress == 0);
    CHECK(g_lastCtorStr != 0 && g_lastCtorStr[0] == 0);
    CHECK(g_vfnCalls == 2);
    CHECK(g_lastVfnThis == &gsi);

    /* Path 6: suppress == 0 but gate == 0 -> give but NO ui refresh. */
    resetCounters(); resetHero(0);
    g_countRet = 3; g_heroRet = g_heroBuf; g_giveRet = 0; g_pScriptRunner = 0;
    CGameScriptInterface_GiveHeroObject(&gsi, 0, 5, 6, 0);
    CHECK(g_giveCalls == 1);
    CHECK(g_vfnCalls == 0);

    printf("FSE2_008902e0_TEST PASS\n");
    return 0;
}
