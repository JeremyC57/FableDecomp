// std::vector::resize(size_type new_size, const value_type& x) -- (vector<pair<unsigned long, CAnimationEntry> >) STLport resize: shrink by erasing [begin() + new_size, end()) (copy down, destroy the stale tail), or grow by inserting new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@U?$pair@KVCAnimationEntry@@@std@@V?$allocator@U?$pair@KVCAnimationEntry@@@std@@@2@@std@@QAEXIABU?$pair@KVCAnimationEntry@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E39E0, 105 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<pair<unsigned long, CAnimationEntry> >::resize(unsigned int, const pair<unsigned long, CAnimationEntry>&); evidence: CAnimationSet::TransferBinaryIn 0x005DD440 (binding ?TransferBinaryIn@CAnimationSet@@QAEXAAVCMemoryDataInputStream@@@Z) calls it (0x005DD4BE) to size CAnimationSet::Anims (Ego_r.pdb CVectorMap<unsigned long, CAnimationEntry>, i.e. a vector of pair<unsigned long, CAnimationEntry>) with a default pair (CDefStrings -1); the other caller 0x005E3C60 is resize(n); the destroy callee 0x00454404 is bound over pair<unsigned long, CAnimationEntry> (20 bytes in retail), and FableWin.pdb holds ?resize@?$vector@U?$pair@KVCAnimationEntry@@@std@@...@std@@QAEXI@Z.
// Label: the manifest's "resize" is right in kind but propagated from a byte-identical body elsewhere (family J087: the
//   105-byte resize(n, x) of 20/24/40-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x005E18B0 (erase's __copy_ptrs,
//   __fastcall, `ret 0Ch`); __destroy_aux<T*>(first, last, const __false_type&) 0x00454404 (the template spelling; the bindings' non-template ?__destroy_aux@std@@YIXPAU?$pair@KVCAnimationEntry@@@1@... and five invented aliases are not used) (erase's _Destroy, __fastcall,
//   `ret 4`); _M_fill_insert(pos, n, x) 0x005E32F0 (insert(end(), n, x), thiscall, `ret 0Ch`).
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
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // out of line
};
} // namespace std
class CIVCountedPointeeBase;
template <class T> class CIVCountedPointer {               // 4 bytes
public:
    CIVCountedPointeeBase* Object;                         // +0x00
};
class CDefString {                                         // 4 bytes (Ego_r.pdb)
public:
    long TablePos;                                         // +0x00
};
class CAnimComponentMap;
class CAnimationEntry {                                    // 16 bytes (Ego_r.pdb)
public:
    long BankIndex;                                        // +0x00
    CDefString AnimName;                                   // +0x04
    CDefString GroupName;                                  // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0C
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<unsigned long, CAnimationEntry> TElement_005e39e0;
template void std::vector<TElement_005e39e0 >::resize(unsigned int, const TElement_005e39e0&);