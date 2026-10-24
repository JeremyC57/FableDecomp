// _global::CopyPtrs_CShopItemDef_004ed102(const CShopItemDef* first, const CShopItemDef* last, CShopItemDef* result, const __false_type&) -- STLport __copy_ptrs<CShopItemDef*> (non-POD): counted forward assignment of 72-byte shop item defs, returns the advanced result
// Leaf: CopyPtrs_CShopItemDef_004ed102
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ED102, length 95 bytes, `ret 8`; size-optimised TU.
// Callees: CDefClassBase::operator=(const CDefClassBase&) @ 0x00431F10 (landed; catalog "CThingBuildingDef::operator=").
// Caller: vector<CShopItemDef>::erase(first, last) @ 0x004ED0C6 (its _M_fill_insert @ 0x004ED161 stores the CShopItemDef
// vtable 0x0123B644 at 0x004ED1AF); byte-identical to this chunk's 0x004E9BDA (vector::operator=), a separate COMDAT copy.
// Shape: STLport 4.x `__copy_ptrs(first, last, result, const __false_type&)` with `__copy(first, last, result,
// random_access_iterator_tag(), (ptrdiff_t*)0)` inlined (its own `result` parameter re-homed in the same slot: `mov esi,[ebp+8] ;
// mov [ebp+8],esi`). The count is the size-optimised division (`push 48h ; cdq ; pop edi ; idiv edi`); the loop keeps the
// running result in its argument slot (`add [ebp+8],48h`), the count and `result - first` in two frame words, and runs
// CShopItemDef's assignment: CDefClassBase::operator= out of line, Type (+0x28), then the 28-byte POD CShopItem (+0x2C) as
// `push 7 ; pop ecx ; rep movsd` from src+0x2C to (result - first) + src+0x2C. Under "s" the implicit operator= would be a
// call, so the memberwise assignment is spelled out and __forceinline (as the landed _Construct @ 0x004E9C39 does for the
// copy constructor), and the inner __copy is __forceinline too (a plain `inline` __copy stays a call under "s": 21 bytes).
// Frame pointer + `leave` with esi/edi free: optimize("y", off) + ("s", on) (GOTCHAS).
// Layout: engine/CShopItemDef.h (PDB: Type +0x28, Item +0x2C..+0x48, sizeof 0x48); modelled locally because the generated
// header is a flat byte-packed POD that cannot express the base assignment. CDefClassBase is 0x25 bytes, pack 1.
// Label correction: the catalog's `_Copy_impl<std::pair<unsigned long,C3DDummyObject>*,...>` is propagated.
#pragma optimize("y", off)
#pragma optimize("s", on)

#pragma pack(push, 1)
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x21];                       // +0x04..+0x25
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)
struct CShopItem {                                   // 28 bytes, POD
    float Quantity;                                  // +0x00
    float PriceMultiplier;                           // +0x04
    float MaxStock;                                  // +0x08
    long  RestockPeriod;                             // +0x0c
    float RestockPeriodRandomness;                   // +0x10
    float SalesPerDay;                               // +0x14
    float SalesPerDayRandomness;                     // +0x18
};
struct CShopItemDef : CDefClassBase {                // vtable 0x0123B644
    long      Type;                                  // +0x28
    CShopItem Item;                                  // +0x2c
    virtual void v00();
    __forceinline CShopItemDef& operator=(const CShopItemDef& o)
    {
        CDefClassBase::operator=(o);
        Type = o.Type;
        Item = o.Item;
        return *this;
    }
};

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

__forceinline CShopItemDef* __fastcall __copy(const CShopItemDef* first, const CShopItemDef* last, CShopItemDef* result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

CShopItemDef* __fastcall CopyPtrs_CShopItemDef_004ed102(const CShopItemDef* first, const CShopItemDef* last, CShopItemDef* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}