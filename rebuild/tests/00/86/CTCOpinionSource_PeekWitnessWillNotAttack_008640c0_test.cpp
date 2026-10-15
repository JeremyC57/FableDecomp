// Behaviour test for CTCOpinionSource::PeekWitnessWillNotAttack @ 0x008640C0: 1 only when WitnessesWillNotAttack is set and the source applies.
#include <cstdio>
struct CThingAICreature { bool applies; };
struct CTCOpinionSource {
    bool UseNavLineOfSight, WitnessesWillNotAttack, WitnessesWillOverrideFactionEnemy;
    mutable int m_calls;
    bool AppliesToWitness(CThingAICreature* w) const { ++m_calls; return w->applies; }
    long PeekWitnessWillNotAttack(CThingAICreature* witness) const
    {
        if (WitnessesWillNotAttack && AppliesToWitness(witness)) return 1;
        return 0;
    }
};
int main()
{
    CThingAICreature yes = { true }, no = { false };
    CTCOpinionSource s; s.UseNavLineOfSight = false; s.WitnessesWillNotAttack = false; s.WitnessesWillOverrideFactionEnemy = false; s.m_calls = 0;
    if (s.PeekWitnessWillNotAttack(&yes) != 0 || s.m_calls != 0) { std::printf("008640c0_TEST FAIL\n"); return 1; }
    s.WitnessesWillNotAttack = true;
    if (s.PeekWitnessWillNotAttack(&yes) != 1 || s.PeekWitnessWillNotAttack(&no) != 0 || s.m_calls != 2) { std::printf("008640c0_TEST FAIL\n"); return 1; }
    std::printf("008640c0_TEST PASS\n");
    return 0;
}