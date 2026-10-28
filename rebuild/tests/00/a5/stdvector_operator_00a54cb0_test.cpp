// Behaviour test for 0x00A54CB0 std::vector<C3DAnimationSequenceData::C3DVectorWord>::operator= (STLport 4.6)
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct VectorWord { short X, Y, Z; };
struct Vec {
    VectorWord* s; VectorWord* f; VectorWord* e;
    Vec() : s(0), f(0), e(0) { }
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    void assign(const VectorWord* a, unsigned int n) { s = (VectorWord*)std::malloc(n * 6 + 6); std::memcpy(s, a, n * 6); f = s + n; e = s + n + 1; }
    Vec& operator=(const Vec& x)                                        // the row's body
    {
        if (&x != this) {
            unsigned int xlen = x.size();
            if (xlen > capacity()) {
                VectorWord* tmp = (VectorWord*)std::malloc(xlen * sizeof(VectorWord));   // _M_allocate_and_copy
                for (unsigned int i = 0; i < xlen; ++i) tmp[i] = x.s[i];
                if (s) std::free(s);
                s = tmp;
                e = s + xlen;
            }
            else if (size() >= xlen) {
                for (unsigned int i = 0; i < xlen; ++i) s[i] = x.s[i];
            }
            else {
                unsigned int n = size();
                for (unsigned int i = 0; i < n; ++i) s[i] = x.s[i];
                for (unsigned int i = n; i < xlen; ++i) f[i - n] = x.s[i];
            }
            f = s + xlen;
        }
        return *this;
    }
};

int main()
{
    int ok = 1;
    VectorWord big[5] = { {1,2,3}, {4,5,6}, {7,8,9}, {10,11,12}, {13,14,15} };
    VectorWord small[2] = { {-1,-2,-3}, {-4,-5,-6} };
    Vec a, b, c;
    a.assign(big, 5); b.assign(small, 2);
    b = a;                                                             // grows: capacity 3 < 5
    ok &= b.size() == 5 && b.capacity() == 5 && b.s[4].Z == 15;
    c.assign(small, 2);
    a = c;                                                             // shrinks in place
    ok &= a.size() == 2 && a.s[1].X == -4 && a.capacity() == 6;
    a.f = a.s + 1;                                                     // size 1, capacity 6
    a = b;                                                             // fits: copy one, construct four
    ok &= a.size() == 5 && a.s[0].X == 1 && a.s[3].Y == 11;
    Vec& r = (a = a);
    ok &= &r == &a && a.size() == 5;
    if (ok) std::printf("00a54cb0_TEST PASS\n"); else std::printf("00a54cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}