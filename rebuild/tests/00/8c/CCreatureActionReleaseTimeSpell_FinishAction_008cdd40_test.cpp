// Behaviour test for 0x008CDD40 CCreatureAction_ReleaseTimeSpell::FinishAction: base FinishAction, camera released when a manager
// exists; a live removed item -> a fast-unsheathe action carrying the haste key; otherwise, with bullet time, the bullet-time
// multiplier registered as a haste multiplier under the key (a haste TC added when missing).
#include <cstdio>
#include <map>
#include <string>
struct Haste { std::map<void*, float> Multipliers; int Updates; void SetActionSpeedMultiplier(void* k, float m) { Multipliers.insert(std::make_pair(k, m)); ++Updates; } };
struct Creature { bool HasBulletTime; float TimeMultiplier; Haste* PHaste; Haste Owned; std::string Action; void* ActionKey; };
struct Item { bool Dead; };
struct Camera { bool HasManager; int Released; };
struct Action {
    void* HasteKey; Creature* PCreature; Item* PItemRemoved; Camera* Cam; int BaseFinished;
    void FinishAction()
    {
        ++BaseFinished;
        if (Cam->HasManager) ++Cam->Released;
        if (PItemRemoved && !PItemRemoved->Dead) {
            PCreature->Action = "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE"; PCreature->ActionKey = HasteKey;
        } else if (PCreature->HasBulletTime) {
            Haste* h = PCreature->PHaste;
            if (!h) { PCreature->Owned.Updates = 0; h = PCreature->PHaste = &PCreature->Owned; }
            h->SetActionSpeedMultiplier(HasteKey, PCreature->TimeMultiplier);
        }
    }
};
int main()
{
    int key = 0;
    Camera cam = { true, 0 };
    Creature c; c.HasBulletTime = true; c.TimeMultiplier = 0.5f; c.PHaste = 0; c.ActionKey = 0;
    Item live = { false }, dead = { true };
    Action a = { &key, &c, &live, &cam, 0 };
    a.FinishAction();
    bool ok = c.Action == "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE" && c.ActionKey == &key && cam.Released == 1 && c.PHaste == 0;
    a.PItemRemoved = &dead; cam.HasManager = false;
    a.FinishAction();
    ok = ok && c.PHaste == &c.Owned && c.Owned.Multipliers[&key] == 0.5f && c.Owned.Updates == 1 && cam.Released == 1 && a.BaseFinished == 2;
    c.HasBulletTime = false; a.PItemRemoved = 0; a.FinishAction();
    ok = ok && c.Owned.Updates == 1;
    if (!ok) { std::printf("008cdd40_TEST FAIL\n"); return 1; }
    std::printf("008cdd40_TEST PASS\n");
    return 0;
}