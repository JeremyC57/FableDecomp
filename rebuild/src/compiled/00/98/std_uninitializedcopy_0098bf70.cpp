// std::__uninitialized_copy(first, last, result, const __false_type&) -- (C4DVector* -> C4DVector*) STLport placement copy-construct of a run of 16-byte C4DVector elements into raw storage, returns the end of the constructed run
// Leaf: ??$__uninitialized_copy@PAVC4DVector@@PAV1@@std@@YIPAVC4DVector@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098BF70, 62 bytes (+ int3 padding to 64), `ret 8`: /Gr STLport TU (first ecx, last edx;
//   result and the const __false_type& tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): std::__uninitialized_copy<C4DVector*, C4DVector*>; evidence: its callers are the _M_insert_overflow 0x0098D240 and the _M_fill_insert 0x0098D360 of the vector that CShaderRenderManager::SetLightGlobal 0x0098B2C0 resizes at this+0x194 (and Initialise 0x0098ACF0 / AllocateLights 0x0098B420 resize alike) -- Ego_r.pdb's CShaderRenderManager::LightGlobals / LightAttenuations / PSConstTable, all CArray<C4DVector>, the class's only 16-byte-element containers; both helpers pass pointer iterators, hence <T*, T*>
// Label: the manifest's "_Copy_impl<C2DLineF*,C2DLineF*>" is propagated (family H076: six 16-byte-element __uninitialized_copy COMDATs; retail has no
//   identical-code folding, so each address is one instantiation). `ret 8` + the placement-new null test = __uninitialized_copy.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);`; the element's implicit copy constructor
//   copies four dwords through ebx (esi = source, edi = destination); /O2 aligns the loop head (`jmp` over `lea ecx,[ecx]`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class C4DVector {                                  // 16 bytes (Ego_r.pdb class)
public:
    float X;
    float Y;
    float Z;
    float W;
};

namespace std {

struct __false_type { __false_type() { } };      // STLport empty tag; passed by const reference (one stack slot)

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template C4DVector* __fastcall __uninitialized_copy<C4DVector*, C4DVector*>(C4DVector*, C4DVector*, C4DVector*, const __false_type&);

}   // namespace std