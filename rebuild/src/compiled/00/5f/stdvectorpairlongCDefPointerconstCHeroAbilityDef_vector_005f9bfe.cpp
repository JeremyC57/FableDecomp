// std::vector<pair<long,CDefPointer<const CHeroAbilityDef> > >::vector(unsigned int n, const pair<long,CDefPointer<const CHeroAbilityDef> >& value, const allocator& a) -- STLport fill constructor: allocate n slots through _Vector_base and copy-construct value into each
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F9BFE, 44 bytes, `ret 0Ch`; caller: the CTCInventoryAbilities constructor
//   0x005F3BCF (the Abilities member, retail +0x160: vector<pair<long, CDefPointer<const CHeroAbilityDef> > >).
// Callees: _Vector_base(size_t, const allocator&) 0x005F885C (8-byte elements; same-TU body below), the out-of-line
//   `__uninitialized_fill_n(first, n, x, const __false_type&)` 0x005F9379 (landed; it placement-copies through the pair
//   copy constructor 0x005F8578).
// Label correction: the catalog `Vector_InitializeContent` is behavioural; the body is STLport
//   `vector(size_type n, const _Tp& value, const allocator_type& a) : _Vector_base(n, a)
//   { _M_finish = uninitialized_fill_n(_M_start, n, value); }` with the inline front end dispatching on __false_type.
// Shape: frame pointer kept in a size TU (`#pragma optimize("y", off)` + `("s", on)`); the empty `__false_type`
//   temporary is homed in the high byte of n's slot (`lea eax,[ebp+0Bh]`).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CDefPointeeBase {                    // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy() const;           // slot 1
    mutable long m_refCount;                // +0x04
};

struct CHeroAbilityDef : CDefPointeeBase { };

template <class T> struct CDefPointer {
    T* Object;                              // +0x00
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};

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
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&);   // 0x005F9379

template <class ForwardIter, class Size, class T>
__forceinline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T, class Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int n, const Alloc& a);                                      // 0x005F885C
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
    vector(size_type n, const T& value, const allocator_type& a);
};

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const T& value, const allocator_type& a) : _Vector_base<T, Alloc>(n, a)
{
    this->_M_finish = uninitialized_fill_n(this->_M_start, n, value);
}

template class vector<pair<long, CDefPointer<const CHeroAbilityDef> > >;
}