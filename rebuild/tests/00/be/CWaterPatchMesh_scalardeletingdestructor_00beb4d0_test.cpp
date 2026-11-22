// Behaviour test for CWaterPatchMesh::`scalar deleting destructor' @ 0x00BEB4D0 on the shared headers (NF272, 2026-11-22): links the row
// object (the destructor's TU folded); the stubs as in 0x00BEAAA0's test. The CWaterPatchMesh vptr is read while the row's destructor runs,
// then placed in fresh raw storage so `delete` / an unqualified destructor call reach ??_GCWaterPatchMesh through vtable slot 0.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_CWATERPATCHMESH_OPERATORDELETE
#include <stdio.h>
#include <string.h>
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CWaterPatchMesh.h"

CEngineLandscapeRenderer* NEngineInternal::GPLandscapeRenderer;
static double g_renderer_storage[sizeof(CEngineLandscapeRenderer) / sizeof(double) + 1];
static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_buffer_storage[4];
static CWaterPatchMesh* g_mesh;
static void* g_vptrDuringDelete;
static int g_deleteCalls, g_freeCalls;
static CVertexBufferWin32* g_deleted;
static void* g_freed;
static CEngineLandscapeMemoryManager* g_deleteList;
static CEngineLandscapeMemoryManager* g_freeList;

void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* buffer)
{
    ++g_deleteCalls;
    g_deleted = buffer;
    g_deleteList = this;
    if (g_mesh) memcpy(&g_vptrDuringDelete, g_mesh, sizeof(void*));   // the vptr while the derived body runs
}
void CEngineLandscapeMemoryManager::FreeVirtual(void* block) { ++g_freeCalls; g_freed = block; g_freeList = this; }
// The emitted vtables' other slots (never called here).
void CWaterPatchMesh::RelocateData(long, void*, void*, long) { }
bool CWaterPatchMesh::CanRelocateData(long) const { return true; }
bool CMovableResource::CanRelocateData(long) const { return true; }
unsigned long CMovableResource::Verify() const { return 0x7B; }

static CEngineLandscapeMemoryManager* SetUpRenderer()
{
    CEngineLandscapeRenderer* renderer = reinterpret_cast<CEngineLandscapeRenderer*>(g_renderer_storage);
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    renderer->MemoryManager = manager;
    NEngineInternal::GPLandscapeRenderer = renderer;
    g_deleteCalls = g_freeCalls = 0;
    g_deleted = 0; g_freed = 0; g_deleteList = 0; g_freeList = 0; g_vptrDuringDelete = 0;
    return manager;
}

static CWaterPatchMesh* Populate(double* storage, bool withData, bool withBuffer)
{
    CWaterPatchMesh* mesh = reinterpret_cast<CWaterPatchMesh*>(storage);
    memset(storage, 0, sizeof(CWaterPatchMesh));
    mesh->VertexHeightData = withData
        ? static_cast<CWaterPatchMesh::CVertexHeightData*>(operator new(sizeof(CWaterPatchMesh::CVertexHeightData))) : 0;
    mesh->VertexBuffer = withBuffer ? reinterpret_cast<CVertexBufferWin32*>(g_buffer_storage) : 0;
    g_mesh = mesh;
    return mesh;
}

// The row's destructor, called non-virtually (??1CWaterPatchMesh@@UAE@XZ): frees and clears both members; the buffer goes to the
// manager's DeleteVertexBuffer while the CWaterPatchMesh vptr is in place; ends on the CMovableResource vptr; never frees the object.
static int RunDestructor(bool withData, bool withBuffer, void** derivedVptr)
{
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    double storage[sizeof(CWaterPatchMesh) / sizeof(double) + 1];
    CWaterPatchMesh* mesh = Populate(storage, withData, withBuffer);
    mesh->CWaterPatchMesh::~CWaterPatchMesh();
    void* finalVptr;
    memcpy(&finalVptr, mesh, sizeof(void*));
    if (mesh->VertexHeightData != 0) return 1;
    if (mesh->VertexBuffer != 0) return 2;
    if (g_deleteCalls != (withBuffer ? 1 : 0)) return 3;
    if (withBuffer && (g_deleted != reinterpret_cast<CVertexBufferWin32*>(g_buffer_storage) || g_deleteList != manager)) return 4;
    if (g_freeCalls != 0) return 5;
    if (finalVptr == 0) return 6;
    if (withBuffer && (g_vptrDuringDelete == 0 || g_vptrDuringDelete == finalVptr)) return 7;
    if (derivedVptr && withBuffer) *derivedVptr = g_vptrDuringDelete;
    g_mesh = 0;
    return 0;
}

static int RunScalarDeleting(bool freeIt, void* derivedVptr)
{
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    double storage[sizeof(CWaterPatchMesh) / sizeof(double) + 1];
    CWaterPatchMesh* mesh = Populate(storage, true, true);
    memcpy(mesh, &derivedVptr, sizeof(void*));
    if (freeIt)
        delete mesh;                    // slot 0 with flag 1: destroy, then the class delete (FreeVirtual)
    else
        mesh->~CWaterPatchMesh();       // slot 0 with flag 0: destroy only
    if (mesh->VertexHeightData != 0 || mesh->VertexBuffer != 0) return 1;
    if (g_deleteCalls != 1 || g_deleted != reinterpret_cast<CVertexBufferWin32*>(g_buffer_storage) || g_deleteList != manager) return 2;
    if (g_freeCalls != (freeIt ? 1 : 0)) return 3;
    if (freeIt && (g_freed != mesh || g_freeList != manager)) return 4;
    return 0;
}

int main()
{
    void* derivedVptr = 0;
    const int d = RunDestructor(false, true, &derivedVptr);
    if (d != 0 || derivedVptr == 0)
    {
        printf("OUTLINEGLOW_SCALAR_TEST FAIL dtor=%d\n", d);
        return 100 + d;
    }
    const int retained = RunScalarDeleting(false, derivedVptr);
    if (retained != 0)
    {
        printf("OUTLINEGLOW_SCALAR_TEST FAIL retained=%d\n", retained);
        return 200 + retained;
    }
    const int freed = RunScalarDeleting(true, derivedVptr);
    if (freed != 0)
    {
        printf("OUTLINEGLOW_SCALAR_TEST FAIL freed=%d\n", freed);
        return 300 + freed;
    }
    printf("OUTLINEGLOW_SCALAR_TEST PASS\n");
    return 0;
}
