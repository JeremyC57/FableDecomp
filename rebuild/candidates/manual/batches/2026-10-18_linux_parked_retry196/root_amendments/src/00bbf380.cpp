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
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
#include "engine/CEngineResourceManager.h"
enum EVertexType {VertexTypeDefault=0};
enum EVertexBufferPool {VertexPoolManaged=1};
enum EIndexBufferFormat {IndexFormat16=101};
enum EIndexBufferPool {IndexPoolManaged=1};
enum EPrimitiveType {PrimitiveTriangleList=4};
struct CEngineResourceManager_Methods:CEngineResourceManager {
 unsigned long GetVertexBufferUsage(); //00B3B030
 unsigned long GetIndexBufferUsage(); //00B3B060
 EIndexBufferPool GetIndexBufferMemPool(); //00B3B0A0
};
extern CEngineResourceManager_Methods* g_ResourceManager; //01436E98
struct CVertexBufferWin32_Retail {
 void* NativeVTable;void* PVertexBuffer;unsigned long Length;EVertexType Type;unsigned char Stride;unsigned char Alignment[3];unsigned char CompressionParams[32];
 bool Initialise(EVertexType,long,long,unsigned long,EVertexBufferPool); //00A63150
 unsigned char* Lock(unsigned long,unsigned long,unsigned long); //00AC20A0
 void Unlock(); //00AC20B0
};
struct CIndexBuffer_Methods:CIndexBuffer {
 void Initialise(unsigned long,unsigned long,EIndexBufferFormat,EIndexBufferPool,EPrimitiveType); //00AC1F70
 long GetSize()const; //00AC1F10
 unsigned char* Lock(unsigned long,unsigned long,unsigned long); //00AC1FB0
 void Unlock(); //00AC1F00
};
struct ShaderArray {void* First;void* Last;void* End;};
struct ShaderHandle {void* NativeVTable;void* Data;};
struct CEnginePrimitiveManagerParticleSpriteGroup {
 void* NativeVTable;void* Scenes;bool Enable;unsigned char Alignment[3];
 ShaderArray VertexShadersBillboardSpritePointLights,VertexShadersCrossedSpritePointLights,VertexShaders3DFaceMeSpritePointLights;
 ShaderHandle VertexShadersBillboardSpriteDisplacement,VertexShadersCrossedSpriteDisplacement,VertexShaders3DFaceMeSpriteDisplacement;
 ShaderHandle PixelShader,PixelShaderStipple,PixelShaderDisplacement;
 CVertexBufferWin32_Retail VertexBuffer;CIndexBuffer_Methods IndexBuffer;
 float ConstantBlock[40][4];
 void BuildBuffers();
};
struct SIndexedParticleVertex {float Index;unsigned short U,V;};
void CEnginePrimitiveManagerParticleSpriteGroup::BuildBuffers(){
 VertexBuffer.Initialise(VertexTypeDefault,40,8,g_ResourceManager->GetVertexBufferUsage(),VertexPoolManaged);
 SIndexedParticleVertex* vertex=(SIndexedParticleVertex*)VertexBuffer.Lock(0,0,VertexBuffer.Length);
 int shaderIndex=4;
 for(int i=10;i;--i){
  vertex->Index=(float)shaderIndex;vertex->U=0;vertex->V=0;++vertex;
  vertex->Index=(float)shaderIndex;vertex->U=1;vertex->V=0;++vertex;
  vertex->Index=(float)shaderIndex;vertex->U=1;vertex->V=1;++vertex;
  vertex->Index=(float)shaderIndex;vertex->U=0;vertex->V=1;++vertex;
  shaderIndex+=4;
 }
 VertexBuffer.Unlock();
 IndexBuffer.Initialise(120,g_ResourceManager->GetIndexBufferUsage(),IndexFormat16,g_ResourceManager->GetIndexBufferMemPool(),PrimitiveTriangleList);
 unsigned short* index=(unsigned short*)IndexBuffer.Lock(0,0,IndexBuffer.GetSize());
 int base=0;
 for(int j=10;j;--j){*index++=(unsigned short)base;*index++=(unsigned short)(base+1);*index++=(unsigned short)(base+3);*index++=(unsigned short)(base+1);*index++=(unsigned short)(base+2);*index++=(unsigned short)(base+3);base+=4;}
 IndexBuffer.Unlock();
}
