// Behaviour test for 0x00759570 CVectorMap<EBodyLocation, CWoundMorphs::CEntry>::Add: the {location, entry} pair is
// appended unsorted (whole 40-byte entry copied) and the map is marked dirty.
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

enum EBodyLocation { BODY_HEAD, BODY_TORSO, BODY_ARM };
struct CEntry {
    EBodyLocation Location;
    long ReplacingTextureIndex, ReplacingBumpMapIndex, WoundTextureIndex, WoundBumpMapIndex;
    long ScarTextureIndex, ScarBumpMapIndex, FadeToScarDuration, TotalDuration;
    float PermanentScarStrength;
};
struct Pair {
    EBodyLocation first;
    CEntry second;
    Pair(const EBodyLocation& a, const CEntry& b) : first(a), second(b) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};
struct CWoundMap {
    CRetailVector<Pair> m_vector;
    bool Compare;
    bool Dirty;
    void Add(const EBodyLocation& key, const CEntry& value) { m_vector.push_back(Pair(key, value)); Dirty = true; }
};

int main()
{
    CWoundMap m; m.Dirty = false;
    CEntry e; std::memset(&e, 0, sizeof(e));
    e.Location = BODY_ARM; e.TotalDuration = 300; e.PermanentScarStrength = 0.5f;
    m.Add(BODY_TORSO, e);
    e.TotalDuration = 99;
    m.Add(BODY_HEAD, e);
    int ok = m.Dirty && m.m_vector.size() == 2u;
    if (m.m_vector._M_start[0].first != BODY_TORSO || m.m_vector._M_start[0].second.TotalDuration != 300) ok = 0;
    if (m.m_vector._M_start[1].first != BODY_HEAD || m.m_vector._M_start[1].second.TotalDuration != 99) ok = 0;
    if (m.m_vector._M_start[1].second.PermanentScarStrength != 0.5f || m.m_vector._M_start[1].second.Location != BODY_ARM) ok = 0;
    if (!ok) { std::printf("00759570_TEST FAIL\n"); return 1; }
    std::printf("00759570_TEST PASS\n");
    return 0;
}