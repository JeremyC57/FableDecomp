// NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::Update() -- re-sort whichever of the two parameter maps was marked dirty and mark it clean
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076C750, length 61 bytes (64 with int3 padding).
// Leaf: ?Update@?$CParticleUpdateParameters
// Callees: STLport sort over pair<unsigned long, C3DVector> @ 0x0076C000 and over
// pair<unsigned long, CParticleUpdateParamDesc<unsigned long,unsigned long> > @ 0x0076C1A0 (both __fastcall: first in
// ecx, last in edx, the empty comparator by value on the stack -- manifest labels "std::vector_Relocate*" are
// propagated; the bodies compute (last - first) >> 4 and run the introsort).
// Name: FableWin.pdb ?Update@?$CParticleUpdateParameters@KK@NParticleEngine@@QAEXXZ; FableWin also lists the two
// _Sort instantiations over exactly these pair types with the CParticleUpdateCCompareVector<unsigned long> /
// CParticleUpdateCCompareParamDesc<unsigned long> comparators.
// Layout: as the constructor 0x006E24D0 / the landed CParticleEmitter: VectorParams +0x04 (data +4/+8, comparator
// byte +0x10, Dirty +0x11), UpdateParams +0x14 (data +0x14/+0x18, comparator +0x20, Dirty +0x21).
// Shape: each map's inline Sort() -- `if (Dirty) { sort(begin, end, Compare); Dirty = false; }`; the comparator
// byte is zero-extended into the pushed dword.
class C3DVector { public: float X, Y, Z; };

namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class RandomIt, class Compare> void __fastcall sort(RandomIt first, RandomIt last, Compare comp);
}

template <class T> struct CAllocProxy { T* _M_data; };
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
template <class K, class V, class C> class CVectorMap { public:   // retail 16 bytes
    CVector<std::pair<K, V> > Data;                   // +0x00
    C                          Compare;                // +0x0c empty comparator
    bool                       Dirty;                  // +0x0d
    void Sort()
    {
        if (Dirty) {
            std::sort(Data.begin(), Data.end(), Compare);
            Dirty = false;
        }
    }
};

namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParamCallback;
template <class A, class B> class CParticleUpdateParamDesc { public:
    void*                                 __vftable;   // +0x00
    CParticleUpdateParamCallback<A, B>*   Callback;    // +0x04
    unsigned long                         Index;       // +0x08
};

template <class A, class B> class CParticleUpdateParameters { public:        // vtable 0x0125FFDC (CBase-derived)
    template <class K> struct CParticleUpdateCCompareVector { };
    template <class K> struct CParticleUpdateCCompareParamDesc { };
    void* __vftable;                                                                         // +0x00
    CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> >                       VectorParams;   // +0x04
    CVectorMap<A, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<A> > UpdateParams;   // +0x14
    void Update();
};

template <class A, class B> void CParticleUpdateParameters<A, B>::Update()
{
    VectorParams.Sort();
    UpdateParams.Sort();
}

template void CParticleUpdateParameters<unsigned long, unsigned long>::Update();
}