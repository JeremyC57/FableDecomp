// std::vector<long>::vector(const vector<long>& x) -- STLport vector copy constructor for a 4-byte POD element: size the base from x, then memmove-copy x's elements and set the finish
// Leaf: ??0?$vector@
// Retail: Fable.exe @ 0x005D85DE, 52 bytes, `ret 4` (thiscall, returns this). Only caller: CTCInventoryStats::
// CreateExperienceSliders @ 0x005CE0C1. Label correction: the catalog's "lua_stackspace" is propagated (no Lua here).
// Callees: _Vector_base<long>(size_t n, const allocator<long>&) @ 0x005D82F3 (out of line; catalog "LuaStack_Initialize"),
// STLport __copy_trivial(const void* first, const void* last, void* result) @ 0x00405BA0 (`__fastcall` memmove helper shared
// by every POD copy; catalog "Std_Move_Backward").
// Body: STLport 4.x _vector.h
//     vector(const vector& __x) : _Vector_base<_Tp,_Alloc>(__x.size(), __x.get_allocator())
//     { _M_finish = __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, _IsPODType()); }
// where the POD __uninitialized_copy is the inline forwarder to __copy_trivial.
// Shape: frame-pointer size TU; get_allocator()'s empty result is evaluated first (right-to-left arguments) and homed in the
// dead high byte of x's slot (`lea eax,[ebp+0Bh]`), size() is `(finish - start) >> 2`. The 4-byte POD element type is not
// pinned by the body (`sar eax,2`, memmove copy); modelled as long.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
void* __fastcall __copy_trivial(const void* first, const void* last, void* result);   // 0x00405BA0
namespace std {
struct __true_type {};
template <class T> class allocator {
public:
    allocator() {}
};
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
};
template <class T, class Alloc>
class _Vector_base {
public:
    _Vector_base(size_t n, const Alloc& a);                // 0x005D82F3
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;     // +0x08
};
template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __true_type&)
{
    return (ForwardIter)__copy_trivial(first, last, result);
}
template <class T, class Alloc = allocator<T> >
class vector : public _Vector_base<T, Alloc> {
public:
    typedef size_t size_type;
    typedef Alloc allocator_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector& x)
        : _Vector_base<T, Alloc>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
};
}
template class std::vector<long>;