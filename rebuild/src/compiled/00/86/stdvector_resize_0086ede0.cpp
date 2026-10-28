// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CArray<CRandomAppearanceMorph::CBodyPartMesh> >) STLport resize: shrink by erasing [begin() + new_size, end()) (copy down, destroy the stale tail), or grow by inserting new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@V?$allocator@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@@std@@@std@@QAEXIABV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086EDE0, 104 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<CArray<CRandomAppearanceMorph::CBodyPartMesh> >::resize(unsigned int, const CArray<CRandomAppearanceMorph::CBodyPartMesh>&); evidence: CRandomAppearanceMorph::CRandomAppearanceMorph() 0x0086C4D0 (binding ??0CRandomAppearanceMorph@@QAE@XZ) calls it (0x0086C51B) as BodyParts.resize(3, CArray<CBodyPartMesh>()) on this+0 with a zeroed 12-byte temporary -- Ego_r.pdb CRandomAppearanceMorph::BodyParts is CArray<CArray<CRandomAppearanceMorph::CBodyPartMesh> > at +0; the destroy callee 0x00675850 is bound as __destroy_aux over CArray<CBodyPartMesh>, and FableWin.pdb holds ?resize@?$vector@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@...@std@@QAEXI@Z; the other caller 0x0086EE50 is resize(n).
// Label: the manifest's "resize" is right in kind but propagated from a byte-identical body elsewhere (family J086: the
//   104-byte resize(n, x) of 12-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x0086E950 (erase's __copy_ptrs,
//   __fastcall, `ret 0Ch`); __destroy_aux<T*>(first, last, const __false_type&) 0x00675850 (the bindings' ??$__destroy_aux@PAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@@std@@... spelling) (erase's _Destroy, __fastcall,
//   `ret 4`); _M_fill_insert(pos, n, x) 0x0086ECA0 (insert(end(), n, x), thiscall, `ret 0Ch`).
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
class CRandomAppearanceMorph {                             // partial view: only the nested type is used here
public:
    class CBodyPartMesh;                                   // Ego_r.pdb, 28 bytes
};
template <class T> class CArray : public std::vector<T> {  // 12 bytes in retail (16 in Ego_r.pdb)
};
typedef CArray<CRandomAppearanceMorph::CBodyPartMesh> TElement_0086ede0;
template void std::vector<TElement_0086ede0 >::resize(unsigned int, const TElement_0086ede0&);