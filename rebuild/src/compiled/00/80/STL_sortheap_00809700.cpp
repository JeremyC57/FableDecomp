// std::sort_heap(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CDoNotUseBinaryPredicate comp) -- STLport heap sort of intelligent thing pointers under the coin-flip predicate: pop the root to the back until one element remains
// Leaf: ??$sort_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809700, 174 bytes, /Gr `__fastcall` (first in ecx, last in edx), the
//   4-byte predicate by value, `ret 4`; an `and esp,-8` aligned frame (the 8-byte pointer temporary in the loop).
// Name: STLport `sort_heap<CIntelligentPointer<CThing>*, `anonymous namespace'::CDoNotUseBinaryPredicate>` (the
//   partial_sort tail of the thing-pointer shuffle); the catalog's "_Sort_heap<CIntelligentPointer<CThing const>*,
//   bool (__fastcall*)(..)>" is propagated (the vtable stored is CIntelligentPointer<CThing>'s 0x01238C6C and the
//   callee is the coin-flip __adjust_heap).
// Callees: STLport __adjust_heap(first, hole, len, value, comp) 0x00807520 (landed), CBaseIntelligentPointer ctor
//   0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Shape: STLport `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap -> __pop_heap_aux ->
//   __pop_heap all inlined: `_Tp val(*(last - 1)); *(last - 1) = *first; __adjust_heap(first, 0, (last - 1) - first,
//   val, comp);` -- val is a frame temporary (copy ctor: base ctor, vtable store, SetPItem(GetPItem())), copied again
//   into the callee's argument slot, and destroyed after the call.
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
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& p)
    {
        SetPItem(p.GetPItem());
        return *this;
    }
};
typedef CIntelligentPointer<CThing> ThingPointer;

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const;
};
}

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp);   // 0x00807520

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, (ThingPointer*)0, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

template void __fastcall sort_heap(ThingPointer*, ThingPointer*, CDoNotUseBinaryPredicate);
}