// Behaviour test for CThingAICreature::IsAvailableForInteractionType @ 0x00830320: def lists the type and no current interaction.
#include <cstdio>
struct CCreatureDef { unsigned long m_interactionTypes; };
struct CThingAICreature {
    CCreatureDef* m_pCreatureDef; void* m_pCurrentInteraction;
    bool IsAvailableForInteractionType(long interactionType)
    {
        return (m_pCreatureDef->m_interactionTypes & interactionType) && !m_pCurrentInteraction;
    }
};
int main()
{
    CCreatureDef def; def.m_interactionTypes = 0x5;
    CThingAICreature c; c.m_pCreatureDef = &def; c.m_pCurrentInteraction = 0;
    if (!c.IsAvailableForInteractionType(4) || c.IsAvailableForInteractionType(2)) { std::printf("00830320_TEST FAIL\n"); return 1; }
    c.m_pCurrentInteraction = &def;
    if (c.IsAvailableForInteractionType(4)) { std::printf("00830320_TEST FAIL\n"); return 1; }
    std::printf("00830320_TEST PASS\n");
    return 0;
}