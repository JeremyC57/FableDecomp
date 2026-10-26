#include <cstdio>
// Behaviour of CTCHeroSpecialMovement::SetSprinting (0x007E0670): starting asks the thing only when not already
// sprinting and needs its approval; stopping always clears.
class CThing { public: bool canSprint; int asked; };
bool IsAvailableForHeroSprint(CThing* thing) { ++thing->asked; return thing->canSprint; }
struct CTCHeroSpecialMovement_Retail {
    CThing* ParentThing;
    bool Sprinting;
    void SetSprinting(bool sprint)
    {
        if (sprint) {
            if (!Sprinting && IsAvailableForHeroSprint(ParentThing))
                Sprinting = true;
        } else {
            if (Sprinting)
                Sprinting = false;
        }
    }
};
int main()
{
    CThing thing; thing.canSprint = false; thing.asked = 0;
    CTCHeroSpecialMovement_Retail m; m.ParentThing = &thing; m.Sprinting = false;
    m.SetSprinting(true);
    if (m.Sprinting || thing.asked != 1) { std::printf("007e0670_TEST FAIL\n"); return 1; }
    thing.canSprint = true;
    m.SetSprinting(true);
    if (!m.Sprinting || thing.asked != 2) { std::printf("007e0670_TEST FAIL\n"); return 1; }
    m.SetSprinting(true);   // already sprinting: no query
    if (!m.Sprinting || thing.asked != 2) { std::printf("007e0670_TEST FAIL\n"); return 1; }
    m.SetSprinting(false);
    if (m.Sprinting || thing.asked != 2) { std::printf("007e0670_TEST FAIL\n"); return 1; }
    std::printf("007e0670_TEST PASS\n");
    return 0;
}