class CLandscapeLayerMesh { public:
    void* vtbl;        // +0x00
    int ForegroundTextureIndex;           // +0x04
    int BackgroundTextureIndex;           // +0x08
    int BumpMapTextureIndex;           // +0x0c
    int f10;           // +0x10
    int ForegroundTexture;           // +0x14
    int BackgroundTexture;           // +0x18
    int BumpMap;           // +0x1c
    int MinTextureMipMap;           // +0x20
    int MinBumpMapMipMap;           // +0x24
    int f28;           // +0x28
    int ForegroundTextureMipmapLevel;           // +0x2c
    int ForegroundBumpmapTextureMipmapLevel;           // +0x30
    int IndexBuffer;           // +0x34
    int ForegroundVertexBuffer;           // +0x38
    int Next;           // +0x3c
    int Map;           // +0x40
    short VertexCount;         // +0x44
    short PolyCountIncDegenerates;         // +0x46
    char UsesSharedIndexBuffer;          // +0x48
    CLandscapeLayerMesh* CLandscapeLayerMesh_ctor(int arg);
};

extern void* CLandscapeLayerMesh_vtbl;

CLandscapeLayerMesh* CLandscapeLayerMesh::CLandscapeLayerMesh_ctor(int arg) {
    this->Map = arg;
    this->vtbl = &CLandscapeLayerMesh_vtbl;
    this->Next = 0;
    this->VertexCount = 0;
    this->PolyCountIncDegenerates = 0;
    this->ForegroundTextureIndex = 0;
    this->BackgroundTextureIndex = 0;
    this->BumpMapTextureIndex = 0;
    this->ForegroundTexture = 0;
    this->BackgroundTexture = 0;
    this->BumpMap = 0;
    this->f28 = 0;
    this->IndexBuffer = 0;
    this->ForegroundVertexBuffer = 0;
    this->UsesSharedIndexBuffer = 0;
    this->ForegroundBumpmapTextureMipmapLevel = 0;
    this->ForegroundTextureMipmapLevel = 0;
    this->MinTextureMipMap = 0;
    this->MinBumpMapMipMap = 0;
    return this;
}