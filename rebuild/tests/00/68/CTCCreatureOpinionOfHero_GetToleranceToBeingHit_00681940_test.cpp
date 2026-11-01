// Test for 00681940 GetToleranceToBeingHitRemaining: override when >= 0, else manager tolerance + personality bonus.
#include <cstdio>
class CThing { public: float Tolerance; };
class COpinionReactionManager { public: float GetToleranceToBeingHit(const CThing* t) const { return t->Tolerance; } };
class CWorld { public: COpinionReactionManager* POpinionReactionManager; };
class COpinionPersonalityDef { public: float ToleranceToBeingHit; };
class CTCCreatureOpinionOfHero { public:
    CThing* ParentThing; float ToleranceToBeingHitOverride; COpinionPersonalityDef* PPersonalityDef; CWorld* World;
    CWorld* GetWorld_00686a80() const { return World; }
    float GetToleranceToBeingHitRemaining() const
    {
        if (ToleranceToBeingHitOverride >= 0.0f)
            return ToleranceToBeingHitOverride;
        CThing* thing = ParentThing;
        COpinionReactionManager* manager = GetWorld_00686a80()->POpinionReactionManager;
        float tolerance = manager->GetToleranceToBeingHit(thing);
        if (PPersonalityDef)
            tolerance += PPersonalityDef->ToleranceToBeingHit;
        return tolerance;
    }
};
int main()
{
    COpinionReactionManager m; CWorld w = { &m }; CThing t = { 3.0f }; COpinionPersonalityDef d = { 1.5f };
    CTCCreatureOpinionOfHero o = { &t, 0.0f, &d, &w };
    if (o.GetToleranceToBeingHitRemaining() != 0.0f) { std::printf("00681940_TEST FAIL\n"); return 1; }   // 0 counts as set
    o.ToleranceToBeingHitOverride = 7.0f;
    if (o.GetToleranceToBeingHitRemaining() != 7.0f) { std::printf("00681940_TEST FAIL\n"); return 1; }
    o.ToleranceToBeingHitOverride = -1.0f;
    if (o.GetToleranceToBeingHitRemaining() != 4.5f) { std::printf("00681940_TEST FAIL\n"); return 1; }
    o.PPersonalityDef = 0;
    if (o.GetToleranceToBeingHitRemaining() != 3.0f) { std::printf("00681940_TEST FAIL\n"); return 1; }
    std::printf("00681940_TEST PASS\n");
    return 0;
}