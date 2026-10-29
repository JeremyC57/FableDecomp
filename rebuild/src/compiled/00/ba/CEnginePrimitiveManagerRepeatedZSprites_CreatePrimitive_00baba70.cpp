// CEnginePrimitiveManagerRepeatedZSprites::CreatePrimitive -- allocate repeated Z sprite, adopt into temporary handle, update/register, return counted handle copy; retail00baba70 224bytes. Calls new00bfea1a,base00b8ff00,handlebase0099a2d0/dtor0099a2e0,update00baa780,register00bacdd0. Native polymorphic views complement actual flat shared headers; native primitive size94, count+4,Type+8,OcclusionInfo+48,bounds+64.
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

#ifdef _MSC_VER
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveRepeatedZSprite.h"
#else
#define __declspec(x)
#define __forceinline inline
#define __cdecl
#endif
#include <stddef.h>
void* __cdecl operator new(size_t);
struct CEnginePrimitiveHandleNative;
struct CEngineInternalPrimitiveNative {
 virtual ~CEngineInternalPrimitiveNative();
 virtual void Destroy();
 virtual void ReservedSlot2();
 virtual void OnLastHandleReleased(CEnginePrimitiveHandleNative*);
 long IVRefCount;long Type;unsigned long EnginePrimitiveFlags;float AlphaMipBias;float FadeBeginDistance,MaxDrawDistance;unsigned long OcclusionLayerBitMask;void* GridCell;signed char GridLevel;unsigned char GridX,GridY,Alpha,AlphaRef,PreloadStat;unsigned short Reserved2a;unsigned long InterpolationSet,RenderLayerMask;unsigned short PostRenderEnable,LastFrameRendered;void* NextPrimitive;void* RefPrimitive;void* NextLayerMask;void* RefLayerMask;
 CEngineInternalPrimitiveNative(long,unsigned long); //00b8ff00
 long RemoveReference(){return --IVRefCount;}
};
struct __declspec(novtable) CEngineInternalPrimitiveOcclusionNative:CEngineInternalPrimitiveNative {void* OcclusionInfo[2];CEngineInternalPrimitiveOcclusionNative(long type,unsigned long flags):CEngineInternalPrimitiveNative(type,flags){OcclusionInfo[0]=0;OcclusionInfo[1]=0;}};
struct CRepeatedSphere {float X,Y,Z,Radius;CRepeatedSphere():X(0),Y(0),Z(0),Radius(0){}};
struct CRepeatedBox {float MinX,MinY,MinZ,MaxX,MaxY,MaxZ;};
struct CEngineInternalRepeatedZSpriteNative:CEngineInternalPrimitiveOcclusionNative {
 void* Mesh;float FadeInStart,FadeInEnd;void* ObjectsEntries;long ObjectCount;CRepeatedSphere BoundingSphere;CRepeatedBox BoundingBox;float MinDistance,MaxScale;
 virtual ~CEngineInternalRepeatedZSpriteNative();
 CEngineInternalRepeatedZSpriteNative():CEngineInternalPrimitiveOcclusionNative(0x2b,0x10),BoundingSphere(){}
};
class CBaseClass { public:virtual ~CBaseClass();CBaseClass();CBaseClass(const CBaseClass&){};};
struct CPrimitiveCountedPointerNative {
 CEngineInternalPrimitiveNative* Object;
 CPrimitiveCountedPointerNative(CEngineInternalPrimitiveNative* p):Object(p){}
 CPrimitiveCountedPointerNative(const CPrimitiveCountedPointerNative& o):Object(o.Object){if(Object)++Object->IVRefCount;}
 __forceinline ~CPrimitiveCountedPointerNative(){if(Object){CEngineInternalPrimitiveNative* p=Object;if(!p->RemoveReference())p->Destroy();}Object=0;}
};
struct CEnginePrimitiveHandleNative:CBaseClass {
 CPrimitiveCountedPointerNative Data;
 CEnginePrimitiveHandleNative(CEngineInternalPrimitiveNative* p):Data(p){}
 CEnginePrimitiveHandleNative(const CEnginePrimitiveHandleNative& o):CBaseClass(o),Data(o.Data){}
 __forceinline virtual ~CEnginePrimitiveHandleNative(){if(Data.Object && Data.Object->IVRefCount==1)Data.Object->OnLastHandleReleased(this);}
};
struct CRepeatedSpriteDescription;
struct CEnginePrimitiveManagerRepeatedZSpritesMethods:CEnginePrimitiveManagerRepeatedZSprites {
 void UpdateRepeatedZSprite(const CEnginePrimitiveHandleNative&,const CRepeatedSpriteDescription&); //00baa780
 void RegisterPrimitive(CEngineInternalPrimitiveNative*,bool); //00bacdd0
 CEnginePrimitiveHandleNative CreatePrimitive(const CRepeatedSpriteDescription&);
};
CEnginePrimitiveHandleNative CEnginePrimitiveManagerRepeatedZSpritesMethods::CreatePrimitive(const CRepeatedSpriteDescription& description)
{
 CEngineInternalRepeatedZSpriteNative* primitive=new CEngineInternalRepeatedZSpriteNative;
 CEnginePrimitiveHandleNative handle(primitive);
 UpdateRepeatedZSprite(handle,description);
 RegisterPrimitive(primitive,true);
 return handle;
}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CEngineInternalRepeatedZSpriteNative)==sizeof(CEngineInternalPrimitiveRepeatedZSprite));
#endif