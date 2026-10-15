#include <cstdio>
// CBuyableHouseDef::operator=(const CBuyableHouseDef&) -- def copy: CDefClassBase part, flags, price/rent vectors, rent scalars, flags
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

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CBuyableHouseDef : CDefClassBase {
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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
CLongVector& CLongVector::operator=(const CLongVector& other) { m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEndOfStorage = other.m_pEndOfStorage; return *this; }

int main()
{
    static CBuyableHouseDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.IsBuyable = true;
    b.InitiallyEmpty = true;
    b.Price.m_pFirst = (long*)&b;
    b.Price.m_pLast = (long*)&b + 2;
    b.Rent.m_pEndOfStorage = (long*)&b + 3;
    b.DaysBetweenRentPayments = 7;
    b.MaxRentBags = 4;
    b.IsScripted = true;
    b.IsBuyableWithoutSignpost = true;
    CBuyableHouseDef* r = &(a = b);
    if (r != &a) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e2391"); return 1; }
    if (!(a.IsBuyable == b.IsBuyable)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.InitiallyEmpty == b.InitiallyEmpty)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.Price.m_pFirst == b.Price.m_pFirst)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.Price.m_pLast == b.Price.m_pLast)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.Rent.m_pEndOfStorage == b.Rent.m_pEndOfStorage)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.DaysBetweenRentPayments == b.DaysBetweenRentPayments)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.MaxRentBags == b.MaxRentBags)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.IsScripted == b.IsScripted)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    if (!(a.IsBuyableWithoutSignpost == b.IsBuyableWithoutSignpost)) { std::printf("004e2391_TEST FAIL\n"); return 1; }
    std::printf("004e2391_TEST PASS\n");
    return 0;
}
