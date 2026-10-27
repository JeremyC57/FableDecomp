// CVtable_012b2f88::Compare(CUnprovenKeyPair_00c19e90* item) -- hand the item's two key words (its virtual slots 1 and 2) to the comparer held at +4 (its slot 1) and return its answer
// Leaf: ?Compare@CVtable_012b2f88@@UAEJPAUCUnprovenKeyPair_00c19e90@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19E90, 43 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B2F88 (slot 1 its scalar deleting destructor 0x00C19F20);
//   the next vtable 0x012B2F90 holds the one-key twin 0x00C19EE0 (`m_p4->slot1(*item->slot1())`). libpk has no PDB or
//   RTTI, so the adaptor is named by its vtable and the types by address.
// Shape: the item's slot 2 (+8) first, its word read at once and parked in ebx, then slot 1 (+4), then the comparer's
//   slot 1 with (first, second). A named local reads the second word straight after its call; the one-expression
//   `Compare(*item->GetFirst(), *item->GetSecond())` keeps the second pointer and dereferences it late (48v43), and a
//   second named local for the first word moves its load above the comparer load (43v43).
struct CUnprovenKeyPair_00c19e90 {
    virtual void v00();
    virtual long* GetFirst();                   // slot 1 (+4)
    virtual long* GetSecond();                  // slot 2 (+8)
};

struct CUnprovenComparer_00c19e90 {
    virtual void v00();
    virtual long Compare(long first, long second);   // slot 1 (+4)
};

class CVtable_012b2f88 {                        // vtable 0x012B2F88: { Compare, scalar deleting destructor }
public:
    virtual long Compare(CUnprovenKeyPair_00c19e90* item);
    CUnprovenComparer_00c19e90* m_pComparer;    // +0x04
};

long CVtable_012b2f88::Compare(CUnprovenKeyPair_00c19e90* item)
{
    long second = *item->GetSecond();
    return m_pComparer->Compare(*item->GetFirst(), second);
}