// std::vector<unsigned short>::push_back(const unsigned short& x) -- append one 16-bit element, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@G
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B4A20, 52 bytes, `ret 4` (__fastcall). Catalog name "Vector_Append_uint16"
// (descriptive). Callers 0x00A86AC6, 0x00A86AD8, 0x00F3DE3A. Callee: vector<unsigned short>::_M_insert_overflow(pos, x,
// __true_type(), 1, true) @ 0x005135D0 (catalog "Vector_InsertN_uint16").
// Layout: STLport vector {_M_start +0, _M_finish +4, _M_end_of_storage +8}.
// Shape: STLport 4.6 verbatim: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);` -- the placement new keeps VC7.1's null check, and the
// empty __true_type temporary is homed on the (still live) x argument slot (`lea edx,[esp+0Ch]`), which VC7.1 only does
// when it can see the overflow body never touches the tag: hence the same-TU noinline stand-in body below (it is a
// separate symbol; the real 0x005135D0 reallocates).
inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type {
    __true_type() {}
};

namespace std {
template <class _Tp> class vector {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned long __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};
}

#pragma auto_inline(off)
template <> void std::vector<unsigned short>::_M_insert_overflow(unsigned short* __position, const unsigned short& __x,
                                                                const __true_type&, unsigned long __fill_len, bool __atend)   // 0x005135D0 (stand-in)
{
    if (__atend && __fill_len == 1)
        _M_finish = __position + (__x != 0);
}
#pragma auto_inline(on)

template void std::vector<unsigned short>::push_back(const unsigned short& __x);