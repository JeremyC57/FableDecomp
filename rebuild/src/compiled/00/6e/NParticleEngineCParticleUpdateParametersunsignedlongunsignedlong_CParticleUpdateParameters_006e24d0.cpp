// NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateParameters() -- CBase part, this class's vtable, then both parameter maps empty and clean
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E24D0, length 44 bytes (48 with int3 padding).
// Leaf: ??0?$CParticleUpdateParameters
// Callees: CBaseClass::CBaseClass() @ 0x0099A2D0. Vtable 0x0125FFDC = CParticleUpdateParameters (rtti-vtables.tsv).
// Layout: Ego_r.pdb CParticleUpdateParameters<unsigned long,unsigned long> (vptr, VectorParams, UpdateParams) with
// the retail 16-byte CVectorMap (STLport 12-byte vector, the empty comparator byte at +0xc, Dirty at +0xd):
// VectorParams +0x04, UpdateParams +0x14 -- the model of the landed CParticleEmitter constructor 0x00AF0380, which
// embeds this class at +0x14.
// Shape: base call, then one `xor eax,eax` zero for the six vector words and the two Dirty bytes (the comparator
// byte is never written), with the vtable store scheduled after the zero.
class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();
};

class C3DVector { public: float X, Y, Z; };

template <class T> struct CAllocProxy {               // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CAllocProxy(T* p) : _M_data(p) {}
};
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class K, class V> class CVectorMap { public:        // retail 16 bytes
    CVector<V> Data;                                   // +0x00
    char       Compare;                                // +0x0c empty comparator
    bool       Dirty;                                  // +0x0d
    CVectorMap() : Dirty(false) {}
};

namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParamDesc { public: A Type; B Value; };

template <class A, class B> class CParticleUpdateParameters : public CBaseClass { public:     // vtable 0x0125FFDC, retail 0x24 bytes
    CVectorMap<A, C3DVector>                         VectorParams;   // +0x04
    CVectorMap<A, CParticleUpdateParamDesc<A, B> >   UpdateParams;   // +0x14
    CParticleUpdateParameters();
    virtual ~CParticleUpdateParameters();
};

template <class A, class B> CParticleUpdateParameters<A, B>::CParticleUpdateParameters()
{
}

template CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateParameters();
}