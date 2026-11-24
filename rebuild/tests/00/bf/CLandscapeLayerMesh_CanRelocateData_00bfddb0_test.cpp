// Behaviour test for CLandscapeLayerMesh::CanRelocateData(long) const @ 0x00BFDDB0 on the shared headers (NF277, 2026-11-22): links the
// row object; replaces the July standalone host model (struct S / C stand-ins with their own copy of the body).
// The class blocks are the row's, verbatim (CLandscapeLayerMesh and CEngineLandscapeMap have only quarantined headers: header-less for the
// local-model rule, EXTRA_RULES 2026-11-20), plus one test-only friend declaration so the test can call the private member; a friend does not
// enter any mangled name, so the call binds the row's ?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z. The call is qualified (non-virtual):
// the class is never constructed here, so this TU emits no CLandscapeLayerMesh vtable.
#define FABLE_RETAIL_CMOVABLERESOURCE
#include <stdio.h>
#include <string.h>
#include "engine/CMovableResource.h"

class CEngineLandscapeMap { public:
    unsigned char _pad00[0x5c];
    bool          MemoryFreeingLocked;          // +0x5c
};
class CIndexBuffer;
class CVertexBufferWin32;
class CGraphicInfoFrame;
class CVertex;
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND   // the shared global enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION
#include "engine/CEngineLandscapeMeshBuilder.h"

class CLandscapeLayerMesh : public CMovableResource {    // vtable 0x012A835C, 0x54 bytes
    friend struct CLandscapeLayerMeshTestAccess;         // test only (not in the row)
public:
    virtual ~CLandscapeLayerMesh();
private:
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
    virtual bool CanRelocateData(long handle) const;
public:
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

struct CLandscapeLayerMeshTestAccess {
    static bool CanRelocate(const CLandscapeLayerMesh* mesh, long handle) { return mesh->CLandscapeLayerMesh::CanRelocateData(handle); }
};

int main()
{
    double storage[sizeof(CLandscapeLayerMesh) / sizeof(double) + 1];
    CLandscapeLayerMesh* mesh = reinterpret_cast<CLandscapeLayerMesh*>(storage);
    memset(storage, 0, sizeof(storage));
    CEngineLandscapeMap map;
    memset(&map, 0, sizeof(map));
    mesh->Map = &map;
    map.MemoryFreeingLocked = false;                     // unlocked: the data may move, whatever the handle
    if (!CLandscapeLayerMeshTestAccess::CanRelocate(mesh, 1) || !CLandscapeLayerMeshTestAccess::CanRelocate(mesh, -7)) { printf("B1\n"); return 1; }
    map.MemoryFreeingLocked = true;                      // locked: never
    if (CLandscapeLayerMeshTestAccess::CanRelocate(mesh, 1) || CLandscapeLayerMeshTestAccess::CanRelocate(mesh, 0)) { printf("B2\n"); return 2; }
    if (mesh->Map != &map || map.MemoryFreeingLocked != true) { printf("B3\n"); return 3; }   // reads only
    printf("RB_bfddb0_OK\n");
    return 0;
}
