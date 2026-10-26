class CLandscapeLayerMesh { public:
    void* vtbl;      // +0x00
    void* pParam;    // +0x04
    int BackgroundTextureIndex; int BumpMapTextureIndex; int f10; int ForegroundTexture; int BackgroundTexture; int BumpMap;
    int MinTextureMipMap; int MinBumpMapMipMap; int f28; int ForegroundTextureMipmapLevel;
    short f30; short f32;
    char pad34[0x40-0x34];
    int Map;
    char f44; char pad45[3];
    int f48; int ProceduralTextureVertexBuffer; int Vertices;
    short f54; short f56;
    int f58; int f5c;
};

CLandscapeLayerMesh* __fastcall CLandscapeLayerMesh_CLandscapeLayerMesh(CLandscapeLayerMesh* self, void* edx, void* param) {
    self->pParam = param;
    int z = 0;
    self->vtbl = (void*)0x12a8334;
    self->BackgroundTextureIndex = z;
    self->BumpMapTextureIndex = z;
    self->f30 = (short)z;
    self->f32 = (short)z;
    self->Map = z;
    self->f44 = 1;
    self->f48 = z;
    self->ProceduralTextureVertexBuffer = z;
    self->Vertices = z;
    self->f54 = (short)z;
    self->f56 = (short)z;
    self->f58 = z;
    self->f5c = z;
    self->f10 = z;
    self->MinTextureMipMap = z;
    self->ForegroundTexture = z;
    self->MinBumpMapMipMap = z;
    self->BackgroundTexture = z;
    self->f28 = z;
    self->BumpMap = z;
    self->ForegroundTextureMipmapLevel = z;
    return self;
}