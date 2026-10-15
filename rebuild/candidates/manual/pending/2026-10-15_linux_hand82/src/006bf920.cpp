// CTCBuyableHouse::GetPriceForNextDressLevel() const -- the price step from the current dress level to the next one (Price[level+1] - Price[level]), or -1 when there is no next level
//
// Retail 0x006BF920, 51 bytes. No callees. PDef_Object +0x0c, CurrentDressLevel +0x1c (Ego_r.pdb
// CTCBuyableHouse); CBuyableHouseDef::Price is the retail 12-byte vector<long> {first, last, end} at
// +0x28 (PDB +0x28 with a 16-byte vector).
// Shape: def null test, `level >= 0` (signed `jl`), `level + 1 < size()` (unsigned `jae`, size() a
// `sub ; sar 2` pointer difference); the subtraction re-reads the def from `this` and addresses the
// element pair through one `lea ecx,[edx+eax*4]` (`[ecx+4] - [ecx]`). All failures share `or eax,-1`.

template <class T>
struct CRetailVector                                // retail std::vector: {first, last, end}
{
    T* _First;
    T* _Last;
    T* _End;
    unsigned int size() const { return _Last - _First; }
    const T& operator[](unsigned int i) const { return _First[i]; }
};

struct CBuyableHouseDef
{
    unsigned char       _pad_0x00[0x28];
    CRetailVector<long> Price;                      // +0x28
    CRetailVector<long> Rent;                       // +0x34
    long                DaysBetweenRentPayments;    // +0x40
    long                MaxRentBags;                // +0x44
};

#pragma pack(push, 1)
struct CTCBuyableHouse
{
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    const CBuyableHouseDef* PDef_Object;            // +0x0c CDefPointer<const CBuyableHouseDef>::Object
    bool          IsScripted;                       // +0x10
    bool          OwnedByPlayer;                    // +0x11
    bool          Rented;                           // +0x12
    unsigned char _pad_0x13[0x1];                   // +0x13
    long          DaySomeoneWillMoveIn;             // +0x14
    long          DayNextRentIsDue;                 // +0x18
    long          CurrentDressLevel;                // +0x1c

    long GetPriceForNextDressLevel() const;
};
#pragma pack(pop)

long CTCBuyableHouse::GetPriceForNextDressLevel() const
{
    if (PDef_Object != 0 && CurrentDressLevel >= 0 && (unsigned int)(CurrentDressLevel + 1) < PDef_Object->Price.size())
        return PDef_Object->Price[CurrentDressLevel + 1] - PDef_Object->Price[CurrentDressLevel];
    return -1;
}
