// std::__uninitialized_copy(first, last, result, const __false_type&) -- (CUnprovenElement_005269b0* -> CUnprovenElement_005269b0*) STLport placement copy-construct of a run of 16-byte CUnprovenElement_005269b0 elements into raw storage, returns the end of the constructed run
// Leaf: ??$__uninitialized_copy@PAVCUnprovenElement_005269b0@@PAV1@@std@@YIPAVCUnprovenElement_005269b0@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005269B0, 62 bytes (+ int3 padding to 64), `ret 8`: /Gr STLport TU (first ecx, last edx;
//   result and the const __false_type& tag on the stack), spelled here as explicit __fastcall.
// Identity (descriptive): std::__uninitialized_copy<CUnprovenElement_005269b0*, CUnprovenElement_005269b0*>; evidence: a 16-byte element (the copy is four dwords); its callers are the inline push_back overflow at 0x00521920 in 0x00521620 (catalog "CEditWorldMap_LoadRivers", reached from 0x00524290 "CThingManager_SaveGameState"; it pushes the 16-byte value at node+0x18 of every map node whose byte at node+0x20 is set into a local vector) and the out-of-line _M_insert_overflow 0x0052B030 (twice: prefix and remainder) of push_back 0x0052B970; no PDB type of that shape could be tied to the map, so the element keeps a descriptive name; both callers pass pointer iterators (`_M_start`/position), hence <T*, T*>
// Label: the manifest's "_Copy_impl<C2DLineF*,C2DLineF*>" is propagated (family H076: six 16-byte-element __uninitialized_copy COMDATs; retail has no
//   identical-code folding, so each address is one instantiation). `ret 8` + the placement-new null test = __uninitialized_copy.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);`; the element's implicit copy constructor
//   copies four dwords through ebx (esi = source, edi = destination); /O2 aligns the loop head (`jmp` over `lea ecx,[ecx]`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CUnprovenElement_005269b0 {                                  // 16 bytes (descriptive: element type not proven)
public:
    unsigned long Word0;
    unsigned long Word1;
    unsigned long Word2;
    unsigned long Word3;
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

template CUnprovenElement_005269b0* __fastcall __uninitialized_copy<CUnprovenElement_005269b0*, CUnprovenElement_005269b0*>(CUnprovenElement_005269b0*, CUnprovenElement_005269b0*, CUnprovenElement_005269b0*, const __false_type&);

}   // namespace std