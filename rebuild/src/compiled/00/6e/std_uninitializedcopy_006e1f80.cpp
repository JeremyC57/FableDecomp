// std::__uninitialized_copy(const SParamDescEntry* first, const SParamDescEntry* last, SParamDescEntry* result, const __false_type&) -- pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > elements: key, vtable, two words
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@KU?$CParticleUpdateParamDesc@KK@FableCopy_006e1f80@@@FableCopy_006e1f80@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E1F80, length 64 bytes, `ret 8`.
// Element: 16 bytes = pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >,
// the entry of CParticleUpdateParameters<unsigned long, unsigned long>::UpdateParams (a CVectorMap). The vtable
// 0x0125FEE4 stored at +4 is CParticleUpdateParamDesc's (landed ctor 0x00B0F310, fill 0x00B0F6B0). Its implicit copy
// constructor stores only the derived vtable (the CBase vtable store is dead) and copies the two words at +8/+0xC
// through the second pointer pair at +0xC (`lea esi,[eax+0Ch] ; lea edi,[ecx+0Ch]`).
// Callers: 0x006E2000 region (vector assign) and the landed 0x006E22F0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006e1f80 {
struct CBase {                                          // root with a vptr only (ctor 0x0099A2D0 not used here)
    virtual void v00();
};
template <class T1, class T2>
struct CParticleUpdateParamDesc : CBase {               // 12 bytes, vtable 0x0125FEE4
    virtual void v00();
    void* m_pName;                                      // +0x04
    void* m_pParam;                                     // +0x08
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > SParamDescEntry;
}
namespace std {
FableCopy_006e1f80::SParamDescEntry* __fastcall __uninitialized_copy(const FableCopy_006e1f80::SParamDescEntry* first, const FableCopy_006e1f80::SParamDescEntry* last,
    FableCopy_006e1f80::SParamDescEntry* result, const __false_type&)
{
    FableCopy_006e1f80::SParamDescEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006e1f80::SParamDescEntry(*first);
    return cur;
}
}