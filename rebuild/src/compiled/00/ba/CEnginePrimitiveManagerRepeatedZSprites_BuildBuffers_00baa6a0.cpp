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
// NF149 (q4552 / q4555 / q4557..q4560): the buffers (flat storage in the manager header) are reached as the shared retail classes
// (engine/CVertexBufferWin32.h, engine/CIndexBuffer.h; Ego_r.pdb + FableWin.pdb CVertexBufferWin32::Initialise 0x00A63150,
// CVertexBufferBase::Lock / Unlock 0x00AC20A0 / 0x00AC20B0, CIndexBuffer::Initialise / Lock / Unlock 0x00AC1F70 / 0x00AC1FB0 / 0x00AC1F00)
// with the shared buffer enums; were the stand-ins CRepeatedZVertexBufferMethods / CRepeatedZIndexBufferMethods.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
struct CEnginePrimitiveManagerRepeatedZSpritesMethods:CEnginePrimitiveManagerRepeatedZSprites {void BuildBuffers();};
void CEnginePrimitiveManagerRepeatedZSpritesMethods::BuildBuffers()
{
 CVertexBufferWin32* vertexBuffer=reinterpret_cast<CVertexBufferWin32*>(VertexBuffer);
 vertexBuffer->Initialise(VERTEX_NULL,128,16,0,VB_POOL_MANAGED);
 CIndexBuffer* indexBuffer=reinterpret_cast<CIndexBuffer*>(IndexBuffer);
 indexBuffer->Initialise(384,0,IB_FORMAT_16,IB_POOL_MANAGED,PT_TRIANGLE_LIST);
 unsigned short* indices=(unsigned short*)indexBuffer->Lock(0,0,0);
 CRepeatedZSpriteVertex* vertices=(CRepeatedZSpriteVertex*)vertexBuffer->Lock(0,0,0);
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