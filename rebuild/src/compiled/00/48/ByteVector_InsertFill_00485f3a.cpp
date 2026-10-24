// std::vector<unsigned char>::_M_fill_insert(unsigned char* position, size_t n, const unsigned char& x) -- STLport fill-insert of n copies of x
// Retail 0x00485F3A, 174 bytes, `ret 0Ch` (catalog label "std::deque::insert" / "ByteVector_InsertFill" is a
// propagated placeholder; the body is STLport 4.x vector::_M_fill_insert for a POD element). Used by
// LoadGameStateInternal to grow its byte buffer.
// Body (STLport verbatim, POD flavour): with room for n more bytes, copy x, then either slide the tail up
// (uninitialized copy of the last n bytes, _M_finish += n, backward copy, fill the hole) or fill past the end
// first (uninitialized_fill_n, then copy the old tail after it, fill the old range); without room hand over to
// _M_insert_overflow(position, x, __true_type(), n) (0x00485B6C).
// Callees (all `__fastcall` STLport helpers: first in ecx, second in edx): __copy_trivial @ 0x00405BA0 (the POD
// __uninitialized_copy), __copy_trivial_backward @ 0x00405BC0 (the POD __copy_backward_ptrs), fill(unsigned
// char*, unsigned char*, const unsigned char&) @ 0x0047CE40 (memset; also the POD uninitialized_fill_n).
// Shape: frame-pointer size TU; x_copy lives in the high byte of the `x` slot, elems_after / old_finish - n in
// the dead `n` slot, and the __true_type temporary for _M_insert_overflow overlays the high byte of the still-live
// `position` slot (`lea eax,[ebp+0Bh]`). That last homing needs _M_insert_overflow as a same-TU noinline
// template body with the tag parameter unnamed (GOTCHAS empty-tag rule; the stand-in reads x through its
// reference and never touches the tag).
// Leaf: ?_M_fill_insert@?$vector@E

#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);

namespace std {

struct __true_type { __true_type() {} };

void* __fastcall __copy_trivial(const void* first, const void* last, void* result);            // 0x00405BA0
void* __fastcall __copy_trivial_backward(const void* first, const void* last, void* result);   // 0x00405BC0
void __fastcall fill(unsigned char* first, unsigned char* last, const unsigned char& value);    // 0x0047CE40

template <class _Tp> class vector
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    template <class _Tag> __declspec(noinline) void _M_insert_overflow(_Tp* position, const _Tp& x, const _Tag&,
                                                                       unsigned int fill_len, bool atend = false)
    {
        // Same-TU stand-in for 0x00485B6C: grow, copy the head, fill, copy the tail (reads x, never the tag).
        unsigned int old_size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
        _Tp* new_start = (_Tp*)malloc(len);
        _Tp* new_finish = (_Tp*)__copy_trivial(_M_start, position, new_start);
        for (unsigned int i = 0; i < fill_len; ++i)
            *new_finish++ = x;
        if (!atend)
            new_finish = (_Tp*)__copy_trivial(position, _M_finish, new_finish);
        free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }

    void _M_fill_insert(_Tp* position, unsigned int n, const _Tp& x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(_Tp* position, unsigned int n, const _Tp& x)
{
    if (n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
            _Tp x_copy = x;
            const unsigned int elems_after = _M_finish - position;
            _Tp* old_finish = _M_finish;
            if (elems_after > n) {
                __copy_trivial(_M_finish - n, _M_finish, _M_finish);
                _M_finish += n;
                __copy_trivial_backward(position, old_finish - n, old_finish);
                fill(position, position + n, x_copy);
            }
            else {
                fill(_M_finish, _M_finish + (n - elems_after), x_copy);
                _M_finish += n - elems_after;
                __copy_trivial(position, old_finish, _M_finish);
                _M_finish += elems_after;
                fill(position, old_finish, x_copy);
            }
        }
        else
            _M_insert_overflow(position, x, __true_type(), n);
    }
}

template void vector<unsigned char>::_M_fill_insert(unsigned char*, unsigned int, const unsigned char&);

}
