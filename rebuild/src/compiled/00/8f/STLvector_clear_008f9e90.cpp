// _STL::vector::clear() -- empty a vector<char> byte buffer: erase(begin(), end()) with STLport's __copy_trivial inlined
// Leaf: ?clear@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F9E90, 41 bytes, thiscall, `ret`.
// Identity: vector<char>::clear. Both callers sit in LHNetworkLib::LSocket::Send (0x008F8F50: after `send` consumed the
//   whole buffer, and again after the unsent tail was copied out), whose PDB signature takes `vector<char>&`; the PDB
//   name set also lists `?clear@?$vector@DV?$allocator@D@std@@@std@@QAEXXZ`. The catalog label
//   CComponent::SetParentColour is propagated over byte-identical bodies (landed twins 0x005156D0, 0x00616C30).
// Callees: memmove @ 0x00BFEAE6 (never reached with a non-zero length).
// Shape: STLport 4.6 `erase(first, last)` = `__copy_ptrs(last, _M_finish, first, __true_type())` -> __copy_trivial in its
//   `(last == first) ? result : memmove(...) + n` form, the trivial _Destroy, `_M_finish = i`; called as erase(begin(), end())
//   it keeps the never-true `cmp eax,eax ; jne` and the zero-length memmove + `add eax,esi` tail.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

namespace _STL {
struct __true_type { __true_type() {} };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
            + ((const char*)__last - (const char*)__first);
}

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) {}

template <class _Tp> class allocator { public: allocator() {} };

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

template void vector<char, allocator<char> >::clear();
}