// std::__uninitialized_copy(const basic_string* first, const basic_string* last, basic_string* result, const __false_type&) -- STLport placement copy of a range of strings into raw storage
// Leaf: ??$__uninitialized_copy
// Bounds: int3-after-noreturn row: RELOCATION_MATCH against the 149-byte row read straight from Fable.exe (byte 148 is the 0xCC VC7.1 emits after the final noreturn _M_throw_length_error call; the chunk row and q.py strip it as padding). Land against an N+1 oracle row (GOTCHAS: int3-after-noreturn rows).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00568E70, 148 bytes (the noreturn length-error call is the last instruction),
//   /Gr `__fastcall` (first in ecx, last in edx, result and the tag on the stack: `ret 8`). Callers:
//   vector<string>::_M_insert_overflow 0x00569E30 (twice). The catalog label "std_uninitialized_copy_range_strings"
//   is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator<char>::allocate), memmove @ 0x00BFEAE6 (__copy_trivial),
//   _String_base<char>::_M_throw_length_error() const @ 0x00565A30 (never returns).
// Layout: STLport basic_string<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` with the
//   whole string copy constructor inline: `_String_base(s.get_allocator())` (three zero stores after the
//   placement null test), `_M_range_initialize(s._M_start, s._M_finish)` = `_M_allocate_block(n + 1)` (the
//   never-true `cmp ebp,-1 ; ja` then `jbe` on zero), `uninitialized_copy` -> `__copy_trivial` (memmove when
//   non-empty) and the terminator. first/last live in the two frame words, the source's finish in the dead
//   `result` argument slot.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };

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

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

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
    void _M_range_initialize(const char* __f, const char* __l)
    {
        int __n = __l - __f;
        _M_allocate_block(__n + 1);
        _M_finish = uninitialized_copy(__f, __l, _M_start);
        _M_terminate_string();
    }
    basic_string(const basic_string& __s) : _String_base(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template basic_string* __fastcall __uninitialized_copy<const basic_string*, basic_string*>(
    const basic_string*, const basic_string*, basic_string*, const __false_type&);

}