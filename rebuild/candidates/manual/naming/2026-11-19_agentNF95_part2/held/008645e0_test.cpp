// Test for NCreatureAI::IsAwareOfThing @ 0x008645E0 (NF95 2026-11-19): it forwards the creature, the thing and the bool to the shared
// awareness test with CTCPerceiveThing::CanSeeOrSmellPerceivedThing as the perception predicate, and returns its answer.
#include <cstdio>
class CThing { public: int id; };
class CThingAICreature { public: int id; };
class CTCPerceiveThing { public: bool CanSeeOrSmellPerceivedThing() const { return true; } bool Other() const { return false; } };
typedef bool (CTCPerceiveThing::*Predicate)() const;
static const CThingAICreature* gCreature; static const CThing* gThing; static bool gFlag; static Predicate gPred;
static bool IsAwareOfThingImplementation(const CThingAICreature* c, const CThing* t, bool f, Predicate p)
{ gCreature = c; gThing = t; gFlag = f; gPred = p; return t != 0 && f; }
namespace NCreatureAI {
static bool IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight)
{ return IsAwareOfThingImplementation(creature, thing, check_line_of_sight, &CTCPerceiveThing::CanSeeOrSmellPerceivedThing); }
}
int main()
{
    CThingAICreature c; CThing t; int fails = 0;
    if (!NCreatureAI::IsAwareOfThing(&c, &t, true)) fails |= 1;
    if (gCreature != &c || gThing != &t || !gFlag || gPred != &CTCPerceiveThing::CanSeeOrSmellPerceivedThing) fails |= 2;
    if (NCreatureAI::IsAwareOfThing(&c, &t, false) || gFlag) fails |= 4;
    if (fails) { std::printf("008645e0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008645e0_TEST PASS\n");
    return 0;
}
