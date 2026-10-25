// CVectorMap::Add(const EHeroMorphType& key, const CParticleMorphs::CEntry& value) -- CVectorMap<EHeroMorphType, CParticleMorphs::CEntry, CKeyPairCompareLess<..> >: append a {morph type, particle-morph entry} pair to the map's vector and mark the map dirty
// Leaf: ?Add@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081D820, 122 bytes (+int3 pad), `ret 8`. No direct caller or reference (an
//   unreferenced out-of-line copy of the inline CVectorMap::Add; the morph loaders inline it).
// Callee: vector<pair<EHeroMorphType, CParticleMorphs::CEntry> >::_M_insert_overflow(pos, x, tag, 1, true) @ 0x0081C990
//   (`ret 14h`; also called from 0x0081B4E4 / 0x0081CE36 / 0x0081D6BF).
// Label: the catalog's "_Pop_heap_0<CDebugSphere*,CDebugSphere>" is propagated (byte-identical skeleton family).
// Identity: the 40-byte stride with a signed first-dword key is the CParticleMorphs map (Ego_r.pdb CParticleMorphs::Morphs
//   = CVectorMap<EHeroMorphType, CEntry>, CEntry 36 bytes -- the landed GetAllMorphsOfType 0x0081B160 walks the same
//   vector); the retail STLport vector is 12 bytes, so Compare is the byte at +0xC and Dirty the byte at +0xD.
// Shape: the landed CVectorMap::Add 0x00759570 with a 36-byte value: the pair temporary is built FIRST in the frame (key
//   dword, then the entry as `rep movsd` x9), then STLport `push_back(const T&)` inline: the placement `_Construct` with its
//   null test (pair's memberwise user copy ctor), `++_M_finish` re-read from the member, else _M_insert_overflow with the
//   empty __false_type homed in the dead `key` argument slot; `Dirty = true` duplicated into both exits.
inline void* operator new(unsigned int, void* where) { return where; }

enum EHeroMorphType { EHeroMorphType_FIRST };

struct CDefString {                                         // 4 bytes: the definition string-table position
    long TablePos;
};

class CParticleMorphs {
public:
    class CEntry {                                          // Ego_r.pdb, 36 bytes
    public:
        EHeroMorphType MorphType;                           // +0x00
        bool       Blending;                                // +0x04
        long       Location;                                // +0x08 EMeshLocationPoint
        CDefString LocationName;                            // +0x0c
        long       LocationIndex;                           // +0x10
        long       ParticleIndex;                           // +0x14
        long       SecondParticleIndex;                     // +0x18
        float      StartAtMorphStrength;                    // +0x1c
        float      EndAtMorphStrength;                      // +0x20
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x0081C990
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

template <class K, class V> struct CKeyPairCompareLess {
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

template class CVectorMap<EHeroMorphType, CParticleMorphs::CEntry, CKeyPairCompareLess<EHeroMorphType, CParticleMorphs::CEntry> >;