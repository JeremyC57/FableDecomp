// Behaviour test for CCreatureAction_CastWillLightningSpell(owner, state, param, target) (0x008BCA00): action 150,
// both parameters stored, the animation named, the cast sound played only when the creature has a sound component,
// and the target handed to PTarget.
#include <cstdio>
#include <string>

class CThing { public: bool HasSound; std::string Played; };
class CLightningDef { public: long RefCount; std::string CastSound; };
static CLightningDef g_def = { 1, "SND_LIGHTNING_CAST" };
struct CAction {
    CThing* PCreature; long Code; CThing* PTarget; std::string Anim; long State, Param;
    CAction(CThing* owner, long state, long param, CThing* target)
        : PCreature(owner), Code(150), PTarget(0), State(state), Param(param)
    {
        Anim = "WILL_CAST_LIGHTNING";
        CLightningDef* def = &g_def; ++def->RefCount;
        if (PCreature->HasSound)
            PCreature->Played = def->CastSound;
        PTarget = target;
        --def->RefCount;
    }
};

int main()
{
    CThing me = { true, "" }, foe = { false, "" };
    CAction a(&me, 2, 7, &foe);
    bool ok = a.Code == 150 && a.State == 2 && a.Param == 7 && a.PTarget == &foe && a.Anim == "WILL_CAST_LIGHTNING"
        && me.Played == "SND_LIGHTNING_CAST" && g_def.RefCount == 1;
    CAction b(&foe, 0, 0, 0);
    ok = ok && foe.Played.empty() && b.PTarget == 0;
    if (!ok) { std::printf("008bca00_TEST FAIL\n"); return 1; }
    std::printf("008bca00_TEST PASS\n");
    return 0;
}