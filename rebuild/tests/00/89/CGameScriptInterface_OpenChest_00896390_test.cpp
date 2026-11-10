// Behaviour model of CGameScriptInterface::OpenChest (0x00896390).
#include <cstdio>
struct Thing {
    bool alive, dead, isChest, opened, isHero, playingOpen, tutorialPending, actionAccepted;
    Thing* owner; bool villageMember, hasVillage, trespassable, shop, buyableHouse, ownedByPlayer; int theftDeeds;
};
static Thing MakeThing() { Thing t = { true, false, false, false, false, false, false, true, 0, false, false, false, false, false, false, 0 }; return t; }
static bool OpenChest(Thing* chest, bool useHero, Thing* hero, bool inCutscene)
{
    if (chest->alive && chest->isChest) {
        if (useHero) {
            if (!hero || hero->dead) return false;
            if (!inCutscene && hero->isHero && hero->tutorialPending) return false;
            hero->playingOpen = false;                                   // FinishCurrentAction
            if (hero->actionAccepted) {
                hero->playingOpen = true;
                Thing* owner = chest->owner;
                if (owner && !owner->dead && owner->villageMember && owner->hasVillage && (owner->trespassable || owner->shop)
                    && (!owner->buyableHouse || !owner->ownedByPlayer))
                    ++hero->theftDeeds;
                return true;
            }
        }
        else {
            chest->opened = true;
            return true;
        }
    }
    return false;
}
static int fail(int n) { std::printf("00896390_TEST FAIL %d\n", n); return 1; }
int main()
{
    Thing chest = MakeThing(); chest.isChest = true;
    Thing hero = MakeThing(); hero.isHero = true;
    if (!OpenChest(&chest, false, &hero, false) || !chest.opened || hero.playingOpen) return fail(1);
    Thing notChest = MakeThing();
    if (OpenChest(&notChest, false, &hero, false) || notChest.opened) return fail(2);
    if (!OpenChest(&chest, true, &hero, false) || !hero.playingOpen || hero.theftDeeds != 0) return fail(3);
    Thing owner = MakeThing(); owner.villageMember = true; owner.hasVillage = true; owner.shop = true;
    chest.owner = &owner;
    if (!OpenChest(&chest, true, &hero, false) || hero.theftDeeds != 1) return fail(4);
    owner.buyableHouse = true; owner.ownedByPlayer = true;
    if (!OpenChest(&chest, true, &hero, false) || hero.theftDeeds != 1) return fail(5);
    hero.tutorialPending = true;
    if (OpenChest(&chest, true, &hero, false)) return fail(6);
    if (!OpenChest(&chest, true, &hero, true)) return fail(7);                       // no tutorial during a cut-scene
    hero.actionAccepted = false;
    if (OpenChest(&chest, true, &hero, true)) return fail(8);
    hero.dead = true;
    if (OpenChest(&chest, true, &hero, true) || OpenChest(&chest, true, 0, true)) return fail(9);
    std::printf("00896390_TEST PASS\n");
    return 0;
}