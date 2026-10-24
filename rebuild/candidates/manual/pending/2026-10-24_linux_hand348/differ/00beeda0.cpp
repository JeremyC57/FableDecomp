// Grade: DIFFER(ours v retail) -- 147v147 scheduling only: retail loads value.first.second into edx before `sub esp,1Ch` for the by-value copy, ours after
// std::__pop_heap(SDecalPair* first, SDecalPair* last, SDecalPair* result, SDecalPair value, CKeyPairCompareLess comp, int*) -- SGI pop_heap core for a heap of ((long,long), CEngineTextureDecal) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe @ 0x00BEEDA0, 147 bytes, `ret 28h` (result, the 0x1C-byte value, the comparator and the distance pointer on the
//   stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr, as in the landed __pop_heap siblings 0x00CBBA60 / 0x00CBBAD0).
//   Callers: the sort_heap/partial_sort loops at 0x00BEEF7A, 0x00BEF16A, 0x00BEF402, 0x00BEF7C8. The catalog label
//   _Cons_val<pair<CCharString,C3DVector>> is propagated; the callee 0x00BEEA00 carries the manifest name
//   _Adjust_heap<pair<pair<long,long>,CEngineTextureDecal>*, int, ..., CKeyPairCompareLess<pair<long,long>,CEngineTextureDecal>>.
// Callees: vector<CCompositeBlendDesc>::operator= 0x006CADE0, vector<CCompositeBlendDesc> copy ctor 0x005BB346, __adjust_heap
//   0x00BEEA00, free 0x00BFEA14 (the by-value value's vector storage).
// Layout: Ego_r.pdb CEngineTextureDecal (OriginalTextureIndex +0, DecalID +4, OverlaidBlends +8; retail 12-byte STLport vector), so
//   the pair is 0x1C bytes (`last - first` divides by 28 through the 0x92492493 multiply).
// Shape: implicit pair assignment (four key/index words, then the vector operator=), the by-value copy built in its argument slot
//   (four words + the vector copy ctor), the empty comparator's slot pushed first; the callee-owned `value` is destroyed at the end.
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
struct CCompositeBlendDesc;
struct CBlendVector {                            // STLport vector<CCompositeBlendDesc>, 12 bytes
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
    CBlendVector(const CBlendVector& other);     // 0x005BB346
    CBlendVector& operator=(const CBlendVector& other);   // 0x006CADE0
    ~CBlendVector() { if (_M_start) free(_M_start); }   // trivially destructible elements; malloc allocator
};
struct CEngineTextureDecal {
    long         OriginalTextureIndex;           // +0x00
    long         DecalID;                        // +0x04
    CBlendVector OverlaidBlends;                 // +0x08
};
struct SLongPair { long first, second; };        // std::pair<long,long>
struct SDecalPair {                              // std::pair<std::pair<long,long>, CEngineTextureDecal>, 0x1C bytes
    SLongPair           first;                   // +0x00
    CEngineTextureDecal second;                  // +0x08
};
struct CKeyPairCompareLess { };                  // CKeyPairCompareLess<pair<long,long>, CEngineTextureDecal>, empty, by value

void __fastcall __adjust_heap(SDecalPair* first, int holeIndex, int len, SDecalPair value, CKeyPairCompareLess comp);   // 0x00BEEA00

void __fastcall __pop_heap(SDecalPair* first, SDecalPair* last, SDecalPair* result, SDecalPair value, CKeyPairCompareLess comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), value, comp);
}
