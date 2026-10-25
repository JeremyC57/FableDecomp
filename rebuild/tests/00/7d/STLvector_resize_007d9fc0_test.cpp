// Behaviour test for vector<KnickKnack>::resize(n) (0x007D9FC0): growing appends default-constructed elements after the kept
// prefix, shrinking erases the tail, resizing to the current size changes nothing.
#include <cstdio>
#include <cstdlib>
#include <new>

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
}

template <class T> class TVector {          // STLport vector stand-in: start, finish, end of storage
public:
    T* Start;
    T* Finish;
    T* End;
    TVector() : Start(0), Finish(0), End(0) { }
    ~TVector() { Clear(); std::free(Start); }
    void Clear()
    {
        for (T* p = Start; p != Finish; ++p)
            p->~T();
        Finish = Start;
    }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    T* erase(T* first, T* last)
    {
        T* d = first;
        for (T* s = last; s != Finish; ++s, ++d)
            *d = *s;
        for (T* p = d; p != Finish; ++p)
            p->~T();
        Finish = d;
        return first;
    }
    void fill_insert_at_end(unsigned int n, const T& x)
    {
        if ((unsigned int)(End - Finish) < n) {
            unsigned int len = size() + n + 1;
            T* mem = (T*)std::malloc(len * sizeof(T));
            T* cur = mem;
            for (T* p = Start; p != Finish; ++p, ++cur)
                new (cur) T(*p);
            for (; n != 0; --n, ++cur)
                new (cur) T(x);
            Clear();
            std::free(Start);
            Start = mem;
            Finish = cur;
            End = mem + len;
            return;
        }
        for (; n != 0; --n, ++Finish)
            new (Finish) T(x);
    }
    // the row's logic: STLport resize(n, x) and the one-argument resize(n) { resize(n, _Tp()); }
    void resize(unsigned int n, const T& x)
    {
        if (n < size())
            erase(Start + n, Finish);
        else
            fill_insert_at_end(n - size(), x);
    }
    void resize(unsigned int n) { resize(n, T()); }
};

static bool Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
};
class CThingObject { public: int Id; };
static CThingObject gThings[4];
struct KnickKnack {                            // empty constructors: new entries are uninitialised
    C3DVector     Pos;
    CThingObject* Thing;
    KnickKnack() { }
};
typedef KnickKnack E;

int main()
{
    int fails = 0;
    {
    TVector<E> v;
    v.resize(3);
    if (v.size() != 3u) ++fails;
    for (int i = 0; i < 3; ++i) { const E& e = v.Start[i]; if (!(true)) ++fails; }
    for (int i = 0; i < 3; ++i) { E& e = v.Start[i]; e.Pos.X = (float)i; e.Pos.Y = 2.0f; e.Pos.Z = -1.0f; e.Thing = &gThings[i]; }
    v.resize(6);
    if (v.size() != 6u) ++fails;
    for (int i = 0; i < 3; ++i) { const E& e = v.Start[i]; if (!(Near(e.Pos.X, (float)i) && Near(e.Pos.Y, 2.0f) && e.Thing == &gThings[i])) ++fails; }
    for (int i = 3; i < 6; ++i) { const E& e = v.Start[i]; if (!(true)) ++fails; }
    v.resize(2);
    if (v.size() != 2u) ++fails;
    for (int i = 0; i < 2; ++i) { const E& e = v.Start[i]; if (!(Near(e.Pos.X, (float)i) && Near(e.Pos.Y, 2.0f) && e.Thing == &gThings[i])) ++fails; }
    v.resize(2);
    if (v.size() != 2u) ++fails;
    }
    if (fails) { std::printf("007d9fc0_TEST FAIL\n"); return 1; }
    std::printf("007d9fc0_TEST PASS\n");
    return 0;
}