// Behaviour test for COpinionReactionManager::COpinionReactionManager (0x007004B0): the definition's per-reaction maps
// are copied into the 79-entry tables with the retail defaults for absent keys (attitude 0x12, targeting 0, delay 0,
// flags false), the delay truncated to whole seconds, the attitude mask set for the first 0x12 reaction types, and the
// blocking state and the 92-entry speech table cleared.
#include <cstdio>
#include <cstring>
#include <map>
struct Def {
    std::map<long, long> AttitudeCondition, TargetingCondition;
    std::map<long, float> PreReactionDelay;
    std::map<long, bool> Block, Speech, Carrying, Following;
};
template <class V> static V Look(const std::map<long, V>& m, long k, V d)
{
    typename std::map<long, V>::const_iterator it = m.find(k);
    return it != m.end() ? it->second : d;
}
struct Manager {
    const Def* PDef; int Attitude[79], Targeting[79]; unsigned int Delay[79];
    bool Block[79], Speech[79], Carrying[79], Following[79], Mask[79]; bool Blocked; unsigned int Reenable; int SpeechPlaying[92];
    Manager(const Def* def)
    {
        PDef = def;
        for (int i = 0; i < 79; ++i) {
            Attitude[i] = Look(PDef->AttitudeCondition, (long)i, 0x12L);
            std::map<long, long>::const_iterator it = PDef->TargetingCondition.find(i);
            if (it != PDef->TargetingCondition.end()) Targeting[i] = it->second; else Targeting[i] = 0;
            Delay[i] = (int)Look(PDef->PreReactionDelay, (long)i, 0.0f);
            Block[i] = Look(PDef->Block, (long)i, false);
            Speech[i] = Look(PDef->Speech, (long)i, false);
            Carrying[i] = Look(PDef->Carrying, (long)i, false);
            Following[i] = Look(PDef->Following, (long)i, false);
            Mask[i] = i < 0x12;
        }
        Reenable = 0;
        Blocked = false;
        for (int j = 0; j < 92; ++j) SpeechPlaying[j] = 0;
    }
};
int main()
{
    int fails = 0;
    Def d;
    d.AttitudeCondition[3] = 5; d.TargetingCondition[3] = 2; d.TargetingCondition[78] = 9;
    d.PreReactionDelay[3] = 2.75f; d.Block[40] = true; d.Speech[0] = true; d.Carrying[3] = false; d.Following[77] = true;
    d.AttitudeCondition[100] = 1;    // outside the 79 reaction types: ignored
    Manager m(&d);
    if (m.Attitude[3] != 5 || m.Attitude[0] != 0x12 || m.Attitude[78] != 0x12) fails++;
    if (m.Targeting[3] != 2 || m.Targeting[78] != 9 || m.Targeting[4] != 0) fails++;
    if (m.Delay[3] != 2 || m.Delay[0] != 0) fails++;
    if (!m.Block[40] || m.Block[41] || !m.Speech[0] || m.Carrying[3] || !m.Following[77]) fails++;
    if (!m.Mask[0] || !m.Mask[0x11] || m.Mask[0x12] || m.Mask[78]) fails++;
    if (m.Blocked || m.Reenable != 0 || m.SpeechPlaying[0] != 0 || m.SpeechPlaying[91] != 0) fails++;
    if (fails == 0) printf("007004b0_TEST PASS\n"); else printf("007004b0_TEST FAIL %d\n", fails);
    return fails;
}