// _global::__pop_heap(SKeyedLong* first, SKeyedLong* last, SKeyedLong* result, SKeyedLong value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of 8-byte (long key, 4-byte value) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F8D4D, length 53 bytes, `ret 14h` (result, the 8-byte value, the comparator and
//   the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Unaligned: a /Os module.
// Label correction: the manifest's `_Pop_heap<pair<C3DVector,long>*,...,CCompareTargetedPositionsForFirstPersonTargeting>` is
//   propagated (that instantiation is 0x00808BD0, 16-byte elements). The element here is 8 bytes (`sar edx,3`, two dword
//   copies, no add-ref), and the callee 0x005F8648 (catalog "std_PopHeapRebalance") is the matching __adjust_heap whose
//   comparison is a signed compare of the first word (`cmp ; jge`): CKeyPairCompareLess over pair<long, 4-byte POD>.
//   Callers: the __pop_heap_aux front ends 0x005F8E86 / 0x005F9D83 (`push 0 ; push [comp] ; add edx,-8 ; push ecx ; push ecx`).
//   Byte-identical twin with another adjust callee: 0x005F8D82.
// Callee: __adjust_heap @ 0x005F8648.
// Shape: size TU (`push dword ptr [esp+14h]` pass-through push of the comparator, `#pragma optimize("s", on)`); the pair's
//   implicit assignment copies the two words through esi, the by-value copy is built in the argument area by STLport's
//   explicit pair copy constructor (`push ecx ; push ecx ; mov eax,esp`), the count is `(last - first) >> 3`.
#pragma optimize("s", on)

struct SKeyedLong {                                    // std::pair<long, 4-byte value>, 8 bytes
    long first;                                        // +0x00 the key (signed compare)
    long second;                                       // +0x04
    SKeyedLong(const SKeyedLong& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                           // empty, passed by value
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(SKeyedLong* first, long holeIndex, long len, SKeyedLong value,
                              CKeyPairCompareLess comp);   // 0x005F8648

void __fastcall __pop_heap(SKeyedLong* first, SKeyedLong* last, SKeyedLong* result, SKeyedLong value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}