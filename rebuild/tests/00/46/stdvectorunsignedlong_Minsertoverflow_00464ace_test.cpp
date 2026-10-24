// Behaviour test for 0x00464ACE std::vector<unsigned long>::_M_insert_overflow(pos, x, tag, fill_len, atend)
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct ULongVector {
    unsigned int* start; unsigned int* finish; unsigned int* eos;
    static void* CopyTrivial(const void* f, const void* l, void* r)
    {
        return (l == f) ? r : (char*)std::memmove(r, f, (const char*)l - (const char*)f) + ((const char*)l - (const char*)f);
    }
    void InsertOverflow(unsigned int* pos, const unsigned int& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = (unsigned int)(finish - start);
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        unsigned int* ns = len ? static_cast<unsigned int*>(std::malloc(len * sizeof(unsigned int))) : 0;
        unsigned int* nf = (unsigned int*)CopyTrivial(start, pos, ns);
        for (unsigned int n = fill_len; n > 0; --n, ++nf) *nf = x;
        if (!atend) nf = (unsigned int*)CopyTrivial(pos, finish, nf);
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    ULongVector v; v.start = static_cast<unsigned int*>(std::malloc(3 * sizeof(unsigned int)));
    v.start[0] = 1; v.start[1] = 2; v.start[2] = 3; v.finish = v.eos = v.start + 3;
    unsigned int x = 9;
    v.InsertOverflow(v.start + 1, x, 2, false);                // 1 9 9 2 3, capacity 3 + 3
    if (v.finish - v.start != 5 || v.eos - v.start != 6 || v.start[0] != 1 || v.start[1] != 9 || v.start[2] != 9 || v.start[3] != 2 || v.start[4] != 3) { std::printf("00464ace_TEST FAIL\n"); return 1; }
    unsigned int y = 7;
    v.InsertOverflow(v.finish, y, 1, true);                    // push_back growth: 1 9 9 2 3 7, capacity 10
    if (v.finish - v.start != 6 || v.eos - v.start != 10 || v.start[5] != 7) { std::printf("00464ace_TEST FAIL\n"); return 1; }
    ULongVector e; e.start = e.finish = e.eos = 0;
    e.InsertOverflow(e.finish, y, 1, true);                    // empty vector grows to one element
    if (e.finish - e.start != 1 || e.start[0] != 7) { std::printf("00464ace_TEST FAIL\n"); return 1; }
    std::free(v.start); std::free(e.start);
    std::printf("00464ace_TEST PASS\n");
    return 0;
}