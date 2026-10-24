// _STL::swap(CIntelligentPointer<const CThing>& a, CIntelligentPointer<const CThing>& b) -- STLport swap of two intelligent const-thing pointers through a temporary copy
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009267F0, 90 bytes (96 with padding), plain `ret` (a in ecx, b in edx).
// Name: STLport `swap<CIntelligentPointer<const CThing> >`; its only caller is the iter_swap tail-jump 0x00927440. The
//   catalog's "_Swap_adl<CIntelligentPointer<CThing> >" is the Dinkumware spelling (landed twin: 0x00807370 for
//   CIntelligentPointer<CThing>).
// Element: CIntelligentPointer<const CThing> (8 bytes: vtable 0x01237F1C + the CBaseIntelligentPointer link), sorted under a
//   plain `bool (__fastcall*)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)` function pointer -- the
//   sort of the CAIStateGroup_OpenAndCloseShop TU (landed siblings: __unguarded_insertion_sort_aux 0x009267A0, __pop_heap
//   0x00927450). /Gr algorithm TU: pointer/int arguments take ecx/edx in order (the comparator pointer is a register
//   candidate), the class-typed value goes on the stack and is destroyed by the callee.
// Callees (all): CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Model: the landed CIntelligentPointer<CThing> twins of the 0x0080xxxx thing-pointer shuffle (copy constructor = base
//   ctor + own vtable store + SetPItem(GetPItem()), assignment = SetPItem(GetPItem()), the generated destructor a direct
//   base-destructor call), both __forceinline.
// Shape: STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` -- the temporary built in the frame (base ctor, vtable store,
//   SetPItem(a.GetPItem())), two SetPItem(GetPItem()) assignments, the temporary destroyed by a direct base-dtor call.
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
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap(ConstThingPointer&, ConstThingPointer&);
}