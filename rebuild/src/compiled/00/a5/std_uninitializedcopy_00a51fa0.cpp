// std::__uninitialized_copy(first, last, result, const __false_type&) -- (const CQuaternion* -> CQuaternion*) STLport placement copy-construct of a run of 16-byte CQuaternion elements into raw storage, returns the end of the constructed run
// Leaf: ??$__uninitialized_copy@PBVCQuaternion@@PAV1@@std@@YIPAVCQuaternion@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A51FA0, 62 bytes (+ int3 padding to 64), `ret 8`: /Gr STLport TU (first ecx, last edx;
//   result and the const __false_type& tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): std::__uninitialized_copy<const CQuaternion*, CQuaternion*>; evidence: its callers are 0x00A54BB0 (twice: the reallocating and the tail branch) and 0x00A535B0 -- 0x00A54BB0 is the operator= that 0x00A4EFC0 (catalog "C3DKeyframe_InitialiseAsConstant") applies at 0x00A4F543 / 0x00A4F599 to its local vector<CQuaternion> from the vector whose first element it passes to CQuaternion::IsIdentity 0x00A88CB0; STLport's operator= and copy constructor copy from `(const_pointer)__x._M_start`, hence <const T*, T*> (the overflow / fill-insert copy is the separate <T*, T*> COMDAT 0x00A51BE0)
// Label: the manifest's "_Copy_impl<C2DLineF*,C2DLineF*>" is propagated (family H076: six 16-byte-element __uninitialized_copy COMDATs; retail has no
//   identical-code folding, so each address is one instantiation). `ret 8` + the placement-new null test = __uninitialized_copy.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);`; the element's implicit copy constructor
//   copies four dwords through ebx (esi = source, edi = destination); /O2 aligns the loop head (`jmp` over `lea ecx,[ecx]`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CQuaternion {                                  // 16 bytes (Ego_r.pdb class)
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

template CQuaternion* __fastcall __uninitialized_copy<const CQuaternion*, CQuaternion*>(const CQuaternion*, const CQuaternion*, CQuaternion*, const __false_type&);

}   // namespace std