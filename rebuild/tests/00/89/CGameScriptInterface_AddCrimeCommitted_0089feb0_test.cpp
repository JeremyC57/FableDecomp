// Behaviour model of CGameScriptInterface::AddCrimeCommitted (0x0089FEB0).
#include <cstdio>
#include <list>
#include <set>
struct Creature { bool dead, isAI, guard, hasOpinion; std::set<long> known; long informed; Creature() : dead(false), isAI(true), guard(false), hasOpinion(true), informed(-1) {} };
struct Village { std::list<Creature*> people; };
struct Thing { bool alive; Village* village; Creature* creature; Thing() : alive(true), village(0), creature(0) {} };
struct DeedLog { long next; Creature* lastWitness; Thing* lastCriminal; DeedLog() : next(100), lastWitness(0), lastCriminal(0) {} };
struct Hero { bool dead; DeedLog* log; };
static void AddCrimeCommitted(Hero* hero, Thing& victim, int crime, bool informWitnesses, Thing* criminal, Thing* witness)
{
    if (!hero || hero->dead) return;
    if (!victim.alive) return;
    Thing* criminalThing = 0;
    if (criminal) criminalThing = criminal;
    Creature* witnessCreature = 0;
    if (witness && witness->alive && witness->creature && witness->creature->isAI) witnessCreature = witness->creature;
    long deed = -1;
    if (informWitnesses && !witnessCreature && victim.village && victim.village->people.size())
        witnessCreature = victim.village->people.front();
    if (hero->log) { deed = hero->log->next++; hero->log->lastWitness = witnessCreature; hero->log->lastCriminal = criminalThing; }
    if (informWitnesses && deed != -1 && victim.alive && victim.village) {
        for (std::list<Creature*>::iterator it = victim.village->people.begin(), end = victim.village->people.end(); it != end; ++it) {
            Creature* m = *it;
            if (m && !m->dead && m->guard && m->hasOpinion) {
                if (!m->known.count(deed * 16 + crime)) m->informed = deed;
            }
        }
    }
}
static int fail(int n) { std::printf("0089feb0_TEST FAIL %d\n", n); return 1; }
int main()
{
    DeedLog log; Hero hero = { false, &log };
    Village v; Creature farmer, guard, deadGuard; guard.guard = true; deadGuard.guard = true; deadGuard.dead = true;
    v.people.push_back(&farmer); v.people.push_back(&guard); v.people.push_back(&deadGuard);
    Thing victim; victim.village = &v; Thing crook;
    AddCrimeCommitted(&hero, victim, 3, true, &crook, 0);
    if (log.lastWitness != &farmer || log.lastCriminal != &crook) return fail(1);
    if (guard.informed != 100 || farmer.informed != -1 || deadGuard.informed != -1) return fail(2);
    AddCrimeCommitted(&hero, victim, 3, false, 0, 0);
    if (log.lastWitness != 0 || guard.informed != 100 || log.next != 102) return fail(3);
    victim.alive = false; AddCrimeCommitted(&hero, victim, 3, true, 0, 0);
    if (log.next != 102) return fail(4);
    victim.alive = true; hero.dead = true; AddCrimeCommitted(&hero, victim, 3, true, 0, 0);
    if (log.next != 102) return fail(5);
    hero.dead = false; Thing w; Creature wc; w.creature = &wc; AddCrimeCommitted(&hero, victim, 3, true, 0, &w);
    if (log.lastWitness != &wc || guard.informed != 102) return fail(6);
    std::printf("0089feb0_TEST PASS\n");
    return 0;
}