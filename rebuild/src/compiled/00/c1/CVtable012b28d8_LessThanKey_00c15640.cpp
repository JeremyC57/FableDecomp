// CVtable_012b28d8::LessThanKey(const unsigned long& key, const CUnprovenItem_012b28d8* b) const -- whether key is below item b's key (unsigned)
// Leaf: ?LessThanKey@CVtable_012b28d8@@UBEHABKPBUCUnprovenItem_012b28d8@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C15640, 22 bytes, thiscall (this unused), `ret 8`, int result:
//   `cmp / sbb / neg`.
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

int CVtable_012b28d8::LessThanKey(const unsigned long& key, const CUnprovenItem_012b28d8* b) const
{
    return key < *b->m_pKey;
}