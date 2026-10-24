// _STL::__linear_insert(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, CIntelligentPointer<const CThing> val, ConstThingCompare comp) -- STLport insertion of *last: a value that goes before *first shifts the run up one slot (copy_backward) and goes first, anything else takes the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00927740, 199 bytes, `ret 0Ch` (first in ecx, last in edx; the value and comp on the stack).
// Name: STLport `__linear_insert<CIntelligentPointer<const CThing>*, CIntelligentPointer<const CThing>, bool (__fastcall*)(..)>`;
//   called by __insertion_sort 0x009278C0. Callees: __copy_backward 0x00926760, __unguarded_linear_insert 0x009265C0.
//   The catalog's "_Med3<...>" names the template arguments only.
// Element: CIntelligentPointer<const CThing> (8 bytes: vtable 0x01237F1C + the CBaseIntelligentPointer link), sorted under a
//   plain `bool (__fastcall*)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)` function pointer -- the
//   sort of the CAIStateGroup_OpenAndCloseShop TU (landed siblings: __unguarded_insertion_sort_aux 0x009267A0, __pop_heap
//   0x00927450). /Gr algorithm TU: pointer/int arguments take ecx/edx in order (the comparator pointer is a register
//   candidate), the class-typed value goes on the stack and is destroyed by the callee.
// Callees (all): CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Model: the landed CIntelligentPointer<CThing> twins of the 0x0080xxxx thing-pointer shuffle (copy constructor = base
//   ctor + own vtable store + SetPItem(GetPItem()), assignment = SetPItem(GetPItem()), the generated destructor a direct
//   base-destructor call), both __forceinline.
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward's empty iterator tag is a const-reference temporary homed
//   over the LIVE comparator slot (`lea eax,[esp+20h]`), which needs the real __copy_backward body out of line in the TU
//   (auto_inline(off) + noinline, GOTCHAS "empty tag over a LIVE argument slot"); comp is held in ebp and passed in edx.
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

namespace _STL {
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

// The real __copy_backward body (0x00926760), out of line in the TU.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009265C0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __fastcall __linear_insert(ConstThingPointer*, ConstThingPointer*, ConstThingPointer, ConstThingCompare);
}