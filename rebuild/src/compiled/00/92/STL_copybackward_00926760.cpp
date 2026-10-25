// std::__copy_backward(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, CIntelligentPointer<const CThing>* result, const random_access_iterator_tag&, int*) -- STLport random-access copy_backward: assign last-1 .. first into result-1 downwards, n = last - first times; returns the new result
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00926760, 62 bytes (64 with padding), `ret 0Ch` (first in ecx, last in edx; result, tag, distance on the stack).
// Name: STLport `__copy_backward<CIntelligentPointer<const CThing>*, CIntelligentPointer<const CThing>*, int>`; four callers
//   (__linear_insert 0x00927740 and the vector insert helpers 0x0092696C, 0x0092752C, 0x009276DC). The catalog's
//   "Median_GetByIndex" is an autoname.
// Element: CIntelligentPointer<const CThing> (8 bytes: vtable 0x01237F1C + the CBaseIntelligentPointer link), sorted under a
//   plain `bool (__fastcall*)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)` function pointer -- the
//   sort of the CAIStateGroup_OpenAndCloseShop TU (landed siblings: __unguarded_insertion_sort_aux 0x009267A0, __pop_heap
//   0x00927450). /Gr algorithm TU: pointer/int arguments take ecx/edx in order (the comparator pointer is a register
//   candidate), the class-typed value goes on the stack and is destroyed by the callee.
// Callees (all): CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Model: the landed CIntelligentPointer<CThing> twins of the 0x0080xxxx thing-pointer shuffle (copy constructor = base
//   ctor + own vtable store + SetPItem(GetPItem()), assignment = SetPItem(GetPItem()), the generated destructor a direct
//   base-destructor call), both __forceinline.
// Shape: STLport `for (_Distance n = last - first; n > 0; --n) *--result = *--last; return result;` -- the count in ebx
//   (`dec ebx ; jne`), each step one SetPItem(GetPItem()) assignment; the empty range returns result straight from its slot.
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
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template ConstThingPointer* __fastcall __copy_backward(ConstThingPointer*, ConstThingPointer*, ConstThingPointer*,
                                                       const random_access_iterator_tag&, int*);
}