// std::vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::_M_fill_insert(NEnvironmentInternal::CBlendedEnvironmentTheme* __position, unsigned int __n, const NEnvironmentInternal::CBlendedEnvironmentTheme& __x) -- insert n copies of x
// Leaf: ?_M_fill_insert@?$vector@VCBlendedEnvironmentTheme@NEnvironmentInternal@@V?$allocator@VCBlendedEnvironmentTheme@NEnvironmentInternal@@@std@@@std@@QAEXPAVCBlendedEnvironmentTheme@NEnvironmentInternal@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088A180, 322 bytes, thiscall, `ret 0Ch`.
// Identity (proven): copy constructor ??0CBlendedEnvironmentTheme@NEnvironmentInternal@@... 0x008894A0 (bound), destructor ??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ 0x006B5D80 (bound), _M_insert_overflow bound ?_M_insert_overflow@?$vector@VCBlendedEnvironmentTheme@NEnvironmentInternal@@... 0x0088A070, 500-byte stride; callers the vector's resize 0x0088A390 / 0x0088A520.
// Callees: NEnvironmentInternal::CBlendedEnvironmentTheme copy constructor 0x008894A0 and destructor 0x006B5D80 (the x copy), __uninitialized_copy 0x00889700,
//   __copy_backward 0x008898C0, uninitialized_fill_n 0x00889800, fill 0x00889700 (all `__fastcall`), _M_insert_overflow 0x0088A070.
// Shape (family M115, representative 0x0051D0A0): STLport 4.6 vector.c `_M_fill_insert` (the M051/M065 model) with every
//   helper out of line: __copy_backward_ptrs inline over the out-of-line counted __copy_backward (tag reference, null
//   distance pointer), the empty __false_type tags homed in the dead `__n` / `__x` argument slots.
// Header debt: the record is modelled by its retail size (0x1F4) with its copy constructor and destructor.
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme& other);                          // 0x008894A0
    ~CBlendedEnvironmentTheme();                                            // 0x006B5D80
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other);
    unsigned char _data[0x1F4];
};
}
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);
template <class _BI1, class _BI2>
inline _BI2 __copy_backward_ptrs(_BI1 __first, _BI1 __last, _BI2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x);
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if (unsigned int(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            _Tp* __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}
template void vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::_M_fill_insert(NEnvironmentInternal::CBlendedEnvironmentTheme*, unsigned int, const NEnvironmentInternal::CBlendedEnvironmentTheme&);
}