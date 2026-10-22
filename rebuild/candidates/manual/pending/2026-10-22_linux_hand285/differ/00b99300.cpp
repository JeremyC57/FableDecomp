// Grade: DIFFER(855v871) -- register/schedule only: retail loads the bool and count args at entry (dl/ebp), keeps the two SetVSConstants tails apart (ours cross-jumps them, -16 bytes) and orders the position/normal temporaries the other way in the frame
// CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW(prim, x, y, z, normals, count, separateMainLight) -- software-light a batch of repeated-mesh instances into the SIMD variable block: the SSE vector lighting path when enabled (count rounded up to 4), else one CalcSWLighting* call per instance; with a separate main light the main-light colours may be forced to (1,0,0,0) (debug) and are uploaded to the main-light register, otherwise the combined colours go to the colour register
// Leaf: ProcessLightingSW
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B99300, `ret 1Ch`.
// Callees: CEngineLightingManager::CalcSWLightingNoClipSeparateMainLightVector 0x00B44A60, ::CalcSWLightingNoClipSeparateMainLight
//   0x00B45A40, ::CalcSWLightingNoClipVector 0x00B44820, ::CalcSWLightingNoClip 0x00B45CF0, CShaderRenderManager::SetVSConstants 0x0098B930.
// Globals: SIMD enable byte 0x013D2880, debug count override 0x0143B958, debug main-light override 0x0143B954, lighting manager
//   0x01436E9C, shader render manager 0x01436E14, engine 0x01436E98 (VS constant registers at +0x3018 colour / +0x3024 main light).
// Layout: engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h (SIMDVariableBlock +0x90); engine/CEngineInternalPrimitiveRepeatedStaticMesh.h
//   (RenderInfo +0xa4, whose first word is the CEngineLocalLightArray*); CSIMDVariableBlock from Ego_r.pdb (Colour +0x700, MainLightColour +0x900).
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C4DVector { float X, Y, Z, W; };
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() {}
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
    void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
};
struct CEngineLocalLightArray;

struct CSIMDVariableBlock {
    unsigned char   _pad000[0x700];
    CRGBFloatColour Colour[32];                    // +0x700
    CRGBFloatColour MainLightColour[32];           // +0x900
};

struct CEngineLightingManagerCalls {
    void CalcSWLightingNoClipSeparateMainLightVector(CRGBFloatColour* colour, CRGBFloatColour* mainLight, C4DVector* normals,
        float* x, float* y, float* z, long count, CEngineLocalLightArray* lights);                                   // 0x00B44A60
    void CalcSWLightingNoClipVector(CRGBFloatColour* colour, C4DVector* normals, float* x, float* y, float* z, long count,
        CEngineLocalLightArray* lights);                                                                              // 0x00B44820
    void CalcSWLightingNoClipSeparateMainLight(const C3DVector& position, const C3DVector& normal, const CEngineLocalLightArray& lights,
        CRGBFloatColour& mainLight, CRGBFloatColour& colour);                                                         // 0x00B45A40
    void CalcSWLightingNoClip(const C3DVector& position, const C3DVector& normal, const CEngineLocalLightArray& lights,
        CRGBFloatColour& colour);                                                                                     // 0x00B45CF0
};
struct CShaderRenderManagerCalls {
    void SetVSConstants(long reg, void* data, unsigned long count);                                                   // 0x0098B930
};
struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x3018];
    long          ColourRegister;                  // +0x3018
    unsigned char _pad301c[8];
    long          MainLightRegister;               // +0x3024
};
extern unsigned char g_UseSIMDLighting_013d2880;                   // 0x013D2880
extern long g_DebugLightCountOverride_0143b958;                    // 0x0143B958
extern long g_DebugMainLightOverride_0143b954;                     // 0x0143B954
extern CEngineLightingManagerCalls* g_pEngineLightingManager_01436e9c;   // 0x01436E9C
extern CShaderRenderManagerCalls* g_pShaderRenderManager;          // 0x01436E14
extern CEngineGlobal_01436e98* g_pEngine_01436e98;                 // 0x01436E98

static __forceinline CEngineLocalLightArray* Lights(CEngineInternalPrimitiveRepeatedStaticMesh* prim)
{
    return *(CEngineLocalLightArray**)prim->RenderInfo;
}

void __fastcall ProcessLightingSW(CEnginePrimitiveManagerRepeatedStaticMeshes* self, int /*edx*/, CEngineInternalPrimitiveRepeatedStaticMesh* prim,
                                  float* x, float* y, float* z, C4DVector* normals, long count, bool separateMainLight)
{
    long simdCount = (count + 3) & ~3;
    if (separateMainLight) {
        if (g_UseSIMDLighting_013d2880) {
            g_pEngineLightingManager_01436e9c->CalcSWLightingNoClipSeparateMainLightVector(self->SIMDVariableBlock->Colour,
                self->SIMDVariableBlock->MainLightColour, normals, x, y, z, simdCount, Lights(prim));
        } else {
            for (long i = 0; i < count; ++i) {
                g_pEngineLightingManager_01436e9c->CalcSWLightingNoClipSeparateMainLight(C3DVector(x[i], y[i], z[i]), C3DVector(normals[i].X, normals[i].Y, normals[i].Z), *Lights(prim),
                    self->SIMDVariableBlock->MainLightColour[i], self->SIMDVariableBlock->Colour[i]);
            }
        }
        if (g_DebugLightCountOverride_0143b958)
            count = 16;
        if (g_DebugMainLightOverride_0143b954) {
            for (long i = 0; i < count; ++i)
                self->SIMDVariableBlock->MainLightColour[i].Set(1.0f, 0.0f, 0.0f, 0.0f);
        }
        g_pShaderRenderManager->SetVSConstants(g_pEngine_01436e98->MainLightRegister, self->SIMDVariableBlock->MainLightColour, count);
    } else {
        if (g_UseSIMDLighting_013d2880) {
            g_pEngineLightingManager_01436e9c->CalcSWLightingNoClipVector(self->SIMDVariableBlock->Colour, normals, x, y, z, simdCount, Lights(prim));
        } else {
            for (long i = 0; i < count; ++i) {
                g_pEngineLightingManager_01436e9c->CalcSWLightingNoClip(C3DVector(x[i], y[i], z[i]), C3DVector(normals[i].X, normals[i].Y, normals[i].Z), *Lights(prim), self->SIMDVariableBlock->Colour[i]);
            }
        }
        g_pShaderRenderManager->SetVSConstants(g_pEngine_01436e98->ColourRegister, self->SIMDVariableBlock->Colour, count);
    }
}
