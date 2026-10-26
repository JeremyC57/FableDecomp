// CUnprovenKeyedList_00c3afb0::IsKeyLess(const unsigned long* key, CUnprovenItem_00c3afb0* item) const -- libpk keyed-list comparator: is the probe key below the item's key (unsigned)
// Leaf: ?IsKeyLess@?$CUnprovenKeyedList_00c3afb0@VCUnprovenItem_00c3afb0@@K$1?GetKey@1@QBEABKXZ@@UBEHPBKPAVCUnprovenItem_00c3afb0@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3AFB0, 24 bytes (+ int3 padding to 32), thiscall (`this` unused), `ret 8`.
// Identity (descriptive): CUnprovenKeyedList_00c3afb0<CUnprovenItem_00c3afb0, unsigned long, &CUnprovenItem_00c3afb0::GetKey>::IsKeyLess(const unsigned long*, CUnprovenItem_00c3afb0*) const; evidence: the address is the fourth entry of the comparator group {0x00C3AF40 equal, 0x00C3AF70 less, 0x00C3AF90 key-equal, 0x00C3AFB0 key-less} referenced from .rdata at 0x012B553C and 0x012B5584 (two list vtables: each group is followed by a different deleting destructor, 0x00C3AFD0 / 0x00C3B090); all four read the item key through the non-virtual getter 0x00C3AB20 (key at +4)
// Label: the manifest's "sub_00c3afb0" is a gap-scan placeholder (family J029: six libpk keyed-list "key < item key" comparators,
//   one per list template instance).
// Callees: the non-virtual key getter 0x00C3AB20 (`lea eax,[ecx+4] ; ret`, called directly).
// Naming: libpk (the audio library, trace strings "d:\dev\audio\ver_fable_1.5_b\libpk\PKCTRList.h"; GOTCHAS: no PDB, no RTTI)
//   -- the list template, the item class and the member names are descriptive CUnproven*_<addr> stand-ins; only the shape is
//   evidence: four comparators (equal, less, key equal, key less) sit in vtable order before each list's deleting destructor,
//   and every one reads the item key through the same getter, passed as a pointer-to-member template argument (VC7.1 calls
//   a virtual getter through the shared vcall thunk `??_9@$B<slot>AE`, a non-virtual one directly).
// Shape: `return *key < (item->*GetKeyFn)();` -- an unsigned 4-byte key (`cmp ; sbb ; neg`); the comparators return int (the
//   equal sibling widens with `xor ecx,ecx ; sete cl ; mov eax,ecx`, the J030 siblings with `movzx eax,al`).
class CUnprovenItem_00c3afb0 {
public:
    const unsigned long& GetKey() const;                    // 0x00C3AB20 `lea eax,[ecx+4] ; ret` (descriptive name; key at +4)
};

template <class T, class K, const K& (T::*GetKeyFn)() const> class CUnprovenKeyedList_00c3afb0 {
public:
    virtual int IsEqual(T* a, T* b) const;                  // 0x00C3AF40
    virtual int IsLess(T* a, T* b) const;                   // 0x00C3AF70
    virtual int IsKeyEqual(const K* key, T* item) const;    // 0x00C3AF90
    virtual int IsKeyLess(const K* key, T* item) const;     // 0x00C3AFB0
};

template <class T, class K, const K& (T::*GetKeyFn)() const>
int CUnprovenKeyedList_00c3afb0<T, K, GetKeyFn>::IsKeyLess(const K* key, T* item) const
{
    return *key < (item->*GetKeyFn)();
}

template class CUnprovenKeyedList_00c3afb0<CUnprovenItem_00c3afb0, unsigned long, &CUnprovenItem_00c3afb0::GetKey>;