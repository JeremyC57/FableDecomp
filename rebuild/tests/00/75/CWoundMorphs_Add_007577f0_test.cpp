// Behaviour test for 0x007577F0 CWoundMorphs::Add(CWoundMorphs::CEntry&)
#include <cstdio>
#include <cstdlib>
#include <new>

enum EBodyLocation { LOC0 = 0, LOC3 = 3, LOC6 = 6 };

struct __false_type { __false_type() {} };

template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

static int g_overflowCalls = 0;

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
    {
        ++g_overflowCalls;
        int size = (int)(_M_finish - _M_start);
        int capacity = size ? size * 2 : 1;
        T* block = (T*)std::malloc(capacity * sizeof(T));
        T* out = block;
        for (T* p = _M_start; p != position; ++p, ++out) new (out) T(*p);
        for (unsigned int i = 0; i < fillLength; ++i, ++out) new (out) T(x);
        if (!atEnd) for (T* p = position; p != _M_finish; ++p, ++out) new (out) T(*p);
        std::free(_M_start);
        _M_start = block;
        _M_finish = out;
        _M_end_of_storage = block + capacity;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

template <class K, class V> class CVectorMap : public vector<pair<K, V> > {
public:
    char Compare;
    bool Dirty;
};

class CWoundMorphs {
public:
    class CEntry {
    public:
        EBodyLocation Location;
        long Rest[8];
        float PermanentScarStrength;
    };
    CVectorMap<EBodyLocation, CEntry> Morphs;
    void Add(CEntry& entry);
};

void CWoundMorphs::Add(CEntry& entry)
{
    Morphs.push_back(pair<EBodyLocation, CEntry>(entry.Location, entry));
    Morphs.Dirty = true;
}

int main()
{
    CWoundMorphs m;
    m.Morphs._M_start = m.Morphs._M_finish = m.Morphs._M_end_of_storage = 0;
    m.Morphs.Dirty = false;
    int ok = 1;
    EBodyLocation locs[5] = { LOC3, LOC0, LOC6, LOC3, LOC0 };
    for (int i = 0; i < 5; ++i) {
        CWoundMorphs::CEntry e;
        e.Location = locs[i];
        for (int k = 0; k < 8; ++k) e.Rest[k] = i * 10 + k;
        e.PermanentScarStrength = 0.5f * i;
        m.Morphs.Dirty = false;
        m.Add(e);
        if (!m.Morphs.Dirty) ok = 0;
    }
    if (m.Morphs._M_finish - m.Morphs._M_start != 5) ok = 0;
    if (g_overflowCalls != 4) ok = 0;          // capacities 1, 2, 4, 8
    for (int i = 0; i < 5; ++i) {
        const pair<EBodyLocation, CWoundMorphs::CEntry>& p = m.Morphs._M_start[i];
        if (p.first != locs[i] || p.second.Location != locs[i]) ok = 0;
        for (int k = 0; k < 8; ++k) if (p.second.Rest[k] != i * 10 + k) ok = 0;
        if (p.second.PermanentScarStrength != 0.5f * i) ok = 0;
    }
    std::free(m.Morphs._M_start);
    if (!ok) { std::printf("007577f0_TEST FAIL\n"); return 1; }
    std::printf("007577f0_TEST PASS\n");
    return 0;
}