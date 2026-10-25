// _STL::__uninitialized_copy(CScriptEntry* first, CScriptEntry* last, CScriptEntry* result, const __false_type&) -- (CRangeCompressor::CScriptEntry) copy-construct [first, last) into raw storage at result, returning the end
// Leaf: ??$__uninitialized_copy@PAUCScriptEntry@CRangeCompressor@@PAU12@@_STL@@YIPAUCScriptEntry@CRangeCompressor@@PAU12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3B1D0, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx), `ret 8`.
// Identity: called twice by vector<CScriptEntry>::_M_insert_overflow 0x00F3B330, whose only caller is push_back
//   0x00F3B420, called three times from CRangeCompressor::CalcCompressionScript 0x00F3ABF0 (recursive, `ret 14h` = its five
//   arguments; it appends to the script CArray<CScriptEntry>). The 24-byte element is Ego_r.pdb CRangeCompressor::
//   CScriptEntry {Flags, BitCount, RangeBias, RangeShift, ShuffleMask, OrMask}. The catalog's
//   "_Move<CTVertexLandscapeBackground*,...>" is a propagated label.
// Shape: STLport 4.6 __uninitialized_copy(..., __false_type) (no compiler POD traits in VC7.1, so the default
//   __false_type path): placement-new null test, the implicit trivial copy as one 24-byte block through esi/edi/ebx.
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

namespace _STL {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1> inline void _Copy_Construct(_T1* __p, const _T1& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CScriptEntry* __fastcall __uninitialized_copy(CScriptEntry*, CScriptEntry*, CScriptEntry*, const __false_type&);

}   // namespace _STL