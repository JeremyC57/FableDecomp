// _global::__pop_heap_aux(SAbilityEntry*, SAbilityEntry*, SAbilityEntry*, SAbilityEntryLess) -- SGI pop_heap front end: move the back element out and sift
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059474E, length 38 bytes (a /Os module: unaligned entry, no
// padding; the SGI algorithm TUs are compiled with /Gr, so every free template function is __fastcall).
// Callees: pair<long, CDefPointer<const CHeroAbilityDef> >'s copy constructor @ 0x00592DFB (copies the long,
// copies the def pointer and add-refs it; manifest "_Pair_base<long,CDefPointer<CHeroAbilityDef const>>"),
// __pop_heap @ 0x005946B2 (`*result = *first`, then __adjust_heap; the manifest's
// "swap<CTCBase::CRegisteredTCInfo>" is a propagated name).
// Identity: the manifest label "CTCActionUseBuyHouse::OnCreate" is propagated and wrong: this is the SGI
//   __pop_heap_aux(first, last, T*, comp) { __pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp,
//                                                      __DISTANCE_TYPE(first)); }
// instantiated for an 8-byte {long, CDefPointer<const CHeroAbilityDef>} element with an empty comparator.
//
// Shape: `ret 8` = first in ecx, last in edx, the unused T* and the by-value comparator on the stack.
// Arguments right to left: the null distance pointer, the comparator's slot (`push dword ptr [esp+14h]`),
// the element COPY built in the argument area by its user copy constructor (`push ecx ; push ecx ;
// mov ecx,esp`), then result = last - 1 (esi, also passed in edx) and first in ecx. No element
// destructor runs here: a by-value class argument is destroyed by the callee.
#pragma optimize("s", on)

struct CHeroAbilityDef;

struct SAbilityEntry {                          // pair<long, CDefPointer<const CHeroAbilityDef> >
    long                   first;
    const CHeroAbilityDef* second;
    SAbilityEntry(const SAbilityEntry& other);  // 0x00592DFB
    ~SAbilityEntry();
};

struct SAbilityEntryLess {                      // the empty comparator, passed by value
    SAbilityEntryLess() { }
};

void __fastcall __pop_heap(SAbilityEntry* first, SAbilityEntry* last, SAbilityEntry* result,
                           SAbilityEntry value, SAbilityEntryLess comp, long* distance);   // 0x005946B2

void __fastcall __pop_heap_aux(SAbilityEntry* first, SAbilityEntry* last, SAbilityEntry*, SAbilityEntryLess comp)
{
    __pop_heap(first, last - 1, last - 1, SAbilityEntry(*(last - 1)), comp, (long*)0);
}
