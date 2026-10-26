#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_morality = 0, g_renown = 0;
class CTCHeroStats { public: unsigned char _pad[0x28]; long Morality; void SetMorality(long m) { g_morality = m; } void AddRenown(long r) { g_renown += r; } };
struct SDef { unsigned char _pad[0x28]; long MoralityChange; unsigned char _pad2c[0xc]; long RenownChange; };
struct CKilledThing { unsigned char _pad[4]; SDef* m_pDef; void ModifyHeroStats(CTCHeroStats* s) { s->SetMorality(s->Morality + m_pDef->MoralityChange); s->AddRenown(m_pDef->RenownChange); } };

int main()
{
    CTCHeroStats s; s.Morality = 10; SDef d; d.MoralityChange = -4; d.RenownChange = 25; CKilledThing k; k.m_pDef = &d; k.ModifyHeroStats(&s); if (g_morality != 6 || g_renown != 25) { std::printf("008f5bf0_TEST FAIL\n"); return 1; }
    std::printf("008f5bf0_TEST PASS\n");
    return 0;
}