// std::__uninitialized_fill_n<std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>*,unsigned int,std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance> >(first, n, x, const __false_type&) -- copy-construct n copies of one (component type, component instance) pair into raw storage and return first + n
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@IU12@@std@@YIPAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF6980, 75 bytes (+5 int3 pad), __fastcall (first ecx, n edx), `ret 8`
//   (x and the empty __false_type tag by reference on the stack), returns the end of the constructed run. No callees.
// Identity: STLport's `__uninitialized_fill_n(__first, __n, __x, __false_type)` -- it RETURNS the advanced pointer and
//   pops 8 bytes; VC7.1 Dinkumware `_Uninit_fill_n(_First, _Count, _Val, _Al, _Nonscalar_ptr_iterator_tag)` returns void
//   and pops 12 -- the fill path of the vector inside CVectorMap<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>
//   (callers 0x00AF7460 _M_fill_insert-shaped "Vector_InsertWithCount_24Byte", 0x00AF7570 "Vector_InsertRangeResize" and
//   0x00AF69D0). Renamed from the stand-in _global::UninitFillN_SPairEPSCTypeCComponentInstance_00af6980 (Dinkumware model).
// Element: the 24-byte pair<EPSCType, CComponentInstance> (first +0, second +4); FableWin.pdb mangles the class
//   `VCComponentInstance@NParticleEngine` (vtable 0x0129F478, RTTI NParticleEngine::CComponentInstance); Ego_r.pdb
//   CComponentInstance (0x14): vptr, Component CCountedPointer<CPSCBase> +4 (Data, Info -> RefCount +0), Enabled bool +0xc,
//   InstanceName ulong +0x10. Its implicit copy constructor is inlined under the placement-new null test (vtable,
//   counted-pointer copy with the addref, then the bool and the name); `&x.second + 4` is strength-reduced into esi.
// Shape: STLport `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` with an unsigned count (`jbe`, `dec ecx`).
#include <new>

namespace NParticleEngine {

enum EPSCType { PSC_TYPE_FIRST = 0 };

class CPSCBase;

}

class CCPPointerInfo {
public:
    unsigned long RefCount;                       // +0
};

template <class T> class CCountedPointer {
public:
    T*              Data;                         // +0
    CCPPointerInfo* Info;                         // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

namespace NParticleEngine {

class CComponentInstance {                        // vtable 0x0129F478
public:
    virtual ~CComponentInstance();
    CCountedPointer<CPSCBase> Component;          // +4
    bool                      Enabled;            // +0xc
    unsigned long             InstanceName;       // +0x10
};

}

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

typedef pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> _PSCComponentPair;
template _PSCComponentPair* __fastcall __uninitialized_fill_n<_PSCComponentPair*, unsigned int, _PSCComponentPair>(_PSCComponentPair*, unsigned int, const _PSCComponentPair&, const __false_type&);

}
