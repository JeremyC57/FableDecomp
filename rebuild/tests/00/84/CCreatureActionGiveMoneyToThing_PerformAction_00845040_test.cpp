// Behaviour test for CCreatureAction_GiveMoneyToThing::PerformAction (0x00845040): a copy of the decision logic over a
// minimal thing model (presence bitset + component table), checking the gold taken, the event posted and the deed gate.
#include <cstdio>

struct Thing {
    bool dead;
    unsigned int bits[9];
    void* comps[0x120];
    unsigned char thingType;
    bool playerCharacter;
    bool Has(int t) const { return (bits[t >> 5] & (1u << (t & 31))) != 0; }
    void Add(int t, void* c) { bits[t >> 5] |= 1u << (t & 31); comps[t] = c; }
};
struct HeroStats { long money; long added; bool notified; void AddMoney(long a, bool n) { added += a; money += a; notified = n; } };
struct EntityEvents { int events; long lastAmount; Thing* lastGiver; };
struct DeedLog { int deeds; int lastType; const Thing* lastTarget; };
struct Wife { bool married; bool reaction; };

static const long& Min(const long& a, const long& b) { return b < a ? b : a; }

static void PerformAction(Thing* receiver, Thing* creature, long Amount)
{
    if (receiver == 0 || receiver->dead)
        return;
    if (!creature->Has(4))
        return;
    HeroStats* stats = (HeroStats*)creature->comps[4];
    long money = stats->money;
    long amount = Min(money, Amount);
    stats->AddMoney(-amount, true);
    EntityEvents* events = (EntityEvents*)receiver->comps[0x42];
    events->events++; events->lastAmount = amount; events->lastGiver = creature;
    if (!creature->Has(0x51))
        return;
    DeedLog* deeds = (DeedLog*)creature->comps[0x51];
    if (receiver->thingType == 1 && !receiver->Has(0xb4) && receiver->Has(0x4f)) {
        if (creature->playerCharacter && receiver->Has(0xd7)) {
            Wife* wife = (Wife*)receiver->comps[0xd7];
            if (wife->married && !wife->reaction)
                return;
        }
        deeds->deeds++; deeds->lastType = 0x3d; deeds->lastTarget = receiver;
    }
}

static Thing g_receiver, g_creature;

int main()
{
    HeroStats stats = { 100, 0, false };
    EntityEvents events = { 0, 0, 0 };
    DeedLog deeds = { 0, 0, 0 };
    Wife wife = { true, false };
    g_creature.Add(4, &stats); g_creature.Add(0x51, &deeds); g_creature.playerCharacter = true;
    g_receiver.Add(0x42, &events); g_receiver.Add(0x4f, 0); g_receiver.thingType = 1;
    int fail = 0;
    PerformAction(&g_receiver, &g_creature, 30);
    if (stats.money != 70 || stats.added != -30 || !stats.notified || events.events != 1 || events.lastAmount != 30 || deeds.deeds != 1 || deeds.lastType != 0x3d) fail |= 1;
    PerformAction(&g_receiver, &g_creature, 500);                  // capped at the gold held
    if (stats.money != 0 || events.lastAmount != 70 || deeds.deeds != 2) fail |= 2;
    g_receiver.Add(0xd7, &wife);                                  // married wife, no gift reaction: no deed
    stats.money = 10;
    PerformAction(&g_receiver, &g_creature, 5);
    if (stats.money != 5 || events.events != 3 || deeds.deeds != 2) fail |= 4;
    wife.reaction = true;
    PerformAction(&g_receiver, &g_creature, 5);
    if (deeds.deeds != 3) fail |= 8;
    g_receiver.Add(0xb4, 0);                                      // excluded receivers post no deed
    PerformAction(&g_receiver, &g_creature, 1);
    if (deeds.deeds != 3 || events.events != 5) fail |= 16;
    g_receiver.dead = true;
    PerformAction(&g_receiver, &g_creature, 1);
    if (events.events != 5) fail |= 32;
    PerformAction(0, &g_creature, 1);
    if (fail) { std::printf("00845040_TEST FAIL %d\n", fail); return 1; }
    std::printf("00845040_TEST PASS\n");
    return 0;
}