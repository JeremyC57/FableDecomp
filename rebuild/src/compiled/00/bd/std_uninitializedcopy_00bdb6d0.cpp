// std::__uninitialized_copy(first, last, result, const __false_type&) -- (CVertexShaderConstant* -> CVertexShaderConstant*) STLport placement copy-construct of a run of 16-byte CVertexShaderConstant elements into raw storage, returns the end of the constructed run
// Leaf: ??$__uninitialized_copy@PAVCVertexShaderConstant@@PAV1@@std@@YIPAVCVertexShaderConstant@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDB6D0, 62 bytes (+ int3 padding to 64), `ret 8`: /Gr STLport TU (first ecx, last edx;
//   result and the const __false_type& tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): std::__uninitialized_copy<CVertexShaderConstant*, CVertexShaderConstant*>; evidence: its callers are the inline push_back overflows at 0x00BDBBBA / 0x00BDBC8F in 0x00BDBB70, the CEngineVSConstantLayoutBasic constructor (vtable 0x012A7D40, RTTI .?AVCEngineVSConstantLayoutBasic@@; it pushes the constants {0, 1, 2, 0.5} and {256, 256, 256, 256} into a local vector and assigns it with 0x00BDB9F0 to this+0x140), and 0x00BDB8E0 -- this+0x140 is Ego_r.pdb's CVertexShaderConstantLayout::ReservedPresets, CArray<CVertexShaderConstant>; pointer iterators, hence <T*, T*>
// Label: the manifest's "_Copy_impl<C2DLineF*,C2DLineF*>" is propagated (family H076: six 16-byte-element __uninitialized_copy COMDATs; retail has no
//   identical-code folding, so each address is one instantiation). `ret 8` + the placement-new null test = __uninitialized_copy.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);`; the element's implicit copy constructor
//   copies four dwords through ebx (esi = source, edi = destination); /O2 aligns the loop head (`jmp` over `lea ecx,[ecx]`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CVertexShaderConstant {                                  // 16 bytes (Ego_r.pdb class)
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

template CVertexShaderConstant* __fastcall __uninitialized_copy<CVertexShaderConstant*, CVertexShaderConstant*>(CVertexShaderConstant*, CVertexShaderConstant*, CVertexShaderConstant*, const __false_type&);

}   // namespace std