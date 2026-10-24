// _STL::vector<unsigned char>::vector(unsigned int, const unsigned char&, const allocator<unsigned char>&) -- fill constructor: allocate n bytes (none for n == 0) and set every byte to the value
// Leaf: ??0?$vector@E
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D2A40, length 101 bytes (+ int3 padding), `ret 0Ch`.
// Identity: the catalog label "Vector_InitFill" is descriptive. Six callers (0x009CCB82, 0x009D3204, 0x00A5B5BB,
// 0x00A5B60B, 0x00AC85F1, 0x00F3AF47) build byte vectors this way; the value is loaded `movzx` (unsigned char).
// Callee: MSVCR71 malloc (0x00BFEA0E) through the allocator (n == 0 gives a null start without a call).
// Shape: STLport 4.6 -- `_M_end_of_storage` is an `_STLP_alloc_proxy` (allocator base + _M_data); with three plain
// pointers and a temporary allocator the `mov esi, ecx` scheduled one slot later (101v101). `_Vector_base(n, a)` zeroes the three pointers, allocates, then stores end-of-storage,
// finish and start (`[esi+8]`, `[esi+4]`, `[esi]`); the body's `uninitialized_fill_n` over unsigned char is STLport's
// memset specialisation (`fill(first, first + n, val)` -> the /Oi inline memset with the byte pattern built in
// ebx/eax), and finish = start + n re-reads n from its argument slot after `rep stos`.
#include <string.h>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace _STL
{
    template <class _Tp>
    class allocator
    {
    public:
        allocator() { }
        _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    };

    inline void fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
    {
        unsigned char __tmp = __val;
        memset(__first, __tmp, __last - __first);
    }

    inline unsigned char* uninitialized_fill_n(unsigned char* __first, unsigned int __n, const unsigned char& __val)
    {
        fill(__first, __first + __n, __val);
        return __first + __n;
    }

    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc
    {
    public:
        _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
        _Value _M_data;
    };

    template <class _Tp, class _Alloc>
    class _Vector_base
    {
    public:
        _Vector_base(unsigned int __n, const _Alloc& __a)
            : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
        {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : public _Vector_base<_Tp, _Alloc>
    {
    public:
        vector(unsigned int __n, const _Tp& __val, const _Alloc& __a = _Alloc())
            : _Vector_base<_Tp, _Alloc>(__n, __a)
        {
            this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
        }
    };

    template class vector<unsigned char>;
}