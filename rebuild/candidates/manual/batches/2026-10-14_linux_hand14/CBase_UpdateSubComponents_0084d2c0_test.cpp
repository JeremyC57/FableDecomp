#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_updates[3] = {0, 0, 0};
struct CSubComponent {
    int m_id;
    virtual void Update() { ++g_updates[m_id]; }
};
struct SSubComponentRef { CSubComponent* pComponent; void* pInfo; };
struct CBase {
    SSubComponentRef* m_subComponentsFirst;
    SSubComponentRef* m_subComponentsLast;
    void UpdateSubComponents()
    {
        for (SSubComponentRef* it = m_subComponentsFirst; it != m_subComponentsLast; ++it)
            it->pComponent->Update();
    }
};
static bool Check()
{
    CSubComponent c[3]; c[0].m_id = 0; c[1].m_id = 1; c[2].m_id = 2;
    SSubComponentRef refs[3] = { {&c[0], 0}, {&c[1], 0}, {&c[2], 0} };
    CBase b; b.m_subComponentsFirst = refs; b.m_subComponentsLast = refs;
    b.UpdateSubComponents();
    if (g_updates[0] || g_updates[1] || g_updates[2]) return false;
    b.m_subComponentsLast = refs + 3; b.UpdateSubComponents();
    return g_updates[0] == 1 && g_updates[1] == 1 && g_updates[2] == 1;
}
int main()
{
    if (!Check()) { std::printf("0084d2c0_TEST FAIL\n"); return 1; }
    std::printf("0084d2c0_TEST PASS\n");
    return 0;
}
