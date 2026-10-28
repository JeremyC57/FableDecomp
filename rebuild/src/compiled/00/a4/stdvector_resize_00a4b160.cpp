// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CLipSyncFrameData >) STLport resize: shrink by erasing [begin() + new_size, end()) (copy down, destroy the stale tail), or grow by inserting new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@VCLipSyncFrameData@@V?$allocator@VCLipSyncFrameData@@@std@@@std@@QAEXIABVCLipSyncFrameData@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4B160, 104 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<CLipSyncFrameData >::resize(unsigned int, const CLipSyncFrameData&); evidence: its only caller is resize(n) 0x00A4B1D0 (FableWin.pdb ?resize@?$vector@VCLipSyncFrameData@@V?$allocator@VCLipSyncFrameData@@@std@@@std@@QAEXI@Z), for CLipSyncSampleData::FrameData (Ego_r.pdb CArray<CLipSyncFrameData>); its fill-insert 0x00A4B020 grows through J085's vector<CLipSyncFrameData>::_M_insert_overflow 0x00A4AF00, and the destroy callee 0x00A43F80 is bound over CLipSyncFrameData.
// Label: the manifest's "resize" is right in kind but propagated from a byte-identical body elsewhere (family J086: the
//   104-byte resize(n, x) of 12-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00A4ADB0 (erase's __copy_ptrs,
//   __fastcall, `ret 0Ch`); __destroy_aux<T*>(first, last, const __false_type&) 0x00A43F80 (template spelling; the bindings' non-template ?__destroy_aux@std@@YIXPAVCLipSyncFrameData@@... is wrong) (erase's _Destroy, __fastcall,
//   `ret 4`); _M_fill_insert(pos, n, x) 0x00A4B020 (insert(end(), n, x), thiscall, `ret 0Ch`).
// Shape: STLport 4.6 _vector.h `resize(n, x)` with erase(first, last) inline; the tag temporaries live in the dead new_size slot.
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
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
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
protected:
    public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // out of line
    protected:
};
} // namespace std
class CLipSyncPhoneme;                                     // Ego_r.pdb, 12 bytes
template <class T> class CArray : public std::vector<T> {  // 12 bytes in retail (16 in Ego_r.pdb)
};
class CLipSyncFrameData {                                  // 12 bytes in retail (16 in Ego_r.pdb)
public:
    CArray<CLipSyncPhoneme> Phonemes;                      // +0x00
};
typedef CLipSyncFrameData TElement_00a4b160;
template void std::vector<TElement_00a4b160 >::resize(unsigned int, const TElement_00a4b160&);