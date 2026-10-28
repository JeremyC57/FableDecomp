// std::vector<basic_string>::_M_insert_overflow(basic_string* pos, const basic_string& x, const __false_type&, size_type fill_len, bool atend) -- STLport vector growth: move into a max(size, n)+size block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569E30, 225 bytes, thiscall `ret 14h`. Callers: vector<string>::push_back
//   0x0056A2D0 (`_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`) and the string-vector inserts of the
//   scoreboard/HTTP TU. The catalog label "Vector_ReallocateStructArray" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate, 12-byte strings), __uninitialized_copy @ 0x00568E70 (x2) and
//   __uninitialized_fill_n @ 0x00568F10 (this chunk's rows), basic_string::_M_range_initialize(const char*, const
//   char*, const forward_iterator_tag&) @ 0x00567D00 (landed; the fill_len == 1 copy construct), _M_clear()
//   @ 0x00569A40 (landed: destroy the old strings, free the block).
// Layout: STLport vector {_M_start, _M_finish, _M_end_of_storage}; basic_string<char> 12 bytes.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` verbatim: `old_size = size(); len = old_size +
//   (max)(old_size, fill_len)` (max through references: the `lea ecx` pick between the spilled size and the
//   fill_len slot), allocate, copy the prefix, `_Copy_Construct` when fill_len == 1 else fill_n, copy the suffix
//   unless atend, `_M_clear()`, `_M_set(..)`. The tag-taking callees carry same-TU `__declspec(noinline)` bodies
//   that never touch the tag, so VC7.1 homes every `__false_type()` / tag temporary on the live `atend` slot
//   (GOTCHAS 0x4ADB50); only _M_insert_overflow is graded.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

class _String_base {
public:
    typedef allocator<char> allocator_type;
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;
    unsigned int max_size() const { return (unsigned int)(-1) / sizeof(char) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        } else
            _M_throw_length_error();
    }
    __declspec(noreturn) void _M_throw_length_error() const;                     // 0x00565A30
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
};

class basic_string : public _String_base {
public:
    allocator_type get_allocator() const { return _M_end_of_storage; }
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }
    __declspec(noinline) void _M_range_initialize(const char* __f, const char* __l, const forward_iterator_tag&)
    {                                                                                          // 0x00567D00
        int __n = __l - __f;
        _M_allocate_block(__n + 1);
        _M_finish = __l == __f ? _M_start : (char*)memmove(_M_start, __f, __n) + __n;
        _M_terminate_string();
    }
    void _M_range_initialize(const char* __f, const char* __l) { _M_range_initialize(__f, __l, forward_iterator_tag()); }
    basic_string(const basic_string& __s) : _String_base(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00568E70
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00568F10
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear();                                                                           // 0x00569A40
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<basic_string>::_M_insert_overflow(basic_string*, const basic_string&, const __false_type&,
                                                       unsigned int, bool);

}