// Behaviour test for 007cf860: src/007cf860.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCTavernGameShoveHaPenny::CalcScores()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTavernGameShoveHaPenny::CalcScores() -- recomputes the score: each coin that lies wholly inside a board region band (between the region's start line and the next one) scores the lowest such region's score
// Leaf: CalcScores
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CF860, 313 bytes (plain `ret`).
// Callees: CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer<const CTavernGameShoveHaPennyDef>*) 0x007D1260
//          (on the owner's def class, owner +0x70; this TU, see the stand-in below); __ftol2 0x00BFEA70 (the `(long)` cast);
//          the def's virtual slot 1 on release.
// Layout (retail): CTCTavernGame::Score +0x2c; CTCTavernGameCoinBase Coins +0xfc (0x3c-byte Coin: Pos +0x04, Radius
//          +0x30), BoardSize +0x130; CTavernGameShoveHaPennyDef::Regions +0x1a4 (PDB +0x1a8; 0x30-byte
//          CShoveHaPennyRegionDef: Start +0x28, Score +0x2c).
// Shape: the region loop re-evaluates `Regions.size() - 1` (unsigned `jb`/`jbe`) every pass; the coin's far edge
//        `Radius + Pos.Y` (written in retail's load order) is loop-invariant and stays on the x87 stack across the region loop; `this` is spilled and its
//        register walks the regions; the best score starts at 1000 and is added to the score only when one was found.
//        Regions[i] goes through the STLport `*(begin() + i)` operator[] with an inline begin(); a raw `*(_M_start + i)`
//        loads Pos.Y before Radius in the hoisted sum (operand order flips).
class CThing;
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };

struct Coin {                                   // 0x3c bytes (packed in retail; only floats are read here)
    long      _pad_0x00;
    C2DVector Pos;                              // +0x04
    unsigned char _pad_0x0c[0x24];
    float     Radius;                           // +0x30
    long      Owner;                            // +0x34
    unsigned long Sound;                        // +0x38
};
struct CShoveHaPennyRegionDef {                 // 0x30 bytes
    unsigned char _defbase[0x28];
    float         Start;                        // +0x28
    float         Score;                        // +0x2c
};
struct CRegionVector {                          // STLport vector<CShoveHaPennyRegionDef> (12 bytes)
    CShoveHaPennyRegionDef* _M_start;
    CShoveHaPennyRegionDef* _M_finish;
    CShoveHaPennyRegionDef* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CShoveHaPennyRegionDef* begin() const { return _M_start; }
    CShoveHaPennyRegionDef& operator[](unsigned long i) const { return *(begin() + i); }
};

struct CTavernGameShoveHaPennyDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          m_count;                      // +0x04
    unsigned char _pad_0x08[0x19c];
    CRegionVector Regions;                      // +0x1a4
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CShoveHaPennyDefPointer {                // CDefPointer<const CTavernGameShoveHaPennyDef>
    CTavernGameShoveHaPennyDef* Object;
    CShoveHaPennyDefPointer() : Object(0) {}
    ~CShoveHaPennyDefPointer() { if (Object) Object->Release(); }
    CTavernGameShoveHaPennyDef* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CShoveHaPennyDefPointer* out);   // 0x007D1260
};
CTavernGameShoveHaPennyDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x007D1260 is in this TU, out of line, see the landed CDefClassBase_GetSubDef007d1260): it only
// writes the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does not escape the call.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CShoveHaPennyDefPointer* out)
{
    CTavernGameShoveHaPennyDef* def = FindRef_CDefClassBase_GetSubDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

struct CThingDefOwner {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* DefClass;                    // +0x70
};

struct CTCTavernGameShoveHaPenny {
    virtual void v00();
    CThingDefOwner* Owner;                      // +0x04
    unsigned char   _pad_0x08[0x24];
    float           Score;                      // +0x2c
    unsigned char   _pad_0x30[0xcc];
    Coin*           CoinsBegin;                 // +0xfc
    Coin*           CoinsEnd;                   // +0x100
    Coin*           CoinsEndOfStorage;          // +0x104
    unsigned char   _pad_0x108[0x28];
    C3DVector       BoardSize;                  // +0x130
    void CalcScores();
};

void CTCTavernGameShoveHaPenny::CalcScores()
{
    CShoveHaPennyDefPointer def;
    Owner->DefClass->GetSubDef(&def);
    Score = 0;
    for (Coin* coin = CoinsBegin; coin != CoinsEnd; ++coin) {
        long best = 1000;
        for (unsigned long i = 0; i < def->Regions.size() - 1; ++i) {
            if (coin->Radius + coin->Pos.Y > BoardSize.Y * def->Regions[i].Start
                && coin->Pos.Y - coin->Radius < def->Regions[i + 1].Start * BoardSize.Y
                && best > def->Regions[i].Score)
                best = (long)def->Regions[i].Score;
        }
        if (best != 1000)
            Score += best;
    }
}

// ---- stubs and driver ----
static int g_find = 0, g_destroy = 0; static CDefClassBase* g_findArg = 0;
static CTavernGameShoveHaPennyDef* g_ret = 0;
CTavernGameShoveHaPennyDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass) { ++g_find; g_findArg = defClass; return g_ret; }
void CTavernGameShoveHaPennyDef::v00() { }
void CTavernGameShoveHaPennyDef::Destroy() { ++g_destroy; }
void CTCTavernGameShoveHaPenny::v00() { }

static CTavernGameShoveHaPennyDef g_def;
static CShoveHaPennyRegionDef g_regions[3];
static Coin g_coins[4];
static CDefClassBase g_defClass;
static CThingDefOwner g_owner;
static void SetCoin(Coin& c, float y, float r) { memset(&c, 0, sizeof(c)); c.Pos.X = 3.0f; c.Pos.Y = y; c.Radius = r; }
int main()
{
    static CTCTavernGameShoveHaPenny game;
    g_owner.DefClass = &g_defClass; game.Owner = &g_owner; game.BoardSize.X = 1.0f; game.BoardSize.Y = 100.0f; game.BoardSize.Z = 0.0f;
    g_regions[0].Start = 0.0f;  g_regions[0].Score = 10.7f;
    g_regions[1].Start = 0.5f;  g_regions[1].Score = 20.0f;
    g_regions[2].Start = 1.0f;  g_regions[2].Score = 5.0f;   // last entry only bounds the band before it
    g_def.Regions._M_start = g_regions; g_def.Regions._M_finish = g_regions + 3; g_def.Regions._M_end_of_storage = g_regions + 3;
    SetCoin(g_coins[0], 20.0f, 5.0f);    // band 0 only -> 10 (truncated 10.7)
    SetCoin(g_coins[1], 70.0f, 5.0f);    // band 1 only -> 20
    SetCoin(g_coins[2], 48.0f, 5.0f);    // straddles both bands -> lowest score 10
    SetCoin(g_coins[3], 200.0f, 1.0f);   // off the board -> nothing
    game.CoinsBegin = g_coins; game.CoinsEnd = g_coins + 4; game.CoinsEndOfStorage = g_coins + 4;

    // all four coins, def kept alive by another reference
    g_ret = &g_def; g_def.m_count = 2; g_find = g_destroy = 0; game.Score = 99.0f;
    game.CalcScores();
    CHECK(g_find == 1 && g_findArg == &g_defClass);
    CHECK(game.Score == 40.0f);
    CHECK(g_def.m_count == 1 && g_destroy == 0);

    // last reference released -> Destroy
    g_def.m_count = 1; g_destroy = 0; game.CoinsEnd = g_coins + 2; game.Score = 1.0f;
    game.CalcScores();
    CHECK(game.Score == 30.0f && g_def.m_count == 0 && g_destroy == 1);

    // only the off-board coin: score reset to 0, nothing added
    g_def.m_count = 2; game.CoinsBegin = g_coins + 3; game.CoinsEnd = g_coins + 4; game.Score = 7.0f;
    game.CalcScores();
    CHECK(game.Score == 0.0f && g_def.m_count == 1);

    // no def and no coins: score cleared, no release
    g_ret = 0; g_find = 0; g_destroy = 0; game.CoinsBegin = game.CoinsEnd = g_coins; game.Score = 5.0f;
    game.CalcScores();
    CHECK(g_find == 1 && game.Score == 0.0f && g_destroy == 0);

    // a single band (2 regions): a coin above the second start is outside
    g_ret = &g_def; g_def.m_count = 2; g_def.Regions._M_finish = g_regions + 2;
    game.CoinsBegin = g_coins; game.CoinsEnd = g_coins + 3; game.Score = 0.0f;
    game.CalcScores();
    CHECK(game.Score == 20.0f);   // coin0 10, coin1 nothing (near edge 65 not below 50), coin2 10
    if (fails == 0) printf("007cf860_TEST PASS\n");
    return fails;
}
