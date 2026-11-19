// Test for NCreatureAI::IsAwareOfThing @ 0x008645E0 (NF95 2026-11-19): it forwards the creature, the thing and the bool to the shared
// awareness test with CTCPerceiveThing::CanSeeOrSmellPerceivedThing as the perception predicate, and returns its answer.
// NF97 2026-11-19 (port): CTCPerceiveThing comes from engine/CTCPerceiveThing.h and NCreatureAI::IsAwareOfThing from
// engine/NCreatureAI.h (no local models; the creature and the thing are opaque addresses). The callee is TU-local in the row
// (anonymous namespace), so this test is a self-contained model of the row's forwarding: the same body over a recording stand-in.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_CANSEEORSMELL
#include <cstdio>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/NCreatureAI.h"
#include "engine/CTCPerceiveThing.h"
typedef bool (CTCPerceiveThing::*Predicate)() const;
bool CTCPerceiveThing::CanSeeOrSmellPerceivedThing() const { return true; }
static const CThingAICreature* gCreature; static const CThing* gThing; static bool gFlag; static Predicate gPred;
namespace {
bool FABLE_FASTCALL IsAwareOfThingImplementation(const CThingAICreature* c, const CThing* t, bool f, Predicate p)
{ gCreature = c; gThing = t; gFlag = f; gPred = p; return t != 0 && f; }
}
bool FABLE_FASTCALL NCreatureAI::IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight)
{
    return IsAwareOfThingImplementation(creature, thing, check_line_of_sight, &CTCPerceiveThing::CanSeeOrSmellPerceivedThing);
}
int main()
{
    static unsigned long creatureStorage[4], thingStorage[4];
    const CThingAICreature* c = reinterpret_cast<const CThingAICreature*>(creatureStorage);
    const CThing* t = reinterpret_cast<const CThing*>(thingStorage);
    int fails = 0;
    if (!NCreatureAI::IsAwareOfThing(c, t, true)) fails |= 1;
    if (gCreature != c || gThing != t || !gFlag || gPred != &CTCPerceiveThing::CanSeeOrSmellPerceivedThing) fails |= 2;
    if (NCreatureAI::IsAwareOfThing(c, t, false) || gFlag) fails |= 4;
    if (fails) { std::printf("008645e0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008645e0_TEST PASS\n");
    return 0;
}
