// CEngineLightingManager::UpdateSIMDVarBlock() -- splats the global lighting state (zero, one, the diffuse light vector and the primitive ambient / diffuse / backlight colours) into all four lanes of the SIMD var block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44540, length 731 bytes (736 with padding). No callees.
// Shape: four lanes written in turn (lane 0 at +0x0 of each float[4], then lane 1 ...); in each lane the order is Zero,
// One, DiffuseVectorX/Y/Z, AmbientColourRGB (+0xB0), DiffuseColourRGB (+0x50), BackColourRGB (+0x80). The block pointer
// is re-read for every store (the stores may alias the manager), 0.0f and 1.0f are CSE'd into edx / eax.
// Layout: PDB CEngineLightingManager has SIMDVarBlock at +0x469C and the primitive colours at +0x4C; the retail
// ThemeChannels CArray is 12 bytes (not 16), so everything after +0x3C moves down 4 (colours at +0x48 / +0x58 / +0x68)
// and the block pointer sits at +0x4690 -- modelled locally with the PDB member names. CLightingSIMDVarBlock is the PDB's
// 0x110-byte record of float[4] lanes.
class C3DVector { public: float X, Y, Z; };
class CRGBFloatColour { public: float R, G, B, A; };

struct CLightingSIMDVarBlock {                     // 0x110 bytes (PDB)
    float Zero[4];                                 // +0x00
    float One[4];                                  // +0x10
    float DiffuseVectorX[4];                       // +0x20
    float DiffuseVectorY[4];                       // +0x30
    float DiffuseVectorZ[4];                       // +0x40
    float DiffuseColourR[4];                       // +0x50
    float DiffuseColourG[4];                       // +0x60
    float DiffuseColourB[4];                       // +0x70
    float BackColourR[4];                          // +0x80
    float BackColourG[4];                          // +0x90
    float BackColourB[4];                          // +0xA0
    float AmbientColourR[4];                       // +0xB0
    float AmbientColourG[4];                       // +0xC0
    float AmbientColourB[4];                       // +0xD0
    float ObjectPosX[4];                           // +0xE0
    float ObjectPosY[4];                           // +0xF0
    float ObjectPosZ[4];                           // +0x100
};

class CEngineLightingManager { public:
    unsigned char          _pad0000[0x30];
    C3DVector              DiffuseVector;                  // +0x30
    unsigned char          ThemeChannels[0xc];             // +0x3C (retail 12-byte CArray)
    CRGBFloatColour        PrimitiveDiffuseColour;         // +0x48
    CRGBFloatColour        PrimitiveAmbientColour;         // +0x58
    CRGBFloatColour        PrimitiveBacklightColour;       // +0x68
    unsigned char          _pad0078[0x4690 - 0x78];
    CLightingSIMDVarBlock* SIMDVarBlock;                   // +0x4690
    void UpdateSIMDVarBlock();
};

void CEngineLightingManager::UpdateSIMDVarBlock()
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