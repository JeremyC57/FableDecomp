#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Behaviour test for 00f3b640: vector<unsigned short>::operator= -- grow path reallocates to exactly x.size(),
// shrink path copies over and trims, the middle path copies the live part then appends; self-assignment is a no-op.
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
int main()
{
    UVec a, b, c, d;
    fill(a, 5, 5, 100);
    fill(b, 2, 2, 7);
    b.assign(a);                                       // grow: 5 > capacity 2
    int ok = b.size() == 5 && b.capacity() == 5 && b.start != a.start && b.start[0] == 100 && b.start[4] == 104;
    fill(c, 3, 8, 1);
    c.assign(a);                                       // capacity 8 >= 5, size 3 < 5: copy 3 then append 2
    ok = ok && c.size() == 5 && c.capacity() == 8 && c.start[2] == 102 && c.start[3] == 103 && c.start[4] == 104;
    fill(d, 2, 4, 50);
    d.assign(c);                                       // capacity 4 < 5: reallocate
    ok = ok && d.size() == 5 && d.capacity() == 5;
    UVec e; fill(e, 1, 1, 9);
    c.assign(e);                                       // size 5 >= 1: copy over, trim
    ok = ok && c.size() == 1 && c.capacity() == 8 && c.start[0] == 9;
    unsigned short* keep = a.start;
    a.assign(a);
    ok = ok && a.start == keep && a.size() == 5;
    if (!ok) { std::printf("00f3b640_TEST FAIL\n"); return 1; }
    std::printf("00f3b640_TEST PASS\n");
    return 0;
}