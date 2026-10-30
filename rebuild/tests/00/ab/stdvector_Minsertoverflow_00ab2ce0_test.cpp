// Behaviour test for 0x00ab2ce0 std::vector<N3DAnimationBuilder::CSequenceInfo>::_M_insert_overflow (STLport 4.6 growth insert, 52-byte elements).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct CSequenceInfo { float m[13]; };            // CSequenceInfo stand-in: 52 bytes, copied memberwise

struct Vec {
    CSequenceInfo* start; CSequenceInfo* finish; CSequenceInfo* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    // the reconstructed body: allocate size + max(size, fill_len), copy head, x (once or fill_len times), tail unless at end
    void insert_overflow(CSequenceInfo* pos, const CSequenceInfo& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CSequenceInfo* new_start = len != 0 ? (CSequenceInfo*)malloc(len * sizeof(CSequenceInfo)) : 0;
        CSequenceInfo* new_finish = new_start;
        for (CSequenceInfo* p = start; p != pos; ++p, ++new_finish) new (new_finish) CSequenceInfo(*p);
        if (fill_len == 1) { new (new_finish) CSequenceInfo(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) CSequenceInfo(x);
        if (!atend)
            for (CSequenceInfo* p = pos; p != finish; ++p, ++new_finish) new (new_finish) CSequenceInfo(*p);
        if (start) free(start);
        start = new_start; finish = new_finish; eos = new_start + len;
    }
};

static CSequenceInfo make(float v) { CSequenceInfo r; for (int i = 0; i < 13; ++i) r.m[i] = v + i; return r; }
static bool same(const CSequenceInfo& a, const CSequenceInfo& b) { return memcmp(&a, &b, sizeof(a)) == 0; }

int main()
{
    if (sizeof(CSequenceInfo) != 52) { std::printf("00ab2ce0_TEST FAIL\n"); return 1; }
    Vec v; v.start = v.finish = v.eos = 0;
    CSequenceInfo a = make(1), b = make(100), c = make(200);
    v.insert_overflow(v.finish, a, 1, true);                 // empty -> 1 (capacity 1)
    if (v.size() != 1 || v.eos - v.start != 1 || !same(v.start[0], a)) { std::printf("00ab2ce0_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, b, 1, true);                 // 1 -> 2 (capacity 2)
    if (v.size() != 2 || v.eos - v.start != 2 || !same(v.start[1], b)) { std::printf("00ab2ce0_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start + 1, c, 3, false);             // middle fill of 3: capacity 2 + 3
    if (v.size() != 5 || v.eos - v.start != 5) { std::printf("00ab2ce0_TEST FAIL\n"); return 1; }
    if (!same(v.start[0], a) || !same(v.start[1], c) || !same(v.start[2], c) || !same(v.start[3], c) || !same(v.start[4], b)) {
        std::printf("00ab2ce0_TEST FAIL\n"); return 1;
    }
    v.insert_overflow(v.start, a, 1, true);                  // atend: the tail is dropped (callers only pass it at the end)
    if (v.size() != 1 || v.eos - v.start != 10) { std::printf("00ab2ce0_TEST FAIL\n"); return 1; }
    free(v.start);
    std::printf("00ab2ce0_TEST PASS\n");
    return 0;
}