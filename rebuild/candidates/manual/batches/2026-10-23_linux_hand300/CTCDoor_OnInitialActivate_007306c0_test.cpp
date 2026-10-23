// Behaviour test for CTCDoor::OnInitialActivate (0x007306C0): the animation is snapped to the end of the open or close
// animation per CurrentOpen, the navigation blocker follows the state, the open/close event fires when non-zero, a
// building (type 5) gets its damage state, and proximity doors (trigger type 0) keep frame-updating.
#include <cstdio>

struct Anim { int lastAnim; float cur, last; int resets; };
struct Thing { int type; float maxHealth, health; bool hasNav; bool navBlocking; long lastEvent; bool forceUpdate; float damage; };
struct DoorDef { long triggerType, openEvent, closeEvent; };
struct Door {
    Thing* parent; DoorDef* def; Anim* anim; bool currentOpen;
    void OnInitialActivate()
    {
        anim->resets++;
        anim->lastAnim = currentOpen ? 1 : 2;      // open / close animation
        anim->cur = 1.0f;
        anim->last = 1.0f;
        if (currentOpen) {
            if (parent->hasNav) parent->navBlocking = false;
            long event = def->openEvent;
            if (event) parent->lastEvent = event;
        } else {
            if (parent->hasNav) parent->navBlocking = true;
            long event = def->closeEvent;
            if (event) parent->lastEvent = event;
        }
        if (parent->type == 5) parent->damage = parent->maxHealth - parent->health;
        if (def->triggerType == 0) parent->forceUpdate = true;
    }
};

int main()
{
    Anim anim = { 0, 0.0f, 0.0f, 0 };
    Thing t = { 5, 10.0f, 4.0f, true, false, 0, false, 0.0f };
    DoorDef def = { 0, 11, 22 };
    Door d = { &t, &def, &anim, true };
    d.OnInitialActivate();
    if (anim.lastAnim != 1 || anim.cur != 1.0f || t.navBlocking || t.lastEvent != 11 || t.damage != 6.0f || !t.forceUpdate)
        { std::printf("007306c0_TEST FAIL\n"); return 1; }
    Thing u = { 1, 0.0f, 0.0f, true, false, 0, false, 0.0f };
    DoorDef def2 = { 1, 0, 0 };
    Door e = { &u, &def2, &anim, false };
    e.OnInitialActivate();
    if (anim.lastAnim != 2 || !u.navBlocking || u.lastEvent != 0 || u.damage != 0.0f || u.forceUpdate || anim.resets != 2)
        { std::printf("007306c0_TEST FAIL\n"); return 1; }
    std::printf("007306c0_TEST PASS\n");
    return 0;
}
