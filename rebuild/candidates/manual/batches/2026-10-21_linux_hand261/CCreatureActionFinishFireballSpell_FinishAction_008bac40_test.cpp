// Behaviour test for CCreatureAction_FinishFireballSpell::FinishAction (0x008BAC40): base FinishAction first; 0xA7 flag and
// counter cleared; 0x9D flag set; combination animations 1 and 0x23 removed; mode 0x19 removed unless the player auto-faces;
// a fast-unsheathe action (priority 100, timeout 250) is set when the removed item still lives.
#include <cstdio>
#include <cstring>

struct CThing;
static int g_baseFinish = 0, g_removedMode = -1, g_actions = 0; static int g_combo[4]; static int g_comboCount = 0;
static char g_actName[64]; static CThing* g_actOwner = 0; static CThing* g_actItem = 0; static long g_actPri = 0, g_actTimeout = 0;
struct CPlayer { unsigned char m_autoFaceTarget; };
struct CTCA7 { bool m_active; long m_count; };
struct CTC9D { bool m_flag; };
struct CAnimateThing { void RemoveCombinationAnimsOfType(long t) { g_combo[g_comboCount++] = (int)t; } };
struct CTCAnimation { CAnimateThing Anim; CAnimateThing* GetAnimation() { return &Anim; } };
struct CTCCreatureModeManager { void RemoveMode(long m) { g_removedMode = (int)m; } };
struct CCreatureAction_UnsheatheItemFromInventory;
struct CThing {
    unsigned char m_flags91; CPlayer* Player; CTCA7* A7; CTC9D* C9D; CTCAnimation* Animation; CTCCreatureModeManager* Modes;
    CPlayer* GetPlayer() const { return Player; }
    bool SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory* action);
    bool GetInterface(CTCA7** o, long) { if (!A7) return false; *o = A7; return true; }
    bool GetInterface(CTC9D** o, long) { if (!C9D) return false; *o = C9D; return true; }
    bool GetInterface(CTCAnimation** o, long) { if (!Animation) return false; *o = Animation; return true; }
    bool GetInterface(CTCCreatureModeManager** o, long) { if (!Modes) return false; *o = Modes; return true; }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCharString { const char* s; CCharString(const char* t) : s(t) {} };
struct CCreatureActionBase {
    CBaseIntelligentPointer PCreature; CBaseIntelligentPointer PItemRemoved;
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_UnsheatheItemFromInventory {
    CThing* Owner; const char* Name; CThing* Item; long Pri, Timeout;
    CCreatureAction_UnsheatheItemFromInventory(CThing* owner, const CCharString& n, CThing* item, long pri, long timeout, CCreatureActionBase*)
        : Owner(owner), Name(n.s), Item(item), Pri(pri), Timeout(timeout) {}
    CCreatureAction_UnsheatheItemFromInventory* operator&() const { return const_cast<CCreatureAction_UnsheatheItemFromInventory*>(this); }
};
bool CThing::SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory* a)
{ ++g_actions; std::strcpy(g_actName, a->Name); g_actOwner = a->Owner; g_actItem = a->Item; g_actPri = a->Pri; g_actTimeout = a->Timeout; return true; }
struct CCombatActionBase : CCreatureActionBase { void FinishAction() { ++g_baseFinish; } };
struct CCreatureAction_FinishFireballSpell : CCombatActionBase { void FinishAction(); };

void CCreatureAction_FinishFireballSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCA7* a7;
    if (PCreature.GetPItem()->GetInterface(&a7, 0xa7)) {
        a7->m_active = false;
        a7->m_count = 0;
    }
    CTC9D* c9d;
    if (PCreature.GetPItem()->GetInterface(&c9d, 0x9d))
        c9d->m_flag = true;
    CTCAnimation* animation;
    if (PCreature.GetPItem()->GetInterface(&animation, 0x5a)) {
        CAnimateThing* anim = animation->GetAnimation();
        anim->RemoveCombinationAnimsOfType(1);
        anim->RemoveCombinationAnimsOfType(0x23);
    }
    if (!PCreature.GetPItem()->GetPlayer()->m_autoFaceTarget) {
        CTCCreatureModeManager* modes;
        if (PCreature.GetPItem()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x19);
    }
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString name("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        GetCreature()->SetCurrentAction(&CCreatureAction_UnsheatheItemFromInventory(GetCreature(), name, PItemRemoved.GetPItem(), 100, 250, 0));
    }
}

int main()
{
    int ok = 1;
    CPlayer player; player.m_autoFaceTarget = 0;
    CTCA7 a7; a7.m_active = true; a7.m_count = 5; CTC9D c9d; c9d.m_flag = false; CTCAnimation anim; CTCCreatureModeManager modes;
    CThing hero; hero.m_flags91 = 0; hero.Player = &player; hero.A7 = &a7; hero.C9D = &c9d; hero.Animation = &anim; hero.Modes = &modes;
    CThing sword; sword.m_flags91 = 0;
    CCreatureAction_FinishFireballSpell a; a.PCreature.m_pItem = &hero; a.PItemRemoved.m_pItem = &sword;
    a.FinishAction();
    if (g_baseFinish != 1 || a7.m_active || a7.m_count != 0 || !c9d.m_flag) ok = 0;
    if (g_comboCount != 2 || g_combo[0] != 1 || g_combo[1] != 0x23 || g_removedMode != 0x19) ok = 0;
    if (g_actions != 1 || std::strcmp(g_actName, "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE") != 0 || g_actOwner != &hero
        || g_actItem != &sword || g_actPri != 100 || g_actTimeout != 250) ok = 0;
    // auto-face keeps the mode; a dead item (flag bit 1) and no item set no action
    player.m_autoFaceTarget = 1; g_removedMode = -1; sword.m_flags91 = 1;
    a.FinishAction();
    if (g_removedMode != -1 || g_actions != 1 || g_baseFinish != 2) ok = 0;
    a.PItemRemoved.m_pItem = 0; hero.A7 = 0; hero.Animation = 0; a7.m_count = 9;
    a.FinishAction();
    if (g_actions != 1 || a7.m_count != 9 || g_comboCount != 4) ok = 0;
    std::printf("008bac40_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
