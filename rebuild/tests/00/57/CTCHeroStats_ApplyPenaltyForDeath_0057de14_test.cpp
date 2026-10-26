// Behaviour test for 0057de14: src/0057de14.cpp copied verbatim, the definition-manager getter and the renown/money
// adders stubbed so calls and arguments are observable. CTCHeroStats::ApplyPenaltyForDeath()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroStats::ApplyPenaltyForDeath() -- takes the hero-stats definition's renown and money losses on death off the hero (always as losses, whatever their sign)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057DE14, length 67 bytes.
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0 (twice), CTCHeroStats::AddRenown(long)
//          0x0057D8BE, CTCHeroStats::AddMoney(long, bool) 0x0057B327 (Ego_r PDB:
//          ?ApplyPenaltyForDeath@CTCHeroStats@@QAEXXZ, ?AddMoney@CTCHeroStats@@QAEXJ_N@Z).
// The game definition manager (no PDB layout) holds the hero-stats def object at +0xdc; in retail
// CHeroStatsDef the PDB's RenownLossOnDeath (+0x58) and MoneyLossOnDeath (+0x5c) sit 4 bytes lower.
// Shape: `-abs(x)` is the intrinsic `cdq ; xor ; sub ; neg`; the definition manager is fetched again
// for the second penalty (two calls, as written).
// Retry 10b: the second penalty reads the def object through a named `const CHeroStatsDef_Retail* def` local (the
// money-loss magnitude local of the parked form kept ecx for the middle load; the def local gives retail's eax chain).
#include <stdlib.h>
#include "engine/CTCHeroStats.h"

struct CHeroStatsDef_Retail {
    unsigned char _pad_0x00[0x54];
    long          RenownLossOnDeath;           // +0x54 (PDB +0x58)
    long          MoneyLossOnDeath;            // +0x58 (PDB +0x5c)
};

class CGameDefinitionManager { public:
    unsigned char               _pad_0x00[0xdc];
    const CHeroStatsDef_Retail* PHeroStatsDef;  // +0xdc
};

struct CTCHeroStats_Methods : CTCHeroStats {
    const CGameDefinitionManager& GetDefinitionManager() const;   // CTCBase, 0x006869D0
    void AddRenown(long renown);                                  // 0x0057D8BE
    void AddMoney(long money, bool countAsEarned);                // 0x0057B327
    void ApplyPenaltyForDeath();
};

void CTCHeroStats_Methods::ApplyPenaltyForDeath()
{
    AddRenown(-abs(GetDefinitionManager().PHeroStatsDef->RenownLossOnDeath));
    const CHeroStatsDef_Retail* def = GetDefinitionManager().PHeroStatsDef;
    AddMoney(-abs(def->MoneyLossOnDeath), true);
}

// ---- stubs ----
static CHeroStatsDef_Retail g_def; static CGameDefinitionManager g_mgr; static int g_mgrCalls = 0;
const CGameDefinitionManager& CTCHeroStats_Methods::GetDefinitionManager() const { ++g_mgrCalls; return g_mgr; }
static long g_renown = 0, g_money = 0; static bool g_earned = false; static int g_order = 0, g_renownAt = 0, g_moneyAt = 0;
void CTCHeroStats_Methods::AddRenown(long renown) { g_renown = renown; g_renownAt = ++g_order; }
void CTCHeroStats_Methods::AddMoney(long money, bool countAsEarned) { g_money = money; g_earned = countAsEarned; g_moneyAt = ++g_order; }
int main()
{
    g_mgr.PHeroStatsDef = &g_def;
    static unsigned char raw[sizeof(CTCHeroStats_Methods)];
    CTCHeroStats_Methods* tc = (CTCHeroStats_Methods*)raw;
    g_def.RenownLossOnDeath = 25; g_def.MoneyLossOnDeath = -40;
    tc->ApplyPenaltyForDeath();
    CHECK(g_renown == -25 && g_money == -40 && g_earned == true); CHECK(g_renownAt == 1 && g_moneyAt == 2); CHECK(g_mgrCalls == 2);
    g_def.RenownLossOnDeath = -7; g_def.MoneyLossOnDeath = 3;
    tc->ApplyPenaltyForDeath();
    CHECK(g_renown == -7 && g_money == -3); CHECK(g_mgrCalls == 4);
    if (fails == 0) printf("0057de14_TEST PASS\n"); else printf("0057de14_TEST FAIL\n");
    return fails;
}