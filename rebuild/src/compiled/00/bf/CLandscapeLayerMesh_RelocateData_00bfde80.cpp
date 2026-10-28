// CLandscapeLayerMesh::RelocateData(long handle, void* oldData, void* newData, long size) -- the landscape memory manager moved a block: rebase the index buffer pointer (and tell the index buffer it moved) or the foreground vertex buffer pointer when it referred to the old address
// Leaf: ?RelocateData@CLandscapeLayerMesh@@EAEXJPAX0J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFDE80, 58 bytes, thiscall, `ret 10h`.
// Identity (proven): slot 1 of the CLandscapeLayerMesh vtable 0x012A835C (RTTI; slot 2 CanRelocateData 0x00BFDDB0,
//   slot 0 ??_G 0x00BFE900); Ego_r.pdb ?RelocateData@CLandscapeLayerMesh@@EAEXJPAX0J@Z (private virtual).
// Callee: CIndexBuffer::OnPostRelocation 0x00AC1F60 (a bare `ret`; it sits between CIndexBuffer::RelocateData 0x00AC1F50
//   and CIndexBuffer::Initialise 0x00AC1F70, exactly the Ego_r donor order RelocateData / OnPostRelocation / Initialise).
// Layout (Ego_r.pdb, 0x54): IndexBuffer +0x34, ForegroundVertexBuffer +0x38.
// Shape: the landed CWaterPatchMesh::RelocateData 0x00BEAB20 rebase helper (`if (p) p = p - oldData + newData`); the
//   argument is written on the left of each compare (retail loads oldData before the member).
class CIndexBuffer {
public:
    void OnPostRelocation();                             // 0x00AC1F60
};
class CVertexBufferWin32;
class CGraphicInfoFrame;
class CEngineLandscapeMap;
class CVertex;
enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION { LANDSCAPE_TEXTURE_MAPPING_DIRECTION_DEFAULT };

template <class T> inline void RelocatePointer(T*& p, void* oldData, void* newData)
{
    if (p)
        p = (T*)((char*)p - (char*)oldData + (char*)newData);
}

class CMovableResource {
public:
    virtual ~CMovableResource();
    virtual void RelocateData(long handle, void* oldData, void* newData, long size) = 0;
    virtual bool CanRelocateData(long handle) const = 0;
};

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

void CLandscapeLayerMesh::RelocateData(long handle, void* oldData, void* newData, long size)
{
    if (oldData == IndexBuffer) {
        RelocatePointer(IndexBuffer, oldData, newData);
        IndexBuffer->OnPostRelocation();
        return;
    }
    if (oldData == ForegroundVertexBuffer)
        RelocatePointer(ForegroundVertexBuffer, oldData, newData);
}