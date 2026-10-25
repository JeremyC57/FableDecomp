// std::__make_heap(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, ConstThingCompare comp, CIntelligentPointer<const CThing>*, int*) -- STLport make_heap: sift every parent down from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009274B0, 102 bytes, `ret 0Ch` (first in ecx, last in edx; comp and the two type tags on the stack).
// Name: STLport `__make_heap<CIntelligentPointer<const CThing>*, bool (__fastcall*)(..), CIntelligentPointer<const CThing>, int>`;
//   callers make_heap 0x009276B9 and __partial_sort 0x0092792C. Callee: __adjust_heap 0x00926850. The catalog's
//   "Sort_MakeHeap_Init" is descriptive.
// Element: CIntelligentPointer<const CThing> (8 bytes: vtable 0x01237F1C + the CBaseIntelligentPointer link), sorted under a
//   plain `bool (__fastcall*)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)` function pointer -- the
//   sort of the CAIStateGroup_OpenAndCloseShop TU (landed siblings: __unguarded_insertion_sort_aux 0x009267A0, __pop_heap
//   0x00927450). /Gr algorithm TU: pointer/int arguments take ecx/edx in order (the comparator pointer is a register
//   candidate), the class-typed value goes on the stack and is destroyed by the callee.
// Callees (all): CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Model: the landed CIntelligentPointer<CThing> twins of the 0x0080xxxx thing-pointer shuffle (copy constructor = base
//   ctor + own vtable store + SetPItem(GetPItem()), assignment = SetPItem(GetPItem()), the generated destructor a direct
//   base-destructor call), both __forceinline.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
//   parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- len computed once (`sar ebp,3`),
//   first parked in the `push ecx` slot, the value copy-constructed in its argument slot, first + parent kept in ebx.
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    void* PData;                                // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C for const CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& p)
    {
        SetPItem(p.GetPItem());
        return *this;
    }
};
typedef CIntelligentPointer<const CThing> ConstThingPointer;
typedef bool (__fastcall* ConstThingCompare)(ConstThingPointer, ConstThingPointer);

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp);   // 0x00926850

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

template void __fastcall __make_heap(ConstThingPointer*, ConstThingPointer*, ConstThingCompare, ConstThingPointer*, int*);
}