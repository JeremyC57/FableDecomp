// std::__copy<CWideString*, CWideString*, int>(CWideString*, CWideString*, CWideString*, const random_access_iterator_tag&, int*) -- assign [first, last) over result, return the end
// Leaf: ??$__copy
// Retail: Fable.exe @ 0x00433CEC, 45 bytes, __fastcall (first in ecx, last in edx; result, the tag and the distance-type
// pointer on the stack, `ret 0Ch`), returns result + n in eax; caller 0x00433D81. Catalog label
// std::_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...> is wrong: the 4-byte elements are assigned with
// CWideString::operator= (0x0099B7D0, landed under that name), i.e. STLport's random-access __copy with a counted loop.
// Layout: CWideString is one 4-byte pointer.
// Opt: size TU (unaligned entry; result is advanced in place on the stack, `add dword ptr [esp+0Ch],4`).
#pragma optimize("s", on)
class CWideString {
public:
    void* PData;
    const CWideString& operator=(const CWideString& s);        // 0x0099B7D0
};
namespace std {
struct random_access_iterator_tag {};
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template CWideString* __fastcall __copy<CWideString*, CWideString*, int>(CWideString*, CWideString*, CWideString*, const random_access_iterator_tag&, int*);
}