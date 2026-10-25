#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Behaviour test for 00f3b760: __uninitialized_copy of CArray<unsigned short> records -- each copy owns an exact-size
// buffer with the same indices, empty arrays stay null, returns result + (last - first).
struct UVec {
    unsigned short* start; unsigned short* finish; unsigned short* eos;
    UVec() : start(0), finish(0), eos(0) { }
    UVec(const UVec& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n != 0 ? (unsigned short*)std::malloc(n * sizeof(unsigned short)) : 0;
        finish = start;
        eos = start + n;
        if (x.finish != x.start) {
            std::memmove(start, x.start, (x.finish - x.start) * sizeof(unsigned short));
            finish = start + (x.finish - x.start);
        }
    }
    ~UVec() { if (start) std::free(start); }
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    UVec& assign(const UVec& x)                         // STLport vector::operator= for a POD element
    {
        if (&x != this) {
            unsigned int xlen = x.size();
            if (xlen > capacity()) {
                unsigned short* tmp = xlen != 0 ? (unsigned short*)std::malloc(xlen * sizeof(unsigned short)) : 0;
                if (x.finish != x.start) std::memmove(tmp, x.start, (x.finish - x.start) * sizeof(unsigned short));
                if (start) std::free(start);
                start = tmp;
                eos = start + xlen;
            }
            else if (size() >= xlen) {
                if (x.finish != x.start) std::memmove(start, x.start, (x.finish - x.start) * sizeof(unsigned short));
            }
            else {
                unsigned int s = size();
                if (s != 0) std::memmove(start, x.start, s * sizeof(unsigned short));
                if (x.finish != x.start + s) std::memmove(finish, x.start + s, (x.finish - (x.start + s)) * sizeof(unsigned short));
            }
            finish = start + xlen;
        }
        return *this;
    }
};
static void fill(UVec& v, unsigned int n, unsigned int cap, unsigned short base)
{
    v.start = (unsigned short*)std::malloc((cap ? cap : 1) * sizeof(unsigned short));
    for (unsigned int i = 0; i < n; ++i) v.start[i] = (unsigned short)(base + i);
    v.finish = v.start + n; v.eos = v.start + cap;
}
static UVec* uninit_copy(UVec* first, UVec* last, UVec* result)
{
    UVec* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) UVec(*first);
    return cur;
}
int main()
{
    UVec src[3];
    fill(src[0], 3, 6, 10);
    fill(src[2], 1, 1, 77);
    UVec* dst = (UVec*)std::malloc(sizeof(UVec) * 3);
    UVec* end = uninit_copy(src, src + 3, dst);
    int ok = end == dst + 3;
    ok = ok && dst[0].size() == 3 && dst[0].capacity() == 3 && dst[0].start != src[0].start && dst[0].start[2] == 12;
    ok = ok && dst[1].start == 0 && dst[1].finish == 0 && dst[1].eos == 0;
    ok = ok && dst[2].size() == 1 && dst[2].start[0] == 77;
    ok = ok && uninit_copy(src, src, dst) == dst;
    for (int i = 0; i < 3; ++i) dst[i].~UVec();
    std::free(dst);
    if (!ok) { std::printf("00f3b760_TEST FAIL\n"); return 1; }
    std::printf("00f3b760_TEST PASS\n");
    return 0;
}