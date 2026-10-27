// Behaviour test for 0x00BDB9F0 std::vector<CVertexShaderConstant>::operator= (16-byte elements)
#include <cstdio>
#include <cstdlib>

struct C { float X, Y, Z, W; };

struct Vec {
    C* start; C* finish; C* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            unsigned int xlen = x.size();
            if (xlen > capacity()) {
                C* tmp = xlen ? (C*)std::malloc(xlen * sizeof(C)) : 0;
                for (unsigned int i = 0; i < xlen; ++i) tmp[i] = x.start[i];
                if (start) std::free(start);
                start = tmp; eos = start + xlen;
            } else if (size() >= xlen) {
                for (unsigned int i = 0; i < xlen; ++i) start[i] = x.start[i];
            } else {
                unsigned int n = size();
                for (unsigned int i = 0; i < n; ++i) start[i] = x.start[i];
                for (unsigned int i = n; i < xlen; ++i) start[i] = x.start[i];
            }
            finish = start + xlen;
        }
        return *this;
    }
};

static Vec make(float base, int n, int cap)
{
    Vec v; v.start = (C*)std::malloc(cap * sizeof(C));
    for (int i = 0; i < n; ++i) { v.start[i].X = base + i; v.start[i].Y = 1; v.start[i].Z = 2; v.start[i].W = base; }
    v.finish = v.start + n; v.eos = v.start + cap; return v;
}

static bool same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a.start[i].X != b.start[i].X || a.start[i].W != b.start[i].W) return false;
    return true;
}

int main()
{
    Vec big = make(10.0f, 6, 6), small = make(20.0f, 2, 2), mid = make(30.0f, 4, 5);
    Vec a = make(0.0f, 1, 1);
    a = big;
    if (!same(a, big) || a.capacity() != 6) { std::printf("00bdb9f0_TEST FAIL\n"); return 1; }
    a = small;
    if (!same(a, small) || a.capacity() != 6) { std::printf("00bdb9f0_TEST FAIL\n"); return 1; }
    a = mid;
    if (!same(a, mid) || a.capacity() != 6) { std::printf("00bdb9f0_TEST FAIL\n"); return 1; }
    Vec& r = (a = a);
    if (&r != &a || !same(a, mid)) { std::printf("00bdb9f0_TEST FAIL\n"); return 1; }
    std::printf("00bdb9f0_TEST PASS\n");
    return 0;
}