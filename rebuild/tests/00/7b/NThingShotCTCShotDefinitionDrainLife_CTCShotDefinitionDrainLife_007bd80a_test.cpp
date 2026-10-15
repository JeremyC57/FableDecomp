#include <cstdio>
struct CThing { bool m_dead; int m_kills; void Kill(bool) { ++m_kills; m_dead = true; } };
struct CIntelligentPointer_CThing {
    CThing* m_pItem;
    CThing* operator->() const { return m_pItem; }
};
static int g_baseDtors = 0;
bool IS_THING_ALIVE(const CIntelligentPointer_CThing& thing) { return thing.m_pItem != 0 && !thing.m_pItem->m_dead; }
struct CTCShotDefinitionBase { virtual ~CTCShotDefinitionBase() { ++g_baseDtors; } };
struct CTCShotDefinitionDrainLife : CTCShotDefinitionBase {
    CIntelligentPointer_CThing PSoulEffect;
    virtual ~CTCShotDefinitionDrainLife()
    {
        if (IS_THING_ALIVE(PSoulEffect))
            PSoulEffect->Kill(true);
    }
};
int main()
{
    CThing soul; soul.m_dead = false; soul.m_kills = 0;
    CTCShotDefinitionBase* s = new CTCShotDefinitionDrainLife; ((CTCShotDefinitionDrainLife*)s)->PSoulEffect.m_pItem = &soul;
    delete s;
    CTCShotDefinitionBase* t = new CTCShotDefinitionDrainLife; ((CTCShotDefinitionDrainLife*)t)->PSoulEffect.m_pItem = &soul;
    delete t;                                   // already dead: not killed again
    CTCShotDefinitionBase* u = new CTCShotDefinitionDrainLife; ((CTCShotDefinitionDrainLife*)u)->PSoulEffect.m_pItem = 0;
    delete u;
    if (soul.m_kills != 1 || g_baseDtors != 3) { std::printf("007bd80a_TEST FAIL\n"); return 1; }
    std::printf("007bd80a_TEST PASS\n");
    return 0;
}