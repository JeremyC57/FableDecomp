// Behaviour test on the shared headers (NF269, 2026-11-22): links the row object; NEngineInternal::GPLandscapeRenderer and
// CEngineLandscapeMemoryManager::DeleteVertexBuffer are stubbed here (raw storage, no constructors run).
#define FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER
#include <new>
#include <stdio.h>
#include <string.h>
#include "candidates/outline_glow_compiled.h"

CEngineLandscapeRenderer* NEngineInternal::GPLandscapeRenderer;
static int g_insertCalls;
static CVertexBufferWin32* g_insertedNode;
static CEngineLandscapeMemoryManager* g_insertList;
static double g_renderer_storage[sizeof(CEngineLandscapeRenderer) / sizeof(double) + 1];
static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_buffer_storage[4];

void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* buffer)
{
    ++g_insertCalls;
    g_insertedNode = buffer;
    g_insertList = this;
}

static CEngineLandscapeMemoryManager* SetUpRenderer()
{
    CEngineLandscapeRenderer* renderer = reinterpret_cast<CEngineLandscapeRenderer*>(g_renderer_storage);
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    renderer->MemoryManager = manager;
    NEngineInternal::GPLandscapeRenderer = renderer;
    return manager;
}

static int TestPopulated()
{
    fable_u8 effectStorage[sizeof(CEngineScreenEffectOutlineGlow)];
    CEngineScreenEffectOutlineGlow* effect =
        reinterpret_cast<CEngineScreenEffectOutlineGlow*>(effectStorage);
    memset(effect, 0, sizeof(*effect));
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    effect->Blur2Texture = operator new(8);
    effect->field_10 = reinterpret_cast<CVertexBufferWin32*>(g_buffer_storage);
    g_insertCalls = 0;
    g_insertedNode = 0;
    g_insertList = 0;

    effect->~CEngineScreenEffectOutlineGlow();

    if (effect->vfptr != reinterpret_cast<void**>(0x0129B860)) return 1;
    if (effect->Blur2Texture != 0) return 2;
    if (effect->field_10 != 0) return 3;
    if (g_insertCalls != 1 || g_insertedNode != reinterpret_cast<CVertexBufferWin32*>(g_buffer_storage)) return 4;
    if (g_insertList != manager) return 5;
    return 0;
}

static int TestEmpty()
{
    fable_u8 effectStorage[sizeof(CEngineScreenEffectOutlineGlow)];
    CEngineScreenEffectOutlineGlow* effect =
        reinterpret_cast<CEngineScreenEffectOutlineGlow*>(effectStorage);
    memset(effect, 0, sizeof(*effect));
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    g_insertCalls = 0;

    effect->~CEngineScreenEffectOutlineGlow();

    if (effect->vfptr != reinterpret_cast<void**>(0x0129B860)) return 1;
    if (g_insertCalls != 0 || manager == 0) return 2;
    return 0;
}

int main()
{
    const int populated = TestPopulated();
    if (populated != 0)
    {
        printf("OUTLINEGLOW_TEST FAIL populated=%d\n", populated);
        return 100 + populated;
    }
    const int empty = TestEmpty();
    if (empty != 0)
    {
        printf("OUTLINEGLOW_TEST FAIL empty=%d\n", empty);
        return 200 + empty;
    }
    printf("OUTLINEGLOW_TEST PASS\n");
    return 0;
}
