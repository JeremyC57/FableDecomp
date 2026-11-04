// CVertexShaderConstantLayout::~CVertexShaderConstantLayout() -- the vertex shader constant layout's implicit (compiler-generated) dtor: free the reserved-presets array, then each CShaderConstantRange member in reverse order (their implicit dtors are ~CBaseClass), the four texture transforms in a counted loop, and the tail jump to ~CBaseClass
// Leaf: ??1CVertexShaderConstantLayout@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098D710, 270 bytes, tail jump to the base destructor.
// Identity: no vptr store (the class and every CShaderConstantRange member have compiler-generated dtors); the member offsets are
//   exactly Ego_r.pdb CVertexShaderConstantLayout's (Reserved +0x04 .. ScreenSpaceTransformOffset +0x134, TextureTransform[4]
//   +0xA4, LightSize +0x4C, ReservedPresets +0x140: no container precedes them). Both PDBs have
//   `??1CVertexShaderConstantLayout@@UAE@XZ`; callers include the derived CEngineVSConstantLayout* dtors (19 call/jmp sites).
//   The catalog label is right.
// Callees: free 0x00BFEA14 (STLport deallocate of ReservedPresets), CBaseClass::~CBaseClass 0x0099A2E0 (settled `??1CBaseClass@@UAE@XZ`;
//   every CShaderConstantRange member, rtti_bases CShaderConstantRange : CBaseClass, and the class itself).
// Layout: CShaderConstantRange {vptr, Offset, Count} (12 bytes, Ego_r.pdb); CVertexShaderConstant {X, Y, Z, W}. The engine header
//   (engine/CVertexShaderConstantLayout.h) flattens the vptr into a data member, so a local polymorphic model is used.
// Shape: the implicit dtor is emitted through a qualified explicit call in a one-line helper (chunk 562 device); VC7.1 destroys
//   the member array with an inline countdown loop (`mov ebx,4 ; sub edi,0Ch`).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#include "engine/CArray.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CVertexShaderConstant { public:
    float X;                                                // +0x00
    float Y;                                                // +0x04
    float Z;                                                // +0x08
    float W;                                                // +0x0c
};
class CShaderConstantRange : public CBaseClass { public:    // 12 bytes
    long          Offset;                                   // +0x04
    unsigned long Count;                                    // +0x08
};

class CVertexShaderConstantLayout : public CBaseClass { public:
    CShaderConstantRange            Reserved;                           // +0x04
    CShaderConstantRange            User;                               // +0x10
    CShaderConstantRange            Compression;                        // +0x1c
    CShaderConstantRange            Ambient;                            // +0x28
    CShaderConstantRange            FogColour;                          // +0x34
    CShaderConstantRange            CameraPos;                          // +0x40
    unsigned long                   LightSize;                          // +0x4c
    CShaderConstantRange            LightArray;                         // +0x50
    CShaderConstantRange            LightGlobals;                       // +0x5c
    CShaderConstantRange            LightAttenuations;                  // +0x68
    CShaderConstantRange            CombinedProjectionMatrix;           // +0x74
    CShaderConstantRange            FogTransform;                       // +0x80
    CShaderConstantRange            WorldMatrix;                        // +0x8c
    CShaderConstantRange            SphereMapMatrix;                    // +0x98
    CShaderConstantRange            TextureTransform[4];                // +0xa4
    CShaderConstantRange            BoneMatrices;                       // +0xd4
    CShaderConstantRange            ShadowTextureTransform;             // +0xe0
    CShaderConstantRange            ShadowFadeTransform;                // +0xec
    CShaderConstantRange            ShadowFadeFactors;                  // +0xf8
    CShaderConstantRange            ShadowedSpotlightFalloffTransform;  // +0x104
    CShaderConstantRange            ShadowedSpotlightAttenuation;       // +0x110
    CShaderConstantRange            ShadowedSpotlightColour;            // +0x11c
    CShaderConstantRange            ScreenSpaceTransformScale;          // +0x128
    CShaderConstantRange            ScreenSpaceTransformOffset;         // +0x134
    CArray<CVertexShaderConstant>   ReservedPresets;                    // +0x140
};

void EmitImplicitDestructor(CVertexShaderConstantLayout* layout)       // emission device only (not a retail function)
{
    layout->CVertexShaderConstantLayout::~CVertexShaderConstantLayout();
}