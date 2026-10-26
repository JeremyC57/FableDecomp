// Behaviour test for 0x00843470 CCreatureAction_LearnExpression constructor: STANDARD_CAST animation, speeds -1/0, silent sound, the player and the learn-from thing pointers set.
#include <cstdio>
#include <cstring>

class CThing { public: int id; };
static int g_liveStrings = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
class CAnimationEntry { public: char name[40]; bool loaded; int liveStrings; };
static CAnimationEntry g_entry;
class CThingCreatureBase { public:
    int id;
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loaded)
    { std::strcpy(g_entry.name, name.m_pData); g_entry.loaded = loaded; g_entry.liveStrings = g_liveStrings; return &g_entry; }
};
static CAnimationEntry* GetRandomLoadedAnim(CThingCreatureBase* owner, const CCharString& name)
{
    return owner->GetRandomLoadedAnimFromName(name, true);
}
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CBaseIntelligentPointer {
    void* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem(0), m_sets(0) { }
    void SetPItem(void* p) { m_pItem = p; ++m_sets; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
};
class CCreatureActionBase { public:
    CThingCreatureBase* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; long m_c, m_d;
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& s, long c, long d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(s), m_c(c), m_d(d) { }
};
static bool BaseOk(const CCreatureActionBase& a, CThingCreatureBase* owner, float b, const char* name)
{
    return a.m_owner == owner && a.m_a == -1.0f && a.m_b == b && a.m_anim == &g_entry && std::strcmp(g_entry.name, name) == 0 &&
        g_entry.loaded && g_entry.liveStrings == 1 && g_liveStrings == 0 && a.m_sound.FirstSound == 0 &&
        a.m_sound.LastSound == 0 && a.m_c == 0 && a.m_d == 0;
}
class CThingPlayerCreature : public CThingCreatureBase { public: };
class CCreatureAction_LearnExpression : public CCreatureActionBase { public:
    CIntelligentPointer<CThingPlayerCreature> PPlayerCreature;
    CIntelligentPointer<CThing> PLearnFromThing;
    CCreatureAction_LearnExpression(CThingPlayerCreature* player, CThing* from)
        : CCreatureActionBase(player, -1.0f, 0.0f, GetRandomLoadedAnim(player, "STANDARD_CAST"), CSoundPair(), 0, 0),
          PPlayerCreature(player), PLearnFromThing(from) { }
};
int main()
{
    CThingCreatureBase owner = { 1 }; CThing thing = { 2 };
    CThingPlayerCreature player; player.id = 3;
    CCreatureAction_LearnExpression a(&player, &thing);
    if (!BaseOk(a, &player, 0.0f, "STANDARD_CAST") || a.PPlayerCreature.m_pItem != &player || a.PLearnFromThing.m_pItem != &thing ||
        a.PPlayerCreature.m_sets != 1 || a.PLearnFromThing.m_sets != 1) { std::printf("00843470_TEST FAIL\n"); return 1; }
    (void)owner;
    std::printf("00843470_TEST PASS\n");
    return 0;
}