#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CTCBase { long type; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_entries[4];
    long m_entryCount;
    CThing() : m_entryCount(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_entries; m_interfaces.last = m_entries; }
    void Add(long type, CTCBase* component)
    {
        m_presentBits[type >> 5] |= (1UL << (type & 31));
        int i = m_entryCount;
        while (i > 0 && m_entries[i - 1].type > type) { m_entries[i] = m_entries[i - 1]; --i; }
        m_entries[i].type = type; m_entries[i].component = component;
        ++m_entryCount; m_interfaces.last = m_entries + m_entryCount;
    }
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
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
};
struct CCharString { const char* m_text; CCharString(const char* text, long length = -1) : m_text(text) { (void)length; } };
static int g_log[16]; static int g_logCount = 0;
static void Log(int v) { if (g_logCount < 16) g_log[g_logCount] = v; ++g_logCount; }
struct CTCPhysicsBase { virtual void SetFlying(bool flying) { Log(flying ? 301 : 300); } };
struct CTCGraphicAppearance { unsigned char m_alpha; CTCGraphicAppearance() : m_alpha(0) { } void SetAlpha(unsigned char alpha) { m_alpha = alpha; Log(400 + alpha); } };
struct CTCCreatureFlags9 : CTCBase { unsigned char m_canBeTargeted; CTCCreatureFlags9() : m_canBeTargeted(2) { } };
struct CTCParticleAttacher : CTCBase { int m_attached; CTCParticleAttacher() : m_attached(0) { } void AttachParticles() { ++m_attached; Log(500); } };
struct CCreatureThing : CThing {
    CTCPhysicsBase m_physics; CTCGraphicAppearance m_graphics; bool m_invisible;
    CCreatureThing() : m_invisible(false) { }
    CTCPhysicsBase* GetPhysics() { return &m_physics; }
    CTCGraphicAppearance* GetGraphicAppearance() { return &m_graphics; }
    virtual void SetInvisible(bool invisible) { m_invisible = invisible; Log(invisible ? 601 : 600); }
};
static CThing* g_willableThing = 0; static int g_willableFlag = -1;
void FASTCALL SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable) { g_willableThing = thing; g_willableFlag = notWillable ? 1 : 0; Log(notWillable ? 701 : 700); }
static const char* g_animName = 0;
struct CCombatActionBase {
    virtual ~CCombatActionBase() { }
    virtual void Initialise() { Log(100); }
    virtual void FinishAction() { Log(101); }
    CBaseIntelligentPointer PCreature;
    void SetAnimationName(const CCharString& name) { g_animName = name.m_text; Log(200); }
};
struct CCreatureAction_BalverineDropInFromSky : CCombatActionBase {
    virtual void FinishAction();
};
void CCreatureAction_BalverineDropInFromSky::FinishAction()
{
    CTCCreatureFlags9* pFlags;
    if (PCreature.GetPItem()->GetInterface(&pFlags, 9))
        pFlags->m_canBeTargeted = 1;
    ((CCreatureThing*)PCreature.GetPItem())->GetGraphicAppearance()->SetAlpha(255);
    SetThingAsNotBloodyWillableAtAll(PCreature.GetPItem(), false);
    CCombatActionBase::FinishAction();
}
int main()
{
    CCreatureThing creature; CTCCreatureFlags9 flags; creature.Add(9, &flags);
    CCreatureAction_BalverineDropInFromSky action; action.PCreature.m_pItem = &creature;
    action.FinishAction();
    const int expect[] = { 655, 700, 101 };
    bool ok = g_logCount == 3 && std::memcmp(g_log, expect, sizeof(expect)) == 0 && flags.m_canBeTargeted == 1
        && creature.m_graphics.m_alpha == 255 && g_willableThing == &creature && g_willableFlag == 0;
    if (!ok) { std::printf("008ccb00_TEST FAIL\n"); return 1; }
    std::printf("008ccb00_TEST PASS\n");
    return 0;
}
