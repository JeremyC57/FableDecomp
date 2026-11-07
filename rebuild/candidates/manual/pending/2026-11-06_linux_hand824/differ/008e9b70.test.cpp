// Behaviour test for NCreatureMode::CModeStrafe::AnimUpdate (0x008E9B70): the main strafe anim restarts only when the strafing-anim
// name changed or the main set is empty, finished or of another type (with no blend-in when ForceAnimNoBlendIn); without a usable
// projectile weapon (or under an ability mutex lock) the aim / build-up / pull-back combination anims are dropped and the upper-body
// anim follows the ability's strafe anim, else PAnimStrafeUpperBody, else is removed; with one the aim anim is added at the best frame
// (0.5 when none), the build-up anim restarts when finished and the pull-back anim is removed while an interrupting action runs or the
// weapon is not loaded; ForceAnimNoBlendIn is always cleared.
#include <cstdio>
#include <string>
#include <set>

struct Mode {
    std::string strafeName; bool forceNoBlend; bool hasUpperBody; float pullBack;
    // inputs
    std::string currentName; bool mainEmpty, mainFinished; int mainType;
    bool projectile, locked; std::string abilityAnim; bool interrupting, loaded; float bestFrame;
    // outputs
    int mainRestarts; int lastBlend; std::set<int> combos; float aimFrame;
    Mode() : forceNoBlend(false), hasUpperBody(false), pullBack(0.0f), mainEmpty(false), mainFinished(false), mainType(0), projectile(false),
        locked(false), interrupting(false), loaded(false), bestFrame(0.0f), mainRestarts(0), lastBlend(0), aimFrame(0.0f) { }   // VC7.1: no value-init
    void AnimUpdate()
    {
        if (currentName != strafeName || mainEmpty || mainFinished || mainType != 0x1b) {
            strafeName = currentName; ++mainRestarts; lastBlend = forceNoBlend ? 0 : 3; mainEmpty = false; mainFinished = false; mainType = 0x1b;
        }
        if (locked || !projectile) {
            combos.erase(0x24); combos.erase(0x25); combos.erase(0x1f);
            if (locked && abilityAnim != "") combos.insert(0x23);
            else if (hasUpperBody) combos.insert(0x23);
            else combos.erase(0x23);
        } else {
            combos.erase(0x23);
            if (!combos.count(0x24)) { combos.insert(0x24); aimFrame = bestFrame > -1e-4f ? bestFrame : 0.5f; }
            combos.insert(0x25);
            if (interrupting || !loaded) combos.erase(0x1f);
            else combos.insert(0x1f);
        }
        forceNoBlend = false;
    }
};

static Mode Make()
{
    Mode m = Mode();
    m.strafeName = "STRAFE_L"; m.currentName = "STRAFE_L"; m.mainType = 0x1b; m.loaded = true; m.bestFrame = -1.0f;
    return m;
}

int main()
{
    bool ok = true;
    Mode a = Make(); a.AnimUpdate();
    ok = ok && a.mainRestarts == 0 && a.combos.empty();
    Mode b = Make(); b.currentName = "STRAFE_R"; b.forceNoBlend = true; b.hasUpperBody = true; b.AnimUpdate();
    ok = ok && b.mainRestarts == 1 && b.lastBlend == 0 && b.strafeName == "STRAFE_R" && b.combos.count(0x23) && !b.forceNoBlend;
    Mode c = Make(); c.projectile = true; c.AnimUpdate();
    ok = ok && c.combos.count(0x24) && c.combos.count(0x25) && c.combos.count(0x1f) && !c.combos.count(0x23) && c.aimFrame == 0.5f;
    Mode d = Make(); d.projectile = true; d.interrupting = true; d.bestFrame = 0.25f; d.AnimUpdate();
    ok = ok && !d.combos.count(0x1f) && d.aimFrame == 0.25f;
    Mode e = Make(); e.projectile = true; e.locked = true; e.abilityAnim = "ABILITY_STRAFE"; e.AnimUpdate();
    ok = ok && e.combos.size() == 1 && e.combos.count(0x23);
    Mode f = Make(); f.mainFinished = true; f.AnimUpdate();
    ok = ok && f.mainRestarts == 1 && f.lastBlend == 3;
    if (!ok) { std::printf("008e9b70_TEST FAIL\n"); return 1; }
    std::printf("008e9b70_TEST PASS\n");
    return 0;
}
