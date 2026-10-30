// std::__push_heap(CIntelligentPointer<const CThing>* first, int holeIndex, int topIndex, CIntelligentPointer<const CThing> val, ConstThingCompare comp) -- STLport push_heap core: move the parents that compare below val down the hole, then drop val in
// Leaf: ??$__push_heap@
// Grade: RELOCATION_MATCH (was DIFFER 192v198 nd=32) -- lever: the intelligent-pointer copy ctor and operator= read the
//   source item into a named CBaseObject* local before SetPItem (same lever as 0x00946400): retail's first-in-ebp /
//   holeIndex-spill register assignment follows.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00926690, 198 bytes (208 with padding), `ret 10h` (first in ecx, holeIndex in edx;
//   topIndex, the value and comp on the stack).
// Name: STLport `__push_heap<CIntelligentPointer<const CThing>*, int, CIntelligentPointer<const CThing>, bool (__fastcall*)(..)>`;
//   caller __adjust_heap 0x0092693A. The catalog's "_Push_heap<...>" is the Dinkumware spelling.
// Element: CIntelligentPointer<const CThing> (8 bytes: vtable 0x01237F1C + the CBaseIntelligentPointer link), sorted under a
//   plain `bool (__fastcall*)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)` function pointer -- the
//   sort of the CAIStateGroup_OpenAndCloseShop TU (landed siblings: __unguarded_insertion_sort_aux 0x009267A0, __pop_heap
//   0x00927450). /Gr algorithm TU: pointer/int arguments take ecx/edx in order (the comparator pointer is a register
//   candidate), the class-typed value goes on the stack and is destroyed by the callee.
// Callees (all): CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Model: the landed CIntelligentPointer<CThing> twins of the 0x0080xxxx thing-pointer shuffle (copy constructor = base
//   ctor + own vtable store + SetPItem(GetPItem()), assignment = SetPItem(GetPItem()), the generated destructor a direct
//   base-destructor call), both __forceinline.
// Shape: STLport `parent = (hole - 1) / 2; while (hole > top && comp(*(first + parent), val)) { *(first + hole) =
//   *(first + parent); hole = parent; parent = (hole - 1) / 2; } *(first + hole) = val;` -- same instructions as retail
//   with first/holeIndex swapped between ebp and the frame slot. Tried: the iterator-arithmetic form, the landed
//   0x00806990 index form (`first[parent]`): identical output.
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
    __forceinline CIntelligentPointer(const CIntelligentPointer& p) { CBaseObject* item = p.GetPItem(); SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& p)
    {
        CBaseObject* item = p.GetPItem();
        SetPItem(item);
        return *this;
    }
};
typedef CIntelligentPointer<const CThing> ConstThingPointer;
typedef bool (__fastcall* ConstThingCompare)(ConstThingPointer, ConstThingPointer);

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template void __fastcall __push_heap(ConstThingPointer*, int, int, ConstThingPointer, ConstThingCompare);
}