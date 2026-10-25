// std::string::_M_range_initialize(const char* first, const char* last, const forward_iterator_tag&) -- STLport range initialiser: allocate last - first + 1 bytes, copy the range, terminate it
// Bounds: int3-after-noreturn row, landed against the N+1 span: int3-after-noreturn: byte-exact over the 89-byte row plus the 0xCC VC7.1 emits after the trailing noreturn _M_throw_length_error call (retail 0x00567329 is that int3); land against the 90-byte span
// Leaf: ??$_M_range_initialize@PBD@?$basic_string@D
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005672D0, 89 bytes, thiscall, `ret 0Ch` (first, last, the empty tag);
//   ends in the noreturn _M_throw_length_error call.
// Identity: the manifest's "std::string::string" is close but not it: the body never zeroes the members and returns
//   the finish pointer, not `this` -- it is the member template the range constructor
//   `basic_string(const _CharT* f, const _CharT* l, const allocator_type&)` forwards to (after `_String_base(a)`),
//   `_M_range_initialize(f, l, forward_iterator_tag())`; ~70 call sites in the CTCHeroOnlineScoreboard TU
//   (0x0055EDFA ...). The retail string is 12 bytes {start, finish, end_of_storage}.
// Callees: malloc 0x00BFEA0E (allocator<char>::allocate), memmove 0x00BFEAE6 (__copy_trivial),
//   _String_base::_M_throw_length_error 0x00565A30 (noreturn).
// Shape: `n = l - f; _M_allocate_block(n + 1)` -- STLport 4.6's `if (n <= max_size() + 1 && n > 0)` gives the
//   `cmp esi,-1 ; ja` / `test esi,esi ; jbe` pair to the throw --, start = finish = block, end = block + n + 1;
//   `uninitialized_copy` of chars is `__copy_trivial` (`l == f ? result : memmove(result, f, l - f) + (l - f)`, the
//   `last` argument re-read from its slot), then `*finish = 0`.
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};

template <class _CharT> class char_traits {
public:
};

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}

inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08

    unsigned int max_size() const { return (unsigned int(-1) / sizeof(_Tp)) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }
        else
            _M_throw_length_error();
    }
    __declspec(noreturn) void _M_throw_length_error() const;   // 0x00565A30
    _String_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : public _String_base<_CharT, _Alloc> {
public:
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
    template <class _ForwardIter>
    void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&)
    {
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
};

template void basic_string<char>::_M_range_initialize<const char*>(const char*, const char*, const forward_iterator_tag&);

} // namespace _STL