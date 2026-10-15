// CTCBuyableHouse::GetPrice() const -- the house's sale price: the value of the trophies mounted in it plus the def's Price for the current dress level, or -1 without a valid level
//
// Retail 0x006C0120, 57 bytes. Callee: CTCBuyableHouse::GetValueOfTrophies() const @ 0x006BFFD0.
// PDef_Object +0x0c, CurrentDressLevel +0x1c (Ego_r.pdb CTCBuyableHouse); CBuyableHouseDef::Price is
// the retail 12-byte vector<long> {first, last, end} at +0x28.
// Grade: DIFFER(57v57) -- size() computed as `mov edi,[eax+28h] ; mov eax,[eax+2Ch] ; sub eax,edi`
//        where retail loads `mov edx,[eax+2Ch] ; mov edi,[eax+28h] ; sub edx,edi`; everything else exact.
// Shape: def null test, `level >= 0` (signed `jl`), `level < size()` (unsigned `jae`); the Price
// first pointer loaded for size() stays in edi across the GetValueOfTrophies call while the level
// is re-read after it -- that only reproduces with the first pointer in a named local (`prices`);
// `GetValueOfTrophies() + def->Price[level]` computes the element address before the call (48v57),
// a named call-result local re-reads def and first after the call (60v57), and `size()` next to
// the `prices` local adds an ebx spill (61v57). Failures return `or eax,-1`.

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

    long GetValueOfTrophies() const;                // 0x006BFFD0
    long GetPrice() const;
};
#pragma pack(pop)

long CTCBuyableHouse::GetPrice() const
{
    const CBuyableHouseDef* def = PDef_Object;
    if (def != 0 && CurrentDressLevel >= 0)
    {
        const long* prices = def->Price._First;
        if ((unsigned int)CurrentDressLevel < (unsigned int)(def->Price._Last - prices))
            return GetValueOfTrophies() + prices[CurrentDressLevel];
    }
    return -1;
}
