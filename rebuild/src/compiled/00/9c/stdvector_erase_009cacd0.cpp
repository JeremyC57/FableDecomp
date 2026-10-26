// std::vector::erase(iterator first, iterator last) -- (vector<NGameText::CTextTag >) STLport range erase: copy [last, end()) down onto first, destroy the stale tail, set the new end; returns first
// Leaf: ?erase@?$vector@VCTextTag@NGameText@@V?$allocator@VCTextTag@NGameText@@@std@@@std@@QAEPAVCTextTag@NGameText@@PAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CACD0, 77 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<NGameText::CTextTag >::erase(NGameText::CTextTag*, NGameText::CTextTag*); evidence: its callers are resize(n, x) 0x009CB7D0, run by NGameText::CTextEntry::ReadFromDataStream 0x009C8DE0 on the entry's tag array (FableWin.pdb ?GetTags@CTextEntry@NGameText@@QBEABV?$CArray@VCTextTag@NGameText@@@@XZ, ?resize@?$vector@VCTextTag@NGameText@@...@std@@QAEXI@Z), and 0x009CB6C0; the 8-byte element { Pos, Name } has its string destroyed at +4; FableWin.pdb holds vector<NGameText::CTextTag>::erase.
// Label: the manifest's "std::vector_resize_skeletal_morph" is propagated / behavioural (family J092: the 77-byte range erases of class elements with one
//   string member; retail has no identical-code folding, so each address is its own element type).
// Callees: std::__copy<NGameText::CTextTag*, NGameText::CTextTag*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x009CA270 (__fastcall,
//   `ret 0Ch`; retail homes the empty tag in erase's LIVE `first` slot, which VC7.1 does only when it sees the same-TU __copy
//   body never touches the tag, so the body is given here), ~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ) on each stale element's +4.
// Shape: STLport 4.6 _vector.h `erase(first, last)`; _Destroy's __destroy_aux loop inline.
class CCharString {                                        // 4 bytes (TLC)
public:
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    void* PStringData;                                     // +0x00
};
namespace NGameText {
class CTextTag {                                           // 8 bytes (Ego_r.pdb)
public:
    long Pos;                                              // +0x00
    CCharString Name;                                      // +0x04
};
}
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>              // 0x009CA270 (same-TU body, out of line)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
} // namespace std
typedef NGameText::CTextTag TElement_009cacd0;
template TElement_009cacd0* std::vector<TElement_009cacd0 >::erase(TElement_009cacd0*, TElement_009cacd0*);