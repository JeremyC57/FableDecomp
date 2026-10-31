// Behaviour test for CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc 0x00BF9FA0 (model copy, no virtuals).
#include <cstdio>
class C3DMeshInfo { public: int dummy; };
struct CObjectEntry { int dummy; };
struct Box { float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; };
struct Sphere { float X, Y, Z, Radius; };
struct Fade { float FadeBeginDistance, MaxDrawDistance; Fade(float b, float m) : FadeBeginDistance(b), MaxDrawDistance(m) { } };
struct CollectionType {
    C3DMeshInfo* ZSpriteMesh; float FadeStart, FadeEnd; long AlphaRef; float ZSpriteFadeStart, ZSpriteFadeEnd;
    float PeekFadeStart() const { return FadeStart; }
    float PeekFadeEnd() const { return FadeEnd; }
    long PeekAlphaRef() const { return AlphaRef; }
    const C3DMeshInfo* PeekZSpriteMesh() const { return ZSpriteMesh; }
    float PeekZSpriteFadeStart() const { return ZSpriteFadeStart; }
    float PeekZSpriteFadeEnd() const { return ZSpriteFadeEnd; }
};
struct Desc {
    Sphere BoundingSphere; Box BoundingBox; float MaxScale; const C3DMeshInfo* Mesh; Fade FadeDist;
    float FadeInStart, FadeInEnd; CObjectEntry* ObjectEnts; long ObjectCount, AlphaRef; bool Persistent;
    Desc(const Sphere& s, const Box& b, const C3DMeshInfo* m, float sc, Fade f, float fis, float fie, CObjectEntry* e, long n, long a, bool p)
        : BoundingSphere(s), BoundingBox(b), MaxScale(sc), Mesh(m), FadeDist(f), FadeInStart(fis), FadeInEnd(fie), ObjectEnts(e),
          ObjectCount(n), AlphaRef(a), Persistent(p) { }
};
struct CObject { float Matrix[12]; float Scale; };
struct Batch {
    Box BoundingBox; Sphere BoundingSphere; CollectionType* Type; CObject* Objects; CObjectEntry* ZSpriteEntries; long ObjectCount;
    Desc SetupPrimitiveDesc()
    {
        Fade fade(Type->PeekFadeStart(), Type->PeekFadeEnd());
        float maxScale = 0.0f;
        for (long i = 0; i < ObjectCount; ++i)
            maxScale = maxScale > Objects[i].Scale ? maxScale : Objects[i].Scale;
        return Desc(BoundingSphere, BoundingBox, Type->PeekZSpriteMesh(), maxScale, fade, Type->PeekZSpriteFadeStart(),
                    Type->PeekZSpriteFadeEnd(), ZSpriteEntries, ObjectCount, Type->PeekAlphaRef(), true);
    }
};
int main()
{
    C3DMeshInfo mesh; CObjectEntry entries[7];
    CollectionType t; t.ZSpriteMesh = &mesh; t.FadeStart = 10.0f; t.FadeEnd = 20.0f; t.AlphaRef = 77; t.ZSpriteFadeStart = 1.5f; t.ZSpriteFadeEnd = 2.5f;
    CObject objs[7]; float sc[7] = { 0.5f, 3.0f, 1.0f, 2.0f, 6.25f, 0.25f, 4.0f };
    for (int i = 0; i < 7; ++i) objs[i].Scale = sc[i];
    Batch b; Box bx = { 1, 2, 3, 4, 5, 6 }; Sphere sp = { 7, 8, 9, 10 };
    b.BoundingBox = bx; b.BoundingSphere = sp; b.Type = &t; b.Objects = objs; b.ZSpriteEntries = entries; b.ObjectCount = 7;
    Desc d = b.SetupPrimitiveDesc();
    bool ok = d.MaxScale == 6.25f && d.Mesh == &mesh && d.FadeDist.FadeBeginDistance == 10.0f && d.FadeDist.MaxDrawDistance == 20.0f
        && d.FadeInStart == 1.5f && d.FadeInEnd == 2.5f && d.ObjectEnts == entries && d.ObjectCount == 7 && d.AlphaRef == 77
        && d.Persistent && d.BoundingBox.MaxZ == 6 && d.BoundingSphere.Radius == 10;
    b.ObjectCount = 0;
    Desc e = b.SetupPrimitiveDesc();
    if (e.MaxScale != 0.0f || e.ObjectCount != 0) ok = false;
    if (!ok) { std::printf("00bf9fa0_TEST FAIL\n"); return 1; }
    std::printf("00bf9fa0_TEST PASS\n");
    return 0;
}