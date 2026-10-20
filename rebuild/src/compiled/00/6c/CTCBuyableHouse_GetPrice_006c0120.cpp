// CTCBuyableHouse::GetPrice() const -- the house's sale price: the value of the trophies mounted in it plus the def's Price for the current dress level, or -1 without a valid level
//
// Retail 0x006C0120, 57 bytes. Callee: CTCBuyableHouse::GetValueOfTrophies() const @ 0x006BFFD0.
// PDef_Object +0x0c, CurrentDressLevel +0x1c (Ego_r.pdb CTCBuyableHouse); CBuyableHouseDef::Price is
// the retail 12-byte vector<long> {first, last, end} at +0x28.
// Shape: size() as an inline `_Last - _First` member (loads last first into edx, first into edi), the
// `prices` first-pointer local taken AFTER the size compare (CSEd onto edi, kept across the call) and the
// level re-read from the member after GetValueOfTrophies. Failures return `or eax,-1`.
template <class T>
struct CRetailVector
{
    T* _First;
    T* _Last;
    T* _End;
    unsigned int size() const { return (unsigned int)(_Last - _First); }
};
struct CBuyableHouseDef { unsigned char _pad_0x00[0x28]; CRetailVector<long> Price; CRetailVector<long> Rent; long DaysBetweenRentPayments; long MaxRentBags; };
#pragma pack(push, 1)
struct CTCBuyableHouse
{
    void* __vftable; void* ParentThing; unsigned char _pad_0x08[0x4];
    const CBuyableHouseDef* PDef_Object;
    bool IsScripted; bool OwnedByPlayer; bool Rented; unsigned char _pad_0x13[0x1];
    long DaySomeoneWillMoveIn; long DayNextRentIsDue; long CurrentDressLevel;
    long GetValueOfTrophies() const;
    long GetPrice() const;
};
#pragma pack(pop)
long CTCBuyableHouse::GetPrice() const
{
    const CBuyableHouseDef* def = PDef_Object;
    if (def != 0 && CurrentDressLevel >= 0 && (unsigned int)CurrentDressLevel < def->Price.size())
    {
        const long* prices = def->Price._First;
        return GetValueOfTrophies() + prices[CurrentDressLevel];
    }
    return -1;
}