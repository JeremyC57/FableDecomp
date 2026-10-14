#include <cstdio>
#include <cstddef>
// Behaviour model of CGameComponent::CGameComponent(CGame*) @ 0x0051D8E0.
struct CGame { int x; };
struct CBase { virtual void v00() {} CBase() {} };
struct CGameListener { virtual void v00() {} CGameListener() {} };
struct CGameComponent : CBase, CGameListener {
    bool m_initialised; bool m_active; CGame* m_pGame;
    virtual void v00() {}
    CGameComponent(CGame* pGame) : CBase(), CGameListener(), m_initialised(false), m_active(false), m_pGame(pGame) {}
};
int main()
{
    CGame game; CGameComponent c(&game);
    bool ok = !c.m_initialised && !c.m_active && c.m_pGame == &game;
    if (sizeof(void*) == 4) ok = ok && offsetof(CGameComponent, m_initialised) == 0x8 && offsetof(CGameComponent, m_pGame) == 0xc;
    if (!ok) { std::printf("0051d8e0_TEST FAIL\n"); return 1; }
    std::printf("0051d8e0_TEST PASS\n");
    return 0;
}