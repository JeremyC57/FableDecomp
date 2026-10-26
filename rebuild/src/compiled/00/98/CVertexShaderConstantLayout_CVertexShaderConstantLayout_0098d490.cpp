// CVertexShaderConstantLayout::CVertexShaderConstantLayout() -- default-construct the vertex-shader constant layout: every CShaderConstantRange slot (incl. the four texture transforms) starts empty, LightSize 0 and the reserved-preset array empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098D490, length 608 bytes (`ret`, returns this).
// Callees: CBase::CBase 0x0099A2D0 (out of line, once for the layout's own base and once per range).
// Vtables: CVertexShaderConstantLayout 0x0129A06C, CShaderConstantRange 0x0129A04C.
// Layout: rebuild/include/engine/CVertexShaderConstantLayout.h flattens every CShaderConstantRange to `unsigned char[0xc]`,
//   which cannot run the member constructors, so this source keeps a local model with the same offsets (Ego_r.pdb):
//   CShaderConstantRange = CBase vptr + Offset + Count (12 bytes); LightSize at +0x4c; TextureTransform[4] at +0xa4;
//   ReservedPresets (CArray<CVertexShaderConstant>) at +0x140, whose inline ctor zeroes its three leading words.
// Shape: the TextureTransform[4] member array is the inline `mov ebp,4 ; ... dec ebp ; jne` construction loop.
namespace N_0099a2d0 { struct CBase {
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
}; } using N_0099a2d0::CBase;

class CShaderConstantRange : public CBase { public:
    long Offset;                                   // +0x04
    unsigned long Count;                           // +0x08
    CShaderConstantRange() : Offset(0), Count(0) {}
    virtual ~CShaderConstantRange();
};

class CVertexShaderConstant;
struct CVertexShaderConstantArray {
    CVertexShaderConstant* Data;                   // +0x00
    long Count;                                    // +0x04
    long Allocated;                                // +0x08
    CVertexShaderConstantArray() : Data(0), Count(0), Allocated(0) {}
};

class CVertexShaderConstantLayout : public CBase { public:
    CShaderConstantRange Reserved;                 // +0x04
    CShaderConstantRange User;                     // +0x10
    CShaderConstantRange Compression;              // +0x1c
    CShaderConstantRange Ambient;                  // +0x28
    CShaderConstantRange FogColour;                // +0x34
    CShaderConstantRange CameraPos;                // +0x40
    unsigned long LightSize;                       // +0x4c
    CShaderConstantRange LightArray;               // +0x50
    CShaderConstantRange LightGlobals;             // +0x5c
    CShaderConstantRange LightAttenuations;        // +0x68
    CShaderConstantRange CombinedProjectionMatrix; // +0x74
    CShaderConstantRange FogTransform;             // +0x80
    CShaderConstantRange WorldMatrix;              // +0x8c
    CShaderConstantRange SphereMapMatrix;          // +0x98
    CShaderConstantRange TextureTransform[4];      // +0xa4
    CShaderConstantRange BoneMatrices;             // +0xd4
    CShaderConstantRange ShadowTextureTransform;   // +0xe0
    CShaderConstantRange ShadowFadeTransform;      // +0xec
    CShaderConstantRange ShadowFadeFactors;        // +0xf8
    CShaderConstantRange ShadowedSpotlightFalloffTransform; // +0x104
    CShaderConstantRange ShadowedSpotlightAttenuation;      // +0x110
    CShaderConstantRange ShadowedSpotlightColour;           // +0x11c
    CShaderConstantRange ScreenSpaceTransformScale;         // +0x128
    CShaderConstantRange ScreenSpaceTransformOffset;        // +0x134
    CVertexShaderConstantArray ReservedPresets;             // +0x140
    CVertexShaderConstantLayout();
    virtual ~CVertexShaderConstantLayout();
};

CVertexShaderConstantLayout::CVertexShaderConstantLayout()
    : LightSize(0)
{
}