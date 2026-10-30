// CVectorMap::Insert(const unsigned long& key, const CParticleUpdateParamDesc& value) -- appends a (key, parameter descriptor) pair to the particle update-parameter map and marks it for re-sorting
// Leaf: ?Insert@?$CVectorMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0FB40, length 126 bytes, `ret 8`.
// Signature from Ego_r.pdb: ?Insert@?$CVectorMap@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@V?$CParticleUpdateCCompare
//   ParamDesc@K@?$CParticleUpdateParameters@KK@2@@@QAEXABKABV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@Z
//   (the CParticleUpdateParameters<K,K> descriptor map).
// Callees: vector<pair<K, CParticleUpdateParamDesc>>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00B0F9F0
//   (catalogued Vector_ReallocateWithNew), CBaseClass::~CBaseClass @ 0x0099A2E0 (the pair temporary's descriptor).
// Layout (Ego_r.pdb CVectorMap; retail vector 12 bytes): Data +0, Compare +0x0C (empty), bool Dirty +0x0D. The element is
//   pair<unsigned long, CParticleUpdateParamDesc<K,K>> (16 bytes): key, then the descriptor = CBaseClass vptr (vtable
//   0x0125FEE4, `.?AV?$CParticleUpdateParamDesc@KK@NParticleEngine@@`), Callback +4, Data +8.
// Shape: `Data.push_back(pair<K,V>(key, value)); Dirty = true;` -- the pair temporary is built in the frame (the
//   descriptor's implicit copy constructor stores the derived vtable and the two dwords), STLport push_back copies it
//   into _M_finish under the placement-new null test (`add [esi+4],10h`) or calls the overflow path with the empty
//   __false_type tag homed in the dead `key` slot; the temporary's implicit destructor calls ~CBaseClass out of line
//   before the Dirty store.
// Label: the catalog's `NParticleEngine_CParticle_InsertEffect` is descriptive.
inline void* operator new(unsigned int, void* where) { return where; }

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {   // the end-of-storage pointer
    _Value _M_data;
};

template <class _Tp, class _Alloc> class vector {         // retail STLport vector (12 bytes)
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
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

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

template <class K, class V, class TCompare> class CVectorMap {  // sorted-on-demand vector map, retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                         // +0x00
    TCompare                  Compare;                      // +0x0C (empty comparator)
    bool                      Dirty;                        // +0x0D
    void Insert(const K& key, const V& value)
    {
        Data.push_back(std::pair<K, V>(key, value));
        Dirty = true;
    }
};

namespace NParticleEngine {

template <class A, class B> class CParticleUpdateParamCallback;

template <class A, class B> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4
public:
    CParticleUpdateParamCallback<A, B>* Callback;           // +0x04
    unsigned long                       Data;               // +0x08
    CParticleUpdateParamDesc(CParticleUpdateParamCallback<A, B>* callback, unsigned long data) : Callback(callback), Data(data) { }
};

template <class A, class B> class CParticleUpdateParameters {
public:
    template <class K> struct CParticleUpdateCCompareParamDesc { };
    template <class K> struct CParticleUpdateCCompareVector { };
};

typedef CParticleUpdateParamDesc<unsigned long, unsigned long> CParamDesc;
typedef CVectorMap<unsigned long, CParamDesc,
    CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> > CParamDescMap;

}   // namespace NParticleEngine

template void NParticleEngine::CParamDescMap::Insert(const unsigned long& key, const NParticleEngine::CParamDesc& value);