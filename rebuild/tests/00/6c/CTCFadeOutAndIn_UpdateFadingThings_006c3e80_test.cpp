// Behaviour test for 0x006C3E80 CTCFadeOutAndIn::UpdateFadingThings().
#include <cstdio>
#include <vector>
#include <algorithm>
struct CThing { int id; bool dead; CThing* carried[2]; };
struct IPtr { CThing* item; CThing* GetPItem() const { return item; } };
struct CIsThingDead { bool operator()(const IPtr& p) const { return p.GetPItem() == 0 || p.GetPItem()->dead; } };
struct CTCFadeOutAndIn {
    CThing* ParentThing;
    std::vector<IPtr> FadingThings;
    void AddThingToFade(CThing& t) {
        for (unsigned i = 0; i < FadingThings.size(); ++i) if (FadingThings[i].item == &t) return;
        IPtr p = { &t }; FadingThings.push_back(p);
    }
    void AddCarriedThingsOfThingToFadeList(const CThing& thing) {
        for (int i = 0; i < 2; ++i) if (thing.carried[i]) AddThingToFade(*thing.carried[i]);
    }
    void UpdateFadingThings() {
        if (!FadingThings.empty())
            FadingThings.erase(std::remove_if(FadingThings.begin(), FadingThings.end(), CIsThingDead()), FadingThings.end());
        AddCarriedThingsOfThingToFadeList(*ParentThing);
        std::vector<IPtr> things(FadingThings);
        for (unsigned int i = 0; i < things.size(); ++i)
            AddCarriedThingsOfThingToFadeList(*things[i].GetPItem());
    }
};
int main() {
    int ok = 1;
    CThing e = { 5, false, { 0, 0 } };                  // carried by d (one level below the copy: not reached this pass)
    CThing d = { 4, false, { &e, 0 } };                 // carried by c
    CThing c = { 3, false, { &d, 0 } };                 // carried by the parent
    CThing dead = { 2, true, { 0, 0 } };
    CThing a = { 1, false, { 0, 0 } };
    CThing parent = { 0, false, { &c, 0 } };
    CTCFadeOutAndIn tc;
    tc.ParentThing = &parent;
    IPtr pa = { &a }, pd = { &dead }, pn = { 0 };
    tc.FadingThings.push_back(pd);
    tc.FadingThings.push_back(pa);
    tc.FadingThings.push_back(pn);
    tc.FadingThings.push_back(pd);
    tc.UpdateFadingThings();
    // dead and null entries removed; parent adds c; the copy {a, c} adds d (from c); e waits for the next pass
    ok &= tc.FadingThings.size() == 3;
    ok &= tc.FadingThings.size() == 3 && tc.FadingThings[0].item == &a && tc.FadingThings[1].item == &c && tc.FadingThings[2].item == &d;
    tc.UpdateFadingThings();
    ok &= tc.FadingThings.size() == 4 && tc.FadingThings[3].item == &e;
    dead.dead = true; a.dead = true;
    tc.UpdateFadingThings();
    ok &= tc.FadingThings.size() == 3 && tc.FadingThings[0].item == &c;
    CTCFadeOutAndIn empty;
    CThing lone = { 9, false, { 0, 0 } };
    empty.ParentThing = &lone;
    empty.UpdateFadingThings();
    ok &= empty.FadingThings.empty();
    if (!ok) { std::printf("006c3e80_TEST FAIL\n"); return 1; }
    std::printf("006c3e80_TEST PASS\n");
    return 0;
}