#include <cstdio>
// Behaviour test: IsSpeechAllowed's gates (live talker, hero alive, hero's 0x36 list holds the creature, two time windows).
class CTCBase;
struct SEntry { int type; CTCBase* component; };
struct CInterfaceMap {
    SEntry* first; SEntry* last;
    const SEntry* LowerBound(const int& type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
};
class CThingCreatureBase {
public:
    unsigned int m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Dead : 1;
    template<class T> bool GetInterface(T** ppOut, int interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31))) {
            int type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCombatSpeechTargets {
    const CThingCreatureBase* member;
    bool ContainsCreature(const CThingCreatureBase& c) { return &c == member; }
};
class CPlayer { public: CThingCreatureBase* hero; CThingCreatureBase* GetHeroThing() const { return hero; } };
class CPlayerManager { public: CPlayer* player; CPlayer* GetMainPlayer() { return player; } };
class CWorld { public: CPlayerManager* PlayerManager; int frame; int GetFrameNumber() { return frame; } };
static CWorld g_world;
static CWorld* g_pWorld = &g_world;
static int g_framesPerSecond = 30;
class CCombatDialogueDef { public: float MinTimeBetweenSpeech, MinTimeBetweenSpeechRepeat, MinDelayAfterBattleCry; };
struct CPtr { CThingCreatureBase* p; CThingCreatureBase* Get() const { return p; } CThingCreatureBase* operator->() const { return p; } };
class CombatDialogueManager { public:
    const CCombatDialogueDef* PDialogueDef;
    CPtr PCurrentTalker;
    int LastSpeechFrame, LastBattleCryFrame;
    bool IsSpeechAllowed(const CThingCreatureBase& creature) const
    {
        if (PCurrentTalker.Get() && !PCurrentTalker->Dead)
            return false;
        CThingCreatureBase* hero = g_pWorld->PlayerManager->GetMainPlayer()->GetHeroThing();
        if (hero == 0 || hero->Dead)
            return false;
        CTCCombatSpeechTargets* targets;
        if (!hero->GetInterface(&targets, 0x36))
            return false;
        if (!targets->ContainsCreature(creature))
            return false;
        int frame = g_pWorld->GetFrameNumber();
        if (!((frame - LastSpeechFrame) / (float)g_framesPerSecond < PDialogueDef->MinTimeBetweenSpeech)
            && !((frame - LastBattleCryFrame) / (float)g_framesPerSecond < PDialogueDef->MinDelayAfterBattleCry))
            return true;
        return false;
    }
};
static bool Fail() { std::printf("008af5f0_TEST FAIL\n"); return false; }
int main()
{
    CThingCreatureBase hero, creature, talker;
    int i = 0;
    for (i = 0; i < 9; ++i) { hero.m_presentBits[i] = 0; creature.m_presentBits[i] = 0; talker.m_presentBits[i] = 0; }
    hero.Dead = 0; creature.Dead = 0; talker.Dead = 0;
    CTCCombatSpeechTargets targets; targets.member = &creature;
    SEntry entries[2]; entries[0].type = 0x10; entries[0].component = 0; entries[1].type = 0x36; entries[1].component = (CTCBase*)&targets;
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    hero.m_presentBits[1] = 1U << (0x36 & 31);
    CPlayer player; player.hero = &hero; CPlayerManager pm; pm.player = &player;
    g_world.PlayerManager = &pm; g_world.frame = 300;
    CCombatDialogueDef def; def.MinTimeBetweenSpeech = 2.0f; def.MinTimeBetweenSpeechRepeat = 0; def.MinDelayAfterBattleCry = 1.0f;
    CombatDialogueManager m; m.PDialogueDef = &def; m.PCurrentTalker.p = 0; m.LastSpeechFrame = 240; m.LastBattleCryFrame = 270;
    if (!m.IsSpeechAllowed(creature)) return Fail(), 1;                 // 2.0s and 1.0s elapsed
    m.LastSpeechFrame = 241; if (m.IsSpeechAllowed(creature)) return Fail(), 1;
    m.LastSpeechFrame = 240; m.LastBattleCryFrame = 271; if (m.IsSpeechAllowed(creature)) return Fail(), 1;
    m.LastBattleCryFrame = 270;
    if (m.IsSpeechAllowed(talker)) return Fail(), 1;                    // not in the hero's list
    m.PCurrentTalker.p = &talker; if (m.IsSpeechAllowed(creature)) return Fail(), 1;
    talker.Dead = 1; if (!m.IsSpeechAllowed(creature)) return Fail(), 1; // a dead talker does not block
    hero.Dead = 1; if (m.IsSpeechAllowed(creature)) return Fail(), 1;
    hero.Dead = 0; hero.m_presentBits[1] = 0; if (m.IsSpeechAllowed(creature)) return Fail(), 1;
    player.hero = 0; if (m.IsSpeechAllowed(creature)) return Fail(), 1;
    std::printf("008af5f0_TEST PASS\n");
    return 0;
}