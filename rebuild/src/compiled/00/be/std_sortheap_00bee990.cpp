// std::sort_heap(CSurfacePair* first, CSurfacePair* last, CKeyPairCompareLess comp) -- SGI sort_heap over the morph info's (long, CCompositeSurfaceInfo) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE990, length 100 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<std::pair<long,unsigned long>*>` is propagated over the byte-identical
// skeleton. The value copy stores the CCompositeSurfaceInfo vtable 0x012A8154 (rtti-vtables.tsv) at element +4,
// then a counted pointer; the callee 0x00BEE070 (manifest "CItem::operator=", propagated) is the SGI `__pop_heap`
// of that 16-byte pair (key, then the Cache counted-pointer assignment with CCPPointerInfo release, `ret 1Ch`),
// feeding __adjust_heap 0x00BEDD70. Element = pair<long, CCompositeSurfaceInfo> (Ego_r.pdb CCompositeSurfaceInfo,
// 12 bytes: vptr, CCountedPointer<CCompositeSurfaceCache> Cache +4), the map of the landed 0x00BECBA0.
// Callees: __pop_heap @ 0x00BEE070 (out of line, __fastcall).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride). The value copy is the implicit pair copy: the key, then
// CCompositeSurfaceInfo's IMPLICIT copy constructor inline -- one vtable store (CBase's implicit copy writes nothing
// that survives), then the counted pointer's user copy (Object, Info, `test ; je ; inc [Info]`).
struct CBase {
    virtual ~CBase();                              // 0x0099A2E0 (vtable 0x01231710)
};
struct CCPInfo {
    long  References;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCompositeSurfaceCache;
struct CCountedPointer {                           // CCountedPointer<CCompositeSurfaceCache>: object + count block
    CCompositeSurfaceCache* Object;
    CCPInfo*                Info;
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Info(other.Info) { if (Info) ++Info->References; }
};
struct CCompositeSurfaceInfo : CBase {             // vtable 0x012A8154 (implicit virtual dtor), 12 bytes
    CCountedPointer Cache;                         // +0x04
};
struct CSurfacePair {                              // pair<long, CCompositeSurfaceInfo>, 0x10 bytes
    long                  first;                   // +0x00
    CCompositeSurfaceInfo second;                  // +0x04
};
struct CKeyPairCompareLess { };                    // empty, passed by value

void __fastcall __pop_heap(CSurfacePair* first, CSurfacePair* last, CSurfacePair* result, CSurfacePair value,
                           CKeyPairCompareLess comp, int*);   // 0x00BEE070

inline void __pop_heap_aux(CSurfacePair* first, CSurfacePair* last, CSurfacePair*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, CSurfacePair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(CSurfacePair* first, CSurfacePair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (CSurfacePair*)0, comp);
}

void __fastcall sort_heap(CSurfacePair* first, CSurfacePair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}