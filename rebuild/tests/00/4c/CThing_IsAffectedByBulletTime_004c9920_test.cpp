#include <cstdio>
// Behaviour model of CThing::IsAffectedByBulletTime (0x004C9920): same decision tree on a flat model.
struct Thing {
    int type; bool dead, overrideBT, notPausedCut, pausedCut, scripted;
    bool door, bob, regionExit, bulletTime, alwaysUpdate, fading;
    bool hasFade, hasCarry, hasEmitter;
    Thing* carrier; Thing* attached;
};
struct Mgr { bool nonScripted, all; Thing* player; } g;
static bool Affected(const Thing* t)
{
    for (;;) {
        if (g.nonScripted || g.all) {
            if (g.nonScripted) {
                if (t->notPausedCut && !t->scripted) return false;
                if (t->pausedCut && t->scripted) return true;
            }
            if (t->type == 2 || t->door || t->bob || t->regionExit) return false;
            if (t->hasFade) return !t->fading;
            if (t->hasCarry && t->carrier) { t = t->carrier; continue; }
            if (t->hasEmitter) return !t->alwaysUpdate;
            if (g.nonScripted) return !t->scripted;
            return true;
        }
        if (t->regionExit) return false;
        const Thing* p = g.player;
        if (p && !p->dead && !t->overrideBT && p->bulletTime) return false;
        if (t->type == 2) return false;
        if (t->hasCarry && t->carrier) { t = t->carrier; continue; }
        if (t->hasEmitter && t->attached && t->attached != t) { t = t->attached; continue; }
        return true;
    }
}
int main()
{
    Thing player = {}; player.bulletTime = true;
    Thing a = {}, b = {};
    g.player = &player;
    int bad = 0;
    if (Affected(&a)) bad |= 1;                      // hero in bullet time: things unaffected
    a.overrideBT = true; if (!Affected(&a)) bad |= 2;
    a.overrideBT = false; player.dead = true; if (!Affected(&a)) bad |= 4;
    player.dead = false; player.bulletTime = false;
    a.type = 2; if (Affected(&a)) bad |= 8; a.type = 0;
    a.hasCarry = true; a.carrier = &b; b.regionExit = true; if (Affected(&a)) bad |= 16; b.regionExit = false;
    a.hasCarry = false; a.hasEmitter = true; a.attached = &a; if (!Affected(&a)) bad |= 32;
    g.all = true; a.alwaysUpdate = true; if (Affected(&a)) bad |= 64;
    a.hasEmitter = false; a.hasFade = true; a.fading = true; if (Affected(&a)) bad |= 128;
    a.hasFade = false; g.all = false; g.nonScripted = true; a.scripted = true; if (Affected(&a)) bad |= 256;
    a.pausedCut = true; if (!Affected(&a)) bad |= 512;
    if (bad) { std::printf("004c9920_TEST FAIL\n"); return 1; }
    std::printf("004c9920_TEST PASS\n");
    return 0;
}