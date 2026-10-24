// std::basic_string::basic_string() -- STLport 4.x default string constructor: a zeroed _String_base, then an 8-byte block holding just the terminator
// Leaf: ??0basic_string
// Retail: Fable.exe (TLC, VC7.1) @ 0x00566E60, 51 bytes, plain `ret` (thiscall, returns this). No callers: an
//   unreferenced COMDAT instantiation in the networking/scoreboard string TU (0x00566xxx..0x0056Axxx) whose
//   string is the STLport basic_string<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes). The catalog
//   label "Vector_InitializeEmpty" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator<char>::allocate; the `n != 0` test folds against the constant 8).
// Shape: STLport 4.5's out-of-class default ctor verbatim: `_String_base(allocator_type())` zeroes the three
//   words (the end-of-storage word through the _STLP_alloc_proxy ctor), then the body allocates 8 bytes,
//   sets start/finish/end and writes the terminator (_M_terminate_string -> _M_construct_null). The empty
//   allocator has user-declared ctors so the `allocator_type()` temporary costs no store.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

namespace std {

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

class _String_base {
public:
    typedef allocator<char> allocator_type;
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
};

class basic_string : public _String_base {
public:
    basic_string();
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }
};

basic_string::basic_string()
    : _String_base(allocator_type())
{
    _M_start = _M_end_of_storage.allocate(8);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + 8;
    _M_terminate_string();
}

}