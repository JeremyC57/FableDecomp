// std::__uninitialized_fill_n(CScriptEntry* first, unsigned int n, const CScriptEntry& x, const __false_type&) -- copy-construct n range-compressor script entries into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@PAUCScriptEntry@CRangeCompressor@@IU12@@std@@YIPAUCScriptEntry@CRangeCompressor@@PAU12@IABU12@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3B220, 70 bytes, `__fastcall` (first ecx, n edx; x and the empty tag by reference on
//   the stack, `ret 8`), the end pointer in eax; each element copied as one 24-byte block (the implicit trivial copy), the
//   placement new keeping its null test, n counting down.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 453 "landed 0xF3B220 (std::pair<EHeroMorphType,...>) and 0xF3B420 (SRecord24)
//   are CScriptEntry containers"; hand453 README label table): its only caller is the landed
//   std::vector<CRangeCompressor::CScriptEntry>::_M_insert_overflow 0x00F3B330 (call at 0x00F3B3D5), which already binds
//   this Leaf -> 00f3b220 in reloc_bindings; the element is Ego_r.pdb CRangeCompressor::CScriptEntry (24 bytes: Flags,
//   BitCount, RangeBias, RangeShift, ShuffleMask, OrMask). The pair<EHeroMorphType, CTextureMorphEntry> of the old row has
//   the same size and shape (the body is byte-identical to 0x007183E0, where that pair is real).
// Namespace: std as in this TU's landed rows (0x00F3B1D0, 0x00F3B330) and the existing binding.
// Model: the 0x00F3B330 row's STLport pieces.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                    // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class CRangeCompressor {
public:
    struct CScriptEntry {                        // 24 bytes (Ego_r.pdb), all scalars: implicit trivial copy
        unsigned long Flags;                     // +0x00
        long          BitCount;                  // +0x04
        unsigned long RangeBias;                 // +0x08
        long          RangeShift;                // +0x0c
        unsigned long ShuffleMask;               // +0x10
        unsigned long OrMask;                    // +0x14
    };
};
typedef CRangeCompressor::CScriptEntry CScriptEntry;
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CScriptEntry* __fastcall __uninitialized_fill_n(CScriptEntry*, unsigned int, const CScriptEntry&, const __false_type&);
}   // namespace std
