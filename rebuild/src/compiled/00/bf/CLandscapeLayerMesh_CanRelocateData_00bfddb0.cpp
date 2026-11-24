// CLandscapeLayerMesh::CanRelocateData(long handle) const -- the mesh's blocks may be moved unless its landscape map (+0x40) is locked from memory freeing (CEngineLandscapeMap +0x5C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDDB0, length 18 bytes, thiscall, `ret 4`.
// Leaf: ?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z
// Identity (NF276, 2026-11-22): the private virtual ?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z (Ego_r.pdb 00a493d0 + FableWin.pdb 02dde0c0),
//   slot 2 of the CLandscapeLayerMesh vtable 0x012A835C (RTTI; slot 1 RelocateData 0x00BFDE80, slot 0 ??_G 0x00BFE900), which the
//   destructor row 0x00BFDE20's emitted vtable binds. Was the stand-in ?CanRelocateData@CLandscapeLayerMesh@@QAE_NH@Z on `struct S` /
//   `struct CLandscapeLayerMesh` placeholders (owner rule 2026-11-05 stand-in). No call site: reached only through the vtable.
// Layout: CLandscapeLayerMesh in the landed siblings' local model (quarantined header; 0x00BFDE80 / 0x00BFDE20) on the shared retail
//   CMovableResource; Map +0x40 (Ego_r). CEngineLandscapeMap's only header is quarantined too: local model in the shape of the landed
//   0x00BF7F70 (MemoryFreeingLocked at retail +0x5C, Ego_r +0x60). The body reads the member directly: FableWin.pdb's inline
//   ?IsLockedFromMemoryFreeing@CEngineLandscapeMap@@QBE_NXZ (02cca8d0) gave the same leaf bytes but made VC7.1 emit a pick-any copy of it
//   (no retail out-of-line copy), so it is not used.
#define FABLE_RETAIL_CMOVABLERESOURCE
#include "engine/CMovableResource.h"

class CEngineLandscapeMap { public:
    unsigned char _pad00[0x5c];
    bool          MemoryFreeingLocked;          // +0x5c
};
class CIndexBuffer;
class CVertexBufferWin32;
class CGraphicInfoFrame;
class CVertex;
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND   // the shared global enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION (NF276: was a local
#include "engine/CEngineLandscapeMeshBuilder.h"            //   copy with a made-up enumerator)

class CLandscapeLayerMesh : public CMovableResource {    // vtable 0x012A835C, 0x54 bytes
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

bool CLandscapeLayerMesh::CanRelocateData(long handle) const
{
    return Map->MemoryFreeingLocked == false;
}
