#include <cstdio>
class CDisplayManager {
public:
    bool IsTextureBitDepthSupported(unsigned long depth);   // 0x009BE530
};
extern CDisplayManager* g_pDisplayManager;                  // 0x013B8390
extern long g_TextureColourDepth;                           // 0x013B7D68
class CWorld {
public:
    void ChangeTextureColourDepth(long depth);              // 0x0049D830
};
class CDisplayEngine {
public:
    void ChangeTextureColourDepth(long depth);              // 0x00434DC0
};
class CMainGameComponent {
public:
    virtual void ChangeTextureColourDepth(long depth);
    unsigned char   _pad04[0x24 - 0x4];
    CWorld*         PWorld;                                 // +0x24 scoped_ptr<CWorld>
    CDisplayEngine* PDisplayEngine;                         // +0x28 scoped_ptr<CDisplayEngine>
protected:
    bool InitGraphics();                                    // 0x00416C8A
};
void CMainGameComponent::ChangeTextureColourDepth(long depth)
{
    if (depth == 16 || depth == 24 || depth == 32)
    {
        if (g_pDisplayManager->IsTextureBitDepthSupported(depth) && depth != g_TextureColourDepth)
        {
            g_TextureColourDepth = depth;
            InitGraphics();
            PDisplayEngine->ChangeTextureColourDepth(depth);
            PWorld->ChangeTextureColourDepth(depth);
        }
    }
}

static int g_init, g_engine_depth, g_world_depth, g_order, g_engine_order, g_world_order;
static unsigned long g_supported_mask = (1u << 16) | (1u << 32 % 32) | (1u << 24);   // 32 maps to bit 0
bool CDisplayManager::IsTextureBitDepthSupported(unsigned long depth) { return (g_supported_mask >> (depth % 32)) & 1; }
static CDisplayManager g_dm;
CDisplayManager* g_pDisplayManager = &g_dm;
long g_TextureColourDepth = 16;
void CWorld::ChangeTextureColourDepth(long depth) { g_world_depth = depth; g_world_order = ++g_order; }
void CDisplayEngine::ChangeTextureColourDepth(long depth) { g_engine_depth = depth; g_engine_order = ++g_order; }
bool CMainGameComponent::InitGraphics() { ++g_init; return true; }
int main()
{
    CMainGameComponent c; CWorld w; CDisplayEngine e; c.PWorld = &w; c.PDisplayEngine = &e;
    c.ChangeTextureColourDepth(16);                    // unchanged: nothing
    bool ok = g_init == 0 && g_order == 0;
    c.ChangeTextureColourDepth(8);                     // not a valid depth
    ok = ok && g_init == 0 && g_TextureColourDepth == 16;
    g_supported_mask = 1u << 16;                       // 32 unsupported
    c.ChangeTextureColourDepth(32);
    ok = ok && g_init == 0 && g_TextureColourDepth == 16;
    g_supported_mask = (1u << 16) | 1u | (1u << 24);
    c.ChangeTextureColourDepth(32);
    ok = ok && g_init == 1 && g_TextureColourDepth == 32 && g_engine_depth == 32 && g_world_depth == 32 && g_engine_order == 1 && g_world_order == 2;
    c.ChangeTextureColourDepth(24);
    ok = ok && g_init == 2 && g_TextureColourDepth == 24 && g_world_depth == 24;
    std::printf(ok ? "0041799b_TEST PASS\n" : "0041799b_TEST FAIL\n");
    return ok ? 0 : 1;
}