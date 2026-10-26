#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 004e2170: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CVillageDef::~CVillageDef() -- destroy the four sound-name strings and free the eight time/task vectors in reverse order, then the inlined CDefPointeeBase destructor (its vtable, then the base destructor as a tail jump)
// Leaf: ??1CVillageDef

// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2170, length 182 bytes. Called only by the deleting destructor 0x004E2154
// (slot 0 of CVillageDef's vtable 0x01241DDC); the catalog label "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip"
// is propagated.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (`pop ecx` after each free), as the landed ctor.
// Callees: ~CCharString @ 0x0099EAE0 (x4), free @ 0x00BFEA14 (x8, the vectors' storage), CBase_009fc550's destructor
//          @ 0x009FC550 (labelled "~CMemoryBlock"; it is the CResource-style unlink) as the tail jump.
// Layout: as the landed CVillageDef::CVillageDef 0x004DFF04 (retail offsets, 12-byte STLport vectors). The base chain is
// the landed CDefPointeeBase model (0x007ACAC0): the derived destructor stores no vtable (`novtable`), the inlined
// CDefPointeeBase destructor stores 0x01230BA0.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CCharString { public:
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CDefString { public:
    long TablePos;
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector() { if (_M_start) free(_M_start); }
};
struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                             // 0x009FC550 (labelled "~CMemoryBlock")
};
class CDefPointeeBase : public CBase_009fc550 { public:          // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                         // inlined: the vtable store then the base tail jump
};
struct __declspec(novtable) CVillageDef : CDefPointeeBase {   // vtable 0x01241DDC
    unsigned char       _defBase[0x24];            // +0x04 the rest of the parent def base
    CRetailVector<long> Tasks;                     // +0x28 vector<EVillageTaskType>
    CDefString          PatrolTrackName;           // +0x34
    long                DefaultStanceToHero;       // +0x38
    long                SecondsSecuritySweepLasts; // +0x3c
    long                WarehouseCrateDef;         // +0x40
    CRetailVector<long> StoryStartTimes;           // +0x44
    long                StoryDuration;             // +0x50
    CRetailVector<long> SchoolStartTimes;          // +0x54
    long                SchoolDuration;            // +0x60
    CRetailVector<long> WorkStartTimes;            // +0x64
    long                WorkDuration;              // +0x70
    CRetailVector<long> CookStartTimes;            // +0x74
    long                CookDuration;              // +0x80
    CRetailVector<long> EatStartTimes;             // +0x84
    long                EatDuration;               // +0x90
    CRetailVector<long> TavernStartTimes;          // +0x94
    long                TavernDuration;            // +0xa0
    CRetailVector<long> GamingStartTimes;          // +0xa4
    long                GamingDuration;            // +0xb0
    long                SleepAndCurfewTimes[12];   // +0xb4 .. +0xe0
    bool                EnableAutoPlacement;       // +0xe4
    bool                AllowHouseBuying;          // +0xe5
    bool                HearTownCrier;             // +0xe6
    CCharString         CurfewStartSound;          // +0xe8
    CCharString         CurfewEndSound;            // +0xec
    CCharString         WorkStartSound;            // +0xf0
    CCharString         WorkEndSound;              // +0xf4
    long                BribeAndSnoopValues[5];    // +0xf8 .. +0x108
    virtual void v00();
    ~CVillageDef();
};

CVillageDef::~CVillageDef()
{
}

// ---- stubs and driver ----
#include <new>
static char g_log[1024];
static char g_num[64];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
static bool g_armed = false;
static void* g_freed[32];
static int g_nfreed = 0;
extern "C" void test_free(void* p) { if (g_armed && g_nfreed < 32) g_freed[g_nfreed++] = p; }
static bool Freed(void* p) { for (int i = 0; i < g_nfreed; ++i) if (g_freed[i] == p) return true; return false; }
static long g_dummy[64];
#define FAKE(i) ((void*)&g_dummy[i])
CCharString::~CCharString() { std::sprintf(g_num, "S%ld", (long)(size_t)PStringData); Log(g_num); }
void CBase_009fc550::v00() {}
CBase_009fc550::~CBase_009fc550() { Log("Base"); }
void CDefPointeeBase::v00() {}
void CVillageDef::v00() {}
int main()
{
    static unsigned char buf[sizeof(CVillageDef)];
    CVillageDef* d = new (buf) CVillageDef;
    CRetailVector<long>* vecs[8] = { &d->Tasks, &d->StoryStartTimes, &d->SchoolStartTimes, &d->WorkStartTimes,
                                     &d->CookStartTimes, &d->EatStartTimes, &d->TavernStartTimes, &d->GamingStartTimes };
    for (int i = 0; i < 8; ++i) vecs[i]->_M_start = (long*)FAKE(i);
    d->GamingStartTimes._M_start = 0;                 // empty vector: no free
    d->CurfewStartSound.PStringData = (void*)1; d->CurfewEndSound.PStringData = (void*)2;
    d->WorkStartSound.PStringData = (void*)3; d->WorkEndSound.PStringData = (void*)4;
    g_armed = true;
    d->~CVillageDef();
    g_armed = false;
    bool ok = std::strcmp(g_log, "S4,S3,S2,S1,Base,") == 0 && g_nfreed == 7 && g_freed[0] == FAKE(6) && g_freed[6] == FAKE(0);
    for (int i = 0; i < 7; ++i) ok = ok && Freed(FAKE(i));
    if (!ok) { std::printf("004e2170_TEST FAIL\n"); return 1; }
    std::printf("004e2170_TEST PASS\n");
    return 0;
}