// Behaviour test for CLandscapeLayerMesh::`scalar deleting destructor' @ 0x00BFE900 on the shared headers (NF277, 2026-11-22): links the row
// object (the destructor's TU 0x00BFDE20 folded); replaces the July standalone host model (which defined its own destructor and ForceNew).
// The class block is the landed 0x00BFDE20 row's, verbatim (CLandscapeLayerMesh's only header is quarantined: header-less for the local-model
// rule, EXTRA_RULES 2026-11-20); the class is never constructed here, so this TU emits no CLandscapeLayerMesh vtable, ??_G or destructor.
// The CLandscapeLayerMesh vptr is read while the row's destructor runs, then placed in fresh raw storage so `delete` / an unqualified
// destructor call reach the row's ??_GCLandscapeLayerMesh through vtable slot 0 (flag 1 / flag 0).
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER    // DeleteIndexBuffer 0x00BDA090 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include <stdio.h>
#include <string.h>
#include "engine/CMovableResource.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)

class CIndexBuffer;
class CVertexBufferWin32;
class CGraphicInfoFrame;
class CEngineLandscapeMap;
class CVertex;
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND   // the shared global enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION
#include "engine/CEngineLandscapeMeshBuilder.h"

class CLandscapeLayerMesh : public CMovableResource {    // vtable 0x012A835C, 0x54 bytes
public:
    virtual ~CLandscapeLayerMesh();
private:
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
    virtual bool CanRelocateData(long handle) const;
public:
    static void FABLE_FASTCALL operator delete(void* block) { NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(block); }
    long ForegroundTextureIndex;                         // +0x04
    long BackgroundTextureIndex;                         // +0x08
    long BumpMapTextureIndex;                            // +0x0C
    float SelfIllumination;                              // +0x10
    CGraphicInfoFrame* ForegroundTexture;                // +0x14
    CGraphicInfoFrame* BackgroundTexture;                // +0x18
    CGraphicInfoFrame* BumpMap;                          // +0x1C
    long MinTextureMipMap;                               // +0x20
    long MinBumpMapMipMap;                               // +0x24
    LANDSCAPE_TEXTURE_MAPPING_DIRECTION MappingDirection;// +0x28
    long ForegroundTextureMipmapLevel;                   // +0x2C
    long ForegroundBumpmapTextureMipmapLevel;            // +0x30
    CIndexBuffer* IndexBuffer;                           // +0x34
    CVertexBufferWin32* ForegroundVertexBuffer;          // +0x38
    CLandscapeLayerMesh* Next;                           // +0x3C
    CEngineLandscapeMap* Map;                            // +0x40
    unsigned short VertexCount;                          // +0x44
    unsigned short PolyCountIncDegenerates;              // +0x46
    bool UsesSharedIndexBuffer;                          // +0x48
    CVertexBufferWin32* ProceduralTextureVertexBuffer;   // +0x4C
    CVertex* Vertices;                                   // +0x50
};

CEngineLandscapeRenderer* NEngineInternal::GPLandscapeRenderer;
static double g_renderer_storage[sizeof(CEngineLandscapeRenderer) / sizeof(double) + 1];
static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_index_storage[4];
static double g_vertex_storage[4];
static CLandscapeLayerMesh* g_mesh;
static void* g_vptrDuringDelete;
static char g_seq[32];
static void* g_args[8];
static int g_argCount;
static CEngineLandscapeMemoryManager* g_lastManager;

static void Record(char what, void* arg, CEngineLandscapeMemoryManager* manager)
{
    size_t n = strlen(g_seq);
    if (n + 1 < sizeof(g_seq)) { g_seq[n] = what; g_seq[n + 1] = 0; }
    if (g_argCount < 8) g_args[g_argCount++] = arg;
    g_lastManager = manager;
}
void CEngineLandscapeMemoryManager::DeleteIndexBuffer(CIndexBuffer* buffer)
{
    Record('I', buffer, this);
    if (g_mesh) memcpy(&g_vptrDuringDelete, g_mesh, sizeof(void*));   // the vptr while the derived body runs
}
void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* buffer) { Record('V', buffer, this); }
void CEngineLandscapeMemoryManager::FreeVirtual(void* block) { Record('F', block, this); }
// The emitted vtables' other slots (never called here).
void CLandscapeLayerMesh::RelocateData(long, void*, void*, long) { }
bool CLandscapeLayerMesh::CanRelocateData(long) const { return true; }
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

static CLandscapeLayerMesh* Populate(double* storage, bool withIndex, bool shared, bool withVertex)
{
    CLandscapeLayerMesh* mesh = reinterpret_cast<CLandscapeLayerMesh*>(storage);
    memset(storage, 0, sizeof(CLandscapeLayerMesh));
    mesh->IndexBuffer = withIndex ? reinterpret_cast<CIndexBuffer*>(g_index_storage) : 0;
    mesh->UsesSharedIndexBuffer = shared;
    mesh->ForegroundVertexBuffer = withVertex ? reinterpret_cast<CVertexBufferWin32*>(g_vertex_storage) : 0;
    return mesh;
}

// The row's destructor, called non-virtually (??1CLandscapeLayerMesh@@UAE@XZ): the unshared index buffer and the foreground vertex
// buffer go to the manager (the vertex buffer pointer cleared, the index buffer pointer kept) while the CLandscapeLayerMesh vptr is in
// place; a shared index buffer is left alone; ends on the CMovableResource vptr; never frees the object.
static int RunDestructor(bool shared, void** derivedVptr)
{
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    double storage[sizeof(CLandscapeLayerMesh) / sizeof(double) + 1];
    CLandscapeLayerMesh* mesh = Populate(storage, true, shared, true);
    g_mesh = mesh;
    mesh->CLandscapeLayerMesh::~CLandscapeLayerMesh();
    void* finalVptr;
    memcpy(&finalVptr, mesh, sizeof(void*));
    g_mesh = 0;
    if (mesh->ForegroundVertexBuffer != 0) return 1;
    if (mesh->IndexBuffer != reinterpret_cast<CIndexBuffer*>(g_index_storage)) return 2;
    if (strcmp(g_seq, shared ? "V" : "IV") != 0) return 3;
    if (!shared && g_args[0] != g_index_storage) return 4;
    if (g_args[shared ? 0 : 1] != g_vertex_storage || g_lastManager != manager) return 5;
    if (finalVptr == 0) return 6;
    if (!shared && (g_vptrDuringDelete == 0 || g_vptrDuringDelete == finalVptr)) return 7;
    if (derivedVptr && !shared) *derivedVptr = g_vptrDuringDelete;
    return 0;
}

// Through vtable slot 0 (the row's ??_G): flag 1 destroys then frees through the class delete (FreeVirtual, after the destructor's calls);
// flag 0 destroys only. The chained Next mesh is deleted through its own slot 0 (flag 1) inside the destructor, before this one is freed.
static int RunScalarDeleting(bool freeIt, void* derivedVptr)
{
    CEngineLandscapeMemoryManager* manager = SetUpRenderer();
    double storage[sizeof(CLandscapeLayerMesh) / sizeof(double) + 1];
    double nextStorage[sizeof(CLandscapeLayerMesh) / sizeof(double) + 1];
    CLandscapeLayerMesh* mesh = Populate(storage, true, false, true);
    CLandscapeLayerMesh* next = Populate(nextStorage, false, false, false);
    memcpy(mesh, &derivedVptr, sizeof(void*));
    memcpy(next, &derivedVptr, sizeof(void*));
    mesh->Next = next;
    if (freeIt)
        delete mesh;                        // slot 0 with flag 1
    else
        mesh->~CLandscapeLayerMesh();       // slot 0 with flag 0
    if (mesh->ForegroundVertexBuffer != 0) return 1;
    if (strcmp(g_seq, freeIt ? "IVFF" : "IVF") != 0) return 2;
    if (g_args[0] != g_index_storage || g_args[1] != g_vertex_storage) return 3;
    if (g_args[2] != next) return 4;
    if (freeIt && g_args[3] != mesh) return 5;
    if (g_lastManager != manager) return 6;
    return 0;
}

int main()
{
    void* derivedVptr = 0;
    const int d = RunDestructor(false, &derivedVptr);
    if (d != 0 || derivedVptr == 0) { printf("00bfe900_TEST FAIL dtor=%d\n", d); return 100 + d; }
    const int s = RunDestructor(true, 0);
    if (s != 0) { printf("00bfe900_TEST FAIL shared=%d\n", s); return 200 + s; }
    const int retained = RunScalarDeleting(false, derivedVptr);
    if (retained != 0) { printf("00bfe900_TEST FAIL retained=%d\n", retained); return 300 + retained; }
    const int freed = RunScalarDeleting(true, derivedVptr);
    if (freed != 0) { printf("00bfe900_TEST FAIL freed=%d\n", freed); return 400 + freed; }
    printf("00bfe900_TEST PASS\n");
    return 0;
}
