extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00811190 CEventKilledCreature::CEventKilledCreature(killer, killed): WorldFrame 0, and
// CreatureGroupOfKilledThing is the killed thing's creature-def group (0 when its def has no creature sub-def); the def
// reference taken by the lookup is released again.
class CCreatureDef { public: long RefCount; unsigned long CreatureGroup; };
class CThingBaseDef { public: CCreatureDef* sub; bool GetSubDef(CCreatureDef** out) const { if (!sub) return false; *out = sub; ++sub->RefCount; return true; } };
class CThing { public: CThingBaseDef* PBaseDef; };
struct CEventKilledCreature {
    long WorldFrame; unsigned long CreatureGroupOfKilledThing;
    CEventKilledCreature(const CThing&, const CThing& killed) : WorldFrame(0), CreatureGroupOfKilledThing(0)
    {
        CCreatureDef* def = 0;
        if (killed.PBaseDef->GetSubDef(&def)) CreatureGroupOfKilledThing = def->CreatureGroup;
        if (def) --def->RefCount;
    }
};
int main()
{
    CCreatureDef cd = { 1, 6 }; CThingBaseDef withSub = { &cd }, without = { 0 };
    CThing killer = { &without }, victim = { &withSub };
    CEventKilledCreature e(killer, victim);
    CHECK(e.WorldFrame == 0 && e.CreatureGroupOfKilledThing == 6 && cd.RefCount == 1);
    CEventKilledCreature e2(victim, killer);
    CHECK(e2.CreatureGroupOfKilledThing == 0);
    if (fails == 0) printf("00811190_TEST PASS\n"); else printf("00811190_TEST FAIL\n");
    return fails;
}