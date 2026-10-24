// _STL::__make_heap(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CDoNotUseBinaryPredicate comp, CIntelligentPointer<CThing>*, int*) -- STLport make_heap of intelligent thing pointers under the coin-flip predicate (sift every parent down, last parent first)
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808230, 102 bytes, /Gr `__fastcall` (first in ecx -- spilled to the
//   `push ecx` frame word, last in edx), the 4-byte predicate and the two type pointers on the stack, `ret 0Ch`.
// Name: STLport `__make_heap<CIntelligentPointer<CThing>*, CDoNotUseBinaryPredicate, CIntelligentPointer<CThing>, int>`
//   (the partial_sort front of the thing-pointer introsort); the catalog's "std_make_heap_Generic" is descriptive.
// Callees: STLport __adjust_heap(first, hole, len, value, comp) 0x00807520 (landed), CBaseIntelligentPointer ctor
//   0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the
//   value is copy-constructed in its argument slot (base ctor, vtable store, SetPItem(GetPItem())); `first + parent`
//   is strength-reduced to a pointer stepping down by 8.
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    void* PData;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); }
};

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const CIntelligentPointer<CThing>&, const CIntelligentPointer<CThing>&) const;
};
}

namespace _STL {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp);   // 0x00807520

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template void __fastcall __make_heap(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*, CDoNotUseBinaryPredicate,
                                     CIntelligentPointer<CThing>*, int*);
}