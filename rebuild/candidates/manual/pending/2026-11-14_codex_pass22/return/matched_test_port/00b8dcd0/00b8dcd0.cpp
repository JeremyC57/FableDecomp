// 00B8DCD0 CEngineOcclusionManager::SelectFlareTestVertexBuffer -- cache and select a circle fan vertex buffer.
// Leaf: ?SelectFlareTestVertexBuffer@CEngineOcclusionManager@@AAEXK@Z
// Flags: none
// Retail-asm-helper: GFFloatToLongNear, existing shared header unchanged.
// Retail-asm: native00B8DE0E..00B8DE12 and00B8DE62..00B8DE66 have bare dword FISTP.
// Both per-site ladders:30 plain nearest-even C++ spellings and10 intrinsic spellings,
// retained in PROBES.md/catalogs/history; no C++ or intrinsic emits these native stores.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_PASS22_FLAREBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_COMPARE
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_BODY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_PASS22_COPY_BACKWARD_FASTCALL
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#define FABLE_RETAIL_CGENERICVERTEX_PASS22_SETU1V1
#include <math.h>
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CTVertexNormTex1.h"
#include "stlport/fable_algorithm.h"
namespace NEngineInternal {extern CShaderRenderManager* GPShaderRenderManager;}
extern float GFastCosTable[1025];
namespace {
struct MapperPred {
    template<class T> bool operator()(const std::pair<unsigned long,T>& value,unsigned long key)const {return value.first<key;}
};
}
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFastSin.h"
void CEngineOcclusionManager::SelectFlareTestVertexBuffer(unsigned long circle_segments)
{
    CCountedPointer<CVertexBufferWin32> vertexbuffer;
    std::vector<std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> > >::iterator found=
        std::lower_bound(FlareVertexBuffers.begin(),FlareVertexBuffers.end(),circle_segments,MapperPred());
    if(found==FlareVertexBuffers.end()||found->first!=circle_segments) {
        vertexbuffer.Reset(new CVertexBufferWin32);
        vertexbuffer->Initialise(VERTEX_NULL,circle_segments+2,sizeof(CTVertexNormTex1),0,VB_POOL_MANAGED);
        CTVertexNormTex1* verts=reinterpret_cast<CTVertexNormTex1*>(vertexbuffer->Lock(0,0,0));
        CTVertexNormTex1& vert0=verts[0];
        vert0.SetPosition(C3DVector(0,0,0));
        float anglefactor=1.0f/(float)circle_segments;
        for(unsigned long c_vert=0;c_vert<circle_segments+1;++c_vert) {
            CTVertexNormTex1& vert=verts[c_vert+1];
            vert.SetPosition(C3DVector(GFFastCos(c_vert*anglefactor),-GFFastSin(c_vert*anglefactor),0));
            vert.SetNormal(C3DVector(0,0,0));
            vert.SetU1V1(0,0);
        }
        vertexbuffer->Unlock();
        FlareVertexBuffers.insert(found,std::make_pair(circle_segments,vertexbuffer));
    } else {
        vertexbuffer=found->second;
    }
    NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*vertexbuffer);
}
