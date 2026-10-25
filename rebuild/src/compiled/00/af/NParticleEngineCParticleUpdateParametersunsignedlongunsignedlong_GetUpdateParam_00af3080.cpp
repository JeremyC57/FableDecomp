// NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::GetUpdateParam(C3DVector& result, const unsigned long& type, float timeScale, bool flag, unsigned long depth) const -- look the parameter type up in the update-parameter map and let its registered callback produce the vector (at most 10 levels of nesting)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF3080, length 97 bytes (112 with int3 padding).
// Leaf: ?GetUpdateParam@?$CParticleUpdateParameters
// Callees: CVectorMap<unsigned long, CParticleUpdateParamDesc<unsigned long,unsigned long>, ...>::LowerBound(const unsigned
// long&) @ 0x00AF2C50 (thiscall, returns the element pointer), the callback's virtual slot 1 (+4)
// CParticleUpdateParamCallback::GetUpdateParam(C3DVector&, unsigned long, float, bool, const CParticleUpdateParameters&,
// unsigned long).
// Name: FableWin.pdb ?GetUpdateParam@?$CParticleUpdateParameters@KK@NParticleEngine@@QBE_NAAVC3DVector@@ABKM_NK@Z
// (const; C3DVector&, const unsigned long&, float, bool, unsigned long: `ret 0x14`) and the inline
// ?GetUpdateParam@?$CParticleUpdateParamDesc@KK@NParticleEngine@@QBE_NAAVC3DVector@@M_NABV?$CParticleUpdateParameters@KK@2@K@Z.
// Layout: UpdateParams map at +0x14 (data begin +0x14, end +0x18); its pair<unsigned long, CParticleUpdateParamDesc>
// elements are 16 bytes: key +0, desc +4 (vptr), Callback +8, Index +0xc.
// Shape: a zero type fails first (the key read once into ebx and reused by the inline find -- this needs the
// LowerBound template body in the TU under `#pragma auto_inline(off)`, so VC7.1 knows the call writes no memory;
// declaration-only, the key is re-read through the reference after the call: 98v97); the inline find is
// `i = LowerBound(k); (i == end || k < i->first) ? end : i` with end re-read for the final compare; the depth
// limit is tested only after a hit; the desc forwards to the callback with `depth + 1` and `*this`.
struct C3DVector { float X, Y, Z; };

namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
}

template <class T> struct CAllocProxy { T* _M_data; };
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
};
template <class K, class V, class C> struct CVectorMap {   // retail 16 bytes
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    CVector<value_type> Data;                          // +0x00
    C                   Compare;                       // +0x0c empty comparator
    bool                Dirty;                         // +0x0d
    const_iterator LowerBound(const K& key) const;     // 0x00AF2C50
    const_iterator end() const { return Data._M_finish; }
    const_iterator find(const K& key) const
    {
        const_iterator i = LowerBound(key);
        return (i == end() || key < i->first) ? end() : i;
    }
};

#pragma auto_inline(off)
template <class K, class V, class C>
typename CVectorMap<K, V, C>::const_iterator CVectorMap<K, V, C>::LowerBound(const K& key) const
{
    const_iterator first = Data._M_start;
    int len = Data._M_finish - first;
    while (len > 0) {
        int half = len / 2;
        const_iterator middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}
#pragma auto_inline(on)

namespace NParticleEngine {
template <class A, class B> struct CParticleUpdateParameters;

template <class A, class B> struct CParticleUpdateParamCallback {
    virtual ~CParticleUpdateParamCallback();
    virtual bool GetUpdateParam(C3DVector& result, unsigned long index, float timeScale, bool flag,
                                const CParticleUpdateParameters<A, B>& params, unsigned long depth);   // slot 1
};

template <class A, class B> struct CParticleUpdateParamDesc {
    virtual ~CParticleUpdateParamDesc();
    CParticleUpdateParamCallback<A, B>* Callback;      // +0x04
    unsigned long                       Index;         // +0x08
    bool GetUpdateParam(C3DVector& result, float timeScale, bool flag,
                        const CParticleUpdateParameters<A, B>& params, unsigned long depth) const
    {
        return Callback->GetUpdateParam(result, Index, timeScale, flag, params, depth);
    }
};

template <class A, class B> struct CParticleUpdateParameters {        // vtable 0x0125FFDC (CBase-derived)
    template <class K> struct CParticleUpdateCCompareVector { };
    template <class K> struct CParticleUpdateCCompareParamDesc { };
    void* __vftable;                                                                            // +0x00
    CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> >                         VectorParams;   // +0x04
    CVectorMap<A, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<A> > UpdateParams;   // +0x14
    bool GetUpdateParam(C3DVector& result, const A& type, float timeScale, bool flag, unsigned long depth) const;
};

template <class A, class B>
bool CParticleUpdateParameters<A, B>::GetUpdateParam(C3DVector& result, const A& type, float timeScale, bool flag, unsigned long depth) const
{
    if (type) {
        typename CVectorMap<A, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<A> >::const_iterator i = UpdateParams.find(type);
        if (i != UpdateParams.end() && depth < 10)
            return i->second.GetUpdateParam(result, timeScale, flag, *this, depth + 1);
    }
    return false;
}

template bool CParticleUpdateParameters<unsigned long, unsigned long>::GetUpdateParam(C3DVector&, const unsigned long&, float, bool, unsigned long) const;
}