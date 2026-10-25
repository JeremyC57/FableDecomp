// std::vector::vector(const vector& x) -- (vector<NEnvironmentInternal::CBlendedEnvironmentTheme>) STLport copy constructor: allocate x.size() 500-byte blended themes and placement-copy x's elements into them
// Leaf: ??0?$vector@VCBlendedEnvironmentTheme@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008899C0, 106 bytes (+6 int3 padding), thiscall, `ret 4`, returns this.
// Identity: the manifest's "std::vector<CFeatDef>::vector" is propagated (Ego_r.pdb CFeatDef is 100 bytes). The 0x1F4
//   stride and the callee are the NEnvironmentInternal::CBlendedEnvironmentTheme vector's (see 0x0088A350): the landed
//   __uninitialized_copy 0x00889880 lists 0x00889A1C in this body as its caller. The ten callers 0x00889EE8..0x0088A89E
//   are the helpers of a vector whose element holds this vector (its copies in regrowth / fill / allocate-and-copy).
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x00889880 (`__fastcall`, the
//   tag by reference).
// Shape: STLport 4.6 `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy((const_pointer)x._M_start, (const_pointer)x._M_finish, _M_start, __false_type()); }` with
//   `_Vector_base(n, a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { _M_start = allocate(n); _M_finish =
//   _M_start; _M_end_of_storage._M_data = _M_start + n; }` -- the three zero stores through `mov eax,0` (the size's
//   flags feed the allocate null test), x's pointers re-read for the out-of-line copy, the tag homed in the dead `x`
//   argument slot.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                            // 0x1F4 bytes in retail (only the size matters here)
public:
    unsigned char Data[0x1F4];
};
}

namespace std {

struct __false_type { __false_type() { } };

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);   // 0x00889880

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

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef const _Tp* const_pointer;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Base(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                               this->_M_start, __false_type());
    }
};

template vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::vector(
    const vector<NEnvironmentInternal::CBlendedEnvironmentTheme>&);

} // namespace std