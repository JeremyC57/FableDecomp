#include <cstdio>
// Behaviour test for CTCInventory::StartGameRendering (retail 0x00581C1E).
static long g_frame = 0;
static long g_lastEnable = -1;
static int g_calls = 0;

struct CRenderer {
    virtual void SetGameRendering(long enable) { g_lastEnable = enable; ++g_calls; }
    virtual ~CRenderer() { }
};
struct CDisplaySystem { CRenderer* m_pRenderer; };
struct CGameObject { CDisplaySystem* m_pDisplay; };
static CGameObject* g_pGameObject = 0;

class CWorld { public:
    long m_gameRenderingStartFrame;
    static long GetWorldFrame() { return g_frame; }
};
static CWorld g_world;

class CTCInventory { public:
    CWorld* GetWorld() { return &g_world; }
    void StartGameRendering();
};

void CTCInventory::StartGameRendering()
{
    g_pGameObject->m_pDisplay->m_pRenderer->SetGameRendering(1);
    CWorld* world = GetWorld();
    world->m_gameRenderingStartFrame = CWorld::GetWorldFrame();
}

int main()
{
    CRenderer r; CDisplaySystem d; d.m_pRenderer = &r; CGameObject go; go.m_pDisplay = &d;
    g_pGameObject = &go;
    g_world.m_gameRenderingStartFrame = 0;
    g_frame = 1234;
    CTCInventory inv;
    inv.StartGameRendering();
    if (g_calls != 1 || g_lastEnable != 1 || g_world.m_gameRenderingStartFrame != 1234) { std::printf("00581c1e_TEST FAIL\n"); return 1; }
    g_frame = 99; inv.StartGameRendering();
    if (g_calls != 2 || g_world.m_gameRenderingStartFrame != 99) { std::printf("00581c1e_TEST FAIL\n"); return 1; }
    std::printf("00581c1e_TEST PASS\n");
    return 0;
}