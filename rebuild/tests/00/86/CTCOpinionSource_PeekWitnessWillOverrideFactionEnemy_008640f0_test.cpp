// Behaviour test for CTCOpinionSource::PeekWitnessWillOverrideFactionEnemy @ 0x008640F0: 1 only when WitnessesWillOverrideFactionEnemy is set and the source applies.
#include <cstdio>
class CThingAICreature { public: bool applies; };
class CTCOpinionSource { public:
    bool UseNavLineOfSight, WitnessesWillNotAttack, WitnessesWillOverrideFactionEnemy;
    mutable int m_calls;
    bool AppliesToWitness(CThingAICreature* w) const { ++m_calls; return w->applies; }
    long PeekWitnessWillOverrideFactionEnemy(CThingAICreature* witness) const
    {
        if (WitnessesWillOverrideFactionEnemy && AppliesToWitness(witness)) return 1;
        return 0;
    }
};
int main()
{
    CThingAICreature yes = { true }, no = { false };
    CTCOpinionSource s; s.UseNavLineOfSight = false; s.WitnessesWillNotAttack = false; s.WitnessesWillOverrideFactionEnemy = false; s.m_calls = 0;
    if (s.PeekWitnessWillOverrideFactionEnemy(&yes) != 0 || s.m_calls != 0) { std::printf("008640f0_TEST FAIL\n"); return 1; }
    s.WitnessesWillOverrideFactionEnemy = true;
    if (s.PeekWitnessWillOverrideFactionEnemy(&yes) != 1 || s.PeekWitnessWillOverrideFactionEnemy(&no) != 0 || s.m_calls != 2) { std::printf("008640f0_TEST FAIL\n"); return 1; }
    std::printf("008640f0_TEST PASS\n");
    return 0;
}