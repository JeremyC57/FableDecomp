// Behaviour test for CThingAICreatureInit::CThingAICreatureInit @ 0x004FACD0: arguments stored, names copied, defaults set.
#include <cstdio>
struct CThingPhysical { int id; };
struct CThingBuilding;
struct CCharString {
    const char* m_text; int copies;
    CCharString(const char* t) : m_text(t), copies(0) { }
    CCharString(const CCharString& other) : m_text(other.m_text), copies(other.copies + 1) { }
};
struct CThingAICreatureInit {
    CThingPhysical* OriginalThing; CThingPhysical* PreviousThing;
    CCharString ScriptName; CCharString OverridingBrainName;
    bool PlayerCharacter, Drawable, PlaceAtEndOfList, EnableCreatureAutoPlacing;
    CThingBuilding* HomeBuilding; CThingBuilding* WorkBuilding;
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName),
          OverridingBrainName(overridingBrainName), PlayerCharacter(playerCharacter), Drawable(drawable),
          PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false), HomeBuilding(0), WorkBuilding(0) { }
};
int main()
{
    CThingPhysical a, b; CCharString s("Script"), n("Brain");
    CThingAICreatureInit init(&a, &b, true, s, n, false);
    if (init.OriginalThing != &a || init.PreviousThing != &b || init.ScriptName.m_text != s.m_text || init.ScriptName.copies != 1 ||
        init.OverridingBrainName.m_text != n.m_text || !init.PlayerCharacter || init.Drawable || !init.PlaceAtEndOfList ||
        init.EnableCreatureAutoPlacing || init.HomeBuilding || init.WorkBuilding) {
        std::printf("004facd0_TEST FAIL\n"); return 1;
    }
    std::printf("004facd0_TEST PASS\n");
    return 0;
}
