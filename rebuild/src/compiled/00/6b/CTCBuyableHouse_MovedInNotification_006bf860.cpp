// CTCBuyableHouse::MovedInNotification(long day) -- a tenant moved in on `day`: the next rent falls due no later than the following day
//
// Retail 0x006BF860, 37 bytes (`ret 4`). No callees. Called twice from CTCDummyVillager::CreateVillager.
// DayNextRentIsDue is +0x18 (Ego_r.pdb CTCBuyableHouse).
// Shape: the Dinkumware const-reference `std::min` (`_Right < _Left ? _Right : _Left`, both operands
// by reference): the incremented argument is written back to its own stack slot because its address
// is taken (`mov [esp+4],eax ; lea eax,[esp+4] ; mov eax,[eax]`), and the "keep the member" arm is a
// self-store that re-reads the member (`mov edx,[ecx+18h] ; mov [ecx+18h],edx`).

template <class T>
inline const T& DinkumMin(const T& _Left, const T& _Right)   // VC6/7.1 Dinkumware std::min shape
{
    return _Right < _Left ? _Right : _Left;
}

#pragma pack(push, 1)
class CTCBuyableHouse
{ public:
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    void*         PDef_Object;                      // +0x0c CDefPointer<const CBuyableHouseDef>::Object
    bool          IsScripted;                       // +0x10
    bool          OwnedByPlayer;                    // +0x11
    bool          Rented;                           // +0x12
    unsigned char _pad_0x13[0x1];                   // +0x13
    long          DaySomeoneWillMoveIn;             // +0x14
    long          DayNextRentIsDue;                 // +0x18

    void MovedInNotification(long day);
};
#pragma pack(pop)

void CTCBuyableHouse::MovedInNotification(long day)
{
    ++day;
    DayNextRentIsDue = DinkumMin(DayNextRentIsDue, day);
}