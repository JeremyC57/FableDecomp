// std::__find(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, const CIntelligentPointer<const CThing>& val, const random_access_iterator_tag&) -- linear search of an intelligent-pointer range for the entry pointing at val's object
// Leaf: ??$__find@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074CEE0, 247 bytes, __fastcall (first ecx, last edx), `ret 8` (val, tag).
// Label correction: catalog "std_find<CBaseIntelligentPointer>" -> STLport 4.6's random-access __find (the body
//   std::find forwards to), over CTCStealth's 8-byte intelligent pointers (callers 0x0074A21F .. 0x0074D34E in the
//   CTCStealth TU; T = const CThing is the stealth model's choice, the body only needs CBaseIntelligentPointer).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice per compare).
// Shape: STLport `__find`: trip_count = (last - first) >> 2 (`sar 5` on bytes), four unrolled `if (*first == val)
//   return first; ++first;` per trip, then `switch (last - first)` with cases 3, 2, 1 falling through and the default
//   returning last; the CIntelligentPointer `==` compares this->GetPItem() (first) with other.GetPItem(); last is
//   kept in a frame slot because ebp carries the first item.
class CThing;

class CBaseIntelligentPointer {
public:
    void* PData;                                // +0x04
    virtual void v00();
    void* GetPItem() const;                     // 0x00A01B50
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    bool operator==(const CIntelligentPointer& other) const { return GetPItem() == other.GetPItem(); }
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() {} };

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
        ++__first;
    case 0:
    default:
        return __last;
    }
}

template CIntelligentPointer<const CThing>* __fastcall __find(CIntelligentPointer<const CThing>*, CIntelligentPointer<const CThing>*,
                                                             const CIntelligentPointer<const CThing>&, const random_access_iterator_tag&);

}