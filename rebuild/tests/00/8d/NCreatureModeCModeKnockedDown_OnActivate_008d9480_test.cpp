#include <cstdio>
// Behaviour test for NCreatureMode::CModeKnockedDown::OnActivate (retail 0x008D9480): the emote component (interface 0x45)
// shows icon 8 (0.1 s fade, 1.0 s, alpha 255); the interface-0x30 component's ability timers are invalidated; a state-1
// creature has its animation suspended; then the base activation.
struct CTCBase { int tag; };
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
struct CThingLookup {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_store[9];
    void ResetInterfaces()
    {
        unsigned int w = 0;
        while (w < 9) { m_presentBits[w] = 0; m_store[w].type = 0x7fff; m_store[w].component = 0; ++w; }
        m_interfaces.first = m_store;
        m_interfaces.last = m_store;
    }
    void AddInterface(long type, CTCBase* component)
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type;
        it->component = component;
        ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};
struct CThing : CThingLookup {
    unsigned char m_type;
    int suspends;
    void SetSuspendAnimation(bool suspend, long reason) { if (suspend && reason == 0) ++suspends; }
};
struct CTCEmoteIcon : CTCBase {
    long Icon; long Arg; float Fade; float Time; unsigned char Alpha;
    void SetEmoteIcon(long icon, long arg, float fade, float time, unsigned char alpha)
    {
        Icon = icon; Arg = arg; Fade = fade; Time = time; Alpha = alpha;
    }
};
struct CTCInterface30 : CTCBase {
    int invalidations;
    void InvalidateAllAbilityTimers() { ++invalidations; }
};
static int g_baseActivations;
struct CModeBase {
    CThing* ParentThing;
    void OnActivate() { ++g_baseActivations; }
};
struct CModeKnockedDown : CModeBase {
    void OnActivate()
    {
        CTCEmoteIcon* emote;
        if (ParentThing->GetInterface(&emote, 0x45))
            emote->SetEmoteIcon(8, 0, 0.1f, 1.0f, 0xff);
        if (ParentThing->HasTC(0x30))
            ParentThing->GetTC<CTCInterface30>(0x30)->InvalidateAllAbilityTimers();
        if (ParentThing->m_type == 1)
            ParentThing->SetSuspendAnimation(true, 0);
        CModeBase::OnActivate();
    }
};

int main()
{
    CTCEmoteIcon emote;
    emote.Icon = 0; emote.Alpha = 0; emote.Fade = 0.0f;
    CTCInterface30 abilities;
    abilities.invalidations = 0;
    CThing hero;
    hero.ResetInterfaces();
    hero.AddInterface(0x45, &emote);
    hero.AddInterface(0x30, &abilities);
    hero.m_type = 1; hero.suspends = 0;
    CModeKnockedDown m;
    m.ParentThing = &hero;
    g_baseActivations = 0;
    m.OnActivate();
    int pass = emote.Icon == 8 && emote.Arg == 0 && emote.Fade == 0.1f && emote.Time == 1.0f && emote.Alpha == 0xff
        && abilities.invalidations == 1 && hero.suspends == 1 && g_baseActivations == 1;
    hero.ResetInterfaces();
    hero.m_type = 3;
    m.OnActivate();
    pass = pass && abilities.invalidations == 1 && hero.suspends == 1 && g_baseActivations == 2;
    if (!pass) { std::printf("008d9480_TEST FAIL\n"); return 1; }
    std::printf("008d9480_TEST PASS\n");
    return 0;
}