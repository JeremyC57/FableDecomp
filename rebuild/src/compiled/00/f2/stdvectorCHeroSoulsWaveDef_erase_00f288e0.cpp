// std::vector<CHeroSoulsWaveDef>::erase(iterator first, iterator last) -- STLport range erase: move the tail down, destroy the vacated elements, shrink
// Leaf: ?erase@?$vector@VCHeroSoulsWaveDef@@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAEPAVCHeroSoulsWaveDef@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f288e0, 78 bytes, thiscall, `ret 8`, returns first.
// Identity (probable): its only caller is vector<CHeroSoulsWaveDef>::resize(n, x) 0x00F28870 (L148's callee for the 0x38-byte CHeroSoulsWaveDef, def vtable 0x012FB498; landed under the stand-in CRetailVector_elem56::resize), as erase(begin() + n, end()).
// Callees: __copy_ptrs(last, finish, first, __false_type) 0x00F28930 (/Gr __fastcall, out of line); each vacated element's
//   virtual destructor (vtable slot 0, the scalar deleting destructor with 0).
// Shape (family L150): STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, __false_type()); _Destroy(__i,
//   _M_finish); _M_finish = __i; return __first;` -- the value-initialised empty tag is a zeroed frame byte.
class CHeroSoulsWaveDef {                                        // retail 0x38 bytes, polymorphic (def record)
public:
    virtual ~CHeroSoulsWaveDef();
    unsigned char _body[0x38 - 4];
};
namespace std {
struct __false_type { };
template <class _Tp> class allocator { };
template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&);   // 0x00F28930
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template CHeroSoulsWaveDef* vector<CHeroSoulsWaveDef>::erase(CHeroSoulsWaveDef*, CHeroSoulsWaveDef*);
}