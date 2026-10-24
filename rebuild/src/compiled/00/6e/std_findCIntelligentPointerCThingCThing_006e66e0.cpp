// std::__find<CIntelligentPointer<CThing>*, CThing*>(first, last, CThing* const& value, const random_access_iterator_tag&) -- STLport random-access find: four-way unrolled scan of a vector of thing pointers for the one holding `value`, then the 0..3 leftovers
// Leaf: ??$__find@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E66E0, 192 bytes, /Gr `__fastcall` (first ecx, last edx),
//   `ret 8` (the value reference + the tag reference).
// Identity: the landed CScriptConversation::AddPersonToConversation 0x006E5790 calls this address as its
//   `__find(begin, end, &person, random_access_iterator_tag())` over PeopleInConversation (vector<CIntelligentPointer<
//   CThing> >, 8-byte elements); byte-identical to the landed CBaseObject instantiation 0x006E67A0 that
//   CScriptConversation::AddLineToConversation 0x006E58A0 uses for its `const CThing*` values. The catalog's
//   "BinarySearch_LinearArray" is wrong (a linear find, not a binary search).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50.
// Shape: SGI/STLport `__find` verbatim: `__trip_count = (__last - __first) >> 2`, the unrolled loop, then the
//   fall-through switch on `__last - __first`. The engine's `operator==(const CIntelligentPointer<T>&, T*)` takes the
//   object by value, so the value is read through the reference before each GetPItem call.
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;                                     // +0x00
    void*        PNext;                                     // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

template <class T> inline bool operator==(const CIntelligentPointer<T>& pointer, T* object)
{
    return pointer.GetPItem() == (CBaseObject*)object;
}

namespace std {
struct random_access_iterator_tag { };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}

template CIntelligentPointer<CThing>* __fastcall __find(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*,
                                                        CThing* const&, const random_access_iterator_tag&);
}