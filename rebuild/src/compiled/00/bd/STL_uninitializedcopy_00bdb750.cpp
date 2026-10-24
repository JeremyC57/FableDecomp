// _STL::__uninitialized_copy(C2DLineF* first, C2DLineF* last, C2DLineF* result, const __false_type&) -- placement copy-construct the 16-byte lines [first, last) into result, return the advanced result
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDB750, 62 bytes, __fastcall (first ecx, last edx), `ret 8` (result + the tag reference).
// Callers: vector<C2DLineF>::_M_allocate_and_copy 0x00BDB9B0 (tag homed in its dead `n` slot) and vector::operator=
//   0x00BDB9F0 (three sites). Catalog label `_Copy_impl<C2DLineF*,C2DLineF*>` is a Dinkumware spelling; TLC is STLport.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`: the placement-new null test on
//   the destination, a memberwise 16-byte copy through ebx, the advanced result in eax.
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVectorF {
public:
    float X, Y;
};

class C2DLineF {                         // two 2D points, 16 bytes
public:
    C2DVectorF Start;                    // +0x00
    C2DVectorF End;                      // +0x08
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) C2DLineF(*__first);
    return __result;
}

template C2DLineF* __fastcall __uninitialized_copy<C2DLineF*, C2DLineF*>(C2DLineF*, C2DLineF*, C2DLineF*, const __false_type&);

} // namespace _STL