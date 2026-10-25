// std::vector<unsigned char>::push_back(const unsigned char& x) -- STLport vector push_back of a byte vector: store x at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CB4F, length 48 bytes, `ret 4` (thiscall on the vector, &x).
// Callees: vector<unsigned char>::_M_insert_overflow(pos, x, const __true_type&, 1, true) @ 0x00467831 (landed).
// Callers: NUISystem::CList 0x0053835E, 0x0053840F, 0x0053B092, 0x0053BEDE, 0x0053C2AB and 0x00CDC98C.
// Label: catalog "std::deque_push_back_or_allocate" is descriptive; the body is STLport vector::push_back over a 1-byte
//   POD (the landed _M_insert_overflow it calls is vector<unsigned char>'s).
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the placement copy is null tested (`test eax,eax`),
//   the __true_type temporary sits in the high byte of the `x` argument slot (`lea edx,[ebp+0Bh]`). Size-optimised
//   frame-pointer TU (`push ebp ; mov ebp,esp`, unaligned entry: `optimize("y", off)` + `("s", on)`), as the landed
//   CUIStateMap push_back 0x0053CFC2 of the same CList TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type {
    __true_type() {}
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x00467831
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

template void vector<unsigned char>::push_back(const unsigned char& x);
}