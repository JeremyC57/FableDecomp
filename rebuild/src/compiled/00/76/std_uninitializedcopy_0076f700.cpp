// std::__uninitialized_copy(const SStringPair* first, const SStringPair* last, SStringPair* result, const __false_type&) -- pair<CCharString, CCharString> elements: two string copies per element, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@UCCharString@FableCopy_0076f700@@U12@@FableCopy_0076f700@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076F700, length 58 bytes, `ret 8`.
// Element: pair<CCharString, CCharString> (8 bytes): STLport's pair copy constructor runs the two out-of-line string
// copies (`push esi ; mov ecx,edi` then the +4 pair). Caller: 0x007704A0 (vector insert/reallocation).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_0076f700 {
struct CCharString {                                    // 4 bytes: the shared string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30 (out of line)
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<CCharString, CCharString> SStringPair;
}
namespace std {
FableCopy_0076f700::SStringPair* __fastcall __uninitialized_copy(const FableCopy_0076f700::SStringPair* first, const FableCopy_0076f700::SStringPair* last,
    FableCopy_0076f700::SStringPair* result, const __false_type&)
{
    FableCopy_0076f700::SStringPair* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_0076f700::SStringPair(*first);
    return cur;
}
}