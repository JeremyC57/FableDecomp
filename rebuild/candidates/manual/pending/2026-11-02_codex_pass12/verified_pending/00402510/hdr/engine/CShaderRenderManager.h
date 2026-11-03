#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CSHADERRENDERMANAGER
#define FABLE_ENGINE_GFMAIN_CSHADERRENDERMANAGER
// Retail opt-in (Codex pass12): original complete type; native ctor0098ab00 and adjacent singleton globals prove size3f0.
#include "engine/CBaseClassNonCopyable.h"
class CVertexShaderConstantLayout;class C4DVector;class CRenderManager;
struct IDirect3DDevice9;struct IDirect3DVertexShader9;struct IDirect3DVertexDeclaration9;
class CShaderRenderManager:public CBaseClassNonCopyable{
 class CLightInfo;
 const CVertexShaderConstantLayout*CurrentVSConstantLayout;long CurrentVSConstantLayoutIndex;
 unsigned char VertexShaderConstantLayouts[12]; // CArray<const CVertexShaderConstantLayout*> +c.
 unsigned char NullConstantLayout[0x14c]; // CVertexShaderConstantLayout +18.
 CLightInfo*Lights;C4DVector*LightBuffer;unsigned long LightCount,AllocatedLights,AllocatedLightSize;
 unsigned char LightPool[0x1c]; // CSimpleMemoryPool +178.
 unsigned char LightGlobals[12],LightAttenuations[12]; // CArray<C4DVector> +194/+1a0.
 unsigned char AmbientColour[16],FogColour[16],TextureFactor[4]; // CRGBFloatColour,CRGBColour +1ac/+1bc/+1cc.
 CRenderManager*RenderManager;IDirect3DDevice9*D3DDevice;IDirect3DVertexShader9*CurrentVertexShader;
 IDirect3DVertexDeclaration9*CurrentVertexShaderVertexDeclairation;
 unsigned char CurrentPixelShader[8]; // CPixelShader +1e0.
 bool WorldMatrixIsIdentity;unsigned char Body_1e9[7]; // Original matrix alignment.
 unsigned char WorldMatrix[64],ViewMatrix[64],ProjectionMatrix[64],ViewProjectionMatrix[64],CombinedProjectionMatrix[64],TextureTransforms[64]; // Original CMatrix4x4 values +1f0..330.
 unsigned char FogTransform[16],CameraPos[12]; // C4DVector +370, C3DVector +380.
 long TextureTransformCount;bool VertexShadersEnabled,PixelShadersEnabled,SWLightingEnable,SoftwareVertexShaderEnable,SoftwareFixedFunctionEnable,FixedFunctionVertexMode;
 float MultisampleViewPortScale;void*CycleTextures; // CArray<CTexture>* +39c.
 unsigned long NextCycleTexture,CycleTextureStage;
 unsigned char ResourceManagers[12],PSConstNames[24],PSConstTable[12]; // CArray<CCountedPointer<CShaderResource>>,CIndexedArray<CCharString>,CArray<C4DVector>.
 unsigned long UpdateFlags;unsigned char BoneMatrices[12]; // CArray<CPreTransposedBoneMatrix> +3dc.
 unsigned char Body_3e8[8]; // Original complete-object alignment, next singleton at+3f0.
 static CShaderRenderManager SingletonInstance;
public:virtual~CShaderRenderManager();static CShaderRenderManager&__fastcall Get(){return SingletonInstance;}
};
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager)==0x3f0);
#endif

#else

#endif
