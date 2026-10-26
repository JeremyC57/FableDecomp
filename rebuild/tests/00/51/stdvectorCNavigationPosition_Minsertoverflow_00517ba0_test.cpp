// Behaviour test for 0x00517BA0 vector<CNavigationPosition>::_M_insert_overflow.
#include <cstdio>
#include <cstdlib>
#include <new>

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
class CNavigationPosition { public:
    C2DVector Position;
    long Layer;
    CNavigationPosition(float x, float y, long l) : Position(x, y), Layer(l) { }
};
struct Vec {
    CNavigationPosition* start; CNavigationPosition* finish; CNavigationPosition* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CNavigationPosition* pos, const CNavigationPosition& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CNavigationPosition* ns = len ? (CNavigationPosition*)std::malloc(len * sizeof(CNavigationPosition)) : 0;
        CNavigationPosition* nf = ns;
        for (CNavigationPosition* p = start; p != pos; ++p, ++nf) new (nf) CNavigationPosition(*p);
        if (fill_len == 1) { new (nf) CNavigationPosition(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CNavigationPosition(x);
        if (!atend) for (CNavigationPosition* p = pos; p != finish; ++p, ++nf) new (nf) CNavigationPosition(*p);
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    int ok = 1;
    Vec v; v.start = v.finish = v.eos = 0;
    CNavigationPosition a(1.0f, 2.0f, 0), b(3.0f, 4.0f, 1), c(5.0f, 6.0f, 2);
    v.insert_overflow(v.finish, a, 1, true);              // cap 1
    v.insert_overflow(v.finish, b, 1, true);              // cap 2
    v.insert_overflow(v.start, c, 2, false);              // cap 4: [c c a b]
    if (v.size() != 4 || v.eos - v.start != 4) ok = 0;
    long layers[4] = { 2, 2, 0, 1 };
    float xs[4] = { 5.0f, 5.0f, 1.0f, 3.0f };
    for (int i = 0; i < 4; ++i) {
        float d = v.start[i].Position.X - xs[i];
        if (v.start[i].Layer != layers[i] || d < -0.001f || d > 0.001f) ok = 0;
    }
    std::free(v.start);
    if (!ok) { std::printf("00517ba0_TEST FAIL\n"); return 1; }
    std::printf("00517ba0_TEST PASS\n");
    return 0;
}