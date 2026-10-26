// CUnprovenKeyedList_00c3b540::IsKeyLess(const unsigned long* key, CUnprovenItem_00c3b540* item) const -- libpk keyed-list comparator: is the probe key below the item's key (unsigned)
// Leaf: ?IsKeyLess@?$CUnprovenKeyedList_00c3b540@VCUnprovenItem_00c3b540@@K$1?GetKey@1@QBEABKXZ@@UBEHPBKPAVCUnprovenItem_00c3b540@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B540, 24 bytes (+ int3 padding to 32), thiscall (`this` unused), `ret 8`.
// Identity (descriptive): CUnprovenKeyedList_00c3b540<CUnprovenItem_00c3b540, unsigned long, &CUnprovenItem_00c3b540::GetKey>::IsKeyLess(const unsigned long*, CUnprovenItem_00c3b540*) const; evidence: the address is the fourth entry of the comparator group {0x00C3B4D0 equal, 0x00C3B500 less, 0x00C3B520 key-equal, 0x00C3B540 key-less} referenced from .rdata at 0x012B5614 and 0x012B565C (two list vtables: each group is followed by a different deleting destructor, 0x00C3B560 / 0x00C3B620); all four read the item key through the non-virtual getter 0x00C3B0B0 (key at +4)
// Label: the manifest's "sub_00c3b540" is a gap-scan placeholder (family J029: six libpk keyed-list "key < item key" comparators,
//   one per list template instance).
// Callees: the non-virtual key getter 0x00C3B0B0 (`lea eax,[ecx+4] ; ret`, called directly).
// Naming: libpk (the audio library, trace strings "d:\dev\audio\ver_fable_1.5_b\libpk\PKCTRList.h"; GOTCHAS: no PDB, no RTTI)
//   -- the list template, the item class and the member names are descriptive CUnproven*_<addr> stand-ins; only the shape is
//   evidence: four comparators (equal, less, key equal, key less) sit in vtable order before each list's deleting destructor,
//   and every one reads the item key through the same getter, passed as a pointer-to-member template argument (VC7.1 calls
//   a virtual getter through the shared vcall thunk `??_9@$B<slot>AE`, a non-virtual one directly).
// Shape: `return *key < (item->*GetKeyFn)();` -- an unsigned 4-byte key (`cmp ; sbb ; neg`); the comparators return int (the
//   equal sibling widens with `xor ecx,ecx ; sete cl ; mov eax,ecx`, the J030 siblings with `movzx eax,al`).
class CUnprovenItem_00c3b540 {
public:
    const unsigned long& GetKey() const;                    // 0x00C3B0B0 `lea eax,[ecx+4] ; ret` (descriptive name; key at +4)
};

template <class T, class K, const K& (T::*GetKeyFn)() const> class CUnprovenKeyedList_00c3b540 {
public:
    virtual int IsEqual(T* a, T* b) const;                  // 0x00C3B4D0
    virtual int IsLess(T* a, T* b) const;                   // 0x00C3B500
    virtual int IsKeyEqual(const K* key, T* item) const;    // 0x00C3B520
    virtual int IsKeyLess(const K* key, T* item) const;     // 0x00C3B540
};

template <class T, class K, const K& (T::*GetKeyFn)() const>
int CUnprovenKeyedList_00c3b540<T, K, GetKeyFn>::IsKeyLess(const K* key, T* item) const
{
    return *key < (item->*GetKeyFn)();
}

template class CUnprovenKeyedList_00c3b540<CUnprovenItem_00c3b540, unsigned long, &CUnprovenItem_00c3b540::GetKey>;