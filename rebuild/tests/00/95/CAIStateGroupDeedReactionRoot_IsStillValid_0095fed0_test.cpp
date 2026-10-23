// 0095fed0 behaviour test: CAIStateGroup_DeedReactionRoot::IsStillValid (control flow + counted-pointer release of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stdio.h>

struct CTCHeroOpinionDeedLog;
struct CTCCreatureOpinionOfHero;
static CTCHeroOpinionDeedLog* g_reactions;
static CTCCreatureOpinionOfHero* g_opinion;

struct CThing {
    unsigned char m_flags91;
    bool hasReactions;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (interfaceType == 0x51 && hasReactions) { *ppOut = (T*)g_reactions; return true; }
        if (interfaceType == 0x4f) { *ppOut = (T*)g_opinion; return true; }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (interfaceType == 0x51 && hasReactions) { *ppOut = (T*)g_reactions; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    const CThing* GetPItemConst() const { return GetPItem(); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    CThing* item = p.GetPItem();
    return item != 0 && !(item->m_flags91 & 1);
}
struct CPlayer {
    CBaseIntelligentPointer CharacterThing;
    const CBaseIntelligentPointer& GetCharacterThing() const { return CharacterThing; }
};
struct CWorld { CPlayer* PMainPlayer; };

static int g_deletes, g_liveObjects;
struct CIVCountedObject {
    long IVRefCount;
    void Delete() { ++g_deletes; --g_liveObjects; }
    long Release() { return --IVRefCount; }
};
template<class T> struct CDefPointer {
    T* m_pObject;
    CDefPointer(T* o) : m_pObject(o) { }
    CDefPointer(const CDefPointer& o) : m_pObject(o.m_pObject) { if (m_pObject) ++m_pObject->IVRefCount; }
    ~CDefPointer() { if (m_pObject) { T* p = m_pObject; if (p->Release() == 0) p->Delete(); m_pObject = 0; } }
    T* operator->() const { return m_pObject; }
};
struct COpinionDeedEffectsDef : CIVCountedObject { long Priority; };

static COpinionDeedEffectsDef g_infos[4];
static int g_infoCalls;
struct CTCHeroOpinionDeedLog {
    long priorities[8];
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long type) const
    {
        COpinionDeedEffectsDef* info = &g_infos[g_infoCalls++];
        info->IVRefCount = 1; info->Priority = priorities[type]; ++g_liveObjects;
        return CDefPointer<COpinionDeedEffectsDef>(info);
    }
};
struct COpinionDeed { long Type; };
struct CTCCreatureOpinionOfHero {
    COpinionDeed* head;
    COpinionDeed* GetHeadPendingDeed() { return head; }
};
struct CAIBrain { CThing* Creature; };
static CWorld g_world;
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    CWorld* GetPThingWorld() const { return &g_world; }
};
struct CAIStateGroup_DeedReactionRoot : CAIStateGroupBase {
    CBaseIntelligentPointer Reactee;
    long DeedType;
    bool OnlyReactToHero;
    bool IsStillValid() const;
};

bool CAIStateGroup_DeedReactionRoot::IsStillValid() const
{
    CThing* hero = GetPThingWorld()->PMainPlayer->GetCharacterThing().GetPItem();
    if (IS_THING_ALIVE(Reactee) && (!OnlyReactToHero || hero == Reactee.GetPItem() || hero == 0)) {
        CTCHeroOpinionDeedLog* reactions;
        if (Reactee.GetPItemConst()->GetInterface(&reactions, 0x51)) {
            CTCCreatureOpinionOfHero* opinion;
            PBrain->Creature->GetInterface(&opinion, 0x4f);
            COpinionDeed* deed = opinion->GetHeadPendingDeed();
            if (deed && deed->Type != DeedType && reactions->GetDeedEffectsDef(deed->Type)->Priority < reactions->GetDeedEffectsDef(DeedType)->Priority)
                return false;
        }
        return true;
    }
    return false;
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }

int main()
{
    CTCHeroOpinionDeedLog reactions;
    for (int i = 0; i < 8; ++i) reactions.priorities[i] = i * 10;
    g_reactions = &reactions;
    CTCCreatureOpinionOfHero opinion; opinion.head = 0; g_opinion = &opinion;
    CThing creature; creature.m_flags91 = 0; creature.hasReactions = false;
    CThing heroThing; heroThing.m_flags91 = 0; heroThing.hasReactions = true;
    CThing other; other.m_flags91 = 0; other.hasReactions = true;
    CPlayer player; player.CharacterThing.m_pItem = &heroThing;
    g_world.PMainPlayer = &player;
    CAIBrain brain; brain.Creature = &creature;
    CAIStateGroup_DeedReactionRoot g; g.PBrain = &brain; g.DeedType = 3; g.OnlyReactToHero = false;
    COpinionDeed deed;

#define RESET() (g_infoCalls = 0, g_deletes = 0, g_liveObjects = 0)
    RESET(); g.Reactee.m_pItem = 0;
    Check("no reactee", !g.IsStillValid() && g_infoCalls == 0);
    RESET(); g.Reactee.m_pItem = &other; other.m_flags91 = 1;
    Check("dead reactee", !g.IsStillValid());
    other.m_flags91 = 0;
    RESET(); opinion.head = 0;
    Check("alive, no pending deed", g.IsStillValid() && g_infoCalls == 0);
    RESET(); g.OnlyReactToHero = true; g.Reactee.m_pItem = &other;
    Check("hero-only, reactee not hero", !g.IsStillValid());
    RESET(); g.Reactee.m_pItem = &heroThing;
    Check("hero-only, reactee is hero", g.IsStillValid());
    RESET(); player.CharacterThing.m_pItem = 0; g.Reactee.m_pItem = &other;
    Check("hero-only, no hero", g.IsStillValid());
    player.CharacterThing.m_pItem = &heroThing; g.OnlyReactToHero = false;
    RESET(); other.hasReactions = false; deed.Type = 7; opinion.head = &deed;
    Check("no reactions interface", g.IsStillValid() && g_infoCalls == 0);
    other.hasReactions = true;
    RESET(); deed.Type = 3;
    Check("same deed type", g.IsStillValid() && g_infoCalls == 0);
    RESET(); deed.Type = 1;   // pending priority 10 < reacted 30 -> invalid
    Check("lower-priority pending deed", !g.IsStillValid() && g_infoCalls == 2 && g_deletes == 2 && g_liveObjects == 0);
    RESET(); deed.Type = 7;   // pending priority 70 >= reacted 30 -> still valid
    Check("higher-priority pending deed", g.IsStillValid() && g_infoCalls == 2 && g_deletes == 2 && g_liveObjects == 0);
    RESET(); reactions.priorities[5] = 30; deed.Type = 5;   // equal -> still valid
    Check("equal priority", g.IsStillValid() && g_deletes == 2);
    if (g_fail) return 1;
    printf("0095fed0_TEST PASS\n");
    return 0;
}