// Behaviour test for CCreatureAction_ScorpionCharge::FinishAction (0x008C55B0): base FinishAction; a live target's
// shield re-allows reactions from the scorpion; with distance left (and having moved > 0.1) a new charge for the
// remaining distance is queued, otherwise component 0xC8 is removed and a strike is queued; nothing when blocked.
#include <cstdio>
#include <cmath>

class C3DVector { public: float X, Y, Z; };
static int g_baseFinish = 0, g_allow = 0, g_removed = 0, g_charge = 0, g_strike = 0;
static float g_chargeDist = 0;
class CThing;
static CThing* g_allowFrom = 0;
struct CShield { void AllowReactionsOnHitFromAttacker(CThing* t, bool f) { if (!f) { ++g_allow; g_allowFrom = t; } } };
class CThing { public:
    unsigned char m_flags91; C3DVector Pos; CShield* Shield; int Blocked;
    const C3DVector& GetPosition() const { return Pos; }
    void RemoveTC(long type) { if (type == 0xc8) ++g_removed; }
    bool GetInterface(CShield** o, long) { if (!Shield) return false; *o = Shield; return true; }
    bool IsCollidingWithLandscape() const { return Blocked != 0; }
    void SetCurrentAction(int kind, float d) { if (kind == 1) { ++g_charge; g_chargeDist = d; } else ++g_strike; }
};
struct CPtr { CThing* p; CThing* GetPItem() const { return p; } };
class CCombatActionBase { public:
    CPtr PCreature, PTarget;
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CThing* GetTarget() const { return PTarget.GetPItem(); }
    bool IsTargetAlive() const { return PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1); }
    void FinishAction() { ++g_baseFinish; }
};
inline void DebugLine(const C3DVector&, const C3DVector&) {}
class CCreatureAction_ScorpionCharge : public CCombatActionBase { public:
    float ChargeDistance; C3DVector StartPos;
    void FinishAction();
};
void CCreatureAction_ScorpionCharge::FinishAction()
{
    const C3DVector& pos = GetCreature()->GetPosition();
    float dx = pos.X - StartPos.X;
    float dy = pos.Y - StartPos.Y;
    float travelled = (float)std::sqrt(dx * dx + dy * dy);
    float remaining = ChargeDistance - travelled;
    DebugLine(GetTarget()->GetPosition(), GetCreature()->GetPosition());
    CCombatActionBase::FinishAction();
    if (IsTargetAlive()) {
        CShield* shield;
        if (PTarget.GetPItem()->GetInterface(&shield, 0x93))
            shield->AllowReactionsOnHitFromAttacker(GetCreature(), false);
    }
    if (IsTargetAlive()) {
        if (remaining > 0.0f && travelled > 0.1f) {
            if (!GetCreature()->IsCollidingWithLandscape())
                GetCreature()->SetCurrentAction(1, remaining);
        } else {
            GetCreature()->RemoveTC(0xc8);
            if (!GetCreature()->IsCollidingWithLandscape())
                GetCreature()->SetCurrentAction(2, 0);
        }
    }
}

int main()
{
    int ok = 1;
    CShield shield;
    CThing scorpion; scorpion.m_flags91 = 0; scorpion.Pos.X = 3; scorpion.Pos.Y = 4; scorpion.Pos.Z = 9; scorpion.Shield = 0; scorpion.Blocked = 0;
    CThing target; target.m_flags91 = 0; target.Pos = scorpion.Pos; target.Shield = &shield; target.Blocked = 0;
    CCreatureAction_ScorpionCharge a; a.PCreature.p = &scorpion; a.PTarget.p = &target;
    a.ChargeDistance = 10; a.StartPos.X = 0; a.StartPos.Y = 0; a.StartPos.Z = 0;
    a.FinishAction();   // travelled 5, remaining 5 -> charge again
    if (g_baseFinish != 1 || g_allow != 1 || g_allowFrom != &scorpion || g_charge != 1 || std::fabs(g_chargeDist - 5) > 1e-4f || g_removed != 0) ok = 0;
    a.ChargeDistance = 5;   // nothing left -> strike
    a.FinishAction();
    if (g_charge != 1 || g_removed != 1 || g_strike != 1) ok = 0;
    scorpion.Blocked = 1; a.ChargeDistance = 10;   // blocked: nothing queued
    a.FinishAction();
    if (g_charge != 1 || g_strike != 1) ok = 0;
    target.m_flags91 = 1; scorpion.Blocked = 0;    // dead target: base only
    a.FinishAction();
    if (g_baseFinish != 4 || g_allow != 3 || g_charge != 1) ok = 0;
    std::printf("008c55b0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}