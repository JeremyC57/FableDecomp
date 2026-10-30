// CBettingDef::operator=(const CBettingDef&) -- def copy: CDefClassBase part, then twelve betting UI parameters
// Leaf: ??4CBettingDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1918, 93 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment).
// Identification: reached through the incremental-link thunk 0x004E1913 (manifest "CBettingDef::Copy", vtable 0x0123E400);
//   PDB CBettingDef (0x58) = CDefClassBase + BetX, BetY, MoneyX, MoneyY (floats) +0x28..+0x34, MinBet, MaxBet, BetIncrement
//   +0x38..+0x40, Bet, PlayersMoney, Question, Continue, Cancel (text ids) +0x44..+0x54.
//   Byte-identical to CFishDef::operator= 0x004E12B8.
// Shape: memberwise, floats moved through eax like dwords.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

// def vtable 0x0123E3B4: slot 0 0x004D9701 (implicit inline dtor); real overrides from the def vtable scan
class CBettingDef : public CDefClassBase { public:
    float BetX;                         // +0x28
    float BetY;                         // +0x2c
    float MoneyX;                       // +0x30
    float MoneyY;                       // +0x34
    long MinBet;                        // +0x38
    long MaxBet;                        // +0x3c
    long BetIncrement;                  // +0x40
    unsigned long Bet;                  // +0x44
    unsigned long PlayersMoney;         // +0x48
    unsigned long Question;             // +0x4c
    unsigned long Continue;             // +0x50
    unsigned long Cancel;               // +0x54
    CBettingDef& operator=(const CBettingDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DF5C7
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E1913
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D64F8
};

CBettingDef& CBettingDef::operator=(const CBettingDef& other)
{
    CDefClassBase::operator=(other);
    BetX = other.BetX;
    BetY = other.BetY;
    MoneyX = other.MoneyX;
    MoneyY = other.MoneyY;
    MinBet = other.MinBet;
    MaxBet = other.MaxBet;
    BetIncrement = other.BetIncrement;
    Bet = other.Bet;
    PlayersMoney = other.PlayersMoney;
    Question = other.Question;
    Continue = other.Continue;
    Cancel = other.Cancel;
    return *this;
}