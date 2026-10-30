// Behaviour test for 0x00aafe50 std::vector<CMatrix3x4>::_M_insert_overflow (STLport 4.6 growth insert, 48-byte elements).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CMatrix3x4 { public: float m[3][4]; };          // 48 bytes, copied memberwise

struct Vec {
    CMatrix3x4* start; CMatrix3x4* finish; CMatrix3x4* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    // the reconstructed body: allocate size + max(size, fill_len), copy head, x (once or fill_len times), tail unless at end
    void insert_overflow(CMatrix3x4* pos, const CMatrix3x4& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CMatrix3x4* new_start = len != 0 ? (CMatrix3x4*)malloc(len * sizeof(CMatrix3x4)) : 0;
        CMatrix3x4* new_finish = new_start;
        for (CMatrix3x4* p = start; p != pos; ++p, ++new_finish) new (new_finish) CMatrix3x4(*p);
        if (fill_len == 1) { new (new_finish) CMatrix3x4(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) CMatrix3x4(x);
        if (!atend)
            for (CMatrix3x4* p = pos; p != finish; ++p, ++new_finish) new (new_finish) CMatrix3x4(*p);
        if (start) free(start);
        start = new_start; finish = new_finish; eos = new_start + len;
    }
};

static CMatrix3x4 make(float v) { CMatrix3x4 r; for (int i = 0; i < 3; ++i) for (int j = 0; j < 4; ++j) r.m[i][j] = v + i * 4 + j; return r; }
static bool same(const CMatrix3x4& a, const CMatrix3x4& b) { return memcmp(&a, &b, sizeof(a)) == 0; }

int main()
{
    if (sizeof(CMatrix3x4) != 48) { std::printf("00aafe50_TEST FAIL\n"); return 1; }
    Vec v; v.start = v.finish = v.eos = 0;
    CMatrix3x4 a = make(1), b = make(100), c = make(200);
    v.insert_overflow(v.finish, a, 1, true);                 // empty -> 1 (capacity 1)
    if (v.size() != 1 || v.eos - v.start != 1 || !same(v.start[0], a)) { std::printf("00aafe50_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, b, 1, true);                 // 1 -> 2 (capacity 2)
    if (v.size() != 2 || v.eos - v.start != 2 || !same(v.start[1], b)) { std::printf("00aafe50_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start + 1, c, 3, false);             // middle fill of 3: capacity 2 + 3
    if (v.size() != 5 || v.eos - v.start != 5) { std::printf("00aafe50_TEST FAIL\n"); return 1; }
    if (!same(v.start[0], a) || !same(v.start[1], c) || !same(v.start[2], c) || !same(v.start[3], c) || !same(v.start[4], b)) {
        std::printf("00aafe50_TEST FAIL\n"); return 1;
    }
    v.insert_overflow(v.start, a, 1, true);                  // atend: the tail is dropped (callers only pass it at the end)
    if (v.size() != 1 || v.eos - v.start != 10) { std::printf("00aafe50_TEST FAIL\n"); return 1; }
    free(v.start);
    std::printf("00aafe50_TEST PASS\n");
    return 0;
}