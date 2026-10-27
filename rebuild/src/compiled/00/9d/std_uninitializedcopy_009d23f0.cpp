// std::__uninitialized_copy(CBankFile::CRuntimeData* first, CBankFile::CRuntimeData* last, CBankFile::CRuntimeData* result, const __false_type&) -- copy-construct a range of 12-byte elements into raw storage
// Leaf: ??$__uninitialized_copy@PAVCRuntimeData@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D23F0, 56 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret 8`.
// Identity (probable): its callers are the _M_fill_insert 0x009D3A20 (landed with a stand-in N009d3a20::Element) and _M_insert_overflow 0x009D3580, reached from the bound ?resize@?$vector@VCRuntimeData@CBankFile@@...@Z 0x009D4010 (CBankFile::AllocateBuffers / AddEntryMultipleFiles).
// Shape (family K113, representative 0x004C2AD0): STLport 4.6 __uninitialized_copy(..., const __false_type&): the null-tested
//   placement copy (`test eax,eax ; je`), the trivially copied element as three dword moves.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CBankFile {
public:
    class CRuntimeData {                                 // Ego_r.pdb 12 bytes
    public:
        unsigned long DataOffset;                        // +0x00
        unsigned long DataSize;                          // +0x04
        unsigned char DataType;                          // +0x08
        bool Valid;                                      // +0x09
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CBankFile::CRuntimeData* __fastcall __uninitialized_copy<CBankFile::CRuntimeData*, CBankFile::CRuntimeData*>(CBankFile::CRuntimeData*, CBankFile::CRuntimeData*, CBankFile::CRuntimeData*, const __false_type&);
}