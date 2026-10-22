// _global::remove_copy<SPointee**,SPointee**,SPointee*>(SPointee**, SPointee**, SPointee**, SPointee* const&) -- STLport remove_copy over 4-byte elements: copy every element of [first,last) that differs from value to result
// Leaf: remove_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7940, 39 bytes, __fastcall (first in ecx, last in edx; result and the
//   value reference on the stack, `ret 8`), returns the advanced result. Catalogued FUN_007f7940 (unreferenced);
//   sits next to vector_find_Value_16byte 0x007F7970. The element is a pointer (compare by `cmp esi,[edi]`).
struct SPointee_007f7940;                       // the (unidentified) pointee: elements are pointers

template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter __fastcall remove_copy(_InputIter __first, _InputIter __last, _OutputIter __result, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        if (!(*__first == __val)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}

template SPointee_007f7940** __fastcall remove_copy<SPointee_007f7940**, SPointee_007f7940**, SPointee_007f7940*>(SPointee_007f7940**, SPointee_007f7940**, SPointee_007f7940**, SPointee_007f7940* const&);