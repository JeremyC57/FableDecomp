// _STL::__destroy_aux(CScriptRecord16* first, CScriptRecord16* last, const __false_type&) -- destroy each 16-byte polymorphic script record in [first, last) through its virtual destructor
// Leaf: ??$__destroy_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD2725, 28 bytes (a /Os module: unaligned entry, the loop entered at its test),
//   /Gr `__fastcall` (first in ecx -> esi, last in edx -> edi), `ret 4` (the tag reference).
// Identity: STLport `__destroy_aux(first, last, __false_type)` at a 0x10 stride with a virtual destructor (slot 0, flag
//   0). Callers: the `_Destroy(first, last)` forwarders 0x00CD2BE2 / 0x00CD2E4D and the vector destructor 0x00CD319B
//   (shared by eight quest-script and libpk-side vectors, 0x00CCC91C..0x00DE4C8E: an ICF-folded body); the TU's element
//   copy 0x00CD2AE2 runs CScriptThing's copy constructor 0x004ABE90 and copies a byte at +0xc. No PDB name pins the
//   element, so it is a descriptive polymorphic 16-byte record ("descriptive name"). The catalog's
//   "vector<CIntelligentPointer<CThing const>>::_Destroy" is propagated (that element is 8 bytes).
// Shape: `for (; first != last; ++first) _Destroy(&*first);` in a size TU (`jmp` to the bottom test).
#pragma optimize("s", on)
class CScriptRecord16 {                                 // descriptive: vptr + 12 bytes
public:
    virtual ~CScriptRecord16();
    unsigned char _body[0xc];                           // +0x04
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall __destroy_aux<CScriptRecord16*>(CScriptRecord16*, CScriptRecord16*, const __false_type&);
}