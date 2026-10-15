// Behaviour test for 0x00BAA780 (see src/00baa780.cpp): UpdateRepeatedZSprite copying the description into the internal primitive.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00baa780_TEST FAIL\n"); return 1; } } while (0)

struct C3DVector { float X, Y, Z; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct C3DBoundingBox { C3DVector Min; C3DVector Max; };
struct CEngineFadeDistance { float Near; float Far; };
struct CAddPrimitiveDesc {
    C3DBoundingSphere BoundingSphere; C3DBoundingBox BoundingBox; float MaxScale; void* Mesh; CEngineFadeDistance FadeDist;
    float FadeInStart; float FadeInEnd; void* ObjectEnts; int ObjectCount; int AlphaRef; bool Persistent;
};
struct CSprite {
    unsigned int EnginePrimitiveFlags; CEngineFadeDistance FadeDistance; unsigned char AlphaRef; void* Mesh; float FadeInStart; float FadeInEnd;
    void* ObjectsEntries; int ObjectCount; C3DBoundingSphere BoundingSphere; C3DBoundingBox BoundingBox; float MinDistance; float MaxScale;
};
struct CEnginePrimitiveHandle { void* vptr; CSprite* Data; };
static void Update(const CEnginePrimitiveHandle& handle, const CAddPrimitiveDesc& desc)
{
    CSprite* s = handle.Data;
    s->ObjectsEntries = desc.ObjectEnts; s->ObjectCount = desc.ObjectCount; s->Mesh = desc.Mesh; s->FadeDistance = desc.FadeDist;
    s->AlphaRef = (unsigned char)desc.AlphaRef; s->FadeInStart = desc.FadeInStart; s->FadeInEnd = desc.FadeInEnd;
    s->EnginePrimitiveFlags = 0x300; s->BoundingSphere = desc.BoundingSphere; s->BoundingBox = desc.BoundingBox; s->MaxScale = desc.MaxScale;
}
int main()
{
    int objects[4]; int mesh;
    CAddPrimitiveDesc d;
    std::memset(&d, 0, sizeof(d));
    d.BoundingSphere.Center.X = 1; d.BoundingSphere.Radius = 2; d.BoundingBox.Max.Z = 3; d.MaxScale = 4; d.Mesh = &mesh;
    d.FadeDist.Near = 5; d.FadeDist.Far = 6; d.FadeInStart = 7; d.FadeInEnd = 8; d.ObjectEnts = objects; d.ObjectCount = 4; d.AlphaRef = 0x180;
    CSprite s; std::memset(&s, 0, sizeof(s)); s.MinDistance = 9;
    CEnginePrimitiveHandle h = { 0, &s };
    Update(h, d);
    CHECK(s.ObjectsEntries == objects && s.ObjectCount == 4 && s.Mesh == &mesh && s.FadeDistance.Near == 5 && s.FadeDistance.Far == 6);
    CHECK(s.AlphaRef == 0x80 && s.FadeInStart == 7 && s.FadeInEnd == 8 && s.EnginePrimitiveFlags == 0x300);
    CHECK(s.BoundingSphere.Center.X == 1 && s.BoundingSphere.Radius == 2 && s.BoundingBox.Max.Z == 3 && s.MaxScale == 4 && s.MinDistance == 9);
    std::printf("00baa780_TEST PASS\n");
    return 0;
}