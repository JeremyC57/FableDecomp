// std::vector<basic_string>::push_back(const basic_string& x) -- STLport push_back: copy-construct x at the end, or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056A2D0, 80 bytes, thiscall `ret 4`. The catalog label
//   "CTCElectrocutedEffect_PushElement" is descriptive/propagated.
// Callees: basic_string::_M_range_initialize(const char*, const char*, const forward_iterator_tag&) @ 0x00567D00
//   (landed; the in-place copy construct), vector<string>::_M_insert_overflow(pos, x, const __false_type&, 1, true)
//   @ 0x00569E30 (this chunk's row).
// Layout: STLport vector {_M_start, _M_finish, _M_end_of_storage}; basic_string<char> 12 bytes.
// Shape: STLport 4.x verbatim: `if (_M_finish != _M_end_of_storage._M_data) { _Copy_Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the copy constructor
//   inline (placement null test, zeroed _String_base, range initialise out of line); both tag temporaries take
//   the `x` argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
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
    allocator_type get_allocator() const { return _M_end_of_storage; }
    void _M_range_initialize(const char* __f, const char* __l, const forward_iterator_tag&);      // 0x00567D00
    void _M_range_initialize(const char* __f, const char* __l) { _M_range_initialize(__f, __l, forward_iterator_tag()); }
    basic_string(const basic_string& __s) : _String_base(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
};

template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

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
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x00569E30
    void push_back(const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Copy_Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
}

template void vector<basic_string>::push_back(const basic_string&);

}