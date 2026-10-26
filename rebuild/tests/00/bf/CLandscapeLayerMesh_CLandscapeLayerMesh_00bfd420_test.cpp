#include <cstdio>
#include <cstring>

class CLandscapeLayerMesh { public:
    void* vtbl;
    void* pParam;
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

int main() {
    CLandscapeLayerMesh obj;
    std::memset(&obj, 0xAB, sizeof(obj));
    int dummy = 12345;
    CLandscapeLayerMesh* r = CLandscapeLayerMesh_CLandscapeLayerMesh(&obj, 0, &dummy);
    if (r != &obj) { std::printf("FAIL return\n"); return 1; }
    if (obj.vtbl != (void*)0x12a8334) { std::printf("FAIL vtbl\n"); return 1; }
    if (obj.pParam != &dummy) { std::printf("FAIL pParam\n"); return 1; }
    if (obj.f44 != 1) { std::printf("FAIL f44\n"); return 1; }
    if (obj.BackgroundTextureIndex || obj.BumpMapTextureIndex || obj.f10 || obj.f5c || obj.f30 || obj.f54) { std::printf("FAIL zero\n"); return 1; }
    std::printf("CLandscapeLayerMesh_00bfd420_TEST PASS\n");
    return 0;
}