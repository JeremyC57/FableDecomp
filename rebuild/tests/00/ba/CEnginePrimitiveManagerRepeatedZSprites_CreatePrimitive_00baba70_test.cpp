// CEnginePrimitiveManagerRepeatedZSprites::CreatePrimitive -- allocate repeated Z sprite, adopt into temporary handle, update/register, return counted handle copy; retail00baba70 224bytes. Calls new00bfea1a,base00b8ff00,handlebase0099a2d0/dtor0099a2e0,update00baa780,register00bacdd0. Native polymorphic views complement actual flat shared headers; native primitive size94, count+4,Type+8,OcclusionInfo+48,bounds+64.
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
struct CPrimitiveHandleBaseNative {virtual ~CPrimitiveHandleBaseNative();CPrimitiveHandleBaseNative();CPrimitiveHandleBaseNative(const CPrimitiveHandleBaseNative&){};};
struct CPrimitiveCountedPointerNative {
 CEngineInternalPrimitiveNative* Object;
 CPrimitiveCountedPointerNative(CEngineInternalPrimitiveNative* p):Object(p){}
 CPrimitiveCountedPointerNative(const CPrimitiveCountedPointerNative& o):Object(o.Object){if(Object)++Object->IVRefCount;}
 __forceinline ~CPrimitiveCountedPointerNative(){if(Object){CEngineInternalPrimitiveNative* p=Object;if(!p->RemoveReference())p->Destroy();}Object=0;}
};
struct CEnginePrimitiveHandleNative:CPrimitiveHandleBaseNative {
 CPrimitiveCountedPointerNative Data;
 CEnginePrimitiveHandleNative(CEngineInternalPrimitiveNative* p):Data(p){}
 CEnginePrimitiveHandleNative(const CEnginePrimitiveHandleNative& o):CPrimitiveHandleBaseNative(o),Data(o.Data){}
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
static int failures=0,allocations=0,freed=0,baseConstructed=0,primitiveDestroyed=0,lastHandleCalls=0,updateCalls=0,registerCalls=0;
static bool failAllocation=false,retainInRegister=false;
static CEngineInternalPrimitiveNative* expectedPrimitive=0;
static CEnginePrimitiveManagerRepeatedZSpritesMethods* expectedManager=0;
struct CRepeatedSpriteDescription {unsigned long Marker;};
static const CRepeatedSpriteDescription* expectedDescription=0;
static void check(bool b){if(!b)++failures;}
void* __cdecl operator new(size_t n){++allocations;if(failAllocation)return 0;void* p=std::malloc(n);if(p)std::memset(p,0xa5,n);return p;}
void __cdecl operator delete(void* p){if(p){++freed;std::free(p);}}
CEngineInternalPrimitiveNative::CEngineInternalPrimitiveNative(long type,unsigned long flags):IVRefCount(1),Type(type),RenderLayerMask(flags){++baseConstructed;check(type==0x2b && flags==0x10);expectedPrimitive=this;}
CEngineInternalPrimitiveNative::~CEngineInternalPrimitiveNative(){++primitiveDestroyed;check(IVRefCount==0);}
CEngineInternalRepeatedZSpriteNative::~CEngineInternalRepeatedZSpriteNative(){}
void CEngineInternalPrimitiveNative::Destroy(){check(IVRefCount==0);delete this;}
void CEngineInternalPrimitiveNative::ReservedSlot2(){}
void CEngineInternalPrimitiveNative::OnLastHandleReleased(CEnginePrimitiveHandleNative* h){++lastHandleCalls;check(h->Data.Object==this && IVRefCount==1);}
CPrimitiveHandleBaseNative::CPrimitiveHandleBaseNative(){}
CPrimitiveHandleBaseNative::~CPrimitiveHandleBaseNative(){}
void CEnginePrimitiveManagerRepeatedZSpritesMethods::UpdateRepeatedZSprite(const CEnginePrimitiveHandleNative& handle,const CRepeatedSpriteDescription& desc)
{
 ++updateCalls;check(this==expectedManager && &desc==expectedDescription && handle.Data.Object==expectedPrimitive);
 if(handle.Data.Object){CEngineInternalRepeatedZSpriteNative* p=static_cast<CEngineInternalRepeatedZSpriteNative*>(handle.Data.Object);check(p->Type==0x2b && p->RenderLayerMask==0x10 && p->IVRefCount==1);check(!p->OcclusionInfo[0] && !p->OcclusionInfo[1]);check(p->BoundingSphere.X==0 && p->BoundingSphere.Y==0 && p->BoundingSphere.Z==0 && p->BoundingSphere.Radius==0);p->EnginePrimitiveFlags=desc.Marker;}
}
void CEnginePrimitiveManagerRepeatedZSpritesMethods::RegisterPrimitive(CEngineInternalPrimitiveNative* p,bool enabled){++registerCalls;check(this==expectedManager && p==expectedPrimitive && enabled && updateCalls==registerCalls);if(p){check(p->EnginePrimitiveFlags==expectedDescription->Marker);if(retainInRegister)++p->IVRefCount;}}
static void run(bool fail,bool retain)
{
 failAllocation=fail;retainInRegister=retain;allocations=freed=baseConstructed=primitiveDestroyed=lastHandleCalls=updateCalls=registerCalls=0;expectedPrimitive=0;
 CEnginePrimitiveManagerRepeatedZSpritesMethods manager;expectedManager=&manager;CRepeatedSpriteDescription description;description.Marker=0x345;expectedDescription=&description;
 {
  CEnginePrimitiveHandleNative handle=manager.CreatePrimitive(description);
  check(allocations==1 && baseConstructed==(fail?0:1) && updateCalls==1 && registerCalls==1 && freed==0 && !primitiveDestroyed);
  check(handle.Data.Object==expectedPrimitive);
  if(!fail)check(handle.Data.Object && handle.Data.Object->IVRefCount==(retain?2:1));
 }
 if(fail)check(!freed && !primitiveDestroyed && !lastHandleCalls);
 else if(retain){check(!lastHandleCalls && !primitiveDestroyed && !freed && expectedPrimitive->IVRefCount==1);if(!expectedPrimitive->RemoveReference())expectedPrimitive->Destroy();check(primitiveDestroyed==1 && freed==1);}
 else check(lastHandleCalls==1 && primitiveDestroyed==1 && freed==1);
}
int main(){run(false,false);run(false,true);run(true,false);if(failures){std::printf("00baba70_TEST FAIL %d\n",failures);return 1;}std::printf("00baba70_TEST PASS\n");return 0;}