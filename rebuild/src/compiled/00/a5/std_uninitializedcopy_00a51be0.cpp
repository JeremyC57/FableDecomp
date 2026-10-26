// std::__uninitialized_copy(first, last, result, const __false_type&) -- (CQuaternion* -> CQuaternion*) STLport placement copy-construct of a run of 16-byte CQuaternion elements into raw storage, returns the end of the constructed run
// Leaf: ??$__uninitialized_copy@PAVCQuaternion@@PAV1@@std@@YIPAVCQuaternion@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A51BE0, 62 bytes (+ int3 padding to 64), `ret 8`: /Gr STLport TU (first ecx, last edx;
//   result and the const __false_type& tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): std::__uninitialized_copy<CQuaternion*, CQuaternion*>; evidence: its callers are the _M_insert_overflow 0x00A54FA0 and the _M_fill_insert 0x00A55190 of the local vector at [esp+0x74] in 0x00A4EFC0 (catalog "C3DKeyframe_InitialiseAsConstant"): 0x00A55190 resizes it to the sequence's FrameCount at 0x00A4F2FF, and the same vector is assigned (0x00A54BB0 at 0x00A4F543) from the vector at [esp+0x5c] whose first element goes to CQuaternion::IsIdentity 0x00A88CB0 (?IsIdentity@CQuaternion@@QBE_NXZ) -- vector<CQuaternion>, the rotation track; overflow and fill-insert pass pointer iterators, hence <T*, T*> (the operator= copy is the separate <const T*, T*> COMDAT 0x00A51FA0)
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

template CQuaternion* __fastcall __uninitialized_copy<CQuaternion*, CQuaternion*>(CQuaternion*, CQuaternion*, CQuaternion*, const __false_type&);

}   // namespace std