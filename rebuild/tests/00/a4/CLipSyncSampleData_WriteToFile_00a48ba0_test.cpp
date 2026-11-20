// Behaviour test for 0x00A48BA0 CLipSyncSampleData::WriteToFile (links the REAL row object: the STLport map it builds -- header
// node, operator[] / insert_unique COMDATs, the in-place iterator increment, the erase walk -- and the shared float-to-long helper
// run as compiled, on the CRT's malloc / free).
// Fixtures (raw native layout, every offset read by an instruction of the row): CLipSyncSampleData FrameData begin / end +0 / +4
//   (12-byte CLipSyncFrameData = the phoneme vector's begin / end / end-of-storage), FramesPerSecond +0xC (0x00A48D5D);
//   CLipSyncPhoneme (12 bytes) Symbol +0, ID +4, Weight +8.
// Stubs (the row's out-of-line callees): CCharString default / copy constructor, destructor, operator= (0x0099E4B0 / 0x0099EC30 /
//   0x0099EAE0 / 0x0099EFB0) on a reference-counted record with a live-object count; CDataOutputStream::WriteUBYTE 0x00993E50 /
//   WriteULONG 0x00993EA0 / WriteNullTerminatedString 0x00995AE0 recording the stream; STLport's _Rb_global<bool>::_Rebalance (the
//   library routine the map's insert calls, written here from the algorithm).
// Reference model (independent): the symbol table = the LAST symbol seen for every distinct id, in frame then phoneme order, written
//   in ascending UNSIGNED id order (count, then the id's low byte and the string); the frame rate truncated toward zero to an
//   unsigned long; the frame count; per frame the phoneme count's low byte, then per phoneme the id's low byte and the weight byte =
//   Weight * 255 as a float, rounded to nearest (ties to even), limited to 0..255 (out of range / NaN store the integer indefinite,
//   which limits to 0).
// Checks: (1) hand-written cases (empty sample, empty frames, duplicate ids with different symbols, ids past 255 and past 2^31,
//   a 300-phoneme frame, every rounding tie and both limits, awkward frame rates); (2) 3000 seeded random samples; in each the whole
//   stream is compared with the model, the sample is compared byte for byte, and every string record's reference count and the
//   live CCharString count must be back at their starting values (so every map node and temporary was destroyed exactly once).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned long UL;
struct Rep { int refs; int id; };
static int g_live;
class CCharString
{
public:
    Rep* p;
    CCharString();
    CCharString(const CCharString& other);
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
};
CCharString::CCharString() { p = 0; ++g_live; }
CCharString::CCharString(const CCharString& other) { p = other.p; if (p) ++p->refs; ++g_live; }
CCharString::~CCharString() { if (p) --p->refs; p = 0; --g_live; }
const CCharString& CCharString::operator=(const CCharString& other) { if (other.p) ++other.p->refs; if (p) --p->refs; p = other.p; return *this; }

enum { K_UBYTE = 1, K_ULONG = 2, K_STRING = 3 };
struct Ev { int kind; UL value; };
enum { MAXEV = 6000 };
static Ev g_got[MAXEV], g_want[MAXEV]; static int g_ngot, g_nwant;
static void Put(Ev* list, int& n, int kind, UL value) { if (n < MAXEV) { list[n].kind = kind; list[n].value = value; } ++n; }
class CDataOutputStream
{
public:
    UL marker;
    void WriteUBYTE(unsigned char value);
    void WriteULONG(unsigned long value);
    void WriteNullTerminatedString(const CCharString& text);
};
static CDataOutputStream g_os;
static int g_bad_this;
void CDataOutputStream::WriteUBYTE(unsigned char value) { if (this != &g_os) ++g_bad_this; Put(g_got, g_ngot, K_UBYTE, value); }
void CDataOutputStream::WriteULONG(unsigned long value) { if (this != &g_os) ++g_bad_this; Put(g_got, g_ngot, K_ULONG, value); }
void CDataOutputStream::WriteNullTerminatedString(const CCharString& text) { if (this != &g_os) ++g_bad_this; Put(g_got, g_ngot, K_STRING, text.p ? (UL)text.p->id : 0xFFFFFFFF); }

// STLport 4.6 _tree.c _Rb_global<bool>::_Rebalance (red = false, black = true), the one library routine the row's object imports.
namespace std
{
    struct _Rb_tree_node_base { bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
    template<class Dummy> class _Rb_global
    {
    public:
        static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);
    };
}
static void RotateLeft(std::_Rb_tree_node_base* x, std::_Rb_tree_node_base*& root)
{
    std::_Rb_tree_node_base* y = x->_M_right;
    x->_M_right = y->_M_left;
    if (y->_M_left) y->_M_left->_M_parent = x;
    y->_M_parent = x->_M_parent;
    if (x == root) root = y;
    else if (x == x->_M_parent->_M_left) x->_M_parent->_M_left = y;
    else x->_M_parent->_M_right = y;
    y->_M_left = x; x->_M_parent = y;
}
static void RotateRight(std::_Rb_tree_node_base* x, std::_Rb_tree_node_base*& root)
{
    std::_Rb_tree_node_base* y = x->_M_left;
    x->_M_left = y->_M_right;
    if (y->_M_right) y->_M_right->_M_parent = x;
    y->_M_parent = x->_M_parent;
    if (x == root) root = y;
    else if (x == x->_M_parent->_M_right) x->_M_parent->_M_right = y;
    else x->_M_parent->_M_left = y;
    y->_M_right = x; x->_M_parent = y;
}
static int g_rebalances;
template<class Dummy> void __cdecl std::_Rb_global<Dummy>::_Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    ++g_rebalances;
    x->_M_color = false;
    while (x != root && x->_M_parent->_M_color == false)
    {
        if (x->_M_parent == x->_M_parent->_M_parent->_M_left)
        {
            _Rb_tree_node_base* y = x->_M_parent->_M_parent->_M_right;
            if (y && y->_M_color == false)
            {
                x->_M_parent->_M_color = true; y->_M_color = true; x->_M_parent->_M_parent->_M_color = false; x = x->_M_parent->_M_parent;
            }
            else
            {
                if (x == x->_M_parent->_M_right) { x = x->_M_parent; RotateLeft(x, root); }
                x->_M_parent->_M_color = true; x->_M_parent->_M_parent->_M_color = false; RotateRight(x->_M_parent->_M_parent, root);
            }
        }
        else
        {
            _Rb_tree_node_base* y = x->_M_parent->_M_parent->_M_left;
            if (y && y->_M_color == false)
            {
                x->_M_parent->_M_color = true; y->_M_color = true; x->_M_parent->_M_parent->_M_color = false; x = x->_M_parent->_M_parent;
            }
            else
            {
                if (x == x->_M_parent->_M_left) { x = x->_M_parent; RotateRight(x, root); }
                x->_M_parent->_M_color = true; x->_M_parent->_M_parent->_M_color = false; RotateLeft(x->_M_parent->_M_parent, root);
            }
        }
    }
    root->_M_color = true;
}
template class std::_Rb_global<bool>;

// ---- the sample fixture
struct Phoneme { CCharString Symbol; UL ID; float Weight; };
struct Frame { Phoneme* b; Phoneme* e; Phoneme* c; };
struct Sample { Frame* b; Frame* e; Frame* c; float fps; };
class CLipSyncSampleData { public: void WriteToFile(CDataOutputStream& os) const; };

enum { MAXFRAMES = 12, MAXPH = 320, NREPS = 24 };
static Rep g_reps[NREPS];
static unsigned char g_phmem[MAXFRAMES][MAXPH * 12 + 16];
static Frame g_frames[MAXFRAMES + 1];
static Sample g_sample;
static int g_counts[MAXFRAMES];
static int g_nframes;
static int g_fail, g_cases;

static void Reset()
{
    for (int i = 0; i < NREPS; ++i) { g_reps[i].refs = 0; g_reps[i].id = 1000 + i; }
    g_nframes = 0; g_sample.b = g_frames; g_sample.e = g_frames; g_sample.c = g_frames + MAXFRAMES; g_sample.fps = 30.0f;
    g_live = 0;
}
static void AddFrame()
{
    Frame& f = g_frames[g_nframes]; f.b = (Phoneme*)g_phmem[g_nframes]; f.e = f.b; f.c = f.b + MAXPH;
    g_counts[g_nframes] = 0; ++g_nframes; g_sample.e = g_frames + g_nframes;
}
static void AddPhoneme(int rep, UL id, float weight)                 // rep < 0: a null string
{
    Frame& f = g_frames[g_nframes - 1];
    Phoneme* p = f.e++;
    p->Symbol.p = rep < 0 ? 0 : &g_reps[rep]; if (rep >= 0) ++g_reps[rep].refs;
    p->ID = id; p->Weight = weight; ++g_counts[g_nframes - 1];
}

// round to nearest, ties to even, of a float that already holds Weight * 255; out of range / NaN = the integer indefinite
static long Nearest(float f)
{
    double d = f;
    if (!(d > -2147483648.5 && d < 2147483647.5)) return (long)0x80000000;
    double fl = floor(d); double frac = d - fl; long n = (long)fl;
    if (frac > 0.5) return n + 1;
    if (frac < 0.5) return n;
    return (n & 1) ? n + 1 : n;
}
static UL TruncToULong(float f)
{
    double d = f; double t = d < 0 ? ceil(d) : floor(d);
    if (t < 0) return (UL)(0 - (UL)(0 - t));
    return (UL)t;
}
static void Model()
{
    static UL ids[MAXFRAMES * MAXPH]; static int syms[MAXFRAMES * MAXPH]; int n = 0, i, k;
    g_nwant = 0;
    for (i = 0; i < g_nframes; ++i)
        for (Phoneme* p = g_frames[i].b; p != g_frames[i].e; ++p)
        {
            int sym = p->Symbol.p ? p->Symbol.p->id : -1;
            for (k = 0; k < n; ++k) if (ids[k] == p->ID) break;
            if (k == n) { ids[n] = p->ID; ++n; }
            syms[k] = sym;
        }
    for (i = 1; i < n; ++i)                                           // insertion sort, unsigned ids
        for (k = i; k > 0 && ids[k - 1] > ids[k]; --k)
        {
            UL t = ids[k]; ids[k] = ids[k - 1]; ids[k - 1] = t; int s = syms[k]; syms[k] = syms[k - 1]; syms[k - 1] = s;
        }
    Put(g_want, g_nwant, K_ULONG, (UL)n);
    for (i = 0; i < n; ++i) { Put(g_want, g_nwant, K_UBYTE, ids[i] & 0xFF); Put(g_want, g_nwant, K_STRING, syms[i] < 0 ? 0xFFFFFFFF : (UL)syms[i]); }
    Put(g_want, g_nwant, K_ULONG, TruncToULong(g_sample.fps));
    Put(g_want, g_nwant, K_ULONG, (UL)g_nframes);
    for (i = 0; i < g_nframes; ++i)
    {
        Put(g_want, g_nwant, K_UBYTE, (UL)g_counts[i] & 0xFF);
        for (Phoneme* p = g_frames[i].b; p != g_frames[i].e; ++p)
        {
            Put(g_want, g_nwant, K_UBYTE, p->ID & 0xFF);
            volatile float product = p->Weight * 255.0f;
            long w = Nearest(product);
            if (w < 0) w = 0; else if (w > 255) w = 255;
            Put(g_want, g_nwant, K_UBYTE, (UL)w);
        }
    }
}
static void Fail(const char* what, long got, long want)
{
    if (g_fail++ < 16) printf("FAIL case %d: %s got 0x%lX want 0x%lX\n", g_cases, what, got, want);
}
static void Run()
{
    ++g_cases;
    static unsigned char ph0[sizeof g_phmem]; static Frame fr0[MAXFRAMES + 1]; int refs0[NREPS]; int i;
    memcpy(ph0, g_phmem, sizeof ph0); memcpy(fr0, g_frames, sizeof fr0); Sample s0 = g_sample;
    for (i = 0; i < NREPS; ++i) refs0[i] = g_reps[i].refs;
    Model();
    g_ngot = 0; g_bad_this = 0; g_live = 0;
    ((const CLipSyncSampleData*)&g_sample)->WriteToFile(g_os);
    if (g_bad_this) Fail("stream object", g_bad_this, 0);
    if (g_ngot != g_nwant) Fail("stream length", g_ngot, g_nwant);
    for (i = 0; i < g_ngot && i < g_nwant && i < MAXEV; ++i)
        if (g_got[i].kind != g_want[i].kind || g_got[i].value != g_want[i].value)
        {
            Fail("stream item", (long)g_got[i].value, (long)g_want[i].value); printf("  (item %d kind %d / %d)\n", i, g_got[i].kind, g_want[i].kind); break;
        }
    if (memcmp(ph0, g_phmem, sizeof ph0) || memcmp(fr0, g_frames, sizeof fr0) || memcmp(&s0, &g_sample, sizeof s0)) Fail("sample written", 0, 0);
    for (i = 0; i < NREPS; ++i) if (g_reps[i].refs != refs0[i]) { Fail("string reference count", g_reps[i].refs, refs0[i]); break; }
    if (g_live != 0) Fail("live CCharString objects", g_live, 0);
}

static UL g_seed = 4242;
static UL Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }

static void Exact()
{
    int i;
    Reset(); Run();                                                   // no frames: 0, rate, 0
    Reset(); AddFrame(); Run();                                       // one empty frame
    Reset(); AddFrame(); AddFrame(); AddFrame(); g_sample.fps = 25.0f; Run();
    Reset(); AddFrame(); AddPhoneme(0, 7, 0.5f); Run();               // 127.5 -> 128 (tie to even)
    Reset(); AddFrame(); AddPhoneme(0, 7, 0.25f); AddPhoneme(1, 3, 1.0f); AddFrame(); AddPhoneme(2, 7, 0.0f); AddPhoneme(3, 9, 0.75f); Run();   // id 7 twice: the last symbol
    Reset(); AddFrame(); AddPhoneme(2, 7, 0.1f); AddPhoneme(0, 7, 0.2f); AddPhoneme(1, 7, 0.3f); Run();
    Reset(); AddFrame(); AddPhoneme(-1, 1, 0.4f); AddPhoneme(4, 2, 0.6f); Run();                                                               // a null symbol
    Reset(); AddFrame(); AddPhoneme(0, 300, 0.5f); AddPhoneme(1, 44, 0.5f); AddPhoneme(2, 0x80000001, 0.5f); AddPhoneme(3, 0xFFFFFFFF, 0.5f); AddPhoneme(4, 0, 0.5f); Run();   // unsigned order, low byte written
    Reset(); AddFrame(); AddPhoneme(0, 256, 0.5f); AddPhoneme(1, 0, 0.5f); AddPhoneme(2, 512, 0.5f); Run();                                    // distinct ids, equal low bytes
    // rounding: k + 0.5 over 255 for even and odd k, both limits, out of range, NaN
    static const float W[] = { 0.5f / 255.0f, 1.5f / 255.0f, 2.5f / 255.0f, 3.5f / 255.0f, 0.4999f / 255.0f, 254.5f / 255.0f, 255.4f / 255.0f, 255.6f / 255.0f,
                               1.0f, 1.002f, 2.0f, 1000.0f, -0.001f, -0.002f, -0.0019f, -1.0f, -1000.0f, 1.0e20f, -1.0e20f, 0.0f, 1.0f / 3.0f, 2.0f / 3.0f };
    Reset(); AddFrame();
    for (i = 0; i < (int)(sizeof W / sizeof W[0]); ++i) AddPhoneme(i % 5, 10 + i, W[i]);
    Run();
    Reset(); AddFrame();
    { UL nan = 0x7FC00000; float f; memcpy(&f, &nan, 4); AddPhoneme(0, 1, f); UL inf = 0x7F800000; memcpy(&f, &inf, 4); AddPhoneme(1, 2, f); inf = 0xFF800000; memcpy(&f, &inf, 4); AddPhoneme(2, 3, f); }
    Run();
    Reset(); AddFrame(); AddFrame();                                  // 300 phonemes in one frame: the count byte wraps, the loop does not
    for (i = 0; i < 300; ++i) AddPhoneme(i % NREPS, (UL)(i * 7 % 290), (float)(i % 256) / 255.0f);
    AddFrame(); AddPhoneme(5, 1, 0.5f);
    Run();
    Reset(); AddFrame();                                              // 256 phonemes: count byte 0
    for (i = 0; i < 256; ++i) AddPhoneme(i % NREPS, (UL)i, (float)i / 255.0f);
    Run();
    static const float R[] = { 0.0f, 0.99f, 1.0f, 29.97f, 30.0f, 59.94f, 1.0e6f, 2147483648.0f, 4294967040.0f, -0.5f, -1.5f, -2.0f };
    for (i = 0; i < (int)(sizeof R / sizeof R[0]); ++i) { Reset(); AddFrame(); AddPhoneme(0, 1, 0.5f); g_sample.fps = R[i]; Run(); }
    Reset();                                                          // ascending, descending and zig-zag insertion orders (tree shapes)
    AddFrame(); for (i = 0; i < 40; ++i) AddPhoneme(i % NREPS, (UL)i, 0.5f);
    AddFrame(); for (i = 0; i < 40; ++i) AddPhoneme(i % NREPS, (UL)(200 - i), 0.5f);
    AddFrame(); for (i = 0; i < 40; ++i) AddPhoneme(i % NREPS, (UL)((i & 1) ? 100 + i : 100 - i), 0.5f);
    Run();
}

int main()
{
    g_os.marker = 0x0517EA11;
    Exact();
    int exact = g_cases;
    for (int n = 0; n < 3000; ++n)
    {
        Reset();
        int frames = Rnd() % (MAXFRAMES + 1); UL idrange = (Rnd() & 1) ? 6 : 64;
        for (int f = 0; f < frames; ++f)
        {
            AddFrame();
            int count = (Rnd() % 5 == 0) ? 0 : 1 + Rnd() % 12;
            for (int k = 0; k < count; ++k)
            {
                UL id = Rnd() % idrange; if (Rnd() % 13 == 0) id |= 0x80000000 >> (Rnd() % 24);
                float weight = (Rnd() % 9 == 0) ? ((float)(Rnd() % 512) - 128.0f + 0.5f) / 255.0f : (float)(Rnd() & 0xFFFF) / 65535.0f;
                AddPhoneme((Rnd() % 11 == 0) ? -1 : (int)(Rnd() % NREPS), id, weight);
            }
        }
        g_sample.fps = (Rnd() & 1) ? (float)(Rnd() % 120) : (float)(Rnd() & 0xFFFF) / 256.0f;
        Run();
    }
    if (g_rebalances == 0) { printf("00a48ba0_TEST FAIL (the map never inserted)\n"); return 1; }
    if (g_fail) { printf("00a48ba0_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("00a48ba0_TEST PASS (%d cases, %d hand-written)\n", g_cases, exact);
    return 0;
}