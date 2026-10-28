// Behaviour test for 0x00A46B50 std::vector<C3DSkeletalBoneMorph>::insert(iterator, const value_type&) (STLport 4.6)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct BoneMorph {                       // C3DSkeletalBoneMorph: name, CRC, scaling
    int Name; unsigned int CRC; float X, Y, Z;
    BoneMorph(int n, unsigned int c) : Name(n), CRC(c), X(1.0f), Y(2.0f), Z(3.0f) { ++g_live; }
    BoneMorph(const BoneMorph& o) : Name(o.Name), CRC(o.CRC), X(o.X), Y(o.Y), Z(o.Z) { ++g_live; }
    ~BoneMorph() { --g_live; }
    BoneMorph& operator=(const BoneMorph& o) { Name = o.Name; CRC = o.CRC; X = o.X; Y = o.Y; Z = o.Z; return *this; }
};
struct Vec {
    BoneMorph* s; BoneMorph* f; BoneMorph* e;
    Vec() : s(0), f(0), e(0) { }
    unsigned int size() const { return (unsigned int)(f - s); }
    void overflow(BoneMorph* pos, const BoneMorph& x)
    {
        unsigned int old = size(), len = old + (old > 1 ? old : 1);
        BoneMorph* ns = (BoneMorph*)std::malloc(len * sizeof(BoneMorph));
        BoneMorph* nf = ns;
        for (BoneMorph* p = s; p != pos; ++p, ++nf) new (nf) BoneMorph(*p);
        new (nf) BoneMorph(x); ++nf;
        for (BoneMorph* p = pos; p != f; ++p, ++nf) new (nf) BoneMorph(*p);
        for (BoneMorph* p = s; p != f; ++p) p->~BoneMorph();
        std::free(s);
        s = ns; f = nf; e = ns + len;
    }
    BoneMorph* insert(BoneMorph* position, const BoneMorph& x)        // the row's body
    {
        unsigned int n = (unsigned int)(position - s);
        if (f != e) {
            if (position == f) { new (f) BoneMorph(x); ++f; }
            else {
                new (f) BoneMorph(*(f - 1)); ++f;
                BoneMorph x_copy = x;
                BoneMorph* last = f - 2; BoneMorph* result = f - 1;
                for (int k = (int)(last - position); k > 0; --k) *--result = *--last;
                *position = x_copy;
            }
        }
        else
            overflow(position, x);
        return s + n;
    }
    ~Vec() { for (BoneMorph* p = s; p != f; ++p) p->~BoneMorph(); std::free(s); }
};

int main()
{
    int ok = 1;
    {
        Vec v;
        for (int i = 0; i < 5; ++i) v.insert(v.f, BoneMorph(i, 100u + i));      // grows through overflow
        BoneMorph* r = v.insert(v.s + 2, BoneMorph(9, 999u));                  // shift the tail
        ok &= (r == v.s + 2) && v.size() == 6;
        int want[6] = { 0, 1, 9, 2, 3, 4 };
        for (int i = 0; i < 6; ++i) ok &= v.s[i].Name == want[i];
        ok &= v.s[2].CRC == 999u && v.s[5].CRC == 104u;
        r = v.insert(v.s, BoneMorph(7, 7u));
        ok &= r == v.s && v.s[0].Name == 7 && v.s[1].Name == 0 && v.size() == 7;
    }
    ok &= g_live == 0;
    if (ok) std::printf("00a46b50_TEST PASS\n"); else std::printf("00a46b50_TEST FAIL\n");
    return ok ? 0 : 1;
}