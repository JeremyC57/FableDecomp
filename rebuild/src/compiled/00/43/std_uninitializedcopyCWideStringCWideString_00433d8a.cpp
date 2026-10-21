// std::__uninitialized_copy<CWideString*, CWideString*>(CWideString*, CWideString*, CWideString*, const __false_type&) -- copy-construct [first, last) into raw storage, return the end
// Leaf: ??$__uninitialized_copy
// Retail: Fable.exe @ 0x00433D8A, 41 bytes, __fastcall (first in ecx, last in edx, result + the __false_type tag on the
// stack, `ret 8`), returns the end in eax; callers 0x00433DDB and 0x00433F45. Catalog label
// std::_Uninit_copy<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...> is wrong: the element is 4 bytes and is built
// by the CWideString copy constructor (0x0099B720, landed as CWideString::CWideString) behind a placement-new null test.
// The /O2 instance of the same template is landed at 0x0040DD60.
// Layout: CWideString is one 4-byte pointer (its copy ctor stores [this] = 0, then shares the source's buffer).
// Opt: size TU (unaligned entry; the loop condition is entered with a `jmp`, no 16-byte loop alignment).
#pragma optimize("s", on)
#include <new>
class CWideString {
public:
    void* PData;
    CWideString(const CWideString& s);                   // 0x0099B720
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
template CWideString* __fastcall __uninitialized_copy<CWideString*, CWideString*>(CWideString*, CWideString*, CWideString*, const __false_type&);
}