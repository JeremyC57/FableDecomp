// CVtable_012b28d8::Less(const CUnprovenItem_012b28d8* a, const CUnprovenItem_012b28d8* b) const -- strict order of two items: by their unsigned keys, ties broken by address
// Leaf: ?Less@CVtable_012b28d8@@UBEHPBUCUnprovenItem_012b28d8@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C155F0, 33 bytes, thiscall (this unused), `ret 8`, int result: one shared
//   `sbb eax,eax ; neg eax` tail after either `cmp ka,kb` or (keys equal) `cmp a,b`.
// Identity (descriptive): slots 0..3 of the twin non-RTTI comparator vtables 0x012B28D8 / 0x012B2968 (which differ only
//   from slot 4), the libpk-container comparator family of the landed CUnprovenCompare_00c09d20 (Equal / Less /
//   EqualToValue), here keyed by the dword an item points at from +8 instead of a virtual getter. libpk has no RTTI or
//   PDB, so the class is named by its vtable and the item type by address.
struct CUnprovenItem_012b28d8 {
    unsigned char        _pad00[0x08];
    const unsigned long* m_pKey;                // +0x08 the sort key, read through a pointer
};

class CVtable_012b28d8 {                        // comparator; this unused (vtables 0x012B28D8 / 0x012B2968)
public:
    virtual int Equal(const CUnprovenItem_012b28d8* a, const CUnprovenItem_012b28d8* b) const;      // slot 0 (0x00C155D0: a == b)
    virtual int Less(const CUnprovenItem_012b28d8* a, const CUnprovenItem_012b28d8* b) const;       // slot 1
    virtual int EqualToKey(const unsigned long& key, const CUnprovenItem_012b28d8* b) const;       // slot 2
    virtual int LessThanKey(const unsigned long& key, const CUnprovenItem_012b28d8* b) const;      // slot 3
};

int CVtable_012b28d8::Less(const CUnprovenItem_012b28d8* a, const CUnprovenItem_012b28d8* b) const
{
    unsigned long kb = *b->m_pKey;
    unsigned long ka = *a->m_pKey;
    return ka == kb ? a < b : ka < kb;
}