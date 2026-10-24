// std::vector<unsigned __int64>::push_back(const unsigned __int64& x) -- STLport append of a 64-bit value: copy into the spare slot, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006181A0, 58 bytes (+6 int3), `ret 4`; caller: one site in the
//   CTCInventoryMap persist routine 0x00606300 (`call 0x51E210` returns edx:eax, both halves are stored to a stack
//   temporary and its address is pushed: the element is a 64-bit value).
// Callee: vector<unsigned __int64>::_M_insert_overflow(position, x, const __true_type&, fill_len, atend) 0x00617370
//   (this chunk; its element step is `sar eax,3`).
// Label correction: the catalog `Vector_PushBack_Pair` is behavioural; the body is STLport
//   `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; }
//   else _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);` for the 8-byte vector whose binary writer is
//   GFSerialiseVectorBinaryOut<unsigned __int64> 0x00612D30 (same persist TU).
// Shape: speed TU (no pragma); the landed 0x004CD2F0 8-byte push_back: placement new under VC's null test, the copy as
//   two dwords through esi/edx, `++_M_finish` as a memory `add [ecx+4],8`; the empty `__true_type` temporary is homed in
//   the dead x argument slot (`lea edx,[esp+0Ch]` after the two constant pushes).
#include <stddef.h>
#include <new>

namespace std {

struct __true_type { __true_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp*                                                _M_start;             // +0x00
    _Tp*                                                _M_finish;            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> >       _M_end_of_storage;    // +0x08
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_t __fill_len,
                            bool __atend = false);                            // 0x00617370
    void push_back(const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::push_back(const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage._M_data) {
        _Construct(_M_finish, __x);
        ++_M_finish;
    }
    else
        _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
}

template void vector<unsigned __int64>::push_back(const unsigned __int64&);

}