// std::vector<CCharString>::erase(CCharString* first, CCharString* last) -- assign the tail down over [first, last), destroy the leftover strings, shrink the end
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043336A, length 73 bytes, `ret 8` (__fastcall), returns `first`.
// Name: STLport 4.x `vector::erase(iterator first, iterator last)` for CCharString (4-byte elements, CCharString::operator= per
// element, ~CCharString range); 34 callers (0x00433347, 0x0047FEC5, ... 0x008AEA08) share this folded body. The catalog's
// "Std_Vector_Erase_Range" is descriptive.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, std::_Destroy<CCharString*>(first, last) @ 0x00414E00 (landed,
// __fastcall: first in ecx, last in edx).
// Pragmas: #pragma optimize("y", off) + #pragma optimize("s", on) -- frame pointer; the copy count lives in the dead `last` slot.
// Shape: STLport verbatim -- `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;`
// with copy for a non-trivially-assignable T* range being the counted random-access loop `for (n = last - first; n > 0; --n)
// { *result = *first; ++first; ++result; }` (`sar eax,2 ; jle`, then the source as `result + (last - first)` in ebx). copy and its
// helpers are __forceinline in this size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
class CCharString {
public:
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
    void* PStringData;
};
namespace std {
template <class ForwardIter> void __fastcall _Destroy(ForwardIter first, ForwardIter last);   // 0x00414E00 for CCharString*
template <class RandomAccessIter, class OutputIter>
__forceinline OutputIter copy(RandomAccessIter first, RandomAccessIter last, OutputIter result)
{
    for (ptrdiff_t n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T> class vector {
public:
    typedef T* iterator;
    typedef T* pointer;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator erase(iterator first, iterator last)
    {
        pointer i = copy(last, _M_finish, first);
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
};
template class vector<CCharString>;
}