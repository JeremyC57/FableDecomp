// CVtable_012b4340::Equal(const CUnprovenItem_012b4340* a, const CUnprovenItem_012b4340* b) const -- whether items a and b have the same float key
// Leaf: ?Equal@CVtable_012b4340@@UBEHPBUCUnprovenItem_012b4340@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2B750, thiscall (this unused), `ret 8`, int result (`mov eax,1` / `xor eax,eax`).
// Identity (descriptive): slot 0 of the twin non-RTTI comparator vtables 0x012B4340 / 0x012B4460 (each stored once by
//   its container's constructor, 0x00C2B6AB / 0x00C2A8FE ..., slot 4 the scalar deleting destructor), the libpk
//   container comparator family of the landed CVtable_012b28d8 rows (Equal / Less / EqualToKey / LessThanKey), here
//   ordered by the float key an item keeps at +4. libpk has no PDB or RTTI, so the class is named by its vtable and
//   the item type by address.
// Shape: the x87 compare of the two floats (`fucompp` for ==, `fcomp` for <) and the two return blocks.
struct CUnprovenItem_012b4340 {
    void* _00;                                  // +0x00
    float m_key;                                // +0x04 the sort key
};

class CVtable_012b4340 {                        // comparator; this unused (vtables 0x012B4340 / 0x012B4460)
public:
    virtual int Equal(const CUnprovenItem_012b4340* a, const CUnprovenItem_012b4340* b) const;   // slot 0
    virtual int Less(const CUnprovenItem_012b4340* a, const CUnprovenItem_012b4340* b) const;    // slot 1
    virtual int EqualToKey(const float& key, const CUnprovenItem_012b4340* b) const;            // slot 2
    virtual int LessThanKey(const float& key, const CUnprovenItem_012b4340* b) const;           // slot 3
};

int CVtable_012b4340::Equal(const CUnprovenItem_012b4340* a, const CUnprovenItem_012b4340* b) const
{
    return a->m_key == b->m_key;
}