// std::__copy(CThingTypeInfo* first, CThingTypeInfo* last, CThingTypeInfo* result, const random_access_iterator_tag&, int*) -- (CThingTypeInfo) STLport random-access copy: assign last - first elements forward through CThingTypeInfo's implicit operator=, return the end of the destination
// Leaf: ??$__copy@PAVCThingTypeInfo@@PAV1@H@std@@YIPAVCThingTypeInfo@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00527EB0, 80 bytes (+ int3 padding), /Gr __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (proven): std::__copy<CThingTypeInfo*, CThingTypeInfo*, int>; evidence: its callers are vector<CThingTypeInfo>::erase(iterator, iterator) 0x0052B100 (binding ?erase@?$vector@VCThingTypeInfo@@V?$allocator@VCThingTypeInfo@@@std@@@std@@QAEPAVCThingTypeInfo@@PAV3@0@Z: erase's __copy_ptrs(last, finish, first) over plain pointers) and 0x00528890; the 12-byte element copies DefType +0 and ThingTypeFlags +4 inline and DefClassType +8 through CCharString::operator=, Ego_r.pdb's CThingTypeInfo.
// Label: the manifest's "_Move<CEngineTextureMorph*,CEngineTextureMorph*>" is propagated from a byte-identical body elsewhere (family J088: the 80/81-byte
//   __copy bodies of {dword, dword, class} elements; retail has no identical-code folding, so each address is its own instantiation).
// Callees: CCharString::operator=(const CCharString&) 0x0099EFB0 (FableWin.pdb ??4CCharString@@QAEABV0@ABV0@@Z) (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; } return __result;` -- the count by the reciprocal divide, the element's implicit operator= inline.
class CCharString {                                        // 4 bytes (TLC)
public:
    const CCharString& operator=(const CCharString& other); // 0x0099EFB0
    void* PStringData;                                     // +0x00
};
class CThingTypeInfo {                                     // 12 bytes (Ego_r.pdb); implicit operator=
public:
    long DefType;                                          // +0x00
    unsigned long ThingTypeFlags;                          // +0x04
    CCharString DefClassType;                              // +0x08
};
namespace std {
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CThingTypeInfo* __fastcall __copy<CThingTypeInfo*, CThingTypeInfo*, int>(CThingTypeInfo*, CThingTypeInfo*, CThingTypeInfo*,
                                                                const random_access_iterator_tag&, int*);
} // namespace std