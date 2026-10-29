// std::__unguarded_insertion_sort_aux(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing>*, CDoNotUseBinaryPredicate comp) -- unguarded insertion sort of intelligent thing pointers under the random "do not use" predicate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807320, length 71 bytes (padded to 80).
// Leaf: __unguarded_insertion_sort_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is SGI/STLport
// `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` calling `__unguarded_linear_insert(i, _Tp(*i), comp)`
// @ 0x00806910 (manifest label "_Push_heap<CIntelligentPointer<CThing>*, int, CIntelligentPointer<CThing>,
// (anonymous)::CDoNotUseBinaryPredicate>" names the template arguments; the body is the linear insert: it draws a random
// number for every comparison, `ret 0Ch` = 8-byte value + comparator). Caller 0x00807C27 (NPlayerTargeting
// _Remove_if area) and two more.
// Convention: LTCG register form -- first in ecx, last in edx; the unused value-type pointer and the 4-byte empty
// comparator on the stack (`ret 8`).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
//          std::__unguarded_linear_insert @ 0x00806910. Vtable: CIntelligentPointer 0x01238C6C (rtti-vtables).
// Shape: the by-value CIntelligentPointer is copy-constructed in the argument slot: base constructor, own vtable store,
// then `SetPItem(other.GetPItem())` (GOTCHAS 24m chain); the comparator is loaded once into ebp and pushed first.
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                          // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(((CThing*)other.GetPItem())); }
};
namespace {
struct CDoNotUseBinaryPredicate { };        // empty comparator (random order)
}
typedef CIntelligentPointer<CThing> PThing;
namespace std {
void __fastcall __unguarded_linear_insert(PThing* last, PThing val, CDoNotUseBinaryPredicate comp);   // 0x00806910
void __fastcall __unguarded_insertion_sort_aux(PThing* first, PThing* last, PThing*, CDoNotUseBinaryPredicate comp)
{
    for (PThing* i = first; i != last; ++i)
        __unguarded_linear_insert(i, PThing(*i), comp);
}
}