// std::vector<EHeroAbility>::vector(unsigned int n) -- STLport sized constructor: allocate n slots through the _Vector_base constructor and fill them with value-initialised (zero) abilities
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F8EC0, 47 bytes, `ret 4`; caller 0x005EF308 (CTCInventoryAbilities TU).
// Callees: _Vector_base(size_t, const allocator&) 0x005F8828 (out of line; same-TU body below), uninitialized_fill_n(first,
//   n, x) 0x005F8220 (landed `_Fill_n<unsigned long*,...>`: the 3-argument front end with the placement-new null test, so
//   the element is a non-POD-for-traits 4-byte type -- an enum such as EHeroAbility; `long` takes STLport's POD path).
// Label correction: the catalog `std_Vector_ConstructN_4Byte` is behavioural; the body is STLport
//   `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish = uninitialized_fill_n(_M_start, n, _Tp()); }`.
// Shape: frame pointer kept in a size TU (`#pragma optimize("y", off)` + `("s", on)`, `leave`); the `_Tp()` zero is the one
//   frame word (`and dword ptr [ebp-4],0`). The empty allocator temporary is homed in the high byte of n's still-live
//   slot (`lea eax,[ebp+0Bh]`) only when the _Vector_base constructor has a same-TU `auto_inline(off)` body in STLport's
//   real form (the allocator copied into the `_STLP_alloc_proxy` end-of-storage member, never written); with only a
//   declaration VC7.1 gives the temporary its own frame word (48 v 47).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class Value, class T, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) { }
};

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x);   // 0x005F8220

template <class T, class Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int n, const Alloc& a);                                      // 0x005F8828
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

#pragma auto_inline(off)
template <class T, class Alloc>
_Vector_base<T, Alloc>::_Vector_base(unsigned int n, const Alloc& a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
{
    _M_start = _M_end_of_storage.allocate(n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + n;
}
#pragma auto_inline(on)

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    typedef Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(size_type n);
};

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n) : _Vector_base<T, Alloc>(n, allocator_type())
{
    this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
}

template class vector<EHeroAbility>;
}