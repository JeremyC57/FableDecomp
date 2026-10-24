// Behaviour test for 0x0095F830 CAIStateGroup_OpinionReactionRoot::SetGreetedThingReactingTo: the copied logic over a
// creature whose interface map holds the opinion-of-hero component (type 0x4F).
#include <cstdio>

struct CTCBase { int Tag; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) { it = middle + 1; len = len - half - 1; } else len = half;
        }
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CAIBrain { CThing* PCreature; };
struct CAIStateGroupBase { CAIBrain* PBrain; };
struct CTCCreatureOpinionOfHero : CTCBase {
    bool InteractedFlag;
    bool GreetedFlag;
};
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase {
public:
    bool LastRunWasTryAndLook;
    bool ReactingToPlayer;
    void SetGreetedThingReactingTo();
};

void CAIStateGroup_OpinionReactionRoot::SetGreetedThingReactingTo()
{
    if (ReactingToPlayer) {
        CTCCreatureOpinionOfHero* opinion;
        PBrain->PCreature->GetInterface(&opinion, 0x4F);
        opinion->InteractedFlag = true;
        opinion->GreetedFlag = true;
    }
}

int main()
{
    CTCCreatureOpinionOfHero opinion; opinion.InteractedFlag = false; opinion.GreetedFlag = false;
    CTCBase talk, other;
    CInterfaceMap::SEntry entries[3] = { { 0x10, &talk }, { 0x4F, &opinion }, { 0x60, &other } };
    CThing creature;
    for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[2] = 1UL << 15;     // type 0x4F
    creature.m_interfaces.first = entries;
    creature.m_interfaces.last = entries + 3;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_OpinionReactionRoot group; group.PBrain = &brain; group.LastRunWasTryAndLook = false;

    group.ReactingToPlayer = false;
    group.SetGreetedThingReactingTo();
    if (opinion.InteractedFlag || opinion.GreetedFlag) { std::printf("0095f830_TEST FAIL\n"); return 1; }
    group.ReactingToPlayer = true;
    group.SetGreetedThingReactingTo();
    if (!opinion.InteractedFlag || !opinion.GreetedFlag) { std::printf("0095f830_TEST FAIL\n"); return 1; }
    std::printf("0095f830_TEST PASS\n");
    return 0;
}