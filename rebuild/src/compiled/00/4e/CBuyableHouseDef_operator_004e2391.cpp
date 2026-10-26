// CBuyableHouseDef::operator=(const CBuyableHouseDef&) -- def copy: CDefClassBase part, flags, price/rent vectors, rent scalars, flags
// Leaf: ??4CBuyableHouseDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2391, 81 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CBuyableHouseDef (0x54; retail 0x4c) = CDefClassBase + IsBuyable, InitiallyEmpty (bool) +0x25/+0x26,
//   Price, Rent (std::vector<long>, 12 bytes in retail) +0x28/+0x34, DaysBetweenRentPayments +0x40, MaxRentBags +0x44,
//   IsScripted, IsBuyableWithoutSignpost (bool) +0x48/+0x49 (PDB offsets 4/8 higher: its vectors are 16 bytes).
// Shape: memberwise; the vectors go through the out-of-line vector<long>::operator= (manifest label
//   "CActiveFile::operator=" at 0x00454886 is propagated and wrong).
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

class CBuyableHouseDef : public CDefClassBase { public:
    bool IsBuyable;                     // +0x25
    bool InitiallyEmpty;                // +0x26
    CLongVector Price;                  // +0x28
    CLongVector Rent;                   // +0x34
    long DaysBetweenRentPayments;       // +0x40
    long MaxRentBags;                   // +0x44
    bool IsScripted;                    // +0x48
    bool IsBuyableWithoutSignpost;      // +0x49
    CBuyableHouseDef& operator=(const CBuyableHouseDef& other);
};

CBuyableHouseDef& CBuyableHouseDef::operator=(const CBuyableHouseDef& other)
{
    CDefClassBase::operator=(other);
    IsBuyable = other.IsBuyable;
    InitiallyEmpty = other.InitiallyEmpty;
    Price = other.Price;
    Rent = other.Rent;
    DaysBetweenRentPayments = other.DaysBetweenRentPayments;
    MaxRentBags = other.MaxRentBags;
    IsScripted = other.IsScripted;
    IsBuyableWithoutSignpost = other.IsBuyableWithoutSignpost;
    return *this;
}