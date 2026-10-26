#include <cstdio>
// Behaviour test for NCreatureMode::CModeRockTrollVulnerable::FrameUpdate (retail 0x008E0990): the recovery count runs down;
// once spent the troll recovers when its animation list (interface 0x5a) is empty or its current animation may be
// replaced; then the base update.
class CTCBase { public: int tag; };
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
class CThing : public CThingLookup { public:
    template<class T> T* GetTCIfPresent(long type) { T* p = 0; if (HasTC(type)) p = GetTC<T>(type); return p; }
};
class CAnimateThing { public:
    int AnimsBegin;
    int AnimsEnd;
    unsigned char CurrentAnimFlags;
};
class CTCAnimationComplex : public CTCBase { public:
    CAnimateThing Animate;
    CAnimateThing& GetAnimation() { return Animate; }
};
static int g_baseUpdates;
struct CModeBase {
    CThing* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeRockTrollVulnerable : CModeBase {
    long RecoveryCount;
    int recovers;
    void Recover() { ++recovers; }
    void FrameUpdate()
    {
        CAnimateThing& animate = ParentThing->GetTCIfPresent<CTCAnimationComplex>(0x5a)->GetAnimation();
        if (RecoveryCount)
            --RecoveryCount;
        if (RecoveryCount <= 0 && (animate.AnimsBegin == animate.AnimsEnd || (animate.CurrentAnimFlags & 2)))
            Recover();
        CModeBase::FrameUpdate();
    }
};

int main()
{
    CTCAnimationComplex anim;
    anim.Animate.AnimsBegin = 0; anim.Animate.AnimsEnd = 1; anim.Animate.CurrentAnimFlags = 0;
    CThing troll;
    troll.ResetInterfaces();
    troll.AddInterface(0x5a, &anim);
    CModeRockTrollVulnerable m;
    m.ParentThing = &troll; m.RecoveryCount = 1; m.recovers = 0;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = m.RecoveryCount == 0 && m.recovers == 0;
    anim.Animate.CurrentAnimFlags = 2;
    m.FrameUpdate();
    pass = pass && m.RecoveryCount == 0 && m.recovers == 1;
    anim.Animate.CurrentAnimFlags = 0; anim.Animate.AnimsEnd = 0;
    m.FrameUpdate();
    pass = pass && m.recovers == 2;
    m.RecoveryCount = 3;
    m.FrameUpdate();
    pass = pass && m.RecoveryCount == 2 && m.recovers == 2 && g_baseUpdates == 4;
    if (!pass) { std::printf("008e0990_TEST FAIL\n"); return 1; }
    std::printf("008e0990_TEST PASS\n");
    return 0;
}