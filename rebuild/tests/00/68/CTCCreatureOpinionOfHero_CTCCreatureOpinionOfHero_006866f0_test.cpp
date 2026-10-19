#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
#include <new>
#include <cstdlib>
#define free TestFree   // route the model's free() into the test spy
// CTCCreatureOpinionOfHero::~CTCCreatureOpinionOfHero() -- destructor: the unknown-threat intelligent pointer, the transient offsets, the 92 known-deed lists, the pending deeds, the relative-opinion and three mask vectors, the two def pointers, then CTCBase
// Leaf: ??1CTCCreatureOpinionOfHero
// Retail: Fable.exe (TLC, VC7.1) @ 0x006866F0, 222 bytes; called only from the deleting destructor 0x006866D0 (vtable
//   0x0125BC3C slot 0 = CTCCreatureOpinionOfHero, rtti-vtables.tsv).
// Label correction: the catalog's "CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase" is propagated; the
//   vtable that reaches this body is CTCCreatureOpinionOfHero's.
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (PUnknownThreatOpinionSource), the out-of-line list dtors 0x00450DAB
//   (TransientOffsets) and 0x0042AB73 (PendingDeeds; both size-optimised copies whose manifest label "OnReadFinished" is
//   propagated), free 0x00BFEA14, the def pointees' slot 1 (+4, destroy), ~CTCBase 0x00686830 (tail jump).
// Layout (retail; Ego_r.pdb / engine/CTCCreatureOpinionOfHero.h agree up to +0x24, then the retail containers are 12-byte
//   vectors and 4-byte lists): PDef +0x1c, PPersonalityDef +0x20, AttitudeMask +0x24, ReactionMask +0x30, DeedMask +0x3c,
//   RelativeOpinion (COpinionSet, one POD vector) +0x48, PendingDeeds +0x54, LastFrameAppliedDeedEffect long[92] +0x58,
//   KnownDeeds list<long>[92] +0x1c8, TransientOffsets +0x338, ForcedAttitude +0x33c, four flags +0x340,
//   PUnknownThreatOpinionSource +0x344, FrameLastUpdatedUnknownThreat +0x34c (0x350 bytes).
// Shape: the class is `__declspec(novtable)` here (no vtable store); members die in reverse order; the KnownDeeds array is
//   the inlined last-to-first counted loop over inline STLport list dtors (`clear()` walking `tmp = cur; cur = cur->next;
//   free(tmp)` with the head re-read, then the sentinel reset and freed); the POD vectors free their block; the def pointers
//   release through a local copy (`dec [ecx+4] ; jne ; call [vt+4]`) and clear the member.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

struct SLongListNode {
    SLongListNode* _M_next;
    SLongListNode* _M_prev;
    long           _M_data;
};
struct CLongList {                                     // STLport list<long> (retail 4 bytes: the sentinel node)
    SLongListNode* _M_node;
    void clear()
    {
        SLongListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SLongListNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~CLongList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct CPendingDeedList {                              // list<long> (out-of-line copy)
    void* _M_node;
    ~CPendingDeedList();                               // 0x0042AB73
};
struct CTransientOffsetList {                          // list<COpinionTransientOffset>
    void* _M_node;
    ~CTransientOffsetList();                           // 0x00450DAB
};

template <class T> struct SPodVector {                 // STLport vector of a POD element (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~SPodVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() = 0;                            // slot 1 (+0x04)
    long RefCount;                                     // +0x04
};
struct CDefPointer {
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

struct CBaseIntelligentPointer {                       // CIntelligentPointer<CThing>, 8 bytes
    void* _words[2];
    ~CBaseIntelligentPointer();                        // 0x00A01C10
};

struct COpinionSet {                                   // retail 12 bytes: one POD vector
    SPodVector<float> Opinions;
};

struct CTCBase {
    virtual ~CTCBase();                                // 0x00686830
    unsigned char _body[8];
};

struct __declspec(novtable) CTCCreatureOpinionOfHero : CTCBase {   // vtable 0x0125BC3C
    long                    LastOpinionReactionFrame;  // +0x0c
    float                   NumberOfTimesHit;          // +0x10
    float                   ToleranceToBeingHitOverride;   // +0x14
    long                    FrameToDecayNumberOfTimesHit;  // +0x18
    CDefPointer             PDef;                      // +0x1c
    CDefPointer             PPersonalityDef;           // +0x20
    SPodVector<unsigned long> AttitudeMask;            // +0x24 (vector<bool> words)
    SPodVector<unsigned long> ReactionMask;            // +0x30
    SPodVector<unsigned long> DeedMask;                // +0x3c
    COpinionSet             RelativeOpinion;           // +0x48
    CPendingDeedList        PendingDeeds;              // +0x54
    long                    LastFrameAppliedDeedEffect[92];   // +0x58
    CLongList               KnownDeeds[92];            // +0x1c8
    CTransientOffsetList    TransientOffsets;          // +0x338
    long                    ForcedAttitude;            // +0x33c
    bool                    InteractedFlag;            // +0x340
    bool                    GreetedFlag;               // +0x341
    bool                    HeroOpinionEnemy;          // +0x342
    bool                    InFindAGuardAndYellForHelp;   // +0x343
    CBaseIntelligentPointer PUnknownThreatOpinionSource;  // +0x344
    long                    FrameLastUpdatedUnknownThreat;   // +0x34c
    ~CTCCreatureOpinionOfHero();
};

CTCCreatureOpinionOfHero::~CTCCreatureOpinionOfHero()
{
}

// ---- test doubles and driver ----

static int g_frees = 0; static void* g_lastFree = 0; static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
extern "C" void __cdecl TestFree(void* block) { ++g_frees; g_lastFree = block; }
CPendingDeedList::~CPendingDeedList() { Log(2); }
CTransientOffsetList::~CTransientOffsetList() { Log(3); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { Log(4); }
CTCBase::~CTCBase() { Log(5); }
struct TestDef : CDefPointeeBase { int id; virtual void Destroy() { Log(10 + id); } };
int main()
{
    static SLongListNode sentinels[92]; static SLongListNode extra[3];
    static double raw[256];
    CTCCreatureOpinionOfHero* o = new (raw) CTCCreatureOpinionOfHero;
    for (int i = 0; i < 92; ++i) { sentinels[i]._M_next = sentinels[i]._M_prev = &sentinels[i]; o->KnownDeeds[i]._M_node = &sentinels[i]; }
    // list 5 holds three nodes
    SLongListNode* h = &sentinels[5];
    h->_M_next = &extra[0]; extra[0]._M_next = &extra[1]; extra[1]._M_next = &extra[2]; extra[2]._M_next = h;
    static unsigned long maskWords[3]; static float opinions[2];
    o->AttitudeMask._M_start = &maskWords[0]; o->ReactionMask._M_start = 0; o->DeedMask._M_start = &maskWords[2];
    o->RelativeOpinion.Opinions._M_start = opinions;
    TestDef def1, def2; def1.id = 1; def1.RefCount = 1; def2.id = 2; def2.RefCount = 2;
    o->PDef.Object = &def1; o->PPersonalityDef.Object = &def2;
    o->CTCCreatureOpinionOfHero::~CTCCreatureOpinionOfHero();   // qualified: the class is novtable under VC
    // frees: 3 extra nodes + 92 sentinels + attitude + deed + opinions = 98; def1 destroyed, def2 decremented
    static const int want[] = { 4, 3, 2, 11, 5 };
    bool ok = g_frees == 98 && g_n == 5 && def1.RefCount == 0 && def2.RefCount == 1 && o->PDef.Object == 0 && o->PPersonalityDef.Object == 0
        && sentinels[5]._M_next == &sentinels[5] && sentinels[5]._M_prev == &sentinels[5] && g_lastFree == &maskWords[0];
    for (int i = 0; ok && i < 5; ++i) ok = g_log[i] == want[i];
    if (!ok) { std::printf("006866f0_TEST FAIL %d %d\n", g_frees, g_n); return 1; }
    std::printf("006866f0_TEST PASS\n");
    return 0;
}