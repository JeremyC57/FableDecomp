// CBettingDef::Transfer(CPersistContext& context) -- persist every member without member names
// Leaf: ?Transfer@CBettingDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF5C7, 145 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): slot 18 of vtable 0x0123E3B4 (RTTI .?AVCBettingDef@@; Ego_r.pdb ?Transfer@CBettingDef@@UAEXAAVCPersistContext@@@Z);
//   the 12 Transfer calls take, in call order, the members at +0x28, +0x2c, +0x30, +0x34, +0x38, +0x3c, +0x40, +0x44, +0x48, +0x4c, +0x50, +0x54 with the per-type callees 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431102, 0x00431102, 0x00431102, 0x00431020, 0x00431020, 0x00431020, 0x00431020, 0x00431020, which are
//   Ego_r.pdb CBettingDef's members BetX, BetY, MoneyX, MoneyY, MinBet, MaxBet, BetIncrement, Bet, PlayersMoney, Question, Continue, Cancel.
// Shape (family M053, representative 0x0046923D): `context.Transfer(Member);` per member; `optimize("s", on)` (each member
//   address formed with `lea eax,[esi+N]`, the last as `add esi,N`).
// Header debt: the def base is modelled by its retail size (pack(1), vptr + members) with the virtual Transfer only.
#pragma optimize("s", on)
class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};
#pragma pack(push, 1)
class CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x25 - 4];
};
#pragma pack(pop)
class CBettingDef : public CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    float                    BetX;                          // +0x28
    float                    BetY;                          // +0x2c
    float                    MoneyX;                        // +0x30
    float                    MoneyY;                        // +0x34
    long                     MinBet;                        // +0x38
    long                     MaxBet;                        // +0x3c
    long                     BetIncrement;                  // +0x40
    unsigned long            Bet;                           // +0x44
    unsigned long            PlayersMoney;                  // +0x48
    unsigned long            Question;                      // +0x4c
    unsigned long            Continue;                      // +0x50
    unsigned long            Cancel;                        // +0x54
};
void CBettingDef::Transfer(CPersistContext& context)
{
    context.Transfer(BetX);
    context.Transfer(BetY);
    context.Transfer(MoneyX);
    context.Transfer(MoneyY);
    context.Transfer(MinBet);
    context.Transfer(MaxBet);
    context.Transfer(BetIncrement);
    context.Transfer(Bet);
    context.Transfer(PlayersMoney);
    context.Transfer(Question);
    context.Transfer(Continue);
    context.Transfer(Cancel);
}