#include <cstdio>

// Model of CTCHeroExperience::IsAbleToBreakDownDoors @ 0x006D6370: a creature (type flag bit 3) that is a child cannot.
struct CThing { unsigned long ThingTypeFlags; bool Child; bool IsChild() const { return Child; } };
struct CTCHeroExperience {
    CThing* ParentThing;
    bool IsAbleToBreakDownDoors() const
    {
        CThing* thing = ParentThing;
        if (thing->ThingTypeFlags & 8) return !thing->IsChild();
        return true;
    }
};

int main()
{
    CThing t; t.ThingTypeFlags = 8; t.Child = false; CTCHeroExperience e; e.ParentThing = &t;
    if (!e.IsAbleToBreakDownDoors()) { std::printf("006d6370_TEST FAIL\n"); return 1; }
    t.Child = true;
    if (e.IsAbleToBreakDownDoors()) { std::printf("006d6370_TEST FAIL\n"); return 1; }
    t.ThingTypeFlags = 0x10;                         // not a creature: the child flag is ignored
    if (!e.IsAbleToBreakDownDoors()) { std::printf("006d6370_TEST FAIL\n"); return 1; }
    std::printf("006d6370_TEST PASS\n");
    return 0;
}