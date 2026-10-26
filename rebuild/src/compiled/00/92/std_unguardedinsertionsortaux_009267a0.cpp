// std::__unguarded_insertion_sort_aux(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, CIntelligentPointer<const CThing>*, bool (__fastcall* comp)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)) -- unguarded insertion sort of intelligent thing pointers under a function-pointer comparator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009267A0, length 72 bytes (padded to 80).
// Leaf: __unguarded_insertion_sort_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is SGI/STLport
// `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` calling `__unguarded_linear_insert(i, _Tp(*i), comp)`
// @ 0x009265C0 (manifest label "_Push_heap<CIntelligentPointer<CThing const>*, int, CIntelligentPointer<CThing const>,
// bool (__fastcall*)(CIntelligentPointer<CThing const>, CIntelligentPointer<CThing const>)>" names the template
// arguments; `ret 8` = the 8-byte value only). Callers in CAIStateGroup_OpenAndCloseShop::Update (0x00927437) and two more.
// Convention: LTCG register form -- first in ecx, last in edx; the unused value-type pointer and the comparator on the
// stack (`ret 8`). The callee takes `i` in ecx and the function-pointer comparator in edx (a pointer argument is a
// register candidate, the class-typed value is not), so the comparator goes `mov edx,ebp` instead of a push.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
//          std::__unguarded_linear_insert @ 0x009265C0. Vtable: CIntelligentPointer 0x01237F1C (rtti-vtables).
// Shape: the by-value CIntelligentPointer is copy-constructed in the argument slot: base constructor, own vtable store,
// then `SetPItem(other.GetPItem())`.
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                          // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
typedef CIntelligentPointer<const CThing> PConstThing;
typedef bool (__fastcall* PConstThingCompare)(PConstThing, PConstThing);
namespace std {
void __fastcall __unguarded_linear_insert(PConstThing* last, PConstThing val, PConstThingCompare comp);   // 0x009265C0
void __fastcall __unguarded_insertion_sort_aux(PConstThing* first, PConstThing* last, PConstThing*, PConstThingCompare comp)
{
    for (PConstThing* i = first; i != last; ++i)
        __unguarded_linear_insert(i, PConstThing(*i), comp);
}
}