// _STL::vector::push_back(const CResourcePage& x) -- (vector<CGPUScratchBufferTextureManager::CResourcePage>) STLport push_back: copy-construct x at finish and advance, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@UCResourcePage@CGPUScratchBufferTextureManager@@V?$allocator@UCResourcePage@CGPUScratchBufferTextureManager@@@_STL@@@_STL@@QAEXABUCResourcePage@CGPUScratchBufferTextureManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F38860, 99 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: only caller is CGPUScratchBufferTextureManager::AllocteMemoryPage (0x00F3770C, MemoryPages +0x10);
//   catalog "Vector_Insert_Or_Reallocate" is descriptive.
// Callees: the vector<long> copy constructor @ 0x00514160 (inside the inlined implicit CResourcePage copy),
//   vector::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00F38770 (its body is the same-TU template, kept here so
//   VC7.1 homes the empty __false_type() temporary on the live `x` argument slot, `lea edx,[esp+14h]`, as retail).
// Shape: STLport 4.6 push_back (finish compared with end of storage, placement-new null test, finish re-read after the
//   construct); the 0x00F38770 body here is only context (graded separately in that row).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                    // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

namespace _STL {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    typedef _Tp* pointer;
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, __true_type());
    }
    void _M_clear();                                  // 0x00F38710 for CResourcePage (out of line)
    void _M_set(pointer __s, pointer __f, pointer __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Copy_Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
};

}   // namespace _STL

namespace _STL {

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1> inline void _Copy_Construct(_T1* __p, const _T1& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x00F38440
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x00F384E0
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

}   // namespace _STL
class CGPUScratchBufferTextureManager {
public:
    struct CResourcePage {                       // 0x1c bytes in retail (Ego_r.pdb 32: CArray<long> is 16 there)
        long                ScratchPool;           // +0x00
        _STL::vector<long>  ResourceList;          // +0x04
        unsigned char*      NextFreeBufferAddress; // +0x10
        long                FreeSpaceSize;         // +0x14
        bool                EnableAutoRestore;     // +0x18
    };
};
typedef CGPUScratchBufferTextureManager::CResourcePage CResourcePage;

namespace _STL {

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CResourcePage>::push_back(const CResourcePage&);

}   // namespace _STL