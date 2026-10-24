#include <cstdio>
// Behaviour test for NCreatureMode::CModeCarryingStandBase::OnDeactivate (retail 0x008D8960): the creature's animation
// component (interface 0x5a) drops its type-0x18 combination animations, then the stand-mode deactivation runs.
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
struct CThing : CThingLookup { };
struct CAnimateThing {
    long removedType;
    void RemoveCombinationAnimsOfType(long type) { removedType = type; }
};
struct CTCAnimationComplex : CTCBase {
    CAnimateThing Animate;
    CAnimateThing& GetAnimation() { return Animate; }
};
static int g_standDeactivations;
struct CModeStandBase {
    CThing* ParentThing;
    void OnDeactivate() { ++g_standDeactivations; }
};
struct CModeCarryingStandBase : CModeStandBase {
    void OnDeactivate()
    {
        CTCAnimationComplex* animation = 0;
        ParentThing->GetInterface(&animation, 0x5a);
        CAnimateThing& animate = animation->GetAnimation();
        animate.RemoveCombinationAnimsOfType(0x18);
        CModeStandBase::OnDeactivate();
    }
};

int main()
{
    CTCAnimationComplex anim;
    anim.Animate.removedType = 0;
    CThing hero;
    hero.ResetInterfaces();
    hero.AddInterface(0x43, 0);
    hero.AddInterface(0x5a, &anim);
    CModeCarryingStandBase m;
    m.ParentThing = &hero;
    g_standDeactivations = 0;
    m.OnDeactivate();
    if (anim.Animate.removedType != 0x18 || g_standDeactivations != 1) { std::printf("008d8960_TEST FAIL\n"); return 1; }
    std::printf("008d8960_TEST PASS\n");
    return 0;
}