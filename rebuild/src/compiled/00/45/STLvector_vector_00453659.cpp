// std::vector::vector(int first, int last, const allocator& a) -- STLport vector<long>'s iterator-range constructor instantiated with two ints: empty base, then the integral dispatch fills first longs with the value last
// Leaf: ??$?0H@?$vector@JV?$allocator@J@std@@@std@@QAE@HHABV?$allocator@J@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453659, 38 bytes (size-optimised TU, frame pointer, no padding), thiscall,
//   `ret 0xC`.
// Identity: catalog `vector<long,std::allocator<long>_><int>` = STLport `template <class _InputIterator>
//   vector(_InputIterator, _InputIterator, const allocator_type& = allocator_type())` with _InputIterator = int;
//   callers: the landed CPlayerInventoryDef::CPlayerInventoryDef 0x00459AF3 (0x00459B12 / 0x00459B21 / 0x00459B30,
//   which declares 0x00453659 as this constructor).
// Callees: vector<long>::_M_initialize_aux(int n, int value, const __true_type&) 0x00451C3E (landed; `ret 0xC`).
// Shape: `: _Vector_base<_Tp, _Alloc>(__a) { typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
//   _M_initialize_aux(__first, __last, _Integral()); }` -- the three zero stores, then the empty __true_type temporary
//   (user constructor, no store) homed in the top byte of the `last` slot (`lea eax,[ebp+0Fh]`) and both ints pushed
//   from their slots; `this` stays in ecx. The TU is the module's size-optimised one with the frame pointer on
//   (`#pragma optimize("y", off)` + `("s", on)`, as the landed caller 0x00459AF3).
// Label correction: none (the catalog's `vector<long>::vector<int>` is right; spelled here in STLport's namespace).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> struct _Is_integer { typedef __false_type _Integral; };
template <> struct _Is_integer<int> { typedef __true_type _Integral; };

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

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;

    template <class _Integer>
    void _M_initialize_aux(_Integer __n, _Integer __value, const __true_type&);   // 0x00451C3E for <int>

    template <class _InputIterator>
    vector(_InputIterator __first, _InputIterator __last, const allocator_type& __a = allocator_type())
        : _Vector_base<_Tp, _Alloc>(__a)
    {
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_initialize_aux(__first, __last, _Integral());
    }
};

template vector<long>::vector(int, int, const allocator<long>&);

}