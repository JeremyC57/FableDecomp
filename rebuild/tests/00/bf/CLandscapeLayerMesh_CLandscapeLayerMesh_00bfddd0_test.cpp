#include <cstdio>

class CLandscapeLayerMesh { public:
    void* vtbl;
    int ForegroundTextureIndex, BackgroundTextureIndex, BumpMapTextureIndex, f10, ForegroundTexture, BackgroundTexture, BumpMap, MinTextureMipMap, MinBumpMapMipMap, f28, ForegroundTextureMipmapLevel, ForegroundBumpmapTextureMipmapLevel, IndexBuffer, ForegroundVertexBuffer, Next, Map;
    short VertexCount, PolyCountIncDegenerates;
    char UsesSharedIndexBuffer;
    CLandscapeLayerMesh* CLandscapeLayerMesh_ctor(int arg);
};

void* CLandscapeLayerMesh_vtbl;

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

int main() {
    CLandscapeLayerMesh obj;
    for (char* p = (char*)&obj; p < (char*)&obj + sizeof(obj); ++p) *p = (char)0xAB;
    CLandscapeLayerMesh* r = obj.CLandscapeLayerMesh_ctor(0x12345678);
    if (r != &obj) { std::printf("FAIL: return\n"); return 1; }
    if (obj.Map != 0x12345678) { std::printf("FAIL: arg\n"); return 1; }
    if (obj.vtbl != &CLandscapeLayerMesh_vtbl) { std::printf("FAIL: vtbl\n"); return 1; }
    if (obj.Next != 0 || obj.VertexCount != 0 || obj.PolyCountIncDegenerates != 0 || obj.ForegroundTextureIndex != 0 ||
        obj.BackgroundTextureIndex != 0 || obj.BumpMapTextureIndex != 0 || obj.ForegroundTexture != 0 || obj.BackgroundTexture != 0 ||
        obj.BumpMap != 0 || obj.f28 != 0 || obj.IndexBuffer != 0 || obj.ForegroundVertexBuffer != 0 ||
        obj.UsesSharedIndexBuffer != 0 || obj.ForegroundBumpmapTextureMipmapLevel != 0 || obj.ForegroundTextureMipmapLevel != 0 || obj.MinTextureMipMap != 0 ||
        obj.MinBumpMapMipMap != 0) { std::printf("FAIL: zero fields\n"); return 1; }
    std::printf("CLandscapeLayerMesh_00bfddd0_TEST PASS\n");
    return 0;
}