// Grade: DIFFER(85v97) -- retail keeps a 16-byte frame and spills the temporary's last word to it (`mov [esp+0Ch],eax` early, read back for the argument copy); ours keeps it in edi, saves ebx and copies the root through ebx (no frame)
// _global::__pop_heap_aux(PairULongCacheGroup* first, PairULongCacheGroup* last, PairULongCacheGroup*, CKeyPairCompareLess comp) -- SGI pop_heap front end for a heap of (unsigned long, 12-byte {float, float, dword}) pairs: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076A590, length 97 bytes, `ret 8` (the unused T* and the empty comparator on
//   the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is 16 bytes (`sar edx,4`); the temporary's words +4/+8 travel through the x87 (`fld`/`fstp`), so
//   they are floats, and +0xC is a plain dword. The callee 0x00769D50 is the matching __adjust_heap (16-byte stride,
//   UNSIGNED compare of the first word). The only Ego_r.pdb 12-byte record of shape {float, float, non-float} with a
//   non-pointer third member is CCacheGroup {StartDistance, EndDistance, PrimitiveTypeMask}; the record name is a
//   best-evidence model (the bytes only fix the member kinds). The row's `_Pop_heap_0<pair<long,EHeroAbility>*,...>` label
//   (an 8-byte element) is propagated.
// Callee: __adjust_heap @ 0x00769D50.
// Shape: STLport __pop_heap_aux with __pop_heap inlined -- it needs `__forceinline` here (plain `inline` leaves an
//   out-of-line __pop_heap call: 55 bytes) -- and CCacheGroup's memberwise copy constructor, which is what loads the two
//   floats of the T(*(last - 1)) temporary onto the x87 stack (`fld`/`fstp` straight into the argument slots); the pair's
//   implicit assignment writes the root into the back slot as four dword moves. Still open: retail spills the temporary's
//   third member to a 16-byte frame slot (as if it were not a register candidate) and never touches ebx; ours keeps it in
//   edi and copies the root through ebx. Tried an implicit copy (int moves, 103 bytes), the memberwise copy (85) and a
//   CNavigationPosition-shaped {C2DVector (memberwise copy), long} record (85, same bytes).
struct CCacheGroup {                                    // Ego_r.pdb, 12 bytes (best-evidence record: the bytes fix only {float, float, dword})
    float         StartDistance;                        // +0x00
    float         EndDistance;                          // +0x04
    unsigned long PrimitiveTypeMask;                    // +0x08
    CCacheGroup(const CCacheGroup& o) : StartDistance(o.StartDistance), EndDistance(o.EndDistance), PrimitiveTypeMask(o.PrimitiveTypeMask) { }
};

struct PairULongCacheGroup {                            // std::pair<unsigned long, CCacheGroup>, 16 bytes
    unsigned long first;                                // +0x00
    CCacheGroup   second;                               // +0x04
    PairULongCacheGroup(const PairULongCacheGroup& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<unsigned long, CCacheGroup>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairULongCacheGroup* first, long holeIndex, long len, PairULongCacheGroup value,
                              CKeyPairCompareLess comp);   // 0x00769D50

__forceinline void __fastcall __pop_heap(PairULongCacheGroup* first, PairULongCacheGroup* last, PairULongCacheGroup* result,
                                  PairULongCacheGroup value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(PairULongCacheGroup* first, PairULongCacheGroup* last, PairULongCacheGroup*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, PairULongCacheGroup(*(last - 1)), comp, (long*)0);
}
