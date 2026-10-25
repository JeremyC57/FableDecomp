// Behaviour test for 0x0075B8D0 std::vector<C3DVector>::operator=: grow (allocate-and-copy), shrink-in-place (copy +
// keep capacity), and the middle case (copy over the live part, construct the rest), self-assignment untouched
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct Vec {
    C3DVector* s;
    C3DVector* f;
    C3DVector* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    Vec& assign(const Vec& x)                     // STLport 4.6 operator=
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                C3DVector* tmp = (C3DVector*)std::malloc(xlen * sizeof(C3DVector));
                for (unsigned int i = 0; i < xlen; ++i) new (tmp + i) C3DVector(x.s[i]);
                if (s) std::free(s);
                s = tmp;
                e = s + xlen;
            }
            else if (size() >= xlen) {
                for (unsigned int i = 0; i < xlen; ++i) s[i] = x.s[i];
            }
            else {
                unsigned int old = size();
                for (unsigned int i = 0; i < old; ++i) s[i] = x.s[i];
                for (unsigned int i = old; i < xlen; ++i) new (s + i) C3DVector(x.s[i]);
            }
            f = s + xlen;
        }
        return *this;
    }
};

Vec make(unsigned int n, unsigned int cap, float base)
{
    Vec v;
    v.s = (C3DVector*)std::malloc(cap * sizeof(C3DVector));
    v.f = v.s + n;
    v.e = v.s + cap;
    for (unsigned int i = 0; i < n; ++i) new (v.s + i) C3DVector(base + i, base - i, base * 2);
    return v;
}
bool same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a.s[i].X != b.s[i].X || a.s[i].Y != b.s[i].Y || a.s[i].Z != b.s[i].Z) return false;
    return true;
}

}

int main()
{
    using namespace model;
    int bad = 0;
    Vec big = make(6, 6, 10.0f), small = make(2, 8, 1.0f), mid = make(4, 4, 5.0f);
    Vec d = make(1, 1, 0.0f);
    d.assign(big);                                   // grow: new block of exactly 6
    if (!same(d, big) || d.capacity() != 6 || d.s == big.s) bad = 1;
    C3DVector* keep = d.s;
    d.assign(small);                                 // shrink: same block, capacity kept
    if (!same(d, small) || d.s != keep || d.capacity() != 6) bad = 2;
    d.assign(mid);                                   // 2 live -> 4 within capacity 6
    if (!same(d, mid) || d.s != keep || d.capacity() != 6) bad = 3;
    d.assign(d);                                     // self
    if (!same(d, mid)) bad = 4;
    std::free(big.s); std::free(small.s); std::free(mid.s); std::free(d.s);
    if (bad) { std::printf("0075b8d0_TEST FAIL\n"); return 1; }
    std::printf("0075b8d0_TEST PASS\n");
    return 0;
}