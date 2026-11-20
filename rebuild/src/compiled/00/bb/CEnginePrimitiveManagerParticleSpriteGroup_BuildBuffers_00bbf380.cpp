// CEnginePrimitiveManagerParticleSpriteGroup::BuildBuffers -- fill40 shader-indexed quad vertices and60 triangle indices.
// Retail00BBF380,279bytes. Donor sprite manager arrays16bytes become native12,
// placing VertexBuffer+60/IndexBuffer+94 rather than donor6c/a0. Actual headers
// retained under aliases; the native data views preserve named donor members.
// Resource getters B3B030/B3B060/B3B0A0 have no stack args: pushes1,101,4 belong
// to subsequent buffer Initialise calls. VBInitialiseA63150 takes5args; IBAC1F70 takes5.
// VB LockAC20A0/UnlockAC20B0 are external nonvirtual wrappers dispatching slots1/2.
#define CEnginePrimitiveManagerParticleSpriteGroup CEnginePrimitiveManagerParticleSpriteGroup_PDB
#include "engine/CEnginePrimitiveManagerParticleSpriteGroup.h"
#undef CEnginePrimitiveManagerParticleSpriteGroup
// NF149 (q4552 / q4555..q4560): the buffers are the shared retail classes (engine/CVertexBufferWin32.h, engine/CIndexBuffer.h; Ego_r.pdb +
// FableWin.pdb CVertexBufferWin32::Initialise 0x00A63150, CVertexBufferBase::Lock / Unlock 0x00AC20A0 / 0x00AC20B0 and GetSize (inline:
// Length +0x08), CIndexBuffer::Initialise / GetSize / Lock / Unlock 0x00AC1F70 / 0x00AC1F10 / 0x00AC1FB0 / 0x00AC1F00) and the shared
// buffer enums; were the stand-ins CVertexBufferWin32_Retail / CIndexBuffer_Methods, a CIndexBuffer model under a renaming macro, and
// local enums.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on the retail branch of engine/CEngineResourceManager.h, which declares
// the three out-of-line buffer getters (GetVertexBufferUsage 0x00B3B030, GetIndexBufferUsage 0x00B3B060, GetIndexBufferMemPool 0x00B3B0A0);
// was a CEngineResourceManager_Methods / g_ResourceManager stand-in derived from the flattened default.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#include "engine/CEngineResourceManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
struct ShaderArray {void* First;void* Last;void* End;};
struct ShaderHandle {void* NativeVTable;void* Data;};
class CEnginePrimitiveManagerParticleSpriteGroup { public:
 void* NativeVTable;void* Scenes;bool Enable;unsigned char Alignment[3];
 ShaderArray VertexShadersBillboardSpritePointLights,VertexShadersCrossedSpritePointLights,VertexShaders3DFaceMeSpritePointLights;
 ShaderHandle VertexShadersBillboardSpriteDisplacement,VertexShadersCrossedSpriteDisplacement,VertexShaders3DFaceMeSpriteDisplacement;
 ShaderHandle PixelShader,PixelShaderStipple,PixelShaderDisplacement;
 CVertexBufferWin32 VertexBuffer;CIndexBuffer IndexBuffer;
 float ConstantBlock[40][4];
 void BuildBuffers();
};
struct SIndexedParticleVertex {float Index;unsigned short U,V;};
void CEnginePrimitiveManagerParticleSpriteGroup::BuildBuffers(){
 VertexBuffer.Initialise(VERTEX_NULL,40,8,NEngineInternal::GPResourceManager->GetVertexBufferUsage(),VB_POOL_MANAGED);
 SIndexedParticleVertex* vertex=(SIndexedParticleVertex*)VertexBuffer.Lock(0,0,VertexBuffer.GetSize());
 int shaderIndex=4;
 for(int i=10;i;--i){
  vertex->Index=(float)shaderIndex;vertex->U=0;vertex->V=0;++vertex;
  vertex->Index=(float)shaderIndex;vertex->U=1;vertex->V=0;++vertex;
  vertex->Index=(float)shaderIndex;vertex->U=1;vertex->V=1;++vertex;
  vertex->Index=(float)shaderIndex;vertex->U=0;vertex->V=1;++vertex;
  shaderIndex+=4;
 }
 VertexBuffer.Unlock();
 IndexBuffer.Initialise(120,NEngineInternal::GPResourceManager->GetIndexBufferUsage(),IB_FORMAT_16,NEngineInternal::GPResourceManager->GetIndexBufferMemPool(),PT_TRIANGLE_LIST);
 unsigned short* index=(unsigned short*)IndexBuffer.Lock(0,0,IndexBuffer.GetSize());
 int base=0;
 for(int j=10;j;--j){*index++=(unsigned short)base;*index++=(unsigned short)(base+1);*index++=(unsigned short)(base+3);*index++=(unsigned short)(base+1);*index++=(unsigned short)(base+2);*index++=(unsigned short)(base+3);base+=4;}
 IndexBuffer.Unlock();
}