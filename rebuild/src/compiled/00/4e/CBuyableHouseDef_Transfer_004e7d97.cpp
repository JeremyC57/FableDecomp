// CBuyableHouseDef::Transfer(CPersistContext& context) -- persist the buyable-house def: buyable/empty flags, price and rent tables, rent period, rent-bag cap, scripted and signpost flags
// Leaf: ?Transfer@CBuyableHouseDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7D97, 101 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers, catalog labels propagated): bool 0x0043314A, vector<long> 0x00466A47,
//   long 0x00431102 (the same instantiations the landed CHeroCombatDef::Transfer 0x004665B6 calls).
// Layout: Ego_r.pdb CBuyableHouseDef (0x54) re-based on the retail bytes: the def base data ends at +0x25, so IsBuyable
//   sits at +0x25 and InitiallyEmpty at +0x26 as in the PDB, but the STLport vectors are 12 bytes (PDB 16), so Rent is at
//   +0x34, DaysBetweenRentPayments +0x40, MaxRentBags +0x44, IsScripted +0x48, IsBuyableWithoutSignpost +0x49.
//   No generated engine header for the class; the base bytes are modelled in place (vptr + 0x21 bytes).
// Pragma: #pragma optimize("s", on) -- a size TU: every member address goes through eax.
// Shape: one transfer per member in declaration order, except that IsScripted (+0x48) is transferred ahead of
//   MaxRentBags (+0x44) -- the def's TRANSFER list order.
#pragma optimize("s", on)

template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

class CPersistContext {
public:
    void Transfer(bool& value);                 // 0x0043314A
    void Transfer(long& value);                 // 0x00431102
    void Transfer(CStlVector<long>& value);     // 0x00466A47
};

class CBuyableHouseDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x21];                  // CDefClassBase data (+0x04..+0x24)
    bool IsBuyable;                             // +0x25
    bool InitiallyEmpty;                        // +0x26
    CStlVector<long> Price;                     // +0x28
    CStlVector<long> Rent;                      // +0x34
    long DaysBetweenRentPayments;               // +0x40
    long MaxRentBags;                           // +0x44
    bool IsScripted;                            // +0x48
    bool IsBuyableWithoutSignpost;              // +0x49
};

void CBuyableHouseDef::Transfer(CPersistContext& context)
{
    context.Transfer(IsBuyable);
    context.Transfer(InitiallyEmpty);
    context.Transfer(Price);
    context.Transfer(Rent);
    context.Transfer(DaysBetweenRentPayments);
    context.Transfer(IsScripted);
    context.Transfer(MaxRentBags);
    context.Transfer(IsBuyableWithoutSignpost);
}