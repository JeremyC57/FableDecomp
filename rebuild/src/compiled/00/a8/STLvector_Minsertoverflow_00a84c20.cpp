// _STL::vector::_M_insert_overflow(SKeyedList* position, const SKeyedList& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the (unsigned __int64 key, list<C2DLineF>) pair vector: allocate old size + max(old size, fill_len), copy the head, construct fill_len copies of x, copy the tail unless inserting at the end, destroy and free the old elements
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84C20, 208 bytes, thiscall `ret 14h`.
// Identity: callers 0x00A84FE2 (the landed push_back 0x00A84FB0) and 0x00A85325 (CVectorMap<unsigned __int64,
//   list<C2DLineF> >::Insert 0x00A852B0) in the navigation quad-tree TU. The catalog label "Vector_InsertN_MusicSetEntry"
//   is descriptive (no music type is touched).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x00A83030 (x2), pair copy constructor 0x00A825D0,
//   __uninitialized_fill_n 0x00A830A0, __destroy_aux 0x00A83420 (landed).
// Shape: STLport 4.x `_M_insert_overflow` for a non-POD element (`__false_type`), as the landed 0x00A96B30: the
//   reference-returning `(max)` over the old size in the `push ecx` frame word, `allocate(len)` = `len ? malloc(len * 16)
//   : 0`, every empty tag temporary homed in the `atend` slot (same-TU noinline stand-ins for the tag-taking callees), the
//   one-element `_Construct` calling the out-of-line pair copy constructor behind the placement null test, then `_M_clear`
//   (`_Destroy` -> `__destroy_aux(start, finish, __false_type())`, deallocate) and `_M_set`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (null-tested)

struct _List_node_base;
struct list {                                      // STLport list<C2DLineF>: one header-node pointer
    _List_node_base* _M_node;
    ~list();                                       // 0x005127A0
};

struct SKeyedList {                                // std::pair<unsigned __int64, list<C2DLineF> >, 0x10 bytes
    unsigned __int64 first;                        // +0x00
    list             second;                       // +0x08 (+0x0C padding)
    SKeyedList(const SKeyedList& o);               // 0x00A825D0 (STLport pair's copy constructor, out of line here)
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// Same-TU stand-ins (retail compiled these templates in this TU; VC7.1 homes the empty tag temporaries over the LIVE
// `atend` slot only when it can see that the callee never reads the tag).
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x00A83030
{
    for (; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x00A830A0
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // 0x00A83420
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<SKeyedList>;
}