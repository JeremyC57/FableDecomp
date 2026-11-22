// Behaviour test on the shared headers (NF269, 2026-11-22): links the row object; NEngineInternal::GPLandscapeRenderer and
// CEngineLandscapeMemoryManager::DeleteVertexBuffer / FreeVirtual are stubbed here (raw storage, no constructors run).
#define FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER
#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/outline_glow_compiled.h"

CEngineLandscapeRenderer* NEngineInternal::GPLandscapeRenderer;
static int g_insertCalls;
static int g_freeCalls;
static CVertexBufferWin32* g_insertedNode;
static void* g_freedAllocation;
static double g_renderer_storage[sizeof(CEngineLandscapeRenderer) / sizeof(double) + 1];
static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_buffer_storage[4];

void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* buffer)
{
    ++g_insertCalls;
    g_insertedNode = buffer;
}

void CEngineLandscapeMemoryManager::FreeVirtual(void* allocation)
{
    ++g_freeCalls;
    g_freedAllocation = allocation;
}

static int RunCase(unsigned int freeFlags, int expectedFreeCalls)
{
    fable_u8 effectStorage[sizeof(CEngineScreenEffectOutlineGlow)];
    CEngineScreenEffectOutlineGlow* effect =
        reinterpret_cast<CEngineScreenEffectOutlineGlow*>(effectStorage);
    CEngineLandscapeRenderer* renderer = reinterpret_cast<CEngineLandscapeRenderer*>(g_renderer_storage);
    renderer->MemoryManager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    NEngineInternal::GPLandscapeRenderer = renderer;
    memset(effect, 0, sizeof(*effect));
    effect->Blur2Texture = operator new(8);
    effect->field_10 = reinterpret_cast<CVertexBufferWin32*>(g_buffer_storage);
    g_insertCalls = 0;
    g_freeCalls = 0;
    g_insertedNode = 0;
    g_freedAllocation = 0;

    CEngineScreenEffectOutlineGlow* result = effect->ScalarDeletingDestructor(freeFlags);

    if (result != effect) return 1;
    if (effect->vfptr != reinterpret_cast<void**>(0x0129B860)) return 2;
    if (effect->Blur2Texture != 0 || effect->field_10 != 0) return 3;
    if (g_insertCalls != 1 || g_insertedNode != reinterpret_cast<CVertexBufferWin32*>(g_buffer_storage)) return 4;
    if (g_freeCalls != expectedFreeCalls) return 5;
    if (expectedFreeCalls && g_freedAllocation != effect) return 6;
    return 0;
}

int main()
{
    const int retained = RunCase(0, 0);
    if (retained != 0)
    {
        printf("OUTLINEGLOW_SCALAR_TEST FAIL retained=%d\n", retained);
        return 100 + retained;
    }
    const int freed = RunCase(1, 1);
    if (freed != 0)
    {
        printf("OUTLINEGLOW_SCALAR_TEST FAIL freed=%d\n", freed);
        return 200 + freed;
    }
    printf("OUTLINEGLOW_SCALAR_TEST PASS\n");
    return 0;
}
