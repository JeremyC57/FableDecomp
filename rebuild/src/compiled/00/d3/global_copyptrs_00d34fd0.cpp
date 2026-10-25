// __copy_ptrs(const CFireHeartPatternDef* first, const CFireHeartPatternDef* last, CFireHeartPatternDef* result, const __false_type&) -- STLport assignment copy of a CFireHeartPatternDef range: n = last - first element assignments, returns the end of the destination
// Leaf: ??$__copy_ptrs@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34FD0, 74 bytes, `__fastcall` (first in ecx, last in edx, result and the
//   tag on the stack: `ret 8`). Callers: vector<CFireHeartPatternDef>::operator= @ 0x00D34DEC / 0x00D34E14.
// Label correction: the catalog "std_vector_CConversation_CopyElements" names the wrong element: the 0x34-byte element is
//   CFireHeartPatternDef (CDefClassBase 0x28 bytes + vector<bool> PadActive +0x28; its construct @ 0x00D34EC0 stores
//   vtable 0x012CCC50), not CConversation (Ego_r.pdb 108 bytes, four vectors). 0x00431F10 (manifest
//   "CThingBuildingDef::operator=") is the shared CDefClassBase::operator=, 0x00462E78 ("CSimpleMemoryPool_AssignFrom")
//   is vector<bool>::operator=.
// Callees: CDefClassBase::operator=(const CDefClassBase&) @ 0x00431F10, vector<bool>::operator=(const vector<bool>&) @
//   0x00462E78.
// Shape: speed TU (no pragma). STLport 4.6 `__copy_ptrs(..., __false_type)` with the random-access `__copy` inline:
//   `for (ptrdiff_t n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`; the element's implicit
//   operator= (base part, then PadActive) is inlined; first is kept as result + (first - result) in edi, n in ebx,
//   the divide by 0x34 is the 0x4EC4EC4F multiply. With plain `inline` VC7.1 keeps __copy out of line (the inlined
//   element operator= makes it too big: `push 0 ; lea eax,[esp+8] ; call`, 25 v 74); `__forceinline` on __copy
//   reproduces retail's single body.
class CDefClassBase {                                   // 0x28 bytes in retail (Ego_r.pdb 37 + padding)
public:
    virtual ~CDefClassBase();
    unsigned char _body[0x24];
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector& operator=(const vector& x);                 // 0x00462E78 (vector<bool>)
};
}
class CFireHeartPatternDef : public CDefClassBase {
public:
    std::vector<bool> PadActive;                       // +0x28
};
namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template CFireHeartPatternDef* __fastcall __copy_ptrs<const CFireHeartPatternDef*, CFireHeartPatternDef*>(
    const CFireHeartPatternDef*, const CFireHeartPatternDef*, CFireHeartPatternDef*, const __false_type&);
} // namespace std