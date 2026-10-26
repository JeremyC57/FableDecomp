// Behaviour test for 0x0074AA00 CTCCreatureGenerator::CTCCreatureGenerator: base bound to the thing, empty containers
// (a self-linked list sentinel), active/ready/wait-for-dead set, unlimited triggers, generation radii 1.0, limits -1,
// a null random particle and the nav check requested.
#include <cstdio>
#include <cstdlib>

class CThing { public: int id; };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing* t) : ParentThing(t) {} };
struct SNode { SNode* next; SNode* prev; };
struct CList { SNode* node; CList() : node(0) { SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->next = n; n->prev = n; node = n; } ~CList() { std::free(node); } };
struct CVec { void* s; void* f; void* e; CVec() : s(0), f(0), e(0) {} };
struct CArr { void* d; long n; long c; CArr() : d(0), n(0), c(0) {} };
struct CIntelligentPointer { CThing* p; CIntelligentPointer() : p(0) { SetPItem(0); } void SetPItem(CThing* t) { p = t; } };
struct TGenerationParameters { float GenerationRadius, SelfTriggerRadius; bool SelfTrigger; long SelfTriggerResetInterval; bool TriggerOnActivate; long ActiveCreatureLimit, TotalGenerationLimit, NumTriggers; };
class CTCCreatureGenerator : public CTCBase { public:
    CArr CreatureFamilies; CList CreatedCreatures; CVec CreatureTriggerers; TGenerationParameters GenParams;
    bool Active, Initialised, ReadyToInitialise, ExternalTrigger, WaitForAllCreaturesDead; long NumTriggersLeft, ActivationFrame; bool LastSpaceTest;
    CVec OpenPositions, HiddenPositions, DebugPrimitives; long CreationCount; bool UndeadTriggered, Destroyed; void* PDef;
    CIntelligentPointer PRandomParticle; long FramesUntilRandomEffect, FramesUntilRandomSoundEffect; bool NeedToDoNavCheck, DoNavCheckOnGeneration; void* PExclusionArea;
    CTCCreatureGenerator(CThing* thing)
        : CTCBase(thing), Active(true), Initialised(false), ReadyToInitialise(true), ExternalTrigger(false),
          WaitForAllCreaturesDead(true), NumTriggersLeft(-1), ActivationFrame(0), LastSpaceTest(false),
          CreationCount(0), UndeadTriggered(false), Destroyed(false), PDef(0)
    {
        GenParams.GenerationRadius = 1.0f; GenParams.SelfTriggerRadius = 1.0f;
        FramesUntilRandomEffect = 0; FramesUntilRandomSoundEffect = 0; DoNavCheckOnGeneration = false; PExclusionArea = 0;
        GenParams.TriggerOnActivate = false; GenParams.SelfTriggerResetInterval = 0;
        GenParams.NumTriggers = -1; GenParams.ActiveCreatureLimit = -1; GenParams.TotalGenerationLimit = -1;
        NeedToDoNavCheck = true;
    }
};
int main()
{
    CThing t = { 7 };
    CTCCreatureGenerator g(&t);
    bool ok = g.ParentThing == &t && g.CreatedCreatures.node->next == g.CreatedCreatures.node && g.CreatedCreatures.node->prev == g.CreatedCreatures.node
        && g.CreatureFamilies.d == 0 && g.CreatureTriggerers.s == 0 && g.OpenPositions.f == 0 && g.DebugPrimitives.e == 0
        && g.Active && !g.Initialised && g.ReadyToInitialise && !g.ExternalTrigger && g.WaitForAllCreaturesDead
        && g.NumTriggersLeft == -1 && g.ActivationFrame == 0 && !g.LastSpaceTest && g.CreationCount == 0 && !g.UndeadTriggered && !g.Destroyed
        && g.PDef == 0 && g.PRandomParticle.p == 0 && g.FramesUntilRandomEffect == 0 && g.FramesUntilRandomSoundEffect == 0
        && g.NeedToDoNavCheck && !g.DoNavCheckOnGeneration && g.PExclusionArea == 0
        && g.GenParams.GenerationRadius == 1.0f && g.GenParams.SelfTriggerRadius == 1.0f && g.GenParams.SelfTriggerResetInterval == 0
        && !g.GenParams.TriggerOnActivate && g.GenParams.ActiveCreatureLimit == -1 && g.GenParams.TotalGenerationLimit == -1 && g.GenParams.NumTriggers == -1;
    if (!ok) { std::printf("0074aa00_TEST FAIL\n"); return 1; }
    std::printf("0074aa00_TEST PASS\n");
    return 0;
}