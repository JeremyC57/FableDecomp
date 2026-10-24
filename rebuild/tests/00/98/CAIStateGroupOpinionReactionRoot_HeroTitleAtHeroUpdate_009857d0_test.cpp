// Behaviour test for 0x009857D0 CAIStateGroup_OpinionReactionRoot::HeroTitleAtHeroUpdate: the reacted-to thing's hero
// component (interface 0x29) title def supplies the "comment at hero" tag, spoken by the brain's creature to that thing
// with interrupt=true, priority 0x93; the result is true only when the action returns 2 (completed).
#include <cstdio>
struct CTCBase { int id; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool TestTC(long t) const { return ((m_presentBits[t >> 5] >> (t & 31)) & 1) != 0; }
    template<class T> bool GetInterfaceT(T** ppOut, long interfaceType) const
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
class CHeroTitleDef {
public:
    unsigned char _pad0[0x28];
    long Title, HeroTitleGuiTag, HeroTitleGreetToHeroTag, HeroTitleCommentAtHeroTag, HeroTitleCommentToSelfTag;
};
class CTCHero {
public:
    CTCBase base;
    const CHeroTitleDef* PHeroTitleSubDef;
};
class CBaseIntelligentPointer {
public:
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(const CThing* item) { SetPItem((CThing*)item); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CIntelligentPointerCreature(CThing* item) { SetPItem(item); }
};
struct CAIBrain { CThing* PCreature; };
static CThing* g_speaker; static CThing* g_target; static unsigned long g_speech; static bool g_interrupt;
static long g_priority; static int g_repeat; static int g_result;
class CAIStateGroupBase {
public:
    CAIBrain* PBrain;
    int AIAction_SpeakAISpeechToTarget(int repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, unsigned long speech, bool interrupt, long priority)
    {
        g_repeat = repeat; g_speaker = speaker.GetPItem(); g_target = target.GetPItem(); g_speech = speech;
        g_interrupt = interrupt; g_priority = priority; return g_result;
    }
};
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase {
public:
    CBaseIntelligentPointer ThingReactingTo;
    bool HeroTitleAtHeroUpdate();
};
bool CAIStateGroup_OpinionReactionRoot::HeroTitleAtHeroUpdate()
{
    CTCHero* hero;
    ThingReactingTo.GetPItem()->GetInterfaceT(&hero, 0x29);
    const CHeroTitleDef* title = hero->PHeroTitleSubDef;
    CThing* target = ThingReactingTo.GetPItem();
    return AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, target,
        title->HeroTitleCommentAtHeroTag, true, 0x93) == 2;
}

int main()
{
    CHeroTitleDef def;
    def.HeroTitleCommentAtHeroTag = 0x1234; def.HeroTitleCommentToSelfTag = 0x5678;
    CTCHero heroTC; heroTC.PHeroTitleSubDef = &def;
    CTCHero other; other.PHeroTitleSubDef = 0;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 4; entries[0].component = &other.base;
    entries[1].type = 0x29; entries[1].component = &heroTC.base;
    entries[2].type = 0x31; entries[2].component = &other.base;
    CThing hero;
    for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    hero.m_presentBits[0] = 1UL << 4;
    hero.m_presentBits[1] = (1UL << (0x29 - 32)) | (1UL << (0x31 - 32));
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 3;
    CThing self;
    CAIBrain brain; brain.PCreature = &self;
    CAIStateGroup_OpinionReactionRoot group;
    group.PBrain = &brain;
    group.ThingReactingTo.SetPItem(&hero);
    int fails = 0;
    g_result = 2;
    if (!group.HeroTitleAtHeroUpdate()) ++fails;
    if (g_repeat != 1 || g_speaker != &self || g_target != &hero || g_speech != 0x1234 || !g_interrupt || g_priority != 0x93) ++fails;
    g_result = 1;
    if (group.HeroTitleAtHeroUpdate()) ++fails;
    g_result = 3;
    if (group.HeroTitleAtHeroUpdate()) ++fails;
    std::printf(fails ? "009857d0_TEST FAIL\n" : "009857d0_TEST PASS\n");
    return fails ? 1 : 0;
}