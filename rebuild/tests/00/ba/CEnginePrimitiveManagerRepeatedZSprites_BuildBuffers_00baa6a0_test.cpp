// CEnginePrimitiveManagerRepeatedZSprites::BuildBuffers -- initialize128 vertices and192 indices for32 quads with per-quad shader index; retail00baa6a0 224bytes, tailjmp00ac1f00. Actual shared manager record fields VertexBuffer+38 andIndexBuffer+6c. Buffer entrypoints Initialise00a63150/00ac1f70, Lock00ac1fb0/00ac20a0, Unlock00ac20b0/00ac1f00. Vertex record16 hasXYZ at0/4/8 andushort index+c, padding+e untouched.
#ifdef _MSC_VER
#include "engine/CEnginePrimitiveManagerRepeatedZSprites.h"
#else
class C3DMeshInfo;
class CSurface;
class CTexture;

#pragma pack(push, 1)
class CEnginePrimitiveManagerRepeatedZSprites { public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x4];                            // +0x10
    unsigned char _pad_0x14[0x4];                            // +0x14
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char VertexShader[0x8];                         // +0x20 CVertexShader
    unsigned char PixelShader[0x8];                          // +0x28 CPixelShader
    unsigned char PixelShaderNoStipple[0x8];                 // +0x30 CPixelShader
    unsigned char VertexBuffer[0x34];                        // +0x38 CVertexBufferWin32
    unsigned char IndexBuffer[0x14];                         // +0x6c CIndexBuffer
    unsigned char PrimitiveSortList[0x10];                   // +0x80 CArray<CEngineInternalPrimitiveRepeatedZSprite_*>
    unsigned char BillboardTexture[0x18];                    // +0x90 CEngineBillboard
    long          BillboardTextureHandle;                    // +0xa8
    long          ZBufferTextureHandle;                      // +0xac
    long          DynamicTexturePageHandle;                  // +0xb0
    float         NonStippleAlphaFactor;                     // +0xb4
    bool          EnableZSpriteAlpha;                        // +0xb8
    unsigned char _pad_0xb9[0x3];                            // +0xb9
    float         VerticalControlPointWeightingFactor;       // +0xbc
    float         ControlPointWeightingPower;                // +0xc0
    C3DMeshInfo*  CurrentRenderedMesh;                       // +0xc4
};
#pragma pack(pop)

#endif
struct CRepeatedZSpritePosition {float X,Y,Z;void Set(float x,float y,float z){X=x;Y=y;Z=z;}};
struct CRepeatedZSpriteVertex {CRepeatedZSpritePosition Position;unsigned short ShaderIndex;unsigned short Unused;};
struct CRepeatedZVertexBufferMethods {
 bool Initialise(long type,long count,long stride,unsigned long usage,long pool);
 CRepeatedZSpriteVertex* Lock(unsigned long flags,unsigned long offset,unsigned long size);
 void Unlock();
};
struct CRepeatedZIndexBufferMethods {
 void Initialise(unsigned long length,unsigned long usage,long format,long pool,long primitiveType);
 unsigned short* Lock(unsigned long flags,unsigned long offset,unsigned long size);
 void Unlock();
};
struct CEnginePrimitiveManagerRepeatedZSpritesMethods:CEnginePrimitiveManagerRepeatedZSprites {void BuildBuffers();};
void CEnginePrimitiveManagerRepeatedZSpritesMethods::BuildBuffers()
{
 CRepeatedZVertexBufferMethods* vertexBuffer=reinterpret_cast<CRepeatedZVertexBufferMethods*>(VertexBuffer);
 vertexBuffer->Initialise(0,128,16,0,1);
 CRepeatedZIndexBufferMethods* indexBuffer=reinterpret_cast<CRepeatedZIndexBufferMethods*>(IndexBuffer);
 indexBuffer->Initialise(384,0,101,1,4);
 unsigned short* indices=indexBuffer->Lock(0,0,0);
 CRepeatedZSpriteVertex* vertices=vertexBuffer->Lock(0,0,0);
 for(int i=0;i<32;++i){
  vertices[i*4+0].Position.Set(0,0,0);
  vertices[i*4+1].Position.Set(1,0,0);
  vertices[i*4+2].Position.Set(1,1,0);
  vertices[i*4+3].Position.Set(0,1,0);
  vertices[i*4+0].ShaderIndex=(unsigned short)i;vertices[i*4+1].ShaderIndex=(unsigned short)i;vertices[i*4+2].ShaderIndex=(unsigned short)i;vertices[i*4+3].ShaderIndex=(unsigned short)i;
  indices[i*6+0]=(unsigned short)(i*4);indices[i*6+1]=(unsigned short)(i*4+1);indices[i*6+2]=(unsigned short)(i*4+2);indices[i*6+3]=(unsigned short)(i*4);indices[i*6+4]=(unsigned short)(i*4+2);indices[i*6+5]=(unsigned short)(i*4+3);
 }
 vertexBuffer->Unlock();indexBuffer->Unlock();
}

#include <cstdio>
#include <cstring>
static int failures=0,step=0;
static CEnginePrimitiveManagerRepeatedZSpritesMethods* expectedManager=0;
static CRepeatedZSpriteVertex vertexStorage[130];
static unsigned short indexStorage[194];
static void check(bool b){if(!b)++failures;}
static void event(int n){check(++step==n);}
bool CRepeatedZVertexBufferMethods::Initialise(long type,long count,long stride,unsigned long usage,long pool){event(1);check(this==reinterpret_cast<CRepeatedZVertexBufferMethods*>(expectedManager->VertexBuffer));check(type==0 && count==128 && stride==16 && usage==0 && pool==1);return true;}
void CRepeatedZIndexBufferMethods::Initialise(unsigned long length,unsigned long usage,long format,long pool,long primitiveType){event(2);check(this==reinterpret_cast<CRepeatedZIndexBufferMethods*>(expectedManager->IndexBuffer));check(length==384 && usage==0 && format==101 && pool==1 && primitiveType==4);}
unsigned short* CRepeatedZIndexBufferMethods::Lock(unsigned long flags,unsigned long offset,unsigned long size){event(3);check(this==reinterpret_cast<CRepeatedZIndexBufferMethods*>(expectedManager->IndexBuffer));check(!flags && !offset && !size);return indexStorage+1;}
CRepeatedZSpriteVertex* CRepeatedZVertexBufferMethods::Lock(unsigned long flags,unsigned long offset,unsigned long size){event(4);check(this==reinterpret_cast<CRepeatedZVertexBufferMethods*>(expectedManager->VertexBuffer));check(!flags && !offset && !size);return vertexStorage+1;}
void CRepeatedZVertexBufferMethods::Unlock(){event(5);check(this==reinterpret_cast<CRepeatedZVertexBufferMethods*>(expectedManager->VertexBuffer));}
void CRepeatedZIndexBufferMethods::Unlock(){event(6);check(this==reinterpret_cast<CRepeatedZIndexBufferMethods*>(expectedManager->IndexBuffer));}
int main()
{
 CEnginePrimitiveManagerRepeatedZSpritesMethods manager;expectedManager=&manager;
 for(int repeat=0;repeat<2;++repeat){step=0;std::memset(vertexStorage,0xa5,sizeof(vertexStorage));std::memset(indexStorage,0xa5,sizeof(indexStorage));manager.BuildBuffers();check(step==6);
  for(int q=0;q<32;++q){
   for(int v=0;v<4;++v){CRepeatedZSpriteVertex& out=vertexStorage[1+q*4+v];check(out.Position.X==((v==1 || v==2)?1.0f:0.0f));check(out.Position.Y==((v==2 || v==3)?1.0f:0.0f));check(out.Position.Z==0 && out.ShaderIndex==q && out.Unused==0xa5a5);}
   int offsets[]={0,1,2,0,2,3};for(int j=0;j<6;++j)check(indexStorage[1+q*6+j]==q*4+offsets[j]);
  }
  unsigned char* first=reinterpret_cast<unsigned char*>(vertexStorage);unsigned char* last=reinterpret_cast<unsigned char*>(vertexStorage+129);for(unsigned int b=0;b<sizeof(CRepeatedZSpriteVertex);++b)check(first[b]==0xa5 && last[b]==0xa5);check(indexStorage[0]==0xa5a5 && indexStorage[193]==0xa5a5);
 }
 if(failures){std::printf("00baa6a0_TEST FAIL %d\n",failures);return 1;}std::printf("00baa6a0_TEST PASS\n");return 0;
}