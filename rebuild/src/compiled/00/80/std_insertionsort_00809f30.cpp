// std::__insertion_sort(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CDoNotUseBinaryPredicate comp) -- STLport insertion sort of intelligent thing pointers under the coin-flip predicate: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00809F30, 80 bytes, /Gr `__fastcall` (first ecx, last edx, the 4-byte predicate on the
//   stack, `ret 4`).
// Identity (probable): the player-targeting TU's thing-pointer shuffle (`anonymous namespace'::CDoNotUseBinaryPredicate, as the
//   landed sort_heap 0x00809700 and __adjust_heap 0x00807520 of the same instantiation): 8-byte elements copied with
//   CIntelligentPointer<CThing>'s copy ctor (base ctor, vtable 0x01238C6C, SetPItem(GetPItem())) straight into the callee's
//   by-value argument slot. Callers 0x0080A800 / 0x0080A81E (__final_insertion_sort) and 0x0080B53D / 0x0080B557.
// Callees: STLport __linear_insert(first, last, value, comp) 0x00809650, CBaseIntelligentPointer ctor 0x00A01B10 /
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (the callee destroys its by-value argument).
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
typedef CIntelligentPointer<CThing> ThingPointer;

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const;
};
}

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00809650

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, ThingPointer(*__i), __comp);
}

template void __fastcall __insertion_sort(ThingPointer*, ThingPointer*, CDoNotUseBinaryPredicate);
}