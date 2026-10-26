// Behaviour test for CTCOpinionSource::CTCOpinionSource @ 0x00771360: empty stack, flags false, no deed, deed ID -1.
#include <cstdio>
class CThing { public: long id; };
struct CLongVector { long* f; long* l; long* e; CLongVector() : f(0), l(0), e(0) {} };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing* owner) : ParentThing(owner) {} virtual ~CTCBase() {} };
class CTCOpinionSource : public CTCBase { public:
    CLongVector DefStack;
    bool UseNavLineOfSight, WitnessesWillNotAttack, WitnessesWillOverrideFactionEnemy, BypassesPersonality, RelativeEffect;
    const void* RestrictToFactionAndAllies;
    bool RestrictToFactionIsReversed;
    bool ReactToSelfPermitted, AlwaysAwareOf, UnknownThreatSource, ReactNonstopFlag;
    void* POpinionObject; void* POpinionBlock;
    long VirtualDeed, VirtualDeedID;
    CTCOpinionSource(CThing* owner)
        : CTCBase(owner), UseNavLineOfSight(false), WitnessesWillNotAttack(false), WitnessesWillOverrideFactionEnemy(false),
          BypassesPersonality(false), RelativeEffect(false), RestrictToFactionAndAllies(0), RestrictToFactionIsReversed(false),
          ReactToSelfPermitted(false), AlwaysAwareOf(false), UnknownThreatSource(false), ReactNonstopFlag(false),
          POpinionObject(0), POpinionBlock(0), VirtualDeed(0), VirtualDeedID(-1) {}
};
int main()
{
    CThing t; t.id = 4;
    CTCOpinionSource s(&t);
    if (s.ParentThing != &t || s.DefStack.f != 0 || s.UseNavLineOfSight || s.WitnessesWillNotAttack || s.RelativeEffect ||
        s.RestrictToFactionAndAllies != 0 || s.ReactNonstopFlag || s.POpinionObject != 0 || s.VirtualDeed != 0 || s.VirtualDeedID != -1)
    { std::printf("00771360_TEST FAIL\n"); return 1; }
    std::printf("00771360_TEST PASS\n");
    return 0;
}