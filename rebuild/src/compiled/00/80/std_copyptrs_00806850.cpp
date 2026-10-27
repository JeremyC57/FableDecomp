// std::__copy_ptrs(CUnprovenValue_00806850* first, CUnprovenValue_00806850* last, CUnprovenValue_00806850* result, const __true_type&) -- STLport memmove copy of a trivially assignable range; returns the end of the destination
// Leaf: ??$__copy_ptrs@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00806850, 30 bytes, /Gr `__fastcall` (first ecx, last edx; result and the empty
//   __true_type tag on the stack, `ret 8`).
// Types: unproven -- the body never scales by the element size and nothing calls it (an unreferenced COMDAT of the player
//   targeting TU, between std::find<CBaseIntelligentPointer*> 0x008066A0 and CNearestBuildingWithProperty 0x008068B0);
//   0x00806800 and 0x00806850 are byte-identical instantiations, so the element is the per-row placeholder CUnprovenValue_00806850.
// Model: STLport 4.6 stl/_algobase.h `__copy_ptrs(.., __true_type)` returning `(_OutputIter)__copy_trivial(first, last, result)`,
//   memmove @ 0x00BFEAE6.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

struct CUnprovenValue_00806850 { long _M_unproven; };

namespace std {
struct __true_type { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template CUnprovenValue_00806850* __fastcall __copy_ptrs(CUnprovenValue_00806850*, CUnprovenValue_00806850*, CUnprovenValue_00806850*,
                                                         const __true_type&);
}