// Behaviour test for 004887c0: src/004887c0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CPlayer::SetupInitialPlayerMode()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayer::SetupInitialPlayerMode() -- add the player's initial mode (1 for the main player, 0x18 otherwise), keeping a front-end mode 0x1b on top if one was active, then make this player the master of every co-op spirit in the world
// Leaf: ?SetupInitialPlayerMode@CPlayer_Methods@@QAEXXZ
// (retail ?SetupInitialPlayerMode@CPlayer@@QAEXXZ; defined through the CPlayer_Methods carrier)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004887C0, length 251 bytes, plain `ret`.
// Callees: CPlayerManager::GetMainPlayerID 0x00449700 (returns +0x1c; propagated label "CWorld::DrawGetEnvironment"),
// CPlayer::GetCurrentMode 0x00633BE0, RemoveMode(EPlayerMode) 0x00634190, AddMode(EPlayerMode, bool) 0x006345C0,
// CThingSearchTools::GetAllThings<...> 0x0048D400 (World +0x20; the interface type 0xc7 by reference), CVectorMap<
// ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CPlayerManager::GetMainPlayer 0x00449970, CTCCoopSpirit::SetMaster
// 0x0066FA50 (stores +0x24; propagated label "CObservable::AddExclusiveObserver"), free 0x00BFEA14 (the vector).
// Layout: engine/CPlayer.h (PlayerManager +0x10, World +0x1C, Number +0x28) through the CPlayer_Methods carrier; CThing
// TC mask +0x20 (0xc7 = word 6 bit 7, the sign bit of byte +0x38), Components +0x44.
// Shape: `long number = Number;` read before the call and `(number != id) ? 0x18 : 1` for the sete/dec/and/inc select; a local
// STLport vector<CThing*> (zeros through ebx) with the count cached before the loop; GetAllThings has a same-TU stand-in
// that reads and rewrites the vector words by value, so the vector does not escape (start kept in edi, zero stores live); the spirit is the uninitialised
// `T* tc; if (HasTC) tc = GetTC;` local (stale value = the search key's frame word).
#include "engine/CPlayer.h"

typedef long EPlayerMode;
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

struct CTCBase;
struct CTCEntry {
    long     Type;
    CTCBase* TC;
};
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type);   // 0x0040F020
    __forceinline CTCBase* Find(long type)
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) { return Components.Find(type); }
    template <class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            *ppOut = (T*)GetTC(type);
            return true;
        }
        return false;
    }
};

template <class T> struct vector {                     // STLport vector (12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};

struct CThingSearchTools {
    void GetAllThings(const long& interfaceType, vector<CThing*>& things);   // 0x0048D400
};
CThing** SearchFirst_0048d400();
bool SearchThings_0048d400(CThingSearchTools* tools, const long* interfaceType, CThing** first, CThing** last,
                           CThing** end);
CThing** SearchLast_0048d400();
CThing** SearchEnd_0048d400();
// Same-TU stand-in for the CThingSearchTools::GetAllThings instantiation 0x0048D400 (not a reconstruction of it): the
// interface type goes by address, but the result vector's three words are rewritten by value, so VC7.1 knows the vector
// does not escape and keeps its start in a register (the 0x00726BE0 idiom); the words are read first, so the vector's
// zero stores stay live.
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::GetAllThings(const long& interfaceType, vector<CThing*>& things)
{
    SearchThings_0048d400(this, &interfaceType, things._M_start, things._M_finish, things._M_end_of_storage);
    things._M_start = SearchFirst_0048d400();
    things._M_finish = SearchLast_0048d400();
    things._M_end_of_storage = SearchEnd_0048d400();
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;              // +0x20
};
struct CPlayerManager_Retail {
    long GetMainPlayerID() const;                      // 0x00449700
    CPlayer* GetMainPlayer();                          // 0x00449970
};
struct CTCCoopSpirit {                                 // TC type 0xc7
    void SetMaster(CPlayer* master);                   // 0x0066FA50
};

struct CPlayer_Methods : CPlayer {
    EPlayerMode GetCurrentMode();                      // 0x00633BE0
    void RemoveMode(EPlayerMode mode);                 // 0x00634190
    void AddMode(EPlayerMode mode, bool b);            // 0x006345C0
    void SetupInitialPlayerMode();
};

void CPlayer_Methods::SetupInitialPlayerMode()
{
    long number = Number;
    EPlayerMode mode = (number != ((CPlayerManager_Retail*)PlayerManager)->GetMainPlayerID()) ? 0x18 : 1;
    bool frontEnd = false;
    if (GetCurrentMode() == 0x1b) {
        frontEnd = true;
        RemoveMode(0x1b);
    }
    AddMode(mode, true);
    if (frontEnd)
        AddMode(0x1b, false);
    vector<CThing*> spirits;
    ((CWorld_Retail*)World)->PThingSearchTools->GetAllThings(0xc7, spirits);
    unsigned long count = spirits.size();
    for (unsigned long i = 0; i < count; ++i) {
        CTCCoopSpirit* spirit;
        spirits[i]->GetInterface(&spirit, 0xc7);
        spirit->SetMaster(((CPlayerManager_Retail*)PlayerManager)->GetMainPlayer());
    }
}

// ---- stubs ----
static long g_mainId; static long g_curMode;
static int g_log[32]; static long g_arg[32][2]; static int g_n = 0;
static void Log(int k, long a, long b) { if (g_n < 32) { g_log[g_n] = k; g_arg[g_n][0] = a; g_arg[g_n][1] = b; } ++g_n; }
long CPlayerManager_Retail::GetMainPlayerID() const { return g_mainId; }
static CPlayer* g_mainPlayer;
CPlayer* CPlayerManager_Retail::GetMainPlayer() { return g_mainPlayer; }
EPlayerMode CPlayer_Methods::GetCurrentMode() { return g_curMode; }
void CPlayer_Methods::RemoveMode(EPlayerMode mode) { Log(1, mode, 0); }
void CPlayer_Methods::AddMode(EPlayerMode mode, bool b) { Log(2, mode, b); }
static CThing* g_things[3]; static int g_thingCount = 0; static CThing** g_buf;
static long g_searchType; static CThingSearchTools* g_searchThis; static CThing** g_inWords[3];
bool SearchThings_0048d400(CThingSearchTools* tools, const long* interfaceType, CThing** first, CThing** last, CThing** end)
{
    g_searchThis = tools; g_searchType = *interfaceType; g_inWords[0] = first; g_inWords[1] = last; g_inWords[2] = end;
    g_buf = (CThing**)malloc(4 * sizeof(CThing*));
    for (int i = 0; i < g_thingCount; ++i) g_buf[i] = g_things[i];
    return g_thingCount != 0;
}
CThing** SearchFirst_0048d400() { return g_buf; }
CThing** SearchLast_0048d400() { return g_buf + g_thingCount; }
CThing** SearchEnd_0048d400() { return g_buf + 4; }
CTCEntry* CTCMap::LowerBound(const unsigned long& type)
{
    CTCEntry* it = _M_start;
    while (it != _M_finish && (unsigned long)it->Type < type) ++it;
    return it;
}
static void* g_masterThis[4]; static CPlayer* g_masterArg[4]; static int g_masterN = 0;
void CTCCoopSpirit::SetMaster(CPlayer* master) { if (g_masterN < 4) { g_masterThis[g_masterN] = this; g_masterArg[g_masterN] = master; } ++g_masterN; }

struct SFakeThing { unsigned char raw[0x50]; CTCEntry entries[3]; };
static SFakeThing g_fake[3];
static void MakeThing(int i, CTCCoopSpirit* spirit)
{
    memset(&g_fake[i], 0, sizeof(g_fake[i]));
    CThing* t = (CThing*)&g_fake[i];
    t->UsingComponents[0xc7 >> 5] |= 1UL << (0xc7 & 31);
    g_fake[i].entries[0].Type = 0x10; g_fake[i].entries[0].TC = 0;
    g_fake[i].entries[1].Type = 0xc7; g_fake[i].entries[1].TC = (CTCBase*)spirit;
    g_fake[i].entries[2].Type = 0x0;  g_fake[i].entries[2].TC = 0;   // sentinel at _M_finish
    t->Components._M_start = &g_fake[i].entries[0]; t->Components._M_finish = &g_fake[i].entries[2];
    g_things[i] = t;
}
int main()
{
    static unsigned char raw[sizeof(CPlayer_Methods) + 16];
    memset(raw, 0, sizeof(raw));
    CPlayer_Methods* p = (CPlayer_Methods*)raw;
    static unsigned char mgr[16], world[0x40], tools[8];
    p->PlayerManager = (CPlayerManager*)mgr;
    p->World = (CWorld*)world;
    ((CWorld_Retail*)world)->PThingSearchTools = (CThingSearchTools*)tools;
    g_mainPlayer = (CPlayer*)raw;
    static unsigned char spirits[3][4];
    MakeThing(0, (CTCCoopSpirit*)spirits[0]); MakeThing(1, (CTCCoopSpirit*)spirits[1]); MakeThing(2, (CTCCoopSpirit*)spirits[2]);
    // main player, not in the front end, two spirits
    p->Number = 7; g_mainId = 7; g_curMode = 3; g_thingCount = 2; g_n = 0; g_masterN = 0;
    p->SetupInitialPlayerMode();
    CHECK(g_n == 1 && g_log[0] == 2 && g_arg[0][0] == 1 && g_arg[0][1] == 1);
    CHECK(g_searchThis == (CThingSearchTools*)tools && g_searchType == 0xc7);
    CHECK(g_inWords[0] == 0 && g_inWords[1] == 0 && g_inWords[2] == 0);
    CHECK(g_masterN == 2 && g_masterThis[0] == spirits[0] && g_masterThis[1] == spirits[1]);
    CHECK(g_masterArg[0] == g_mainPlayer && g_masterArg[1] == g_mainPlayer);
    // second player in the front end: remove 0x1b, add 0x18, re-add 0x1b (false), three spirits
    p->Number = 2; g_curMode = 0x1b; g_thingCount = 3; g_n = 0; g_masterN = 0;
    p->SetupInitialPlayerMode();
    CHECK(g_n == 3 && g_log[0] == 1 && g_arg[0][0] == 0x1b);
    CHECK(g_log[1] == 2 && g_arg[1][0] == 0x18 && g_arg[1][1] == 1);
    CHECK(g_log[2] == 2 && g_arg[2][0] == 0x1b && g_arg[2][1] == 0);
    CHECK(g_masterN == 3 && g_masterThis[2] == spirits[2]);
    // no spirits at all
    g_curMode = 0; g_thingCount = 0; g_n = 0; g_masterN = 0;
    p->SetupInitialPlayerMode();
    CHECK(g_n == 1 && g_arg[0][0] == 0x18 && g_masterN == 0);
    char out[64];
    _snprintf(out, sizeof(out), "004887c0_TEST %s", fails ? "FAIL" : "PASS");
    printf("%s\n", out);
    return fails ? 1 : 0;
}