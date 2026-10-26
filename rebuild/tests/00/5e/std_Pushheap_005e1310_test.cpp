#include <cstdio>
// Behaviour test for 0x005E1310: _Push_heap over pair<unsigned long, CAnimationEntry> with an intrusively
// counted component map (assignment releases/adopts, the by-value argument releases its reference).
class CAnimComponentMap { public:
    virtual void v00() { }
    virtual void Destroy() { ++destroyed; }
    long m_refCount;
    int destroyed;
    CAnimComponentMap() : m_refCount(0), destroyed(0) { }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    void Release() { T* object = Object; if (object) { if (--object->m_refCount == 0) object->Destroy(); Object = 0; } }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) { Release(); Object = other.Object; if (Object) ++Object->m_refCount; }
        return *this;
    }
    ~CIVCountedPointer() { T* object = Object; if (object && --object->m_refCount == 0) object->Destroy(); }
};
class CDefString { public: long TablePos; };
class CAnimationEntry { public:
    long BankIndex; CDefString AnimName; CDefString GroupName; CIVCountedPointer<CAnimComponentMap> Components;
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<unsigned long, CAnimationEntry> Elem;
int main()
{
    CAnimComponentMap maps[7];
    int fail = 0;
    {
        Elem h[8]; int n = 0;
        unsigned long keys[7] = { 0x10, 0xFFFF0000u, 0x30, 0x05, 0xFFFF0001u, 0x20, 0x00 };
        for (int i = 0; i < 7; ++i) {
            Elem v; v.first = keys[i]; v.second.BankIndex = i; v.second.AnimName.TablePos = 10 + i; v.second.GroupName.TablePos = 20 + i;
            v.second.Components.Object = &maps[i]; maps[i].m_refCount = 1;   // v owns one reference
            _Push_heap(h, n, 0, v, CKeyPairCompareLess<unsigned long, CAnimationEntry>()); ++n;
            for (int c = 1; c < n; ++c)
                if (h[(c - 1) / 2].first < h[c].first) fail = 1;
        }
        // v (per iteration) is gone: each map is referenced by exactly one heap slot
        for (int i = 0; i < 7; ++i) if (maps[i].m_refCount != 1 || maps[i].destroyed) fail = 1;
        if (h[0].first != 0xFFFF0001u || h[0].second.BankIndex != 4 || h[0].second.Components.Object != &maps[4]) fail = 1;
    }
    // the heap slots release the last reference at scope end
    for (int i = 0; i < 7; ++i) if (maps[i].m_refCount != 0 || maps[i].destroyed != 1) fail = 1;
    if (fail) { std::printf("005e1310_TEST FAIL\n"); return 1; }
    std::printf("005e1310_TEST PASS\n");
    return 0;
}