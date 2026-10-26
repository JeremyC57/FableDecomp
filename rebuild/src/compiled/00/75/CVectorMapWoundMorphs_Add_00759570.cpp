// CVectorMap_WoundMorphs::Add(const EBodyLocation& key, const CWoundMorphs::CEntry& value) -- CVectorMap<EBodyLocation, CWoundMorphs::CEntry, CKeyPairCompareLess<..> >: append a {body location, wound entry} pair to the map's vector and mark the map dirty
// Leaf: ?Add@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00759570, 122 bytes (+int3 pad to 128), `ret 8`. No direct caller or reference (an
//   unreferenced out-of-line copy of the inline Add; CWoundMorphs' loaders inline it).
// Callee: vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::_M_insert_overflow(pos, x, tag, 1, true) 0x00759190
//   (also called from 0x00757854 / 0x007593AA / 0x0075950F in the CWoundMorphs TU).
// Identity: the 44-byte stride is the landed CWoundMorphs map family (LowerBound 0x00758470, SortMap 0x007597C0):
//   Ego_r.pdb CWoundMorphs::Morphs = CVectorMap<EBodyLocation, CWoundMorphs::CEntry, CKeyPairCompareLess<..> >,
//   CEntry = 40 bytes (Location .. PermanentScarStrength), vector (retail 12 bytes) + Compare +0xc + Dirty +0xd.
//   The catalog's "_Pop_heap_0<CDebugSphere*,CDebugSphere>" is propagated.
// Shape: the CVectorMap::Add family of the landed 0x006FCF40 / 0x006A3DA0: an inline pair constructor builds the
//   44-byte temporary in the frame (key dword, then the 40-byte POD entry with `rep movsd`), the STLport push_back
//   is inlined (placement copy with its null test -- STLport pair's user copy constructor moves the key on its own,
//   then the entry as a 10-dword block -- `add eax,2Ch`; else _M_insert_overflow with the empty tag homed in
//   the dead `key` slot), and Dirty = true closes both paths.
inline void* operator new(unsigned int, void* where) { return where; }

enum EBodyLocation { EBodyLocation_FIRST };

class CWoundMorphs {
public:
    class CEntry {                                          // Ego_r.pdb, 40 bytes
    public:
        EBodyLocation Location;                             // +0x00
        long  ReplacingTextureIndex;                        // +0x04
        long  ReplacingBumpMapIndex;                        // +0x08
        long  WoundTextureIndex;                            // +0x0c
        long  WoundBumpMapIndex;                            // +0x10
        long  ScarTextureIndex;                             // +0x14
        long  ScarBumpMapIndex;                             // +0x18
        long  FadeToScarDuration;                           // +0x1c
        long  TotalDuration;                                // +0x20
        float PermanentScarStrength;                        // +0x24
    };
};

struct __false_type { __false_type() { } };

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct SAllocProxy {                   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    _Tp* _M_data;
};

template <class _Tp> class CRetailVector {                  // STLport vector (12 bytes)
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    SAllocProxy<_Tp> _M_end_of_storage;                     // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00759190
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    CRetailVector<value_type> m_vector;                     // +0x00
    C    Compare;                                           // +0x0c (empty)
    bool Dirty;                                             // +0x0d
    void Add(const K& key, const V& value);
};

template <class K, class V, class C> void CVectorMap<K, V, C>::Add(const K& key, const V& value)
{
    m_vector.push_back(value_type(key, value));
    Dirty = true;
}

template class CVectorMap<EBodyLocation, CWoundMorphs::CEntry, CKeyPairCompareLess<EBodyLocation, CWoundMorphs::CEntry> >;