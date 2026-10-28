// Behaviour test for 0x0062D9E0 CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing(creature, receiver, item)
#include <cstdio>
#include <cstring>

class CCharString { public:
    char Text[32];
    CCharString(const char* t, long length = -1) { (void)length; std::strcpy(Text, t); }
};
class CAnimationEntry { public: int Id; };
class CThing { public: int Id; };
class CThingCreatureBase { public: CAnimationEntry GiveAnim; char LastQuery[32]; };
class CSoundPair { public:
    int FirstSound, LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CIntelligentPointer {
    CThing* Item;
    CIntelligentPointer(CThing* t) : Item(t) { }
};
class CCreatureActionBase { public:
    CThingCreatureBase* Creature;
    float TotalTime, BlendTime;
    const CAnimationEntry* Anim;
    CSoundPair Sounds;
    int Group, Priority;
    CCreatureActionBase(CThingCreatureBase& c, float t, float b, const CAnimationEntry& a, const CSoundPair& s, int g, int p)
        : Creature(&c), TotalTime(t), BlendTime(b), Anim(&a), Sounds(s), Group(g), Priority(p) { }
    static const CAnimationEntry& GetRandomAnimFromName(CThingCreatureBase& c, const CCharString& name)
    {
        std::strcpy(c.LastQuery, name.Text);
        return c.GiveAnim;
    }
};
class CCreatureAction_GiveItemToThing : public CCreatureActionBase { public:
    CIntelligentPointer PReceivingThing;
    CIntelligentPointer PItemBeingGiven;
    CCreatureAction_GiveItemToThing(CThingCreatureBase& creature, CThing& receivingThing, CThing& itemBeingGiven)
        : CCreatureActionBase(creature, -1.0f, 0.75f, GetRandomAnimFromName(creature, "STANDARD_GIVE_OBJECT"), CSoundPair(), 0, 0)
        , PReceivingThing(&receivingThing)
        , PItemBeingGiven(&itemBeingGiven)
    {
    }
};

int main()
{
    CThingCreatureBase creature; creature.GiveAnim.Id = 7; creature.LastQuery[0] = 0;
    CThing receiver = { 1 }, item = { 2 };
    CCreatureAction_GiveItemToThing action(creature, receiver, item);
    if (action.Creature != &creature || action.Anim != &creature.GiveAnim) { std::printf("0062d9e0_TEST FAIL\n"); return 1; }
    if (std::strcmp(creature.LastQuery, "STANDARD_GIVE_OBJECT") != 0) { std::printf("0062d9e0_TEST FAIL\n"); return 1; }
    if (action.TotalTime != -1.0f || action.BlendTime != 0.75f || action.Sounds.FirstSound != 0 || action.Sounds.LastSound != 0) { std::printf("0062d9e0_TEST FAIL\n"); return 1; }
    if (action.Group != 0 || action.Priority != 0) { std::printf("0062d9e0_TEST FAIL\n"); return 1; }
    if (action.PReceivingThing.Item != &receiver || action.PItemBeingGiven.Item != &item) { std::printf("0062d9e0_TEST FAIL\n"); return 1; }
    std::printf("0062d9e0_TEST PASS\n");
    return 0;
}