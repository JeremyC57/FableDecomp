#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };
struct CLightingSIMDVarBlock {
    float Zero[4]; float One[4];
    float DiffuseVectorX[4]; float DiffuseVectorY[4]; float DiffuseVectorZ[4];
    float DiffuseColourR[4]; float DiffuseColourG[4]; float DiffuseColourB[4];
    float BackColourR[4]; float BackColourG[4]; float BackColourB[4];
    float AmbientColourR[4]; float AmbientColourG[4]; float AmbientColourB[4];
    float ObjectPosX[4]; float ObjectPosY[4]; float ObjectPosZ[4];
};
struct CEngineLightingManager {
    C3DVector DiffuseVector;
    CRGBFloatColour PrimitiveDiffuseColour, PrimitiveAmbientColour, PrimitiveBacklightColour;
    CLightingSIMDVarBlock* SIMDVarBlock;
    void UpdateSIMDVarBlock()
    {
        for (int i = 0; i < 4; ++i)
        {
            SIMDVarBlock->Zero[i] = 0.0f;
            SIMDVarBlock->One[i] = 1.0f;
            SIMDVarBlock->DiffuseVectorX[i] = DiffuseVector.X;
            SIMDVarBlock->DiffuseVectorY[i] = DiffuseVector.Y;
            SIMDVarBlock->DiffuseVectorZ[i] = DiffuseVector.Z;
            SIMDVarBlock->AmbientColourR[i] = PrimitiveAmbientColour.R;
            SIMDVarBlock->AmbientColourG[i] = PrimitiveAmbientColour.G;
            SIMDVarBlock->AmbientColourB[i] = PrimitiveAmbientColour.B;
            SIMDVarBlock->DiffuseColourR[i] = PrimitiveDiffuseColour.R;
            SIMDVarBlock->DiffuseColourG[i] = PrimitiveDiffuseColour.G;
            SIMDVarBlock->DiffuseColourB[i] = PrimitiveDiffuseColour.B;
            SIMDVarBlock->BackColourR[i] = PrimitiveBacklightColour.R;
            SIMDVarBlock->BackColourG[i] = PrimitiveBacklightColour.G;
            SIMDVarBlock->BackColourB[i] = PrimitiveBacklightColour.B;
        }
    }
};
int main()
{
    CLightingSIMDVarBlock b;
    for (int k = 0; k < 4; ++k) { b.Zero[k] = 9.0f; b.ObjectPosX[k] = 7.0f; }
    CEngineLightingManager m;
    m.DiffuseVector.X = 1.0f; m.DiffuseVector.Y = 2.0f; m.DiffuseVector.Z = 3.0f;
    m.PrimitiveDiffuseColour.R = 4.0f; m.PrimitiveDiffuseColour.G = 5.0f; m.PrimitiveDiffuseColour.B = 6.0f;
    m.PrimitiveAmbientColour.R = 7.0f; m.PrimitiveAmbientColour.G = 8.0f; m.PrimitiveAmbientColour.B = 9.0f;
    m.PrimitiveBacklightColour.R = 10.0f; m.PrimitiveBacklightColour.G = 11.0f; m.PrimitiveBacklightColour.B = 12.0f;
    m.SIMDVarBlock = &b;
    m.UpdateSIMDVarBlock();
    for (int i = 0; i < 4; ++i)
    {
        if (b.Zero[i] != 0.0f || b.One[i] != 1.0f || b.DiffuseVectorY[i] != 2.0f || b.DiffuseColourB[i] != 6.0f
            || b.AmbientColourR[i] != 7.0f || b.BackColourG[i] != 11.0f || b.ObjectPosX[i] != 7.0f)
        { std::printf("00b44540_TEST FAIL\n"); return 1; }
    }
    std::printf("00b44540_TEST PASS\n");
    return 0;
}
