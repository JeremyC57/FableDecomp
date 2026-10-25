// _STL::vector::resize(unsigned int new_size, const C3DAnimationSequenceData::C3DVectorWord& x) -- STLport resize of a quantised-position vector: erase the tail past new_size, or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A55670, 85 bytes (+ int3 padding), `__fastcall` (this -> esi, start -> edi), `ret 8`.
// Identity: element is Ego_r.pdb C3DAnimationSequenceData::C3DVectorWord (short X, Y, Z; 6 bytes; the PDB name sets hold
//   vector<C3DAnimationSequenceData::C3DVectorWord>); the erase copy goes to the landed 6-byte __copy 0x00A52120 (catalog
//   "_Copy_impl<C3DAnimationSequenceData::C3DVectorWord*,...>"); the one caller 0x00A557CD is the resize(n) wrapper
//   0x00A557C0. The catalog's "vector<CBrainBehaviour>::resize" is propagated (CBrainBehaviour is 12 bytes).
// Callees: STLport __copy<C3DVectorWord*, C3DVectorWord*, int>(first, last, result, const random_access_iterator_tag&,
//   int*) 0x00A52120 (`__fastcall`: first ecx, last edx; the empty tag by reference into the dead new_size slot),
//   vector::_M_fill_insert(pos, n, x) 0x00A552B0 (catalog "Vector_InsertN_Triangle2").
// Shape: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size -
//   size(), __x);` -- erase is `__i = __copy_ptrs(__last, _M_finish, __first, __false_type()); _Destroy(__i, _M_finish);
//   _M_finish = __i;` (the destroy loop of the trivially destructible record compiles away); size() is the 0x2AAAAAAB
//   reciprocal divide by 6. Byte twin of 0x00A97A50 (vector<C3DTriangle2>, other callees).
class C3DAnimationSequenceData {
public:
    class C3DVectorWord {                               // Ego_r.pdb, 6 bytes (only copied here)
    public:
        short X;                                        // +0x00
        short Y;                                        // +0x02
        short Z;                                        // +0x04
    };
};

namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x00A52120

template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> inline void __fastcall _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator>
inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x00A552B0
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<C3DAnimationSequenceData::C3DVectorWord>::resize(unsigned int, const C3DAnimationSequenceData::C3DVectorWord&);
}