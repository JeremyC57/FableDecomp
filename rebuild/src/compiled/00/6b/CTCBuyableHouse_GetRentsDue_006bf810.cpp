// CTCBuyableHouse::GetRentsDue() const -- how many rent payments have fallen due on a rented house: (today - DayNextRentIsDue) / DaysBetweenRentPayments + 1, else 0
//
// Retail 0x006BF810, 65 bytes. Callee: the component's world accessor @ 0x00686A80 (a non-static
// member that ignores `this` and returns the global game world's +0x24 object; the manifest label
// "CViewBase::GetDisplayEngine" is propagated) -- called twice, the second time with an explicit
// `mov ecx,esi`, so it is a __fastcall member of the component base. Its +0x1c object holds the
// current day at +0x14 (named descriptively: GameTime / Day).
// Layout (Ego_r.pdb CTCBuyableHouse, offsets confirmed): PDef_Object +0x0c, Rented +0x12,
// DayNextRentIsDue +0x18; CBuyableHouseDef::DaysBetweenRentPayments is +0x40 in retail (PDB +0x48:
// the two 16-byte PDB vectors Price/Rent are 12 bytes each in retail).
// Shape: nested ifs; the day is re-fetched through the accessor for the division (`cdq ; idiv`),
// and the clamp is `if (rents > 0) return rents;` falling into the shared `return 0` (`jg` past the
// `xor eax,eax` tail); the ternary `rents > 0 ? rents : 0` compiles branch-free (`setle ; dec ; and`).

struct CGameTime
{
    unsigned char _pad_0x00[0x14];
    long          Day;                              // +0x14
};

class CWorld
{ public:
    unsigned char _pad_0x00[0x1c];
    CGameTime*    GameTime;                         // +0x1c
};

class CBuyableHouseDef
{ public:
    unsigned char _pad_0x00[0x40];
    long          DaysBetweenRentPayments;          // +0x40
};

#pragma pack(push, 1)
class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCBuyableHouse
: public CTCBase { public:
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

    long GetRentsDue() const;
};
#pragma pack(pop)

long CTCBuyableHouse::GetRentsDue() const
{
    if (PDef_Object != 0 && Rented)
    {
        if (PeekWorld().GameTime->Day >= DayNextRentIsDue)
        {
            long rents = (PeekWorld().GameTime->Day - DayNextRentIsDue) / PDef_Object->DaysBetweenRentPayments + 1;
            if (rents > 0)
                return rents;
        }
    }
    return 0;
}