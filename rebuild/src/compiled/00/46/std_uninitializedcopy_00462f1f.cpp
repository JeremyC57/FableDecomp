// std::__uninitialized_copy(const bool* first, const bool* last, bool* result, const __true_type&) -- STLport POD uninitialized copy over bool: one memmove of [first, last) onto result; returns the end of the copied run
// Leaf: ??$__uninitialized_copy@PB_NPA_N@std@@YIPA_NPB_N0PA_NABU__true_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462F1F, 35 bytes, __fastcall (first ecx, last edx; result and the empty tag reference on the stack), `ret 8`; size-optimised algorithm TU.
// Identity (proven): std::__uninitialized_copy<const bool*, bool*>; evidence: its callers are vector<bool>::operator= 0x00462E78 (the `size() < x.size() <= capacity()` arm: copy-assign the head inline, then uninitialized-copy the tail, through const_pointer) and 0x00462F50 (_M_allocate_and_copy, its only caller the operator=); that operator= assigns 1 Ego_r.pdb vector<bool> def members (def operator= through the slot-19 thunks, offsets rebased to retail), e.g. CPlayerInventoryDef::IsUsable, and no member of another type.
// Label: the manifest's "Copy_MemoryRange" is descriptive (family H056: six POD __uninitialized_copy instantiations, one per element type and pointer constness).
// Callee: memmove 0x00BFEAE6 (__copy_trivial).
// Shape: STLport 4.6 `__uninitialized_copy(first, last, result, const __true_type&)` -> `__copy_aux` -> `__copy_ptrs(..., __true_type)` ->
//   `__copy_trivial` (`last == first ? result : memmove(result, first, n) + n`), all inline; `__fastcall` on the template definition (the /Gr
//   algorithm TUs, as the landed 0x00ADC330), the empty-range arm jumping to the shared `ret 8`. VC7.1 keeps the plain-`inline` chain out of
//   line here (one cdecl call), so the three helpers are `__forceinline` (as the landed __copy in 0x00673700).
#pragma optimize("s", on)
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

namespace std {

struct __true_type { __true_type() { } };

__forceinline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __true_type&)
{
    return __copy_aux(__first, __last, __result, __true_type());
}

template bool* __fastcall __uninitialized_copy<const bool*, bool*>(const bool*, const bool*, bool*, const __true_type&);

}