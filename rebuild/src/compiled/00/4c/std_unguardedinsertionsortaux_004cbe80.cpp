// std::__unguarded_insertion_sort_aux(pair<ETCInterfaceType, CTCBase*>* first, pair<ETCInterfaceType, CTCBase*>* last, pair<ETCInterfaceType, CTCBase*>*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> comp) -- STLport: unguarded linear insert of every component pair of [first, last) by its interface-type key
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CBE80, length 80 bytes (73 + int3 padding), `ret 8` (__fastcall /Gr
// algorithm TU: first in ecx, last in edx, the value-type tag pointer and the 1-byte comparator in a stack slot).
// Identity: the element is CThing::Components' pair<ETCInterfaceType, CTCBase*> (CVectorMap<ETCInterfaceType, CTCBase*,
// CKeyPairCompareLess<...> >, Ego_r.pdb): the callers 0x004CC6F7 / 0x004CD629 are the __final_insertion_sort tails of the
// map's sort 0x004CD5D0 (called by the landed CComponentMap::Resort 0x004CD650) and 0x004CD48C its push_back neighbour.
// The catalog "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is propagated.
// Comparator: CKeyPairCompareLess compares the keys only (`a.first < b.first`, signed: `jge` / `jl`).
// Shape: STLport 4.x `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the insert
// inlined: the key in edi, the pointer in ebx, the predecessor's key re-read into ebp each step. The STLport pair's user copy
// ctor keeps the four callee-saved pushes ahead of the `first == last` exit (a trivially-copied POD pair lets VC7.1
// shrink-wrap them behind it: same size, different order).
enum ETCInterfaceType { TCI_NULL };
class CTCBase;

struct SComponentPair {                   // std::pair<ETCInterfaceType, CTCBase*> (8 bytes)
    ETCInterfaceType first;               // +0x00
    CTCBase*         second;              // +0x04
    SComponentPair(const SComponentPair& o) : first(o.first), second(o.second) {}   // STLport pair's user copy ctor
};

struct CKeyPairCompareLess {              // CKeyPairCompareLess<ETCInterfaceType, CTCBase*> (empty, by value)
    bool operator()(const SComponentPair& a, const SComponentPair& b) const { return a.first < b.first; }
};

inline void __unguarded_linear_insert(SComponentPair* last, SComponentPair value, CKeyPairCompareLess comp)
{
    SComponentPair* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

void __fastcall __unguarded_insertion_sort_aux(SComponentPair* first, SComponentPair* last, SComponentPair*, CKeyPairCompareLess comp)
{
    for (SComponentPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SComponentPair(*i), comp);
}