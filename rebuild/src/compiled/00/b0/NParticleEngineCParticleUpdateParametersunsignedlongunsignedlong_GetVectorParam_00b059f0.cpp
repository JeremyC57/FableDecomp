// NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>::GetVectorParam(C3DVector& vector, const unsigned long& key) const -- looks up the vector parameter registered for a (non-zero) key and copies it out; false when absent
// Leaf: ?GetVectorParam@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B059F0, length 82 bytes, `ret 8`.
// Signature from Ego_r.pdb: ?GetVectorParam@?$CParticleUpdateParameters@KK@NParticleEngine@@QBE_NAAVC3DVector@@ABK@Z.
// Callees: CVectorMap<K, C3DVector, CParticleUpdateCCompareVector<K>>::LowerBound(const K&) const @ 0x00B05890 (out of
//   line binary search over the 16-byte pair<K, C3DVector> elements).
// Layout (Ego_r.pdb; retail vectors 12 bytes): CParticleUpdateParameters = vptr, VectorParams +4 (CVectorMap: Data +0,
//   Compare +0x0C, Dirty +0x0D), UpdateParams +0x14.
// Shape: `if (key) { it = VectorParams.Find(key); if (it != VectorParams.end()) { vector = it->second; return true; } }
//   return false;` -- the GetUpdateParamDesc twin (0x00B15B50) on VectorParams: Find inlined, the key kept in ebx across
//   the LowerBound call (same-TU noinline stand-in that only reads memory), the C3DVector copied as three dwords from
//   `&it->second` (`add eax,4`).
// Label: the catalog's `CVectorMap_LookupAndCopyVolume` is descriptive.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class C3DVector {
public:
    float X, Y, Z;
};

namespace std {

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {   // the end-of-storage pointer
    _Value _M_data;
};

template <class _Tp, class _Alloc> class vector {         // retail STLport vector (12 bytes)
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                // +0x00
    TCompare           Compare;                             // +0x0C (empty comparator)
    bool               Dirty;                               // +0x0D

    const_iterator end() const { return Data.end(); }
    const_iterator LowerBound(const K& key) const;
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        const_iterator last = end();
        if (it == last || Compare(key, *it))
            return last;
        return it;
    }
};

#pragma auto_inline(off)
// Same-TU body of the binary search (retail instantiates LowerBound in this TU; out-of-line copy @ 0x00B05890 for the vector map): it
// only reads memory, which lets VC7.1 keep the caller's key in ebx across the call as retail does.
template <class K, class V, class TCompare>
__declspec(noinline) typename CVectorMap<K, V, TCompare>::const_iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const_iterator first = Data.begin();
    int len = Data.end() - first;
    while (len > 0) {
        int half = len / 2;
        const_iterator middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}
#pragma auto_inline(on)

namespace NParticleEngine {

template <class A, class B> class CParticleUpdateParamCallback;

template <class A, class B> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4
public:
    CParticleUpdateParamCallback<A, B>* Callback;           // +0x04
    unsigned long                       Data;               // +0x08
};

template <class A, class B> class CParticleUpdateParameters {
public:
    template <class K> struct CParticleUpdateCCompareVector {
        bool operator()(const K& key, const std::pair<K, C3DVector>& entry) const { return key < entry.first; }
    };
    template <class K> struct CParticleUpdateCCompareParamDesc {
        bool operator()(const K& key, const std::pair<K, CParticleUpdateParamDesc<A, B> >& entry) const { return key < entry.first; }
    };
    typedef CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> >                         CVectorParamMap;
    typedef CVectorMap<A, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<A> > CUpdateParamMap;

    virtual ~CParticleUpdateParameters();
    CVectorParamMap VectorParams;                           // +0x04
    CUpdateParamMap UpdateParams;                           // +0x14 (retail; PDB +0x18)

    bool GetVectorParam(C3DVector& vector, const A& key) const;
    bool GetUpdateParamDesc(CParticleUpdateParamDesc<A, B>& desc, const A& key) const;
};

}   // namespace NParticleEngine

namespace NParticleEngine {

template <class A, class B>
bool CParticleUpdateParameters<A, B>::GetVectorParam(C3DVector& vector, const A& key) const
{
    if (key) {
        typename CVectorParamMap::const_iterator it = VectorParams.Find(key);
        if (it != VectorParams.end()) {
            vector = it->second;
            return true;
        }
    }
    return false;
}

template bool CParticleUpdateParameters<unsigned long, unsigned long>::GetVectorParam(C3DVector& vector, const unsigned long& key) const;

}   // namespace NParticleEngine