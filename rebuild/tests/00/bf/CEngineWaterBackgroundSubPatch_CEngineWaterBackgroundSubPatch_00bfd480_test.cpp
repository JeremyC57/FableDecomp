// Behaviour test for CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch @ 0x00BFD480 on the shared headers (NF279, 2026-11-22): links the
// row object; replaces the July standalone host model (a local CMovableResource / sub-patch copy of the body with AppendFreeEdgeNode / FreeBlock
// stubs, which never touched the row). The class is engine/CEngineWaterBackgroundSubPatch.h with the row's opt-ins (VIRTUALS on the retail
// CMovableResource base); it is never constructed here, so this TU emits no CEngineWaterBackgroundSubPatch vtable, ??_G or destructor. The row's
// destructor is called non-virtually; the vptr it stores is captured during the manager calls and planted in raw storage, so slot 0 (the row TU's
// ??_GCEngineWaterBackgroundSubPatch, which calls this destructor) also runs it for `delete` (flag 1) and an unqualified destructor call (flag 0).
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER    // DeleteIndexBuffer 0x00BDA090 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22          // the class on its retail base CMovableResource
#define FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS        // the virtual destructor, the two private overrides, the class delete
#include <stdio.h>
#include <string.h>
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CMovableResource.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)

CEngineLandscapeRenderer* NEngineInternal::GPLandscapeRenderer;
static double g_renderer_storage[sizeof(CEngineLandscapeRenderer) / sizeof(double) + 1];
static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_buffers[10][2];                     // 0 main index, 1 main vertex, 2..5 bridge index, 6..9 bridge vertex
static CEngineWaterBackgroundSubPatch* g_patch;
static void* g_vptrDuringDelete;
static char g_seq[32];
static void* g_args[16];
static int g_argCount;
static CEngineLandscapeMemoryManager* g_lastManager;

static void Record(char what, void* arg, CEngineLandscapeMemoryManager* manager)
{
    size_t n = strlen(g_seq);
    if (n + 1 < sizeof(g_seq)) { g_seq[n] = what; g_seq[n + 1] = 0; }
    if (g_argCount < 16) g_args[g_argCount++] = arg;
    g_lastManager = manager;
}
void CEngineLandscapeMemoryManager::DeleteIndexBuffer(CIndexBuffer* buffer)
{
    Record('I', buffer, this);
    if (g_patch) memcpy(&g_vptrDuringDelete, g_patch, sizeof(void*));   // the vptr while the derived body runs
}
void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* buffer) { Record('V', buffer, this); }
void CEngineLandscapeMemoryManager::FreeVirtual(void* block) { Record('F', block, this); }
// The emitted vtables' other slots (never called here).
void CEngineWaterBackgroundSubPatch::RelocateData(long, void*, void*, long) { }
bool CEngineWaterBackgroundSubPatch::CanRelocateData(long) const { return true; }
bool CMovableResource::CanRelocateData(long) const { return true; }
unsigned long CMovableResource::Verify() const { return 0x7B; }

static CEngineLandscapeMemoryManager* SetUpRenderer()
{
    CEngineLandscapeRenderer* renderer = reinterpret_cast<CEngineLandscapeRenderer*>(g_renderer_storage);
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    renderer->MemoryManager = manager;
    NEngineInternal::GPLandscapeRenderer = renderer;
    g_seq[0] = 0; g_argCount = 0; g_lastManager = 0; g_vptrDuringDelete = 0;
    return manager;
}

// Every buffer present except the third bridge's vertex buffer (index 8) and, when asked, the main vertex buffer.
static CEngineWaterBackgroundSubPatch* Populate(double* storage, bool deletable, bool withMainVertex)
{
    CEngineWaterBackgroundSubPatch* patch = reinterpret_cast<CEngineWaterBackgroundSubPatch*>(storage);
    memset(storage, 0, sizeof(CEngineWaterBackgroundSubPatch));
    patch->IndexBuffer = reinterpret_cast<CIndexBuffer*>(g_buffers[0]);
    patch->VertexBuffer = withMainVertex ? reinterpret_cast<CVertexBufferWin32*>(g_buffers[1]) : 0;
    for (int i = 0; i < 4; ++i) {
        patch->BridgeIndexBuffers[i] = reinterpret_cast<CIndexBuffer*>(g_buffers[2 + i]);
        patch->BridgeVertexBuffers[i] = i == 2 ? 0 : reinterpret_cast<CVertexBufferWin32*>(g_buffers[6 + i]);
    }
    patch->DeletableBuffers = deletable;
    return patch;
}

// The order the destructor hands the buffers back: main index, main vertex, then index / vertex per bridge (bridge 2 has no vertex buffer).
static int CheckBuffers(bool withMainVertex, int first)
{
    static const int order[10] = { 0, 1, 2, 6, 3, 7, 4, 5, 9, -1 };
    int k = first;
    for (int j = 0; order[j] >= 0; ++j) {
        if (order[j] == 1 && !withMainVertex) continue;
        if (g_args[k++] != g_buffers[order[j]]) return 1 + j;
    }
    return 0;
}

// The row's destructor, called non-virtually (??1CEngineWaterBackgroundSubPatch@@UAE@XZ): with DeletableBuffers every present buffer goes to the
// manager while the sub-patch vptr is in place; without it nothing is released; it ends on the CMovableResource vptr and never frees the object.
static int RunDestructor(bool deletable, void** derivedVptr)
{
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    double storage[sizeof(CEngineWaterBackgroundSubPatch) / sizeof(double) + 1];
    CEngineWaterBackgroundSubPatch* patch = Populate(storage, deletable, false);
    g_patch = patch;
    patch->CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch();
    void* finalVptr;
    memcpy(&finalVptr, patch, sizeof(void*));
    g_patch = 0;
    if (finalVptr == 0) return 1;
    if (patch->IndexBuffer != reinterpret_cast<CIndexBuffer*>(g_buffers[0])) return 2;    // pointers are left as they were
    if (!deletable) return g_argCount == 0 && g_seq[0] == 0 ? 0 : 3;
    if (strcmp(g_seq, "IIVIVIIV") != 0) return 4;
    if (CheckBuffers(false, 0) != 0) return 5;
    if (g_lastManager != manager) return 6;
    if (g_vptrDuringDelete == 0 || g_vptrDuringDelete == finalVptr) return 7;
    if (derivedVptr) *derivedVptr = g_vptrDuringDelete;
    return 0;
}

// Through vtable slot 0 (the row TU's ??_G): flag 1 destroys then frees through the class delete (FreeVirtual, after the destructor's calls);
// flag 0 destroys only.
static int RunScalarDeleting(bool freeIt, void* derivedVptr)
{
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    double storage[sizeof(CEngineWaterBackgroundSubPatch) / sizeof(double) + 1];
    CEngineWaterBackgroundSubPatch* patch = Populate(storage, true, true);
    memcpy(patch, &derivedVptr, sizeof(void*));
    if (freeIt)
        delete patch;                                  // slot 0 with flag 1
    else
        patch->~CEngineWaterBackgroundSubPatch();      // slot 0 with flag 0
    if (strcmp(g_seq, freeIt ? "IVIVIVIIVF" : "IVIVIVIIV") != 0) return 1;
    if (CheckBuffers(true, 0) != 0) return 2;
    if (freeIt && g_args[9] != patch) return 3;
    if (g_lastManager != manager) return 4;
    return 0;
}

int main()
{
    void* derivedVptr = 0;
    const int d = RunDestructor(true, &derivedVptr);
    if (d != 0 || derivedVptr == 0) { printf("00bfd480_TEST FAIL dtor=%d\n", d); return 100 + d; }
    const int k = RunDestructor(false, 0);
    if (k != 0) { printf("00bfd480_TEST FAIL kept=%d\n", k); return 200 + k; }
    const int retained = RunScalarDeleting(false, derivedVptr);
    if (retained != 0) { printf("00bfd480_TEST FAIL retained=%d\n", retained); return 300 + retained; }
    const int freed = RunScalarDeleting(true, derivedVptr);
    if (freed != 0) { printf("00bfd480_TEST FAIL freed=%d\n", freed); return 400 + freed; }
    printf("00bfd480_TEST PASS\n");
    return 0;
}
