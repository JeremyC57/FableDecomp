#include <cstdio>
class CThing { public: bool m_dead; int m_fades; };
bool IS_THING_ALIVE(CThing* const& thing) { return thing != 0 && !thing->m_dead; }
void FadeOutEffect(CThing* effect) { ++effect->m_fades; }
struct CIntelligentPointer_CThing { CThing* m_pItem; CThing* Get() const { return m_pItem; } };
static int g_baseDtors = 0;
struct CTCShotDefinitionProjectile {
    CIntelligentPointer_CThing PEffect;
    virtual ~CTCShotDefinitionProjectile() { ++g_baseDtors; }
};
struct CTCShotDefinitionFireballSpell : CTCShotDefinitionProjectile {
    virtual ~CTCShotDefinitionFireballSpell()
    {
        if (IS_THING_ALIVE(PEffect.Get()))
            FadeOutEffect(PEffect.Get());
    }
};
int main()
{
    CThing fx; fx.m_dead = false; fx.m_fades = 0;
    CTCShotDefinitionProjectile* p = new CTCShotDefinitionFireballSpell; p->PEffect.m_pItem = &fx; delete p;
    fx.m_dead = true;
    p = new CTCShotDefinitionFireballSpell; p->PEffect.m_pItem = &fx; delete p;
    p = new CTCShotDefinitionFireballSpell; p->PEffect.m_pItem = 0; delete p;
    if (fx.m_fades != 1 || g_baseDtors != 3) { std::printf("007bd7ca_TEST FAIL\n"); return 1; }
    std::printf("007bd7ca_TEST PASS\n");
    return 0;
}