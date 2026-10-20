#include <stdio.h>
// Behaviour test for CQR_EscortTraderScript::GetSuccessType (0x00E24100): perfect when untouched at full health,
// great/good by the script def's hit limits and health fractions, else basic.
struct CScriptThing { float Health; };
struct CGameScriptInterface {
    virtual float GetHealth(const CScriptThing& thing) const { return thing.Health; }
};
struct CScriptDef {
    unsigned char _pad000[0x988];
    long  EscortTraderGreatMaxHits;
    long  EscortTraderGoodMaxHits;
    float EscortTraderGreatMinHealthFraction;
    float EscortTraderGoodMinHealthFraction;
};
static CScriptDef g_def;
CScriptDef* g_pScriptDef = &g_def;
namespace NScript {
struct CScriptBase { unsigned char _body[0x3c]; };
struct CQR_EscortTraderScript : CScriptBase {
    enum EscortTraderSuccessType { ESCORT_TRADER_SUCCESS_BASIC = 0, ESCORT_TRADER_SUCCESS_GOOD = 1, ESCORT_TRADER_SUCCESS_GREAT = 2, ESCORT_TRADER_SUCCESS_PERFECT = 3 };
    CGameScriptInterface* GSI;
    unsigned char _pad044[0x20];
    long  TraderHitCount;
    long  TraderHealthBarID;
    float TraderInitialHealth;
    EscortTraderSuccessType GetSuccessType(const CScriptThing& trader);
};
CQR_EscortTraderScript::EscortTraderSuccessType CQR_EscortTraderScript::GetSuccessType(const CScriptThing& trader)
{
    if (TraderHitCount == 0 && GSI->GetHealth(trader) == TraderInitialHealth)
        return ESCORT_TRADER_SUCCESS_PERFECT;
    if (TraderHitCount <= g_pScriptDef->EscortTraderGreatMaxHits) {
        CScriptDef* def = g_pScriptDef;
        if (def->EscortTraderGreatMinHealthFraction <= GSI->GetHealth(trader) / TraderInitialHealth)
            return ESCORT_TRADER_SUCCESS_GREAT;
    }
    if (TraderHitCount <= g_pScriptDef->EscortTraderGoodMaxHits) {
        CScriptDef* def = g_pScriptDef;
        if (def->EscortTraderGoodMinHealthFraction <= GSI->GetHealth(trader) / TraderInitialHealth)
            return ESCORT_TRADER_SUCCESS_GOOD;
    }
    return ESCORT_TRADER_SUCCESS_BASIC;
}
}
int main()
{
    int ok = 1;
    CGameScriptInterface gsi;
    g_def.EscortTraderGreatMaxHits = 2; g_def.EscortTraderGoodMaxHits = 5;
    g_def.EscortTraderGreatMinHealthFraction = 0.75f; g_def.EscortTraderGoodMinHealthFraction = 0.5f;
    NScript::CQR_EscortTraderScript q; q.GSI = &gsi; q.TraderInitialHealth = 100.0f;
    CScriptThing t;
    q.TraderHitCount = 0; t.Health = 100.0f;
    if (q.GetSuccessType(t) != NScript::CQR_EscortTraderScript::ESCORT_TRADER_SUCCESS_PERFECT) ok = 0;
    q.TraderHitCount = 0; t.Health = 80.0f;
    if (q.GetSuccessType(t) != NScript::CQR_EscortTraderScript::ESCORT_TRADER_SUCCESS_GREAT) ok = 0;
    q.TraderHitCount = 2; t.Health = 75.0f;
    if (q.GetSuccessType(t) != NScript::CQR_EscortTraderScript::ESCORT_TRADER_SUCCESS_GREAT) ok = 0;
    q.TraderHitCount = 3; t.Health = 100.0f;
    if (q.GetSuccessType(t) != NScript::CQR_EscortTraderScript::ESCORT_TRADER_SUCCESS_GOOD) ok = 0;
    q.TraderHitCount = 1; t.Health = 60.0f;
    if (q.GetSuccessType(t) != NScript::CQR_EscortTraderScript::ESCORT_TRADER_SUCCESS_GOOD) ok = 0;
    q.TraderHitCount = 5; t.Health = 49.0f;
    if (q.GetSuccessType(t) != NScript::CQR_EscortTraderScript::ESCORT_TRADER_SUCCESS_BASIC) ok = 0;
    q.TraderHitCount = 6; t.Health = 100.0f;
    if (q.GetSuccessType(t) != NScript::CQR_EscortTraderScript::ESCORT_TRADER_SUCCESS_BASIC) ok = 0;
    if (ok) { printf("00e24100_TEST PASS\n"); return 0; }
    printf("00e24100_TEST FAIL\n"); return 1;
}