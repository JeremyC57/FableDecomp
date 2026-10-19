// CEnginePrimitiveManagerRepeatedZSprites::BuildBuffers -- initialize128 vertices and192 indices for32 quads with per-quad shader index; retail00baa6a0 224bytes, tailjmp00ac1f00. Actual shared manager record fields VertexBuffer+38 andIndexBuffer+6c. Buffer entrypoints Initialise00a63150/00ac1f70, Lock00ac1fb0/00ac20a0, Unlock00ac20b0/00ac1f00. Vertex record16 hasXYZ at0/4/8 andushort index+c, padding+e untouched.
#ifdef _MSC_VER
#include "engine/CEnginePrimitiveManagerRepeatedZSprites.h"
#else
struct C3DMeshInfo;
struct CSurface;
struct CTexture;

#pragma pack(push, 1)
struct CEnginePrimitiveManagerRepeatedZSprites {
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