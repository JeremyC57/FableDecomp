#include <cstdio>
// CBettingDef::operator=(const CBettingDef&) -- def copy: CDefClassBase part, then twelve betting UI parameters
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

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CBettingDef : CDefClassBase {
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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }

int main()
{
    static CBettingDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.BetX = 0.1f;
    b.BetY = 0.2f;
    b.MoneyX = 0.3f;
    b.MoneyY = 0.4f;
    b.MinBet = 10;
    b.MaxBet = 11;
    b.BetIncrement = 12;
    b.Bet = 13;
    b.PlayersMoney = 14;
    b.Question = 15;
    b.Continue = 16;
    b.Cancel = 17;
    CBettingDef* r = &(a = b);
    if (r != &a) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e1918"); return 1; }
    if (!(a.BetX == b.BetX)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.BetY == b.BetY)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.MoneyX == b.MoneyX)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.MoneyY == b.MoneyY)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.MinBet == b.MinBet)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.MaxBet == b.MaxBet)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.BetIncrement == b.BetIncrement)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.Bet == b.Bet)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.PlayersMoney == b.PlayersMoney)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.Question == b.Question)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.Continue == b.Continue)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    if (!(a.Cancel == b.Cancel)) { std::printf("004e1918_TEST FAIL\n"); return 1; }
    std::printf("004e1918_TEST PASS\n");
    return 0;
}