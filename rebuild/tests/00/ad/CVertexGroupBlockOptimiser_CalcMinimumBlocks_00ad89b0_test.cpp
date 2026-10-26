// Behaviour test for 00ad89b0: src/00ad89b0.cpp copied verbatim; the recursion runs for real and is compared with a
// plain reference model, and the in/out state (current, the availability flags) must come back restored.
#include <cstdio>
#include <cstring>
static int fails = 0;
static char g_msg[128];
#define CHECK(c) do { if (!(c)) { ++fails; _snprintf(g_msg, sizeof(g_msg), "FAIL %d: %s", __LINE__, #c); puts(g_msg); } } while (0)
// ---- source (verbatim) ----
// CVertexGroupBlockOptimiser::CalcMinimumBlocks(const CArray<unsigned long>&, unsigned long&, CArray<bool>&, unsigned long, unsigned long) -- recursive search for the minimum number of blocks: try every still-available size from `start` that fits the current block (MaxN), recurse, keep the minimum; when nothing fits, open a new block with the first available size
// Leaf: ?CalcMinimumBlocks@CVertexGroupBlockOptimiser@@AAEKABV?$CArray@K@@AAKAAV?$CArray@_N@@KK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD89B0, 314 bytes, `ret 14h`; recursive (the only callee is itself).
// Shape: every availability access re-indexes available[i] (no named slot reference); the restore after the recursion is
//   current, remaining, then the flag; best is homed in the frame because std::min takes it by reference; the fresh-block
//   arm restores its flag and `break`s to the shared `return best`.
template <class T> class CArray { public:                 // retail 12-byte STLport-style vector
    T* m_first;
    T* m_last;
    T* m_end;
    T* begin() const { return m_first; }
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
template <class T> inline const T& min_of(const T& a, const T& b) { return b < a ? b : a; }   // STLport std::min
struct CVertexGroupBlockOptimiser {                // retail layout (see the landed InsertBlock 0x00AD92E0)
    unsigned char _pad00[0x44];
    unsigned long MaxN;                            // +0x44
    unsigned long CalcMinimumBlocks(const CArray<unsigned long>& sizes, unsigned long& current, CArray<bool>& available,
                                    unsigned long remaining, unsigned long start);
};
unsigned long CVertexGroupBlockOptimiser::CalcMinimumBlocks(const CArray<unsigned long>& sizes, unsigned long& current,
                                                            CArray<bool>& available, unsigned long remaining, unsigned long start)
{
    if (remaining == 0)
        return 0;
    bool noneFitted = true;
    unsigned long best = sizes.size();
    for (unsigned long i = start; i < sizes.size(); ++i) {
        if (available[i]) {
            unsigned long size = sizes[i];
            if (current + size <= MaxN) {
                available[i] = false;
                current += size;
                remaining -= size;
                noneFitted = false;
                unsigned long blocks = CalcMinimumBlocks(sizes, current, available, remaining - 1, i + 1);
                best = min_of(best, blocks);
                current -= size;
                remaining += size;
                available[i] = true;
            }
        }
    }
    if (noneFitted) {
        for (unsigned long i = 0; i < sizes.size(); ++i) {
            if (available[i]) {
                unsigned long size = sizes[i];
                available[i] = false;
                remaining -= size;
                unsigned long fresh = size;
                best = CalcMinimumBlocks(sizes, fresh, available, remaining - 1, i + 1) + 1;
                available[i] = true;
                break;
            }
        }
    }
    return best;
}

// ---- reference model and driver ----
static unsigned long RefCalc(const unsigned long* sizes, unsigned long count, unsigned long maxN, unsigned long& current,
                             bool* available, unsigned long remaining, unsigned long start)
{
    if (remaining == 0)
        return 0;
    bool noneFitted = true;
    unsigned long best = count;
    for (unsigned long i = start; i < count; ++i) {
        if (available[i] && current + sizes[i] <= maxN) {
            available[i] = false;
            current += sizes[i];
            noneFitted = false;
            unsigned long blocks = RefCalc(sizes, count, maxN, current, available, remaining - sizes[i] - 1, i + 1);
            if (blocks < best)
                best = blocks;
            current -= sizes[i];
            available[i] = true;
        }
    }
    if (noneFitted) {
        for (unsigned long i = 0; i < count; ++i) {
            if (available[i]) {
                available[i] = false;
                unsigned long fresh = sizes[i];
                best = RefCalc(sizes, count, maxN, fresh, available, remaining - sizes[i] - 1, i + 1) + 1;
                available[i] = true;
                break;
            }
        }
    }
    return best;
}
static void RunCase(unsigned long* sizeData, unsigned long count, unsigned long maxN, unsigned long current,
                    unsigned long remaining, unsigned long start)
{
    static bool flags[8], refFlags[8];
    for (unsigned long k = 0; k < count; ++k) { flags[k] = true; refFlags[k] = true; }
    CArray<unsigned long> sizes; sizes.m_first = sizeData; sizes.m_last = sizeData + count; sizes.m_end = sizes.m_last;
    CArray<bool> available; available.m_first = flags; available.m_last = flags + count; available.m_end = available.m_last;
    CVertexGroupBlockOptimiser opt; opt.MaxN = maxN;
    unsigned long cur = current, refCur = current;
    unsigned long got = opt.CalcMinimumBlocks(sizes, cur, available, remaining, start);
    unsigned long want = RefCalc(sizeData, count, maxN, refCur, refFlags, remaining, start);
    CHECK(got == want);
    CHECK(cur == current);
    for (unsigned long k = 0; k < count; ++k) CHECK(flags[k]);
}
int main()
{
    static unsigned long a[4] = { 3, 5, 2, 4 };
    static unsigned long b[5] = { 1, 1, 1, 1, 1 };
    static unsigned long c[3] = { 9, 9, 9 };
    {
        CArray<unsigned long> s; s.m_first = a; s.m_last = a + 4;
        CArray<bool> f; static bool fl[4] = { true, true, true, true }; f.m_first = fl; f.m_last = fl + 4;
        CVertexGroupBlockOptimiser opt; opt.MaxN = 8; unsigned long cur = 0;
        CHECK(opt.CalcMinimumBlocks(s, cur, f, 0, 0) == 0);   // nothing remaining
    }
    RunCase(a, 4, 8, 0, 14, 0);
    RunCase(a, 4, 6, 1, 18, 0);
    RunCase(b, 5, 2, 0, 10, 0);
    RunCase(b, 5, 3, 2, 7, 1);
    RunCase(c, 3, 8, 0, 30, 0);   // nothing ever fits: fresh blocks only
    if (fails == 0) printf("00ad89b0_TEST PASS\n");
    return fails;
}