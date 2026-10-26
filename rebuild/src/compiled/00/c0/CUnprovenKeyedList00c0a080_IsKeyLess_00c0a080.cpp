// CUnprovenKeyedList_00c0a080::IsKeyLess(const unsigned long* key, CUnprovenItem_00c0a080* item) const -- libpk keyed-list comparator: is the probe key below the item's key (unsigned)
// Leaf: ?IsKeyLess@?$CUnprovenKeyedList_00c0a080@VCUnprovenItem_00c0a080@@K$1??_9@$BA@AE@@UBEHPBKPAVCUnprovenItem_00c0a080@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A080, 24 bytes (+ int3 padding to 32), thiscall (`this` unused), `ret 8`.
// Identity (descriptive): CUnprovenKeyedList_00c0a080<CUnprovenItem_00c0a080, unsigned long, &CUnprovenItem_00c0a080::GetKey>::IsKeyLess(const unsigned long*, CUnprovenItem_00c0a080*) const; evidence: the address is the fourth entry of the comparator group {0x00C0A010 equal, 0x00C0A040 less, 0x00C0A060 key-equal, 0x00C0A080 key-less} referenced from .rdata at 0x012B1C4C and 0x012B1D5C (two list vtables: each group is followed by a different deleting destructor, 0x00C0A450 / 0x00C0A4B0); all four read the item key through the virtual slot 0 getter through the shared vcall thunk 0x0043E420
// Label: the manifest's "sub_00c0a080" is a gap-scan placeholder (family J029: six libpk keyed-list "key < item key" comparators,
//   one per list template instance).
// Callees: the vcall thunk ??_9@$BA@AE 0x0043E420 (`mov eax,[ecx] ; jmp [eax+0]`: virtual slot 0 of the item, the key getter).
// Naming: libpk (the audio library, trace strings "d:\dev\audio\ver_fable_1.5_b\libpk\PKCTRList.h"; GOTCHAS: no PDB, no RTTI)
//   -- the list template, the item class and the member names are descriptive CUnproven*_<addr> stand-ins; only the shape is
//   evidence: four comparators (equal, less, key equal, key less) sit in vtable order before each list's deleting destructor,
//   and every one reads the item key through the same getter, passed as a pointer-to-member template argument (VC7.1 calls
//   a virtual getter through the shared vcall thunk `??_9@$B<slot>AE`, a non-virtual one directly).
// Shape: `return *key < (item->*GetKeyFn)();` -- an unsigned 4-byte key (`cmp ; sbb ; neg`); the comparators return int (the
//   equal sibling widens with `xor ecx,ecx ; sete cl ; mov eax,ecx`, the J030 siblings with `movzx eax,al`).
class CUnprovenItem_00c0a080 {
public:
    virtual const unsigned long& GetKey() const;            // slot 0 (descriptive name)
};

template <class T, class K, const K& (T::*GetKeyFn)() const> class CUnprovenKeyedList_00c0a080 {
public:
    virtual int IsEqual(T* a, T* b) const;                  // 0x00C0A010
    virtual int IsLess(T* a, T* b) const;                   // 0x00C0A040
    virtual int IsKeyEqual(const K* key, T* item) const;    // 0x00C0A060
    virtual int IsKeyLess(const K* key, T* item) const;     // 0x00C0A080
};

template <class T, class K, const K& (T::*GetKeyFn)() const>
int CUnprovenKeyedList_00c0a080<T, K, GetKeyFn>::IsKeyLess(const K* key, T* item) const
{
    return *key < (item->*GetKeyFn)();
}

template class CUnprovenKeyedList_00c0a080<CUnprovenItem_00c0a080, unsigned long, &CUnprovenItem_00c0a080::GetKey>;