// C3DMeshFileLoader::~C3DMeshFileLoader() -- releases the 22 chunk-type counted pointers, then chains to ~CChunkedFileLoader
// Leaf: ??1C3DMeshFileLoader@@UAE@XZ
// Retail 0x00A9A900, 1021 bytes. Stores vtable 0x0129E0D0 (.?AVC3DMeshFileLoader@@, slot 0 = ??_G 0x00A9D2C0), runs the
// inline ~CCountedPointer of every member in reverse declaration order (+0xB8 .. +0x10; Data then Info zeroed, the
// count block freed through operator delete 0x00BFE9BC), then tail-jumps to ??1CChunkedFileLoader@@UAE@XZ 0x00AA7540.
// Layout from Ego_r.pdb (struct_layouts_egor.tsv): CChunkedFileLoader 0x10 bytes (vptr, ReadUnknownChunkTypes,
// SupportedFileType[5]); C3DMeshFileLoader 0xC0 bytes, 22 CCountedPointer members.
#include "engine/CCountedPointer.h"

class CChunkedFileChunk;
class C3DMeshFileRootChunk;

class CChunkedFileLoader {
public:
    virtual ~CChunkedFileLoader();                                   // 0x00AA7540
    int ReadUnknownChunkTypes;                                       // +0x4
    char SupportedFileType[5];                                       // +0x8
};

class C3DMeshFileLoader : public CChunkedFileLoader {
public:
    virtual ~C3DMeshFileLoader();
    CCountedPointer<C3DMeshFileRootChunk> RootChunkType;             // +0x10
    CCountedPointer<CChunkedFileChunk> SubMeshChunkType;             // +0x18
    CCountedPointer<CChunkedFileChunk> TransformationChunkType;      // +0x20
    CCountedPointer<CChunkedFileChunk> BoneChunkType;                // +0x28
    CCountedPointer<CChunkedFileChunk> PrimitiveChunkType;           // +0x30
    CCountedPointer<CChunkedFileChunk> TrianglesChunkType;           // +0x38
    CCountedPointer<CChunkedFileChunk> SmoothingGroupsChunkType;     // +0x40
    CCountedPointer<CChunkedFileChunk> VerticesChunkType;            // +0x48
    CCountedPointer<CChunkedFileChunk> UniqueVerticesChunkType;      // +0x50
    CCountedPointer<CChunkedFileChunk> GroupChunkType;               // +0x58
    CCountedPointer<CChunkedFileChunk> PrimitiveClothChunkType;      // +0x60
    CCountedPointer<CChunkedFileChunk> SubMeshClothChunkType;        // +0x68
    CCountedPointer<CChunkedFileChunk> MaterialListChunkType;        // +0x70
    CCountedPointer<CChunkedFileChunk> MaterialChunkType;            // +0x78
    CCountedPointer<CChunkedFileChunk> MaterialExtChunkType;         // +0x80
    CCountedPointer<CChunkedFileChunk> MapChunkType;                 // +0x88
    CCountedPointer<CChunkedFileChunk> EngineMaterialChunkType;      // +0x90
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;             // +0x98
    CCountedPointer<CChunkedFileChunk> HelperPointChunkType;         // +0xA0
    CCountedPointer<CChunkedFileChunk> DummyObjectChunkType;         // +0xA8
    CCountedPointer<CChunkedFileChunk> ConvexVolumeChunkType;        // +0xB0
    CCountedPointer<CChunkedFileChunk> MirroredTextureChunkType;     // +0xB8
};

C3DMeshFileLoader::~C3DMeshFileLoader()
{
}