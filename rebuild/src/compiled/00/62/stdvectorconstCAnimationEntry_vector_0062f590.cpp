// std::vector<const CAnimationEntry*>::vector(const vector& x) -- STLport vector copy constructor for the combat action's animation list: allocate x.size() pointers through the base, then copy x's elements with the trivial memmove copy
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F590, 52 bytes (+ int3 padding), `ret 4`.
// Identity: callers 0x0062F527 (the landed CCombatActionBase copy constructor 0x0062F3B0 copying PDB
// CCombatActionBase::CombatAnimations, a vector<const CAnimationEntry*>) and 0x0087D4B1. The catalog label
// "lua_stackspace" is wrong.
// Callees: _Vector_base<const CAnimationEntry*>::_Vector_base(size_t n, const allocator_type&) @ 0x0062F5D0 (out of line,
// `ret 8`: zero the three pointers, malloc n*4, publish), STLport __copy_trivial(first, last, result) @ 0x00405BA0
// (__fastcall, `ret 4`).
// Shape: STLport 4.x `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
// __uninitialized_copy(x.begin(), x.end(), _M_start, _IsPODType()); }`; for a pointer element the __true_type chain
// reduces to __copy_trivial. The by-value allocator temporary is homed in the dead `x` argument slot
// (`lea eax,[esp+0Ch]`), x stays in edi; size() is `(finish - start) >> 2`.
class CAnimationEntry;

void* __fastcall __copy_trivial(const void* __first, const void* __last, void* __result);   // 0x00405BA0

namespace std {

struct __true_type { };

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
};

template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(unsigned int __n, const allocator_type& __a);   // 0x0062F5D0
    _Tp* _M_start;                                               // +0x00
    _Tp* _M_finish;                                              // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    const _Tp* begin() const { return this->_M_start; }
    const _Tp* end() const { return this->_M_finish; }

    vector(const vector<_Tp, _Alloc>& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy(__x.begin(), __x.end(), this->_M_start, __true_type());
    }
};

} // namespace std

template std::vector<const CAnimationEntry*>::vector(const std::vector<const CAnimationEntry*>& __x);