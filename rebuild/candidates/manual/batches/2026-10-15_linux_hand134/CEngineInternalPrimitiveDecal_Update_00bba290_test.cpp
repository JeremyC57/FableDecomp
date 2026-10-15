#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
struct CDesc {
    bool LandscapeOnlyDecal; long DecalIndex; long DecalFrame; C3DVector WorldPos; C2DVector Size; CRGBColour Colour;
    CRightHandedSet RHSet; long BlendMode; long BlendOp; float StencilCubeHeight;
    unsigned short FlagBits0 : 7; unsigned short Lit : 1;
};
struct CDecal {
    unsigned int EnginePrimitiveFlags; bool LandscapeOnlyDecal; long DecalIndex; long DecalFrame; C3DVector WorldPos; C2DVector Size;
    CRGBColour Colour; CRightHandedSet RHSet; long BlendMode; long BlendOp; bool Lit; float StencilCubeHeight;
    void Update(const CDesc& decal, unsigned int flags)
    {
        EnginePrimitiveFlags = flags; LandscapeOnlyDecal = decal.LandscapeOnlyDecal; DecalIndex = decal.DecalIndex; DecalFrame = decal.DecalFrame;
        WorldPos = decal.WorldPos; Size = decal.Size; Colour = decal.Colour; RHSet = decal.RHSet;
        BlendMode = decal.BlendMode; BlendOp = decal.BlendOp; StencilCubeHeight = decal.StencilCubeHeight; Lit = decal.Lit;
    }
};
int main()
{
    CDesc d; d.LandscapeOnlyDecal = true; d.DecalIndex = 7; d.DecalFrame = 3; d.WorldPos.X = 1.0f; d.WorldPos.Y = 2.0f; d.WorldPos.Z = 3.0f;
    d.Size.X = 4.0f; d.Size.Y = 5.0f; d.Colour.R = 200; d.RHSet.Up.Z = 1.0f; d.BlendMode = 2; d.BlendOp = 1; d.StencilCubeHeight = 9.0f;
    d.FlagBits0 = 0x7f; d.Lit = 1;
    CDecal t; t.Lit = false;
    t.Update(d, 0x40);
    if (t.EnginePrimitiveFlags != 0x40 || !t.LandscapeOnlyDecal || t.DecalIndex != 7 || t.DecalFrame != 3 || t.WorldPos.Z != 3.0f || t.Size.Y != 5.0f
        || t.Colour.R != 200 || t.RHSet.Up.Z != 1.0f || t.BlendMode != 2 || t.BlendOp != 1 || t.StencilCubeHeight != 9.0f || !t.Lit) { std::printf("00bba290_TEST FAIL\n"); return 1; }
    d.Lit = 0; t.Update(d, 0);
    if (t.Lit) { std::printf("00bba290_TEST FAIL\n"); return 1; }
    std::printf("00bba290_TEST PASS\n");
    return 0;
}
