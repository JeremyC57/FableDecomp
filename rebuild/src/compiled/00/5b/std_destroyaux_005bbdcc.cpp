// std::__destroy_aux(CUnprovenRecord_005bbdcc* first, CUnprovenRecord_005bbdcc* last, const __false_type&) -- destroy every element of [first, last)
// Leaf: ??$__destroy_aux@PAVCUnprovenRecord_005bbdcc@@@std@@YIXPAVCUnprovenRecord_005bbdcc@@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BBDCC, 29 bytes, `__fastcall` (first ecx, last edx), `ret 4`.
// Identity (descriptive): 12-byte elements whose deleting destructor 0x005BBA28 frees the first word when set (a vector of trivially destructible items, or a record led by one); its callers are the _Destroy wrappers 0x005BBF71 / 0x005BBFBA and the vector destructor 0x005BC080 used by CTCInventoryClothing (constructor 0x005B76D1, ConstructScreenPC 0x005B8743) -- no Ego_r.pdb member pins the type.
// Callees: the element's scalar deleting destructor 0x005BBA28 (flags 0).
// Shape (family M097, representative 0x004DD79F): STLport `for (; __first != __last; ++__first) _Destroy(&*__first);` in a size
//   TU: `optimize("s")` keeps the element destructor out of line, and VC7.1 calls the class's `??_G` with 0 for `p->~T()`.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CUnprovenRecord_005bbdcc {                          // descriptive: 12 bytes, its first word a malloc'd block
public:
    ~CUnprovenRecord_005bbdcc() { if (Block != 0) free(Block); }   // inline in its deleting destructor 0x005BBA28
    void* Block;
    unsigned long Members[2];
};
namespace std {
struct __false_type { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
typedef ::CUnprovenRecord_005bbdcc _E;
template void __fastcall __destroy_aux<_E*>(_E*, _E*, const __false_type&);
}