// Behaviour test for 0x00b7c870 std::vector<CPreTransposedBoneMatrix>::_M_insert_overflow (STLport 4.6 growth insert, 48-byte elements).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct CPreTransposedBoneMatrix { float m[3][4]; };          // 48 bytes, copied memberwise

struct Vec {
    CPreTransposedBoneMatrix* start; CPreTransposedBoneMatrix* finish; CPreTransposedBoneMatrix* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    // the reconstructed body: allocate size + max(size, fill_len), copy head, x (once or fill_len times), tail unless at end
    void insert_overflow(CPreTransposedBoneMatrix* pos, const CPreTransposedBoneMatrix& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CPreTransposedBoneMatrix* new_start = len != 0 ? (CPreTransposedBoneMatrix*)malloc(len * sizeof(CPreTransposedBoneMatrix)) : 0;
        CPreTransposedBoneMatrix* new_finish = new_start;
        for (CPreTransposedBoneMatrix* p = start; p != pos; ++p, ++new_finish) new (new_finish) CPreTransposedBoneMatrix(*p);
        if (fill_len == 1) { new (new_finish) CPreTransposedBoneMatrix(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) CPreTransposedBoneMatrix(x);
        if (!atend)
            for (CPreTransposedBoneMatrix* p = pos; p != finish; ++p, ++new_finish) new (new_finish) CPreTransposedBoneMatrix(*p);
        if (start) free(start);
        start = new_start; finish = new_finish; eos = new_start + len;
    }
};

static CPreTransposedBoneMatrix make(float v) { CPreTransposedBoneMatrix r; for (int i = 0; i < 3; ++i) for (int j = 0; j < 4; ++j) r.m[i][j] = v + i * 4 + j; return r; }
static bool same(const CPreTransposedBoneMatrix& a, const CPreTransposedBoneMatrix& b) { return memcmp(&a, &b, sizeof(a)) == 0; }

int main()
{
    if (sizeof(CPreTransposedBoneMatrix) != 48) { std::printf("00b7c870_TEST FAIL\n"); return 1; }
    Vec v; v.start = v.finish = v.eos = 0;
    CPreTransposedBoneMatrix a = make(1), b = make(100), c = make(200);
    v.insert_overflow(v.finish, a, 1, true);                 // empty -> 1 (capacity 1)
    if (v.size() != 1 || v.eos - v.start != 1 || !same(v.start[0], a)) { std::printf("00b7c870_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, b, 1, true);                 // 1 -> 2 (capacity 2)
    if (v.size() != 2 || v.eos - v.start != 2 || !same(v.start[1], b)) { std::printf("00b7c870_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start + 1, c, 3, false);             // middle fill of 3: capacity 2 + 3
    if (v.size() != 5 || v.eos - v.start != 5) { std::printf("00b7c870_TEST FAIL\n"); return 1; }
    if (!same(v.start[0], a) || !same(v.start[1], c) || !same(v.start[2], c) || !same(v.start[3], c) || !same(v.start[4], b)) {
        std::printf("00b7c870_TEST FAIL\n"); return 1;
    }
    v.insert_overflow(v.start, a, 1, true);                  // atend: the tail is dropped (callers only pass it at the end)
    if (v.size() != 1 || v.eos - v.start != 10) { std::printf("00b7c870_TEST FAIL\n"); return 1; }
    free(v.start);
    std::printf("00b7c870_TEST PASS\n");
    return 0;
}