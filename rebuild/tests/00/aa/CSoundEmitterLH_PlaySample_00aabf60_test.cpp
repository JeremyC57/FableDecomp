// Behaviour test for 0x00AABF60 CSoundEmitterLH::PlaySample (links the REAL row object; its same-TU COMDATs -- the counted pointer,
// list push_back / node creation, DeleteData -- run as compiled).
// Fixtures (raw native layout, every offset read by an instruction of the row): emitter PManager +0x04 (0x00AABF69), Position +0x08
// (0x00AAC094), ActiveSounds +0x14 (0x00AAC19B), Positional +0x18 (0x00AABFD6), Playing +0x19 (0x00AAC1B1), EmitterID +0x1C
// (0x00AAC0B7); manager SoundEnabled +0x24 (0x00AABF77), sound system +0x40 (0x00AABF6C), head vector +0x74 (0x00AAC072); bank's
// engine bank +0x10 (0x00AAC0C5); CSoundInit Pitch +0x04, NoPlays +0x0C, GainFaderBits +0x14, PitchFaderBits +0x18, OverrideID +0x1C.
// The out-of-line callees are provided here with retail's semantics: LHA::CEventPlayOptions ctor 0x00C0A790 (every default it stores),
// SetLoopCount 0x00C0A860 (WhichParams |= 0x20, +0x48), SetInterruptable 0x00C0A880 (|= 0x80, +0x54), SetFlag16 0x00C0A8A0 / SetFlag
// 0x00C0A8C0 (through 0x00C0AE70: an index >= 16 / 8 is ignored, else the bit goes into the touched word and the value word); the
// interface slots (system slot 6, engine bank slot 4, event manager slots 14 / 16) are __fastcall functions with an unused edx.
// Every callee logs an event; an independent reference model predicts the event list, every argument and receiver, the full
// 0x5C-byte options image at each play call, the return value, Playing and the tracked-sound list (node contents and counts).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class CASoundBank;
class CSoundInit;
class CSoundEmitterLH {
public:
    virtual unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);
    class CActiveSound {
    public:
        long rec[9];                                   // 0x24 bytes, as the row allocates
        CActiveSound(long sampleID, CASoundBank& bank, long sample, float startTime, bool is3D, bool headRelative);
    };
};
class CSampleQueryLH {
public:
    long rec[5];                                       // 0x14 bytes
    CSampleQueryLH(void* bank, long group, long sample);
    ~CSampleQueryLH();
};
namespace LHA {
class CEventPlayOptions {
public:
    unsigned char raw[0x5C];
    CEventPlayOptions();
    void SetLoopCount(int loopCount);
    void SetInterruptable(bool interruptable);
    void SetFlag16(unsigned long index, bool set);
    void SetFlag(unsigned long index, bool set);
};
}

// ---- event log
struct Ev { char t; long a, b, c, d; float f; unsigned char opt[0x5C]; };
static Ev g_log[64]; static int g_n;
static Ev& Log(char t, long a = 0, long b = 0, long c = 0, long d = 0)
{
    static Ev junk;
    if (g_n >= 64) return junk;
    Ev& e = g_log[g_n++]; memset(&e, 0, sizeof e); e.t = t; e.a = a; e.b = b; e.c = c; e.d = d; return e;
}

// ---- retail-semantics option helpers (shared by the stubs and the model)
static void W32(unsigned char* p, int off, unsigned long v) { memcpy(p + off, &v, 4); }
static void WF(unsigned char* p, int off, float v) { memcpy(p + off, &v, 4); }
static void W16or(unsigned char* p, int off, unsigned short v) { unsigned short x; memcpy(&x, p + off, 2); x |= v; memcpy(p + off, &x, 2); }
static void W16and(unsigned char* p, int off, unsigned short v) { unsigned short x; memcpy(&x, p + off, 2); x &= v; memcpy(p + off, &x, 2); }
static void OptDefaults(unsigned char* p)          // 0x00C0A790
{
    memset(p, 0xCD, 0x5C);                         // padding bytes the ctor leaves alone stay as they were
    W32(p, 0x00, 0xFFFFFFFF); W32(p, 0x04, 0); p[0x10] = 0; WF(p, 0x08, 1.0f); WF(p, 0x0C, 1.0f);
    W32(p, 0x14, 0); W32(p, 0x18, 0); W32(p, 0x1C, 0); W32(p, 0x20, 0); p[0x24] = 0;
    memset(p + 0x26, 0, 8);
    WF(p, 0x30, 1.0f); WF(p, 0x34, 1.0f); W32(p, 0x38, 0); WF(p, 0x3C, 1.0f); WF(p, 0x40, 20.0f); W32(p, 0x44, 1);
    W32(p, 0x48, 0); memset(p + 0x4C, 0, 4); W32(p, 0x50, 0); p[0x54] = 1; WF(p, 0x58, 1.0f);
}
static void MaskBit(unsigned char* p, unsigned long index, bool set, int maskOff, int touchedOff, unsigned long count)   // 0x00C0AE70
{
    if (index >= count) return;
    unsigned short b = (unsigned short)(1ul << (index % 32));
    W16or(p, touchedOff, b);
    if (set) W16or(p, maskOff, b); else W16and(p, maskOff, (unsigned short)~b);
}

LHA::CEventPlayOptions::CEventPlayOptions() { OptDefaults(raw); Log('C', (long)this); }
void LHA::CEventPlayOptions::SetLoopCount(int n) { raw[0x26] |= 0x20; W32(raw, 0x48, (unsigned long)n); Log('L', (long)this, n); }
void LHA::CEventPlayOptions::SetInterruptable(bool v) { raw[0x26] |= 0x80; raw[0x54] = v; Log('I', (long)this, v); }
void LHA::CEventPlayOptions::SetFlag16(unsigned long i, bool v) { MaskBit(raw, i, v, 0x4C, 0x2A, 16); Log('G', (long)this, (long)i, v); }
void LHA::CEventPlayOptions::SetFlag(unsigned long i, bool v) { MaskBit(raw, i, v, 0x4E, 0x2C, 8); Log('P', (long)this, (long)i, v); }

static const long QMAGIC = 0x51515151;
static int g_liveQueries;
CSampleQueryLH::CSampleQueryLH(void* bank, long group, long sample)
{ rec[0] = QMAGIC; rec[1] = (long)bank; rec[2] = group; rec[3] = sample; rec[4] = 0; ++g_liveQueries; Log('Q', (long)bank, group, sample); }
CSampleQueryLH::~CSampleQueryLH() { Log('q', (long)this); rec[0] = 0; --g_liveQueries; }

static const long AMAGIC = 0x41414141;
CSoundEmitterLH::CActiveSound::CActiveSound(long id, CASoundBank& bank, long sample, float t, bool is3D, bool head)
{
    rec[0] = AMAGIC; rec[1] = id; rec[2] = (long)&bank; rec[3] = sample; memcpy(&rec[4], &t, 4); rec[5] = is3D; rec[6] = head;
    Ev& e = Log('A', id, (long)&bank, sample, is3D * 2 + head); e.f = t;
}
static double g_time = 1234.5;
double __fastcall GFGetTime() { Log('T'); return g_time; }

// ---- interface fakes
static unsigned char g_sys[4], g_evm[4], g_eng[4];
static void* g_sysVt[7]; static void* g_evmVt[17]; static void* g_engVt[5];
static long g_findRet, g_G, g_I, g_playRet;
static void* __fastcall GetEventManager(void* self, int) { Log('E', (long)self); return g_evm; }
static long __fastcall FindSample(void* self, int, long sample, long* g, long* i)
{ Log('F', (long)self, sample); *g = g_G; *i = g_I; return g_findRet; }
static long __fastcall PlayBankSample(void* self, int, void* bank, long g, long i, const unsigned char* o)
{ Ev& e = Log('B', (long)self, (long)bank, g, i); memcpy(e.opt, o, 0x5C); return g_playRet; }
static long __fastcall PlayEvent(void* self, int, const CSampleQueryLH* q, const unsigned char* o)
{
    Ev& e = Log('V', (long)self, q->rec[0] == QMAGIC ? q->rec[1] : -77, q->rec[0] == QMAGIC ? q->rec[2] : -77, q->rec[3]);
    e.f = (float)(long)q; memcpy(e.opt, o, 0x5C); return g_playRet;
}
extern "C" void __cdecl Trap() { printf("FAIL unexpected virtual slot\n"); exit(3); }

// ---- raw objects
static unsigned char g_emit[0x20], g_mgr[0x80], g_bank[0x14], g_init[0x20];
static void* g_sentinel[2];
static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

struct Cfg {
    bool hasSys, enabled; long sample; bool loop, attached; float pitch; long noPlays; unsigned long gainBits, pitchBits;
    long overrideId; bool positional; long emitterId; long findRet, G, I, playRet; bool playing0;
};
static const float HEAD[3] = { 11.5f, -2.25f, 300.0f }, POS[3] = { -7.0f, 0.125f, 42.0f };

static int ListCount() { int n = 0; for (void** p = (void**)g_sentinel[0]; p != g_sentinel && n < 1000; p = (void**)p[0]) ++n; return n; }
static void** ListLast() { return (void**)g_sentinel[1]; }

// reference model: the expected event list (receivers, arguments, option images), return value and tracking
static unsigned long Model(const Cfg& c, Ev* ev, int& n, bool& tracked, bool& playing)
{
    n = 0; tracked = false; playing = c.playing0;
    if (!c.hasSys || !c.enabled || (unsigned long)c.sample > 100000ul) return 0xFFFFFFFFul;
    unsigned char o[0x5C]; OptDefaults(o);
    Ev* e;
#define M(T) (e = &ev[n++], memset(e, 0, sizeof *e), e->t = T, e)
    M('C');
    long lc = c.noPlays == -1 ? -1 : c.noPlays - 1; o[0x26] |= 0x20; W32(o, 0x48, (unsigned long)lc); M('L')->b = lc;
    WF(o, 0x0C, c.pitch);
    bool is3D = c.loop ? false : c.positional; o[0x10] = is3D;
    o[0x24] = c.attached;
    for (unsigned long bits = c.gainBits, i = 0; bits; ++i)
        if (bits & (1ul << i)) { MaskBit(o, i, true, 0x4C, 0x2A, 16); e = M('G'); e->b = (long)i; e->c = 1; bits ^= 1ul << i; }
    if (c.pitchBits == 0x80000000ul) { e = M('P'); e->b = -1; e->c = 1; }  // the hoisted index of the exhausted gain mask: -1, ignored
    const float* pos = c.attached ? HEAD : (is3D ? POS : 0);
    if (pos) { WF(o, 0x14, pos[0]); WF(o, 0x18, pos[1]); WF(o, 0x1C, pos[2]); }
    W32(o, 0x00, (unsigned long)(c.overrideId != -1 ? c.overrideId : c.emitterId));
    e = M('F'); e->a = (long)g_eng; e->b = c.sample;
    if (c.findRet <= -1) {
        e = M('E'); e->a = (long)g_sys;
        e = M('Q'); e->a = (long)g_eng; e->b = 0; e->c = c.sample;
        e = M('V'); e->a = (long)g_evm; e->b = (long)g_eng; e->c = 0; e->d = c.sample; memcpy(e->opt, o, 0x5C);
        M('q');
    } else {
        o[0x26] |= 0x80; o[0x54] = 1; e = M('I'); e->b = 1;
        e = M('E'); e->a = (long)g_sys;
        e = M('B'); e->a = (long)g_evm; e->b = (long)g_eng; e->c = c.G; e->d = c.I; memcpy(e->opt, o, 0x5C);
    }
    long handle = c.playRet;
    if (handle >= 0) {
        tracked = true;
        M('T');
        e = M('A'); e->a = handle; e->b = (long)g_bank; e->c = c.sample; e->d = is3D * 2 + c.attached; e->f = (float)g_time;
    }
    playing = true;
#undef M
    return (unsigned long)handle;
}

static void Run(const Cfg& c, int k)
{
    memset(g_emit, 0, sizeof g_emit); memset(g_mgr, 0, sizeof g_mgr); memset(g_bank, 0, sizeof g_bank); memset(g_init, 0, sizeof g_init);
    unsigned char* mgr = g_mgr;
    *(void**)(g_emit + 0x04) = mgr; memcpy(g_emit + 0x08, POS, 12); *(void**)(g_emit + 0x14) = g_sentinel;
    g_emit[0x18] = c.positional; g_emit[0x19] = c.playing0; memcpy(g_emit + 0x1C, &c.emitterId, 4);
    mgr[0x24] = c.enabled; *(void**)(mgr + 0x40) = c.hasSys ? (void*)g_sys : 0; memcpy(mgr + 0x74, HEAD, 12);
    *(void**)(g_bank + 0x10) = g_eng;
    memcpy(g_init + 0x04, &c.pitch, 4); memcpy(g_init + 0x0C, &c.noPlays, 4); memcpy(g_init + 0x14, &c.gainBits, 4);
    memcpy(g_init + 0x18, &c.pitchBits, 4); memcpy(g_init + 0x1C, &c.overrideId, 4);
    g_findRet = c.findRet; g_G = c.G; g_I = c.I; g_playRet = c.playRet;
    int before = ListCount(); void** lastBefore = ListLast();
    g_n = 0; g_liveQueries = 0;
    unsigned long r = ((CSoundEmitterLH*)(void*)g_emit)->CSoundEmitterLH::PlaySample(*(CASoundBank*)(void*)g_bank, c.sample, c.loop,
                                                                                     *(const CSoundInit*)(void*)g_init, c.attached);
    Ev exp[64]; int n; bool tracked, playing;
    unsigned long er = Model(c, exp, n, tracked, playing);
    check(r == er, "return value", k);
    check((g_emit[0x19] != 0) == playing, "Playing", k);
    check(g_n == n, "event count", k);
    long optAddr = 0, queryAddr = 0;
    for (int i = 0; i < n && i < g_n; ++i) {
        const Ev& a = g_log[i]; const Ev& b = exp[i];
        bool ok = a.t == b.t;
        if (ok) switch (b.t) {
            case 'C': optAddr = a.a; break;
            case 'L': case 'I': case 'G': case 'P': ok = a.a == optAddr && a.b == b.b && a.c == b.c; break;
            case 'E': case 'F': ok = a.a == b.a && a.b == b.b; break;
            case 'Q': ok = a.a == b.a && a.b == b.b && a.c == b.c; break;
            case 'V': ok = a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d && memcmp(a.opt, b.opt, 0x5C) == 0; queryAddr = (long)a.f; break;
            case 'q': ok = a.a == queryAddr; break;
            case 'B': ok = a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d && memcmp(a.opt, b.opt, 0x5C) == 0; break;
            case 'T': break;
            case 'A': ok = a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d && a.f == b.f; break;
        }
        if (!ok) { check(false, "event", k); printf("  case %d event %d: got %c(%lx,%lx,%lx,%lx) want %c(%lx,%lx,%lx,%lx)\n", k, i,
                                                       a.t, a.a, a.b, a.c, a.d, b.t, b.a, b.b, b.c, b.d); break; }
    }
    check(g_liveQueries == 0, "query destroyed", k);
    // tracking: one new node at the back holding {Data = the new CActiveSound, Info = {RefCount 1, DeleteData, Data}}
    int after = ListCount();
    check(after == before + (tracked ? 1 : 0), "tracked count", k);
    if (tracked && after == before + 1) {
        void** node = ListLast();
        check(node[0] == (void*)g_sentinel && node[1] == (void*)lastBefore, "node links", k);
        CSoundEmitterLH::CActiveSound* s = (CSoundEmitterLH::CActiveSound*)node[2];
        unsigned long* info = (unsigned long*)node[3];
        check(s && s->rec[0] == AMAGIC && s->rec[1] == c.playRet, "node data", k);
        check(info && info[0] == 1 && info[2] == (unsigned long)s && info[1] != 0, "node counted pointer", k);
    } else if (!tracked) check(ListLast() == lastBefore, "list untouched", k);
}

int main()
{
    for (int i = 0; i < 7; ++i) g_sysVt[i] = (void*)Trap;
    for (int i = 0; i < 17; ++i) g_evmVt[i] = (void*)Trap;
    for (int i = 0; i < 5; ++i) g_engVt[i] = (void*)Trap;
    g_sysVt[6] = (void*)GetEventManager; g_engVt[4] = (void*)FindSample;
    g_evmVt[14] = (void*)PlayBankSample; g_evmVt[16] = (void*)PlayEvent;
    *(void**)g_sys = g_sysVt; *(void**)g_evm = g_evmVt; *(void**)g_eng = g_engVt;
    g_sentinel[0] = g_sentinel; g_sentinel[1] = g_sentinel;

    //            sys   en    sample  loop  att   pitch  noPl gain        pitchB       ovr  posl  emit find G  I  play pl0
    Cfg base = { true, true, 1234,   false, false, 0.75f, 3,   0x5,        0,           -1,  true, 77,  0,  4, 9, 15, false };
    Cfg c;
    Run(base, 1);                                                     // bank path, 3D at Position, loop count 2, gain bits 0 / 2
    c = base; c.hasSys = false; Run(c, 2);                            // no sound system: -1, nothing touched
    c = base; c.enabled = false; Run(c, 3);                           // sound disabled
    c = base; c.sample = 100000; Run(c, 4);                           // the upper bound itself plays
    c = base; c.sample = 100001; Run(c, 5);                           // one past: -1
    c = base; c.sample = -1; Run(c, 6);                               // negative = huge unsigned: -1
    c = base; c.sample = 0; c.noPlays = -1; Run(c, 7);                // loop count stays -1 (infinite)
    c = base; c.noPlays = 0; Run(c, 8);                               // loop count -1 the other way (0 - 1)
    c = base; c.noPlays = 1; c.loop = true; Run(c, 9);                // looping: never 3D, no position
    c = base; c.positional = false; Run(c, 10);                       // not positional: no position
    c = base; c.attached = true; c.loop = true; Run(c, 11);           // head relative: the manager's head vector, even when not 3D
    c = base; c.attached = true; Run(c, 12);
    c = base; c.overrideId = 0; Run(c, 13);                           // override 0 is used (only -1 means none)
    c = base; c.overrideId = -2; Run(c, 14);
    c = base; c.gainBits = 0x80018001ul; Run(c, 15);                  // bits 16 and 31 are found but ignored by SetFlag16
    c = base; c.gainBits = 0xFFFFul; Run(c, 16);
    c = base; c.gainBits = 0; c.pitchBits = 0x80000000ul; Run(c, 17); // the pitch loop: one ignored SetFlag(-1)
    c = base; c.pitchBits = 0x80000000ul; c.gainBits = 0x30; Run(c, 18);
    c = base; c.findRet = -1; c.playRet = 3; Run(c, 19);              // not in the engine bank: a sample query (group 0)
    c = base; c.findRet = -2; c.playRet = 0; Run(c, 20);              // handle 0 is tracked
    c = base; c.findRet = 0; c.playRet = 0; Run(c, 21);
    c = base; c.findRet = 7; c.playRet = -1; c.playing0 = true; Run(c, 22);   // failed play: not tracked, Playing still set
    c = base; c.findRet = -1; c.playRet = -5; Run(c, 23);
    c = base; c.pitch = -3.5f; c.G = -6; c.I = 100; c.emitterId = 0; c.overrideId = 12; c.attached = true; Run(c, 24);
    g_time = 0.1;                                                     // the start time is narrowed to float
    c = base; c.playRet = 0x7FFFFFFF; Run(c, 25);
    // random sweep
    unsigned long s = 12345;
    for (int k = 0; k < 2000; ++k) {
        s = s * 1103515245ul + 12345ul; unsigned long r = s >> 8;
        c = base;
        c.hasSys = (r & 31) != 0; c.enabled = (r & (31 << 5)) != 0; c.loop = (r >> 10) & 1; c.attached = (r >> 11) & 1;
        c.positional = (r >> 12) & 1; c.playing0 = (r >> 13) & 1;
        s = s * 1103515245ul + 12345ul; r = s >> 4;
        static const long samples[] = { 0, 1, 99999, 100000, 100001, -1, 5000, 0x7FFFFFFF };
        c.sample = samples[r & 7]; c.noPlays = (long)((r >> 3) & 7) - 2; c.overrideId = (long)((r >> 6) & 3) - 2;
        c.findRet = (long)((r >> 8) & 3) - 2; c.playRet = (long)((r >> 10) & 3) - 2; c.G = (long)(r >> 12 & 15); c.I = (long)(r >> 16 & 15);
        s = s * 1103515245ul + 12345ul; c.gainBits = (s >> 3) & 0x1FFFF; if ((s >> 25) & 1) c.gainBits |= 0x80000000ul;
        c.pitchBits = ((s >> 26) & 1) ? 0x80000000ul : 0; c.pitch = (float)(long)((s >> 20) & 63) / 8.0f; c.emitterId = (long)(s >> 27);
        g_time = (double)(k * 3) + 0.5;
        Run(c, 100 + k);
    }
    printf(failures ? "00aabf60_TEST FAIL\n" : "00aabf60_TEST PASS\n");
    return failures ? 1 : 0;
}