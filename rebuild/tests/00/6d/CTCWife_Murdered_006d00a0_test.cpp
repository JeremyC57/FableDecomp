// Behaviour test for CTCWife::Murdered() (0x006D00A0): a model of the row -- only a marriage that has started counts; the wife
// is unmarried, then a living hero gets the murder-spouse deed (59, post type 7) at her position when he has a deed log and
// marriage state 7 for her when he has hero stats.
#include <stdio.h>
struct Thing { bool dead; bool hasLog; bool hasStats; int deed; int deedPost; const float* deedPos; const Thing* spouse; int state; float pos[3]; };
struct Wife {
    long frameMarried; Thing* parent; Thing* hero; long worldFrame; int unmarried;
    void Murdered()
    {
        if (frameMarried > worldFrame)
            return;
        ++unmarried;
        Thing* h = hero;
        if (!h || h->dead)
            return;
        if (h->hasLog) { h->deed = 59; h->deedPost = 7; h->deedPos = parent->pos; }
        if (h->hasStats) { h->spouse = parent; h->state = 7; }
    }
};
int main()
{
    Thing wifeThing = { false, false, false, 0, 0, 0, 0, 0, { 1.0f, 2.0f, 3.0f } };
    Thing hero = { false, true, true, 0, 0, 0, 0, 0, { 0, 0, 0 } };
    Wife w = { 10, &wifeThing, &hero, 20, 0 };
    w.Murdered();
    int ok = w.unmarried == 1 && hero.deed == 59 && hero.deedPost == 7 && hero.deedPos == wifeThing.pos && hero.spouse == &wifeThing && hero.state == 7;
    Thing h2 = hero; h2.deed = 0; h2.state = 0; h2.hasLog = false; w.hero = &h2; w.Murdered();
    ok = ok && w.unmarried == 2 && h2.deed == 0 && h2.state == 7;
    Thing h3 = hero; h3.deed = 0; h3.state = 0; h3.dead = true; w.hero = &h3; w.Murdered();
    ok = ok && w.unmarried == 3 && h3.deed == 0 && h3.state == 0;
    w.hero = 0; w.Murdered(); ok = ok && w.unmarried == 4;
    w.hero = &hero; hero.state = 0; w.frameMarried = 30; w.Murdered(); ok = ok && w.unmarried == 4 && hero.state == 0;
    printf(ok ? "006d00a0_TEST PASS\n" : "006d00a0_TEST FAIL\n");
    return ok ? 0 : 1;
}