// Behaviour test for 0x0076B700 CVectorMap<unsigned long, C3DVector>::Add(const pair&): the pair is appended unsorted
// and the map is marked dirty.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

template <class _Tp> class CRetailVector {                  // STLport-shaped vector with a growth stand-in
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    int  m_overflows;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0), m_overflows(0) { }
    ~CRetailVector() { for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp(); std::free(_M_start); }
    void _M_insert_overflow(_Tp* pos, const _Tp& x)
    {
        ++m_overflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = n ? n * 2 : 1;
        _Tp* s = (_Tp*)std::malloc(cap * sizeof(_Tp));
        _Tp* d = s;
        for (_Tp* p = _M_start; p != pos; ++p, ++d) { new (d) _Tp(*p); p->~_Tp(); }
        new (d) _Tp(x); ++d;
        std::free(_M_start);
        _M_start = s; _M_finish = d; _M_end_of_storage = s + cap;
    }
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) _Tp(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct Pair {
    unsigned int first;
    C3DVector second;
    Pair(unsigned int k, const C3DVector& v) : first(k), second(v) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};
struct CVectorMapKV {
    CRetailVector<Pair> m_vector;
    bool Compare;
    bool Dirty;
    void Add(const Pair& entry) { m_vector.push_back(entry); Dirty = true; }
};

int main()
{
    CVectorMapKV m; m.Dirty = false;
    m.Add(Pair(9u, C3DVector(1.0f, 2.0f, 3.0f)));
    m.Add(Pair(4u, C3DVector(4.0f, 5.0f, 6.0f)));
    m.Add(Pair(7u, C3DVector(7.0f, 8.0f, 9.0f)));
    int ok = m.Dirty && m.m_vector.size() == 3u;
    if (m.m_vector._M_start[0].first != 9u || m.m_vector._M_start[1].first != 4u || m.m_vector._M_start[2].second.Z != 9.0f) ok = 0;
    if (!ok) { std::printf("0076b700_TEST FAIL\n"); return 1; }
    std::printf("0076b700_TEST PASS\n");
    return 0;
}