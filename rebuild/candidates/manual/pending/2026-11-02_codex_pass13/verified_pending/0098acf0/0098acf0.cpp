// CShaderRenderManager::Initialise(CRenderManager*,IDirect3DDevice9*) -- initialise shader state, resources and pixel constants.
// Leaf: ?Initialise@CShaderRenderManager@@QAEXPAVCRenderManager@@PAUIDirect3DDevice9@@@Z
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include "engine/CShaderRenderManager.h"
#include "engine/CShaderResource.h"
// The native STLport copies are out of line; preserve their original template types and overloads.
template<> void std::vector<const CVertexShaderConstantLayout*>::resize(size_type,const CVertexShaderConstantLayout* const&);
template<> C4DVector* std::vector<C4DVector>::erase(C4DVector*,C4DVector*);

void CShaderRenderManager::Initialise(CRenderManager* render_manager,IDirect3DDevice9* d3ddevice)
{
    D3DDevice=0;
    CurrentVertexShader=0;
    CurrentVertexShaderVertexDeclairation=0;
    SWLightingEnable=false;
    VertexShadersEnabled=false;
    PixelShadersEnabled=false;
    FixedFunctionVertexMode=false;
    SoftwareVertexShaderEnable=false;
    SoftwareFixedFunctionEnable=false;
    VertexShaderConstantLayouts.resize(1,(const CVertexShaderConstantLayout*)0);
    VertexShaderConstantLayouts[0]=&NullConstantLayout;
    CurrentVSConstantLayout=&NullConstantLayout;
    CurrentVSConstantLayoutIndex=-1;
    RenderManager=render_manager;
    D3DDevice=d3ddevice;
    for (unsigned long c_res=0;c_res<ResourceManagers.size();++c_res)
        ResourceManagers[c_res]->Initialise(*this);
    WorldMatrix.SetIdentity();
    WorldMatrixIsIdentity=true;
    ViewMatrix.SetIdentity();
    ProjectionMatrix.SetIdentity();
    MultisampleViewPortScale=1.0f;
    TextureTransformCount=0;
    for (unsigned long c_tt=0;c_tt<1;++c_tt)
        TextureTransforms[c_tt].SetIdentity();
    LightCount=0;
    AllocatedLights=0;
    AllocatedLightSize=0;
    AmbientColour.Initialise(1.0f,1.0f,1.0f,1.0f);
    FogColour.Initialise(0.0f,0.0f,0.0f,0.0f);
    TextureFactor=CRGBColour(255,255,255,255);
    PSConstNames.push_back("PSCONST_INPUT_FACTOR_0");
    PSConstNames.push_back("PSCONST_INPUT_FACTOR_1");
    PSConstNames.push_back("PSCONST_OUTPUT_FACTOR");
    PSConstNames.push_back("PSCONST_MAX_FOG_ALPHA");
    PSConstNames.push_back("PSCONST_SHADOW_FACTOR_AND_FADE_COLOUR");
    PSConstNames.push_back("PSCONST_USER_0");
    PSConstNames.push_back("PSCONST_USER_1");
    PSConstNames.push_back("PSCONST_USER_2");
    PSConstNames.push_back("PSCONST_USER_3");
    PSConstTable.resize(9,C4DVector(0.0f,0.0f,0.0f,0.0f));
    PSConstTable[0]=C4DVector(1.0f,1.0f,1.0f,1.0f);
    PSConstTable[1]=C4DVector(1.0f,1.0f,1.0f,1.0f);
    PSConstTable[2]=C4DVector(1.0f,1.0f,1.0f,1.0f);
    UpdateFlags=0;
}
