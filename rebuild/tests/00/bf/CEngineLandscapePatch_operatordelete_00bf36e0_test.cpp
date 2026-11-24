// Behaviour test for CEngineLandscapePatch::operator delete[](void*, CMovableResource*) @ 0x00BF36E0 on the shared headers: links the row object;
// FreeVirtual and the renderer global are stubbed here (raw storage, no constructors run).
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#include <stdio.h>
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"

class CEngineLandscapePatch {
public:
    static void __fastcall operator delete[](void* block, CMovableResource* owner);
};

static void* g_freed; static const void* g_manager_seen; static int g_calls;
void CEngineLandscapeMemoryManager::FreeVirtual(void* block) { g_freed = block; g_manager_seen = this; ++g_calls; }
CEngineLandscapeRenderer* NEngineInternal::GPLandscapeRenderer;

static double g_renderer_storage[sizeof(CEngineLandscapeRenderer) / sizeof(double) + 1];
static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_owner_storage[8];

int main()
{
    CEngineLandscapeRenderer* renderer = reinterpret_cast<CEngineLandscapeRenderer*>(g_renderer_storage);
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    renderer->MemoryManager = manager;
    NEngineInternal::GPLandscapeRenderer = renderer;
    char block[32];
    CEngineLandscapePatch::operator delete[](block, 0);                                            // owner null, as every retail caller passes
    bool ok = g_freed == block && g_manager_seen == manager && g_calls == 1;
    CEngineLandscapePatch::operator delete[](block + 8, reinterpret_cast<CMovableResource*>(g_owner_storage));   // owner ignored
    ok = ok && g_freed == block + 8 && g_manager_seen == manager && g_calls == 2;
    CEngineLandscapePatch::operator delete[](0, 0);                                                // no null test: the manager sees the null block
    ok = ok && g_freed == 0 && g_calls == 3;
    printf(ok ? "00bf36e0_TEST PASS\n" : "00bf36e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
