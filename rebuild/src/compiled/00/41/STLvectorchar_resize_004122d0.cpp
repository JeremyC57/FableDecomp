// _STL::vector<char>::resize(unsigned int new_size, const char& x) -- STLport resize of a byte vector: erase the tail past new_size (memmove of the empty tail), or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004122D0, 85 bytes (+ int3 padding), `__fastcall` (this -> edi), `ret 8`.
// Identity: the 1-byte element and the callee: the landed vector<char>::_M_fill_insert 0x004110D0 (FINDINGS chunk 401;
//   the byte-vector network / bank-file code, LHNetworkLib LSocket::Send takes `vector<char>&`). 14 callers (0x00412890,
//   0x0055FA5E, 0x00560C0F, 0x008F946A.., 0x0090AFC0, 0x00996D0E, 0x00AA6680, 0x00AA6885, 0x00AC68F9). The catalog's
//   "vector<CEditRegion>::resize" is propagated (CEditRegion is not a byte).
// Callees: memmove 0x00BFEAE6 (the MSVCR71 import thunk, a plain rel32 call: not dllimport), vector::_M_fill_insert(pos,
//   n, x) 0x004110D0.
// Shape: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size -
//   size(), __x);` -- for a trivially assignable element erase is `__i = __copy_ptrs(__last, _M_finish, __first,
//   __true_type())` = `__copy_trivial`: `(__last == __first) ? __result : (char*)memmove(__result, __first, __last -
//   __first) + (__last - __first)`. Here __last and __first are both _M_finish in one register, and VC7.1 keeps the
//   unfolded `cmp eax,eax ; jne` and `mov esi,eax ; sub esi,eax`; the trivial _Destroy compiles away.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)

namespace _STL {
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __true_type());
}

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x004110D0
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<char>::resize(unsigned int, const char&);
}