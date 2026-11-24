// Behaviour test for CLandscapeBackgroundPatch::ClearTextures() @ 0x00BE5C30 on the shared headers: links the row object;
// CEngineLandscapeMemoryManager::DeleteTexture and the renderer global are stubbed here (raw storage, no constructors run).
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETETEXTURE
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_CLEARTEXTURES
#include <stdio.h>
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CLandscapeBackgroundPatch.h"

static CTexture* g_deleted; static const void* g_manager_seen; static int g_calls;
void CEngineLandscapeMemoryManager::DeleteTexture(CTexture* texture) { g_deleted = texture; g_manager_seen = this; ++g_calls; }
CEngineLandscapeRenderer* NEngineInternal::GPLandscapeRenderer;

static double g_renderer_storage[sizeof(CEngineLandscapeRenderer) / sizeof(double) + 1];
static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_patch_storage[sizeof(CLandscapeBackgroundPatch) / sizeof(double) + 1];
static double g_texture_storage[4];

int main()
{
    CEngineLandscapeRenderer* renderer = reinterpret_cast<CEngineLandscapeRenderer*>(g_renderer_storage);
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    CLandscapeBackgroundPatch* patch = reinterpret_cast<CLandscapeBackgroundPatch*>(g_patch_storage);
    CTexture* texture = reinterpret_cast<CTexture*>(g_texture_storage);
    renderer->MemoryManager = manager;
    NEngineInternal::GPLandscapeRenderer = renderer;
    patch->Texture = texture;
    patch->IndexBuffer = reinterpret_cast<CIndexBuffer*>(g_texture_storage + 1);
    patch->ClearTextures();                                    // a texture: handed to the manager once, then cleared
    bool ok = g_deleted == texture && g_manager_seen == manager && g_calls == 1 && patch->Texture == 0;
    ok = ok && patch->IndexBuffer == reinterpret_cast<CIndexBuffer*>(g_texture_storage + 1);   // nothing else touched
    patch->ClearTextures();                                    // no texture: the manager is not called
    ok = ok && g_calls == 1 && patch->Texture == 0;
    printf(ok ? "CLandscapeBackgroundPatch_00be5c30_TEST PASS\n" : "CLandscapeBackgroundPatch_00be5c30_TEST FAIL\n");
    return ok ? 0 : 1;
}
