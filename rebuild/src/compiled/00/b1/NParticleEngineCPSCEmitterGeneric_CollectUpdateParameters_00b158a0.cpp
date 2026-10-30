// NParticleEngine::CPSCEmitterGeneric::CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>& params, const unsigned long& prefix) -- vtable slot 10: registers the generic emitter as the update-parameter callback for the key `prefix` + ".Centre"
// Leaf: ?CollectUpdateParameters@CPSCEmitterGeneric@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B158A0, length 169 bytes, `ret 8`.
// Signature from Ego_r.pdb: ?CollectUpdateParameters@CPSCEmitterGeneric@NParticleEngine@@UAEXAAV?$CParticleUpdateParameters@KK@2@ABK@Z;
//   slot 10 of the CPSCEmitterGeneric vtable 0x012A002C (absolute ref 0x012A0054).
// Callees: CCRC::Calc(unsigned long seed, const void* data, unsigned long length) @ 0x004014A0 (static __fastcall: the CRC
//   of ".Centre" @ 0x012A0364, 7 bytes, continued from `prefix`), CBaseClass::CBaseClass @ 0x0099A2D0,
//   vector<pair<K, CParticleUpdateParamDesc>>::_M_insert_overflow @ 0x00B0F9F0, CBaseClass::~CBaseClass @ 0x0099A2E0.
// Layout: as the CPSCSpline twin @ 0x00B1B380 (UpdateParams +0x14 of the parameters, Dirty +0x0D of the map).
// Shape: the key is computed into a named local BEFORE the descriptor temporary (retail calls Calc first), then the
//   `__forceinline` CVectorMap::Insert of 0x00B0FB40; the CRC stays in ebx.
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
    __forceinline void Insert(const K& key, const V& value)   // inlined into the CollectUpdateParameters callers (out of line @ 0x00B0FB40)
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

struct C3DVector { float X, Y, Z; };

template <class A, class B> class CParticleUpdateParameters {
public:
    template <class K> struct CParticleUpdateCCompareParamDesc { };
    template <class K> struct CParticleUpdateCCompareVector { };
    virtual ~CParticleUpdateParameters();
    CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> >                 VectorParams;   // +0x04
    CVectorMap<A, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<A> > UpdateParams;   // +0x14 (retail; PDB +0x18)
};

template <class A, class B> class CParticleUpdateParamCallback {
public:
    virtual ~CParticleUpdateParamCallback();
};

typedef CParticleUpdateParamDesc<unsigned long, unsigned long> CParamDesc;
typedef CParticleUpdateParameters<unsigned long, unsigned long> CParams;

class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {   // the callback base at offset 0
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void CollectUpdateParameters(CParams& params, const unsigned long& prefix);   // slot 10 (+0x28)
};

}   // namespace NParticleEngine


class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};

namespace NParticleEngine {

class CPSCEmitterGeneric : public CPSCBase {
public:
    virtual void CollectUpdateParameters(CParams& params, const unsigned long& prefix);
};

void CPSCEmitterGeneric::CollectUpdateParameters(CParams& params, const unsigned long& prefix)
{
    unsigned long key = CCRC::Calc(prefix, ".Centre", 7);
    params.UpdateParams.Insert(key, CParamDesc(this, 0));
}

}   // namespace NParticleEngine