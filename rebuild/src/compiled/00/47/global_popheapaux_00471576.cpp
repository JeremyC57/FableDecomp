// _global::__pop_heap_aux(PairLongHeroAbility* first, PairLongHeroAbility* last, PairLongHeroAbility*, CKeyPairCompareLess comp) -- SGI pop_heap front end for a heap of (long, EHeroAbility) pairs: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471576, length 53 bytes, `ret 8` (the unused T* and the empty comparator on
//   the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Unaligned: a /Os module.
// Identity: the manifest name `_Pop_heap_0<pair<long,EHeroAbility>*,pair<long,EHeroAbility>,CKeyPairCompareLess<long,EHeroAbility>>`
//   fits the 8-byte POD element (`sar edx,3`, two dword copies); the callee 0x004714AE is the matching __adjust_heap.
// Callee: __adjust_heap @ 0x004714AE.
// Shape: STLport __pop_heap_aux with __pop_heap inlined: the T(*(last - 1)) temporary lives in esi/edi, the comparator is
//   passed straight through (`push dword ptr [esp+10h]`, the size TU's pass-through push), the pair's implicit assignment writes
//   the root into the back slot through eax, and the copy for __adjust_heap is built in the argument area (`push ecx ; push ecx ;
//   mov eax,esp`) by STLport's explicit pair copy constructor. The size TU leaves a plain `inline` __pop_heap out of line
//   (`call`), so it is `__forceinline` here.
#pragma optimize("s", on)

enum EHeroAbility { HERO_ABILITY_NULL = 0 };

struct PairLongHeroAbility {                            // std::pair<long, EHeroAbility>, 8 bytes
    long         first;                                 // +0x00
    EHeroAbility second;                                // +0x04
    PairLongHeroAbility(const PairLongHeroAbility& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<long, EHeroAbility>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairLongHeroAbility* first, long holeIndex, long len, PairLongHeroAbility value,
                              CKeyPairCompareLess comp);   // 0x004714AE

__forceinline void __fastcall __pop_heap(PairLongHeroAbility* first, PairLongHeroAbility* last, PairLongHeroAbility* result,
                                  PairLongHeroAbility value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(PairLongHeroAbility* first, PairLongHeroAbility* last, PairLongHeroAbility*,
                               CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, PairLongHeroAbility(*(last - 1)), comp, (long*)0);
}