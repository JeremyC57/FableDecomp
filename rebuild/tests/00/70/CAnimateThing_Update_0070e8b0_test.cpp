// Behaviour test for 0x0070E8B0 CAnimateThing::Update (links the REAL row object: the row body plus the TU's COMDATs it carries --
// the STLport list<CAnimSet> clear / insert / _M_insert_dispatch, list<CDelayedAnimation>::insert, vector<float>::_M_insert_overflow
// / _M_clear; the inline SetAlpha, IsValid, GFLimit, erase and pop_front are expanded in the row).
// Fixtures (native layout): CAnimSet 0x54 (Anims vector<short> +4, InternalAlphas vector<8-byte> +0x10, CurrentPercentageComplete
// +0x1C, CurrentAlpha +0x2C, LastAlpha +0x30, ChangeAlphaBy +0x38, bit byte +0x51 with HadUpdate = 8); CAnimateThing 0xBC (main set
// +0, TransitionAnimSets +0x54, CombinationAnims +0x58, DelayedAnimQueue +0x60, DelayedAnim.CombinationAnims +0x64, ChangeAlphaBy
// +0x8C, CurrentAnimRefreshStamp +0x98, ParentThing +0xA0 -> thing whose graphic appearance pointer is at +0x64). STLport lists:
// a header node, nodes {next, prev, value}. Every out-of-line callee is a recording stub; malloc / free are the CRT's.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@12=?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z")
#pragma comment(linker, "/alternatename:?UpdateCombinationAnimList@CAnimateThing@@AAEXAAV?$list@VCAnimSet@@V?$allocator@VCAnimSet@@@std@@@std@@ABVCAnimateThingUpdate@@@Z=@StubUCAL@16")
#pragma comment(linker, "/alternatename:?Update@CAnimSet@@AAEXABVCAnimateThingUpdate@@@Z=@StubSetUpdate@12")
#pragma comment(linker, "/alternatename:?UpdateInternalAlphas@CAnimSet@@AAEXXZ=@StubUIA@8")
#pragma comment(linker, "/alternatename:?PreloadAnims@CAnimateThing@@AAEXABVCCompleteAnimation@1@@Z=@StubPreload@12")
#pragma comment(linker, "/alternatename:?SetAsChanged@CTCGraphicAppearance@NGraphicAppearance@@QAEXK@Z=@StubSetAsChanged@12")
#pragma comment(linker, "/alternatename:??0CAnimSet@@QAE@ABV0@@Z=@StubCopySet@12")
#pragma comment(linker, "/alternatename:??3@YAXPAX@Z=_StubDelete")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

extern const float DELTA_NOTIONAL_ZERO;
const float DELTA_NOTIONAL_ZERO = 0.0001f;

struct BA { float c, l; };
struct Set {
    long mode; short* as; short* af; short* ae; BA* is; BA* ifn; BA* ie;
    float pct, lastpct, speed; unsigned long frame; float cur, last, lastupd, change;
    void* animator; unsigned long flags, tflags; long delay, prio; unsigned char type, bits; unsigned char pad[2];
};
struct Node { Node* next; Node* prev; Set v; };
struct QNode { QNode* next; QNode* prev; Node* list; };
struct Upd { float t, d, p; };
struct Thing { unsigned char pad[0x64]; void* app; };
struct Animator {
    Set main; Node* trans; Node* comb; unsigned long canOut; QNode* queue; Node* delayed;
    unsigned char states[0x20]; float scale, change; long delay; unsigned long inter; unsigned long stamp, lastStamp; Thing* parent;
    unsigned char rest[0xbc - 0xa4];
};
typedef char SizeSet[sizeof(Set) == 0x54 ? 1 : -1];
typedef char SizeAnim[sizeof(Animator) == 0xbc ? 1 : -1];

// call log
static char g_kind[64]; static void* g_who[64]; static Upd g_upd[64]; static int g_n = 0;
static void Log(char k, void* w, const Upd* u) { if (g_n < 64) { g_kind[g_n] = k; g_who[g_n] = w; if (u) g_upd[g_n] = *u; g_n++; } }
static Animator* g_self; static const Upd* g_arg; static int g_copies = 0;
extern "C" void __fastcall StubUCAL(Animator* self, void*, Node** list, const Upd* u) { CHECK(self == g_self && u == g_arg); Log('L', list, u); }
extern "C" void __fastcall StubSetUpdate(Set* self, void*, const Upd* u)
{
    Log('U', self, u);
    if (self == &g_self->main) { CHECK(u == g_arg); self->pct = 0.75f; }   // the main set advances before the transition update is built
}
extern "C" void __fastcall StubUIA(Set* self, void*) { CHECK(!(self->bits & 8)); Log('I', self, 0); }
extern "C" void __fastcall StubPreload(Animator* self, void*, const void* anim) { CHECK(self == g_self && anim == (void*)self); Log('P', self, 0); }
extern "C" void __fastcall StubSetAsChanged(void* self, void*, unsigned long flags) { CHECK(flags == 2); Log('C', self, 0); }
extern "C" Set* __fastcall StubCopySet(Set* self, void*, const Set* src) { memcpy(self, src, sizeof(Set)); self->as = self->af = self->ae = 0; self->is = self->ifn = self->ie = 0; ++g_copies; Log('K', (void*)src, 0); return self; }
extern "C" void __cdecl StubDelete(void* p) { free(p); }
extern "C" void __fastcall CallRow(Animator* self, void* edx, const Upd* u);

static Node* NewList() { Node* h = (Node*)malloc(sizeof(Node)); memset(h, 0, sizeof(Node)); h->next = h->prev = h; return h; }
static Node* Append(Node* h, int nAnims, const short* anims, int nAlphas, float cur, float change, bool had)
{
    Node* n = (Node*)malloc(sizeof(Node)); memset(n, 0, sizeof(Node));
    if (nAnims) { n->v.as = (short*)malloc(nAnims * 2); memcpy(n->v.as, anims, nAnims * 2); n->v.af = n->v.ae = n->v.as + nAnims; }
    if (nAlphas) { n->v.is = (BA*)malloc(nAlphas * 8); memset(n->v.is, 0, nAlphas * 8); n->v.ifn = n->v.ie = n->v.is + nAlphas; }
    n->v.cur = cur; n->v.last = -9.0f; n->v.change = change; n->v.bits = had ? 8 : 0;
    n->next = h; n->prev = h->prev; h->prev->next = n; h->prev = n; return n;
}
static int Count(Node* h) { int c = 0; for (Node* n = h->next; n != h; n = n->next) ++c; return c; }
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

static Animator g_a; static Thing g_thing; static int g_app;
static void Setup(float mainAlpha, float change)
{
    memset(&g_a, 0, sizeof g_a); memset(&g_thing, 0, sizeof g_thing);
    g_a.main.cur = mainAlpha; g_a.main.last = -7.0f; g_a.change = change; g_a.stamp = 41; g_a.parent = &g_thing; g_thing.app = &g_app;
    g_a.trans = NewList(); g_a.comb = NewList(); g_a.delayed = NewList();
    QNode* qh = (QNode*)malloc(sizeof(QNode)); qh->next = qh->prev = qh; qh->list = 0; g_a.queue = qh;
    for (int i = 0; i < 2; ++i) {                                   // two queued delayed animations, each with one combination set
        QNode* q = (QNode*)malloc(sizeof(QNode)); q->list = NewList(); short a = (short)(10 + i); Append(q->list, 1, &a, 1, 0.5f, 0, false);
        q->next = qh; q->prev = qh->prev; qh->prev->next = q; qh->prev = q;
    }
    short d[2] = { 7, 8 }; Append(g_a.delayed, 2, d, 2, 0.25f, 0, true); Append(g_a.delayed, 1, d, 1, 0.5f, 0, false);
}
static void Run(const Upd& u)
{
    ++g_run; g_n = 0; g_copies = 0; g_self = &g_a; g_arg = &u;
    QNode* second = g_a.queue->next->next;
    CallRow(&g_a, 0, &u);
    // fixed calls: the two combination lists, then the main set
    CHECK(g_n >= 3 && g_kind[0] == 'L' && g_who[0] == (void*)&g_a.comb && g_kind[1] == 'L' && g_who[1] == (void*)&g_a.delayed);
    CHECK(g_kind[2] == 'U' && g_who[2] == (void*)&g_a.main);
    // the queue rolled: old second at the front, a copy of DelayedAnim (both sets, in order) at the back, and nothing else
    CHECK(g_a.queue->next == second && g_a.queue->next->next == g_a.queue->prev && g_a.queue->prev->next == g_a.queue);
    Node* cpy = g_a.queue->prev->list; CHECK(cpy != g_a.delayed && Count(cpy) == 2 && g_copies == 2);
    CHECK(cpy->next->v.cur == 0.25f && cpy->next->next->v.cur == 0.5f);
    CHECK(g_a.stamp == 42);
    CHECK(g_n >= 3 && g_kind[g_n - 2] == 'P' && g_kind[g_n - 1] == 'C' && g_who[g_n - 1] == (void*)&g_app);
    CHECK(g_n >= 4 && g_kind[g_n - 4] == 'K' && g_who[g_n - 4] == (void*)&g_a.delayed->next->v);   // copies in list order
    CHECK(g_kind[g_n - 3] == 'K' && g_who[g_n - 3] == (void*)&g_a.delayed->next->next->v);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    Upd u = { 0.5f, 2.0f, 0.1f };
    // run 1: main 0.5 + 0.2 = 0.7; seven transition sets, four dropped
    Setup(0.5f, 0.2f);
    short a1[1] = { 5 }, a2[2] = { 3, 4 }, bad2[2] = { 3, 0 }, bad1[1] = { 0 };
    Node* t1 = Append(g_a.trans, 1, a1, 1, 0.6f, 0.1f, false);      // kept, first update: weight 0.5
    Append(g_a.trans, 1, a1, 1, 0.00005f, 0.0f, false);             // alpha <= DELTA_NOTIONAL_ZERO: dropped
    Node* t3 = Append(g_a.trans, 2, a2, 2, 0.3f, 0.5f, true);       // kept, already updated: weight -0.2 -> 0
    Append(g_a.trans, 2, bad2, 2, 0.8f, 0.0f, false);                // second index not positive: dropped
    Append(g_a.trans, 1, bad1, 1, 0.8f, 0.0f, false);                // first index not positive: dropped
    Append(g_a.trans, 1, a1, 2, 0.8f, 0.0f, false);                  // index / internal alpha count mismatch: dropped
    Node* t7 = Append(g_a.trans, 1, a1, 1, 1.0f, -0.5f, false);     // weight 1.5 -> 1
    Run(u);
    CHECK(Near(g_a.main.cur, 0.7f) && g_a.main.last == 0.5f);
    CHECK(Count(g_a.trans) == 3 && g_a.trans->next == t1 && t1->next == t3 && t3->next == t7);
    // total 1.5, multiplier (1 - 0.7) / 1.5 = 0.2
    CHECK(Near(t1->v.cur, 0.1f) && Near(t1->v.last, 0.6f));
    CHECK(t3->v.cur == 0.0f && Near(t3->v.last, 0.3f));
    CHECK(Near(t7->v.cur, 0.2f) && t7->v.last == 1.0f);
    CHECK((t1->v.bits & 8) && (t3->v.bits & 8) && (t7->v.bits & 8));
    // order: U t1, I t1, U t3, U t7, I t7, each with {time, distance, the main set's updated percentage}
    {
        int k = 3; const char* want = "UIUUI"; Node* who[5] = { t1, t1, t3, t7, t7 };
        for (int i = 0; i < 5; ++i, ++k) {
            CHECK(g_kind[k] == want[i] && g_who[k] == (void*)&who[i]->v);
            if (g_kind[k] == 'U') CHECK(g_upd[k].t == 0.5f && g_upd[k].d == 2.0f && g_upd[k].p == 0.75f);
        }
        CHECK(g_kind[k] == 'K');
    }
    // run 2: main 0.9 + 0.5 clamps to 1; one tiny weight: total <= DELTA_NOTIONAL_ZERO so the multiplier is 1
    Setup(0.9f, 0.5f);
    Node* s1 = Append(g_a.trans, 1, a1, 1, 0.00015f, 0.0001f, true);   // weight 0.00005
    Node* s2 = Append(g_a.trans, 1, a1, 1, 0.5f, 0.6f, true);          // weight -0.1 -> 0
    Run(u);
    CHECK(g_a.main.cur == 1.0f && g_a.main.last == 0.9f);
    CHECK(Count(g_a.trans) == 2 && Near(s1->v.cur, 0.00005f) && s2->v.cur == 0.0f && s2->v.last == 0.5f);
    CHECK(g_kind[3] == 'U' && g_kind[4] == 'U' && g_kind[5] == 'K');   // HadUpdate set: no UpdateInternalAlphas
    // run 3: main 0.1 - 0.5 clamps to 0 (the remaining alpha is 1, not 1.4); no transitions
    Setup(0.1f, -0.5f);
    Run(u);
    CHECK(g_a.main.cur == 0.0f && Near(g_a.main.last, 0.1f) && Count(g_a.trans) == 0 && g_kind[3] == 'K');
    Setup(0.1f, -0.5f);
    Node* q1 = Append(g_a.trans, 1, a1, 1, 0.5f, 0.0f, true);
    Node* q2 = Append(g_a.trans, 1, a1, 1, 0.5f, 0.0f, true);
    Run(u);
    CHECK(g_a.main.cur == 0.0f && Near(q1->v.cur, 0.5f) && Near(q2->v.cur, 0.5f));
    // run 5: main 0 and one transition of weight 0.5: multiplier 2, SetAlpha clamps the product to 1
    Setup(0.0f, 0.0f);
    Node* r1 = Append(g_a.trans, 1, a1, 1, 0.5f, 0.0f, true);
    Run(u);
    CHECK(r1->v.cur == 1.0f && r1->v.last == 0.5f);
    // run 4: a heavy pair: weights 0.6 + 0.6, main 0.4: multiplier 0.5
    Setup(0.4f, 0.0f);
    Node* p1 = Append(g_a.trans, 1, a1, 1, 0.6f, 0.0f, false);
    Node* p2 = Append(g_a.trans, 2, a2, 2, 0.7f, 0.1f, false);
    Run(u);
    CHECK(Near(p1->v.cur, 0.3f) && Near(p2->v.cur, 0.3f) && Near(p2->v.last, 0.7f));
    CHECK(g_kind[3] == 'U' && g_kind[4] == 'I' && g_kind[5] == 'U' && g_kind[6] == 'I' && g_who[6] == (void*)&p2->v);
    if (g_fail) { printf("0070e8b0_TEST FAIL\n"); return 1; }
    printf("0070e8b0_TEST PASS\n");
    return 0;
}