// Behaviour test for 0x006E97A0 (CCombatManager::IsCreaturePerformingNonInterruptableSequence).
#include <cstdio>
#include <map>
#include <vector>

struct Creature { unsigned long long UID; };
struct Sequence {
    bool Running; bool Interruptable; int calls;
    virtual bool IsInterruptable() { ++calls; return Interruptable; }
    virtual ~Sequence() { }
};
struct Info { long CurrentSequence; std::vector<Sequence*> CombatSequences; };
struct Manager {
    std::map<unsigned long long, Info> Combatants;
    unsigned char IsCreaturePerformingNonInterruptableSequence(const Creature& c)
    {
        const Info& info = Combatants.find(c.UID)->second;
        if (info.CurrentSequence >= 0) {
            Sequence* sequence = info.CombatSequences[info.CurrentSequence];
            if (sequence->Running) return !sequence->IsInterruptable();
        }
        return 0;
    }
};
#define FAIL { std::printf("006e97a0_TEST FAIL\n"); return 1; }
int main()
{
    Creature c = { 5 };
    Sequence s0, s1;
    s0.Running = true; s0.Interruptable = true; s0.calls = 0;
    s1.Running = false; s1.Interruptable = false; s1.calls = 0;
    Manager m; Info info; info.CurrentSequence = -1; info.CombatSequences.push_back(&s0); info.CombatSequences.push_back(&s1);
    m.Combatants[5] = info;
    if (m.IsCreaturePerformingNonInterruptableSequence(c) != 0) FAIL;     // no current sequence
    m.Combatants[5].CurrentSequence = 0;
    if (m.IsCreaturePerformingNonInterruptableSequence(c) != 0 || s0.calls != 1) FAIL;   // running but interruptable
    s0.Interruptable = false;
    if (m.IsCreaturePerformingNonInterruptableSequence(c) != 1) FAIL;
    m.Combatants[5].CurrentSequence = 1;
    if (m.IsCreaturePerformingNonInterruptableSequence(c) != 0 || s1.calls != 0) FAIL;   // not running: no virtual call
    std::printf("006e97a0_TEST PASS\n");
    return 0;
}