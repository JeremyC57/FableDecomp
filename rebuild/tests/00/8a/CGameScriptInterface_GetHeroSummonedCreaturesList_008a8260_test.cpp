// Behaviour test for 0x008A8260 CGameScriptInterface::GetHeroSummonedCreaturesList (links the REAL row object: the row body and the
// COMDATs it carries -- the TC-map lower bound (retail 0x004365B0), the vector<CIntelligentPointer<CThingAICreature>> copy
// constructor (retail 0x008AC540), the CIntelligentPointer copy / destructors -- run as compiled).
// The row's out-of-line callees are thiscall doubles (thiscall-compatible __fastcall functions bound to the row's mangled names with
// /alternatename): CPlayerManager::GetMainPlayer, CPlayer::GetPControlledCreature, CBaseIntelligentPointer ctor / dtor / GetPItem /
// SetPItem, CThing::GetScriptThing (hidden return pointer), CScriptThing::~CScriptThing, vector<CScriptThing>::push_back.
// Fixtures are raw native-layout images, each offset read by an instruction of the row:
//   interface +0x14 PlayerManager (0x008A8263);
//   hero / minion thing: +0x30 TC presence word (bit 17 = TC 0x91 summon spell, 0x008A8291; bit 26 = TC 0x9A ghost sword, 0x008A8346),
//     +0x44 TC map vector {begin, end, end_of_storage} of {type, CTCBase*} pairs sorted by type (0x008A82A3 / 0x008A82B6),
//     +0x91 bit 0 dead flag (0x008A8284, 0x008A8303, 0x008A83B5);
//   summon-spell TC: its CIntelligentPointer<CThingAICreature> minion at +0x11 (0x008A82D3), item word at +4 of it (the double);
//   ghost-sword TC: vector<CIntelligentPointer<CThingAICreature>> at +0x10 (0x008A837F), 8-byte elements {vptr, item}.
// Reference model: the hero must be non-null and alive; then the summon minion (TC bit and map entry present) if non-null and alive,
// then each ghost-sword minion in order if non-null and alive, each pushed as its script thing; result = any pushed. Also checked:
// every CBaseIntelligentPointer constructed is destroyed, one ~CScriptThing per push, the copied minion vector's storage
// allocated once (non-empty list) and freed (malloc / free counted over the process heap).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace std { template<class T> class allocator; template<class T, class A> class vector; }
class CScriptThing; class CGameScriptInterface {
public:
    virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing, std::allocator<CScriptThing> >& thing_list) const;
};

#pragma comment(linker, "/alternatename:?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ=@GetMainPlayer@8")
#pragma comment(linker, "/alternatename:?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ=@GetPControlledCreature@8")
#pragma comment(linker, "/alternatename:??0CBaseIntelligentPointer@@QAE@XZ=@IPCtor@8")
#pragma comment(linker, "/alternatename:??1CBaseIntelligentPointer@@UAE@XZ=@IPDtor@8")
#pragma comment(linker, "/alternatename:?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ=@IPGet@8")
#pragma comment(linker, "/alternatename:?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z=@IPSet@12")
#pragma comment(linker, "/alternatename:?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ=@GetScriptThing@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@ScriptThingDtor@8")
#pragma comment(linker, "/alternatename:?push_back@?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAEXABVCScriptThing@@@Z=@PushBack@12")

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }
template<class T> static T get(const void* base, int off) { T v; memcpy(&v, (const unsigned char*)base + off, sizeof v); return v; }

static unsigned char iface[0x40], manager[0x10], player[0x10];
static void* heroPtr;
static bool badRecv;
static int ipLive, ipCtors, scriptDtors, deletes;
static void* pushed[64]; static int npushed; static void* listObj;

extern "C" {
void* __fastcall GetMainPlayer(void* self, void*) { if (self != manager) badRecv = true; return player; }
void* __fastcall GetPControlledCreature(void* self, void*) { if (self != player) badRecv = true; return heroPtr; }
void __fastcall IPCtor(void* self, void*) { put<void*>(self, 4, 0); ++ipLive; ++ipCtors; }
void __fastcall IPDtor(void* self, void*) { --ipLive; put<void*>(self, 4, (void*)0xDEAD); }
void* __fastcall IPGet(void* self, void*) { return get<void*>(self, 4); }
void __fastcall IPSet(void* self, void*, void* item) { put<void*>(self, 4, item); }
void* __fastcall GetScriptThing(void* self, void*, void* ret) { put<void*>(ret, 0, 0); put<void*>(ret, 4, self); put<void*>(ret, 8, 0); return ret; }
void __fastcall ScriptThingDtor(void*, void*) { ++scriptDtors; }
void __fastcall PushBack(void* self, void*, const void* item) { if (self != listObj) badRecv = true; if (npushed < 64) pushed[npushed] = get<void*>(item, 4); ++npushed; }
}
// malloc / free: the row's vector copy allocates with malloc and releases with free (retail 0x00BFEA14 `free`); counted here over
// the process heap so the release is observable (the counters run only inside Run's call).
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap(void);
extern "C" __declspec(dllimport) void* __stdcall HeapAlloc(void*, unsigned long, unsigned long);
extern "C" __declspec(dllimport) int __stdcall HeapFree(void*, unsigned long, void*);
static bool counting; static int mallocs; static void* lastAlloc; static void* freedPtr;
extern "C" void* __cdecl malloc(size_t n) { void* p = HeapAlloc(GetProcessHeap(), 0, (unsigned long)(n ? n : 1)); if (counting) { ++mallocs; lastAlloc = p; } return p; }
extern "C" void __cdecl free(void* p) { if (!p) return; if (counting) { ++deletes; freedPtr = p; } HeapFree(GetProcessHeap(), 0, p); }
void __cdecl operator delete(void* p) { if (p) { if (counting) ++deletes; HeapFree(GetProcessHeap(), 0, p); } }

enum { NT = 12 };
static unsigned char things[NT][0x100];
static unsigned char summonTC[0x40], ghostTC[0x40];
static unsigned char ghostElems[8][8];
static long mapEntries[4][2];

struct Case { int heroIdx; bool heroDead; bool sumBit, sumEntry; int sumMinion; bool gstBit, gstEntry; int ngst; int gst[8]; };

static void SetupThing(int i, bool dead) { memset(things[i], 0, 0x100); things[i][0x91] = dead ? 1 : 0; things[i][0x92] = 0xFE; }
static bool alive[NT];

static void Run(const Case& c, int k)
{
    for (int i = 0; i < NT; ++i) SetupThing(i, !alive[i]);
    put<void*>(iface, 0x14, manager);
    heroPtr = c.heroIdx < 0 ? 0 : things[c.heroIdx];
    void* hero = heroPtr;
    if (hero) {
        unsigned char* h = (unsigned char*)hero;
        h[0x91] = c.heroDead ? 1 : 0;
        unsigned long bits = 0xFFFFFFFFul & ~(0x20000ul | 0x4000000ul);   // other TCs present: only the two bits decide
        if (c.sumBit) bits |= 0x20000; if (c.gstBit) bits |= 0x4000000;
        put(h, 0x30, bits);
        int n = 0;
        mapEntries[n][0] = 0x10; mapEntries[n][1] = (long)(size_t)summonTC; ++n;   // a lower type pointing at the wrong TC
        if (c.sumEntry) { mapEntries[n][0] = 0x91; mapEntries[n][1] = (long)(size_t)summonTC; ++n; }
        if (c.gstEntry) { mapEntries[n][0] = 0x9A; mapEntries[n][1] = (long)(size_t)ghostTC; ++n; }
        mapEntries[n][0] = 0xC0; mapEntries[n][1] = (long)(size_t)ghostTC; ++n;     // a higher type
        put<void*>(h, 0x44, mapEntries); put<void*>(h, 0x48, mapEntries + n); put<void*>(h, 0x4C, mapEntries + 4);
    }
    memset(summonTC, 0x5A, sizeof summonTC);
    put<void*>(summonTC, 0x11, (void*)0x1234); put<void*>(summonTC, 0x15, c.sumMinion < 0 ? 0 : things[c.sumMinion]);
    memset(ghostTC, 0x6B, sizeof ghostTC);
    for (int g = 0; g < c.ngst; ++g) { put<void*>(ghostElems[g], 0, (void*)0x5678); put<void*>(ghostElems[g], 4, c.gst[g] < 0 ? 0 : things[c.gst[g]]); }
    put<void*>(ghostTC, 0x10, ghostElems); put<void*>(ghostTC, 0x14, ghostElems + c.ngst); put<void*>(ghostTC, 0x18, ghostElems + 8);
    // expectation
    void* want[16]; int nwant = 0;
    if (hero && !c.heroDead) {
        if (c.sumBit && c.sumMinion >= 0 && alive[c.sumMinion]) want[nwant++] = things[c.sumMinion];
        if (c.gstBit) for (int g = 0; g < c.ngst; ++g) if (c.gst[g] >= 0 && alive[c.gst[g]]) want[nwant++] = things[c.gst[g]];
    }
    static unsigned char listImage[16]; listObj = listImage;
    npushed = 0; ipLive = 0; ipCtors = 0; scriptDtors = 0; deletes = 0; mallocs = 0; lastAlloc = freedPtr = 0; badRecv = false;
    unsigned char ghostBefore[sizeof ghostElems]; memcpy(ghostBefore, ghostElems, sizeof ghostElems);
    counting = true;
    bool r = ((CGameScriptInterface*)(void*)iface)->CGameScriptInterface::GetHeroSummonedCreaturesList(
        *(std::vector<CScriptThing, std::allocator<CScriptThing> >*)(void*)listImage);
    counting = false;
    check(!badRecv, "receivers", k);
    check(r == (nwant > 0), "result = any pushed", k);
    bool same = npushed == nwant;
    for (int i = 0; same && i < nwant; ++i) same = pushed[i] == want[i];
    check(same, "pushed exactly the live minions, summon first then ghost-sword order", k);
    check(scriptDtors == npushed, "one ~CScriptThing per push", k);
    check(ipLive == 0, "every intelligent pointer destroyed", k);
    check(memcmp(ghostBefore, ghostElems, sizeof ghostElems) == 0, "ghost-sword list untouched (copied)", k);
    bool copied = hero && !c.heroDead && c.gstBit;
    check(mallocs == ((copied && c.ngst > 0) ? 1 : 0) && deletes == mallocs && freedPtr == lastAlloc, "copied minion storage allocated and freed once", k);
    check(ipCtors == (hero && !c.heroDead ? (c.sumBit ? 1 : 0) + (copied ? c.ngst : 0) : 0), "intelligent pointers made: summon copy + one per copied minion", k);
}

int main()
{
    for (int i = 0; i < NT; ++i) alive[i] = (i % 4) != 3;   // things 3, 7, 11 dead
    int k = 0;
    Case base = { 0, false, true, true, 1, true, true, 3, { 2, 3, 4 } };
    Case c;
    c = base; Run(c, ++k);                                   // summon 1, ghosts 2 (alive), 3 (dead), 4
    c = base; c.heroIdx = -1; Run(c, ++k);                   // no hero
    c = base; c.heroDead = true; Run(c, ++k);                // dead hero
    c = base; c.sumBit = false; Run(c, ++k);                 // no summon TC (entry present but bit clear)
    c = base; c.gstBit = false; Run(c, ++k);                 // no ghost-sword TC
    c = base; c.sumBit = c.gstBit = false; Run(c, ++k);      // neither: false
    c = base; c.sumMinion = -1; Run(c, ++k);                 // summon TC with no minion
    c = base; c.sumMinion = 7; Run(c, ++k);                  // dead summon minion
    c = base; c.ngst = 0; Run(c, ++k);                       // empty ghost list
    c = base; c.sumMinion = -1; c.ngst = 2; c.gst[0] = -1; c.gst[1] = 11; Run(c, ++k);   // nothing live: false
    c = base; c.sumMinion = -1; c.ngst = 1; c.gst[0] = 5; Run(c, ++k);                   // only a ghost minion: true
    c = base; c.sumBit = false; c.sumEntry = false; c.ngst = 5; c.gst[0] = 5; c.gst[1] = -1; c.gst[2] = 6; c.gst[3] = 7; c.gst[4] = 8; Run(c, ++k);
    c = base; c.gstBit = false; c.gstEntry = false; Run(c, ++k);
    c = base; c.heroIdx = 9; Run(c, ++k);                    // another hero image
    c = base; c.heroIdx = 11; Run(c, ++k);                   // a hero whose dead flag is set by the fixture (thing 11 dead) -> overwritten false
    unsigned long seed = 12345;
    for (int r = 0; r < 2000; ++r) {
        seed = seed * 1103515245ul + 12345ul; unsigned long b = seed >> 8;
        c.heroIdx = (b & 7) == 0 ? -1 : 0; c.heroDead = (b & 0x38) == 0;
        c.sumBit = (b >> 6) & 1; c.sumEntry = c.sumBit || ((b >> 7) & 1);
        c.gstBit = (b >> 8) & 1; c.gstEntry = c.gstBit || ((b >> 9) & 1);
        seed = seed * 1103515245ul + 12345ul; unsigned long b2 = seed >> 8;
        c.sumMinion = (int)(b2 % (NT + 1)) - 1; if (c.sumMinion == 0) c.sumMinion = 1;
        c.ngst = (int)((b2 >> 5) % 9);
        for (int g = 0; g < 8; ++g) { seed = seed * 1103515245ul + 12345ul; int t = (int)((seed >> 8) % (NT + 1)) - 1; c.gst[g] = t == 0 ? 2 : t; }
        Run(c, 1000 + r);
    }
    if (failures) { printf("008a8260_TEST FAIL %d\n", failures); return 1; }
    printf("008a8260_TEST PASS (%d targeted, 2000 random)\n", k);
    return 0;
}