// Behaviour test for 0x00A48F10 CLipSyncSampleData::GetFrameInterpolate (links the REAL row object: the expanded floor helper, both
// limits, the phoneme searches and the STLport vector COMDATs -- erase, push_back, _M_insert_overflow -- run as compiled).
// Fixtures (raw native layout): CLipSyncSampleData = the frame vector (begin / end / end-of-storage, 12-byte CLipSyncFrameData) +
// FramesPerSecond +0x0C; CLipSyncFrameData = a vector of 12-byte CLipSyncPhoneme { CCharString Symbol +0, ID +4, Weight +8 }.
// Stubs (the row's out-of-line callees): CCharString copy constructor 0x0099EC30 / destructor 0x0099EAE0 / assignment (used by the
// vector's erase) keep a live count per string, CLipSyncFrameData::FindPhonemeID 0x00A48A40 (first record with the id, logged).
// Reference model (independent): pos = t * fps; index1 = floor(pos), index2 = index1 + 1, weight2 = pos - index1; each index
// limited to [0, frames - 1] (a negative index to 0); dest cleared; every phoneme of frame1 in order -- in frame2 too: weight
// w1 + (w2 - w1) * weight2 (w2 from the FIRST frame2 record with that id), else w1 * (1 - weight2); then every phoneme of frame2
// that frame1 lacks, weight w2 * weight2. Symbol and ID are copied from the frame the record came from.
// Checks: (1) exact dyadic cases (t before the first / past the last frame, on a frame, between frames, one-frame sample, empty
// frames, duplicate ids, a dest that already holds records, with and without spare capacity); (2) 6000 seeded random cases;
// (3) the sample, its frames and their records are not written; every string's live count matches the records that hold it.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static int g_live[64];
class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
    int* p;
};
CCharString::CCharString(const CCharString& other) { p = other.p; if (p) ++*p; }
CCharString::~CCharString() { if (p) --*p; p = (int*)0xDEADBEEF; }
const CCharString& CCharString::operator=(const CCharString& other) { if (other.p) ++*other.p; if (p) --*p; p = other.p; return *this; }

class CLipSyncPhoneme { public: CCharString Symbol; unsigned long ID; float Weight; };
struct RawPhoneme { int* sym; unsigned long id; float weight; };
class CLipSyncFrameData
{
public:
    const CLipSyncPhoneme FindPhonemeID(unsigned long id) const;
    RawPhoneme* first; RawPhoneme* last; RawPhoneme* eos;
};
class CLipSyncSampleData
{
public:
    void GetFrameInterpolate(CLipSyncFrameData& dest, float t) const;
    CLipSyncFrameData* first; CLipSyncFrameData* last; CLipSyncFrameData* eos; float fps;
};

static const CLipSyncFrameData* g_find_frame[64]; static unsigned long g_find_id[64]; static int g_nfind;
const CLipSyncPhoneme CLipSyncFrameData::FindPhonemeID(unsigned long id) const
{
    if (g_nfind < 64) { g_find_frame[g_nfind] = this; g_find_id[g_nfind] = id; }
    ++g_nfind;
    for (RawPhoneme* r = first; r != last; ++r)
        if (r->id == id)
            return *(const CLipSyncPhoneme*)r;                       // copy constructor: the live count goes up
    static RawPhoneme none = { 0, 0, 0.0f };
    return *(const CLipSyncPhoneme*)&none;
}

enum { NFRAMES = 6, MAXPH = 8 };
static RawPhoneme g_ph[NFRAMES][MAXPH];
static CLipSyncFrameData g_frames[NFRAMES + 1];
static CLipSyncSampleData g_sample;
static int g_fail, g_cases;
static unsigned long g_seed = 4242;
static unsigned long Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }

static void Fail(const char* what, double got, double want)
{
    if (g_fail++ < 16) printf("FAIL case %d: %s got %.9g want %.9g\n", g_cases, what, got, want);
}
struct Want { int* sym; unsigned long id; double weight; };

// count[f] phonemes in frame f are already in g_ph; dest starts with `dest_n` records in a buffer of `dest_cap`
static void Run(int nframes, const int* count, float fps, float t, int dest_n, int dest_cap, bool exact)
{
    ++g_cases;
    int f, i, k;
    memset(g_live, 0, sizeof g_live);
    for (f = 0; f < nframes; ++f)
    {
        g_frames[f].first = g_ph[f]; g_frames[f].last = g_ph[f] + count[f]; g_frames[f].eos = g_ph[f] + MAXPH;
        for (i = 0; i < count[f]; ++i) ++*g_ph[f][i].sym;
    }
    g_sample.first = g_frames; g_sample.last = g_frames + nframes; g_sample.eos = g_frames + NFRAMES; g_sample.fps = fps;
    CLipSyncFrameData dest;
    dest.first = dest_cap ? (RawPhoneme*)malloc(dest_cap * sizeof(RawPhoneme)) : 0;
    dest.last = dest.first + dest_n; dest.eos = dest.first + dest_cap;
    for (i = 0; i < dest_n; ++i) { dest.first[i].sym = &g_live[40 + i]; ++g_live[40 + i]; dest.first[i].id = 900 + i; dest.first[i].weight = 9.0f; }
    // ---- model
    float pos = (float)((double)t * fps);
    long index1 = (long)floor((double)pos), index2 = index1 + 1;
    double weight2 = (double)pos - (double)index1;
    if (index1 < 0) index1 = 0; else if (index1 > nframes - 1) index1 = nframes - 1;
    if (index2 < 0) index2 = 0; else if (index2 > nframes - 1) index2 = nframes - 1;
    Want want[2 * MAXPH]; int nwant = 0;
    const CLipSyncFrameData* wfind_frame[MAXPH]; unsigned long wfind_id[MAXPH]; int nwfind = 0;
    const RawPhoneme* a = g_ph[index1]; const RawPhoneme* b = g_ph[index2]; int na = count[index1], nb = count[index2];
    for (i = 0; i < na; ++i)
    {
        for (k = 0; k < nb; ++k) if (b[k].id == a[i].id) break;
        want[nwant].sym = a[i].sym; want[nwant].id = a[i].id;
        if (k < nb)
        {
            want[nwant].weight = (double)a[i].weight + ((double)b[k].weight - a[i].weight) * weight2;
            wfind_frame[nwfind] = &g_frames[index2]; wfind_id[nwfind++] = a[i].id;
        }
        else want[nwant].weight = (double)a[i].weight * (1.0 - weight2);
        ++nwant;
    }
    for (i = 0; i < nb; ++i)
    {
        for (k = 0; k < na; ++k) if (a[k].id == b[i].id) break;
        if (k < na) continue;
        want[nwant].sym = b[i].sym; want[nwant].id = b[i].id; want[nwant].weight = (double)b[i].weight * weight2; ++nwant;
    }
    int want_live[64]; memcpy(want_live, g_live, sizeof want_live);
    for (i = 0; i < dest_n; ++i) --want_live[40 + i];
    for (i = 0; i < nwant; ++i) ++*(want_live + (want[i].sym - g_live));
    // ---- run
    RawPhoneme ph0[NFRAMES][MAXPH]; CLipSyncFrameData frames0[NFRAMES + 1]; CLipSyncSampleData sample0;
    memcpy(ph0, g_ph, sizeof ph0); memcpy(frames0, g_frames, sizeof frames0); memcpy(&sample0, &g_sample, sizeof sample0);
    g_nfind = 0;
    g_sample.GetFrameInterpolate(dest, t);
    // ---- checks
    if (memcmp(ph0, g_ph, sizeof ph0) || memcmp(frames0, g_frames, sizeof frames0) || memcmp(&sample0, &g_sample, sizeof sample0)) Fail("sample written", 0, 0);
    int n = (int)(dest.last - dest.first);
    if (n != nwant) Fail("phoneme count", n, nwant);
    if (dest.eos - dest.first < n) Fail("capacity", (double)(dest.eos - dest.first), n);
    for (i = 0; i < n && i < nwant; ++i)
    {
        if (dest.first[i].sym != want[i].sym) Fail("Symbol", i, 0);
        if (dest.first[i].id != want[i].id) Fail("ID", dest.first[i].id, want[i].id);
        double got = dest.first[i].weight;
        if (exact ? (float)want[i].weight != dest.first[i].weight : fabs(got - want[i].weight) > 1e-6 * (1 + fabs(want[i].weight))) Fail("Weight", got, want[i].weight);
    }
    if (g_nfind != nwfind) Fail("FindPhonemeID calls", g_nfind, nwfind);
    for (i = 0; i < g_nfind && i < nwfind; ++i)
    {
        if (g_find_frame[i] != wfind_frame[i]) Fail("FindPhonemeID frame", i, 0);
        if (g_find_id[i] != wfind_id[i]) Fail("FindPhonemeID id", g_find_id[i], wfind_id[i]);
    }
    for (i = 0; i < 64; ++i) if (g_live[i] != want_live[i]) { Fail("string live count", g_live[i], want_live[i]); break; }
    free(dest.first);
}
static void SetPh(int f, int i, int sym, unsigned long id, float weight) { g_ph[f][i].sym = &g_live[sym]; g_ph[f][i].id = id; g_ph[f][i].weight = weight; }

static void Exact()
{
    // frame 0: ids 1, 2, 3; frame 1: ids 3, 4, 1; frame 2: none; frame 3: ids 5, 5 (duplicate), 2
    SetPh(0, 0, 1, 1, 0.5f); SetPh(0, 1, 2, 2, 1.0f); SetPh(0, 2, 3, 3, 0.25f);
    SetPh(1, 0, 4, 3, 0.75f); SetPh(1, 1, 5, 4, 0.5f); SetPh(1, 2, 6, 1, 0.0f);
    SetPh(3, 0, 7, 5, 0.5f); SetPh(3, 1, 8, 5, 1.0f); SetPh(3, 2, 9, 2, 0.125f);
    SetPh(4, 0, 10, 2, 1.0f); SetPh(4, 1, 11, 5, 0.25f);
    int count[NFRAMES] = { 3, 3, 0, 3, 2, 0 };
    static const float times[] = { -4.0f, -0.25f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.5f, 2.0f, 2.25f, 3.0f, 3.5f, 3.75f, 4.0f, 4.5f, 5.0f, 9.0f, 1000.0f };
    for (int ti = 0; ti < (int)(sizeof times / sizeof times[0]); ++ti)
    {
        Run(5, count, 1.0f, times[ti], 0, 0, true);                    // no storage: every push_back grows the vector
        Run(5, count, 1.0f, times[ti], 2, 16, true);                   // two old records dropped, room for everything
        Run(5, count, 1.0f, times[ti], 3, 4, true);                    // grows part of the way through
        Run(5, count, 4.0f, times[ti] * 0.25f, 1, 1, true);            // another frame rate, same positions
        Run(5, count, 0.5f, times[ti], 0, 8, true);                    // half rate: positions halve
    }
    Run(1, count, 1.0f, 0.5f, 0, 0, true);                             // one-frame sample: both indices limit to 0
    Run(1, count, 1.0f, -3.0f, 4, 8, true);
    Run(1, count, 1.0f, 7.25f, 0, 8, true);
    Run(2, count, 1.0f, 1.0f, 0, 8, true);                             // index2 past the end: frame1 == frame2 == last
    Run(2, count, 1.0f, 0.5f, 0, 8, true);
    Run(4, count, 2.0f, 1.25f, 0, 8, true);                            // pos 2.5: the empty frame against the duplicate-id frame
    Run(4, count, 2.0f, 1.0f, 0, 8, true);                             // pos 2.0 exactly: weight2 0
    Run(5, count, 1.0f, 3.5f, 0, 8, true);                             // duplicate ids on the frame1 side
}

int main()
{
    Exact();
    for (int n = 0; n < 6000; ++n)
    {
        int nframes = 1 + Rnd() % NFRAMES, count[NFRAMES], f, i;
        for (f = 0; f < NFRAMES; ++f)
        {
            count[f] = Rnd() % (MAXPH + 1);
            for (i = 0; i < MAXPH; ++i) SetPh(f, i, 1 + Rnd() % 30, Rnd() % 10, (float)(Rnd() % 257) / 256.0f);
        }
        float fps = (float)(1 + Rnd() % 60) * 0.5f;
        float t = ((float)(Rnd() % 4096) - 512.0f) / 256.0f;
        int cap = Rnd() % 20, old = cap ? Rnd() % (cap + 1) : 0;
        if (old > 16) old = 16;
        Run(nframes, count, fps, t, old, cap, false);
    }
    if (g_fail) { printf("00a48f10_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("00a48f10_TEST PASS (%d cases)\n", g_cases);
    return 0;
}