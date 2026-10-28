// std::vector::_M_fill_insert(CNavigationAreaUpdate::CNavigationLayerUpdate* __position, unsigned int __n, const CNavigationAreaUpdate::CNavigationLayerUpdate& __x) -- (vector<CNavigationAreaUpdate::CNavigationLayerUpdate>) STLport fill-insert of n copies of a navigation-layer update: shift in place when the capacity suffices (x copied first), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCNavigationLayerUpdate@CNavigationAreaUpdate@@V?$allocator@VCNavigationLayerUpdate@CNavigationAreaUpdate@@@std@@@std@@QAEXPAVCNavigationLayerUpdate@CNavigationAreaUpdate@@IABV34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051D630, 356 bytes, thiscall, `ret 0Ch`.
// Identity (proven): the landed resize(n, x) 0x0051D800 of vector<CNavigationAreaUpdate::CNavigationLayerUpdate> calls this
//   row as insert(end(), n, x) (its reloc binding is the spelling used here, protected); every helper it calls is a
//   landed or bound helper of that element: fill 0x0051CAA0, __uninitialized_fill_n 0x0051BBA0, the two member vector
//   copy constructors 0x00514EB0 (vector<C2DLineF>) / 0x0051B4C0 (vector<CSwitchableBlockage>), __destroy_aux
//   0x00519EF0 (CSwitchableBlockage). Element (Ego_r.pdb, STLport sizes): {vector<C2DLineF> LineList +0;
//   vector<CSwitchableBlockage> SwitchableBlockages +0xC} = 0x18 bytes (`imul 2AAAAAABh ; sar 2` = /0x18).
// Callees: vector<C2DLineF>(const vector&) 0x00514EB0, vector<CSwitchableBlockage>(const vector&) 0x0051B4C0,
//   __uninitialized_copy 0x0051C590 (twice), __copy_backward 0x0051C710 (__copy_backward_ptrs inline), fill 0x0051CAA0
//   (twice), __uninitialized_fill_n 0x0051BBA0 (the uninitialized_fill_n front end inline), __destroy_aux 0x00519EF0
//   (~x_copy), free 0x00BFEA14 (x_copy's two buffers), _M_insert_overflow 0x0051D400. The free helpers are __fastcall.
// Shape: STLport 4.6 vector.c `_M_fill_insert` verbatim (landed model 0x00A4A3D0); x_copy is the 0x18-byte frame and its
//   implicit destructor runs inline (C2DLineF is trivially destructible, so LineList only frees); the tags sit in the
//   dead __n slot. The tag-taking helpers are same-TU `__declspec(noinline)` bodies.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                              // 0x00519EF0
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    vector(const vector<_Tp, _Alloc>& __x);                                                    // out of line
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);                            // out of line
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x0051D400
    public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    protected:
};
} // namespace std

class C2DLineF { public:                                   // 16 bytes, trivially destructible
    float StartX, StartY, EndX, EndY;
};
namespace std { template <> struct __type_traits<C2DLineF> { typedef __true_type has_trivial_destructor; }; }
class CNavigationAreaUpdate { public:
    class CSwitchableBlockage;                             // retail 16 bytes: unsigned __int64 UniqueID + list<C2DLineF>
    class CNavigationLayerUpdate { public:                 // retail 0x18 bytes (Ego_r.pdb 0x20 with Dinkumware vectors)
    public:
        std::vector<C2DLineF> LineList;                    // +0x00
        std::vector<CSwitchableBlockage> SwitchableBlockages;   // +0x0C
    };
    class CSwitchableBlockage { public:
        unsigned __int64 UniqueID;
        void* LineList;                                    // std::list<C2DLineF> (4 bytes)
        CSwitchableBlockage(const CSwitchableBlockage& other);
        CSwitchableBlockage& operator=(const CSwitchableBlockage& other);
        ~CSwitchableBlockage();
    };
};

namespace std {
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x0051C590
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x0051BBA0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x0051C710
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x0051CAA0
    for (; __first != __last; ++__first)
        *__first = __val;
}

// Same-TU stand-ins for the out-of-line copy constructor (0x00514EB0 / 0x0051B4C0) and assignment (0x00515320 /
// 0x0051BCD0) of the two member vectors: with their bodies in the TU VC7.1 sees that x_copy does not escape, and keeps
// x_copy.SwitchableBlockages._M_start in esi across the destroy call as retail does.
template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>::vector(const vector<_Tp, _Alloc>& __x)
{
    this->_M_start = this->_M_end_of_storage.allocate(__x.size());
    this->_M_finish = __uninitialized_copy(__x._M_start, __x._M_finish, this->_M_start, __false_type());
    this->_M_end_of_storage._M_data = this->_M_start + __x.size();
}
template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
        this->_M_start = this->_M_end_of_storage.allocate(__x.size());
        this->_M_finish = __uninitialized_copy(__x._M_start, __x._M_finish, this->_M_start, __false_type());
        this->_M_end_of_storage._M_data = this->_M_finish;
    }
    return *this;
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

typedef CNavigationAreaUpdate::CNavigationLayerUpdate _Elem;
template void vector<_Elem>::_M_fill_insert(_Elem*, unsigned int, const _Elem&);
} // namespace std