// std::__uninitialized_copy<CCharString*, CCharString*>(CCharString*, CCharString*, CCharString*, const __false_type&) -- copy-construct [first, last) into raw storage, return the end
// Leaf: ??$__uninitialized_copy
// Retail: Fable.exe @ 0x0044B120, 44 bytes, __fastcall (first in ecx, last in edx, result + the __false_type tag on the
// stack, `ret 8`), returns the end in eax; callers 0x0052B714 / 0x0052B79B / 0x0052B813 / 0x0052B838. Catalog label
// std::_Uninit_copy<CFeatDef*, ...> is wrong: the 4-byte elements are built by the CCharString copy constructor
// (0x0099EC30) behind a placement-new null test -- the /O2 counterpart of the size-TU CWideString instance 0x00433D8A.
// Layout: CCharString is one 4-byte pointer.
// Shape: /O2 (aligned loop head after a `nop`, condition tested before entry).
#include <new>
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099EC30
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template CCharString* __fastcall __uninitialized_copy<CCharString*, CCharString*>(CCharString*, CCharString*, CCharString*, const __false_type&);
}