// Behaviour test for 0x006C3CA0 CTCFadeOutAndIn::AddCarriedThingsOfThingToFadeList(const CThing&):
// only a thing with a carrying component contributes; null, dead and already-listed carried things are skipped.
#include <cstdio>
struct Thing { bool HasCarrying; bool Dead; Thing* Carried[4]; int CarriedCount; };
struct Fade {
    Thing* List[16]; int Count;
    Fade() : Count(0) { }
    bool Find(Thing* t) const { for (int i = 0; i < Count; ++i) if (List[i] == t) return true; return false; }
    void AddCarriedThingsOfThingToFadeList(const Thing& thing)
    {
        if (!thing.HasCarrying) return;
        for (int i = 0; i < thing.CarriedCount; ++i) {
            Thing* c = thing.Carried[i];
            if (c && !c->Dead && !Find(c))
                List[Count++] = c;
        }
    }
};
int main()
{
    Thing a = { false, false, { 0 }, 0 }, b = a, dead = a, holder = a, empty = a;
    dead.Dead = true;
    holder.HasCarrying = true;
    holder.Carried[0] = &a; holder.Carried[1] = 0; holder.Carried[2] = &dead; holder.Carried[3] = &b; holder.CarriedCount = 4;
    empty.Carried[0] = &a; empty.CarriedCount = 1;
    Fade f;
    f.List[f.Count++] = &b;
    f.AddCarriedThingsOfThingToFadeList(empty);
    bool ok = f.Count == 1;
    f.AddCarriedThingsOfThingToFadeList(holder);
    ok = ok && f.Count == 2 && f.List[1] == &a;
    f.AddCarriedThingsOfThingToFadeList(holder);
    ok = ok && f.Count == 2;
    if (!ok) { std::printf("006c3ca0_TEST FAIL\n"); return 1; }
    std::printf("006c3ca0_TEST PASS\n");
    return 0;
}