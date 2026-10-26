#ifndef _MSC_VER
#define __forceinline inline
class CEngineSceneGridCell;
class CEnginePrimitiveManager;
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
#pragma pack(push, 1)
class CEngineInternalPrimitiveBase { public:
    void*                         __vftable;                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                 _pad_0x04[0x4];            // +0x04
    unsigned char                 _pad_0x08[0x4];            // +0x08
    unsigned long                 EnginePrimitiveFlags;      // +0x0c
    float                         AlphaMipBias;              // +0x10
    float                         FadeDistance_FadeBeginDistance; // +0x14 CEngineFadeDistance::FadeBeginDistance
    float                         FadeDistance_MaxDrawDistance; // +0x18 CEngineFadeDistance::MaxDrawDistance
    unsigned long                 OcclusionLayerBitMask;     // +0x1c
    CEngineSceneGridCell*         GridCell;                  // +0x20
    signed char                   GridPos_Level;             // +0x24 CEngineSceneGridPos::Level
    unsigned char                 GridPos_X;                 // +0x25 CEngineSceneGridPos::X
    unsigned char                 GridPos_Y;                 // +0x26 CEngineSceneGridPos::Y
    unsigned char                 Alpha;                     // +0x27
    unsigned char                 AlphaRef;                  // +0x28
    unsigned char                 PreloadStat;               // +0x29
    unsigned char                 _pad_0x2a[0x2];            // +0x2a
    unsigned char                 InterpolationSet[0x4];     // +0x2c EEngineInterpolationSet
    unsigned long                 RenderLayerMask;           // +0x30
    unsigned char                 PostRenderEnable[0x2];     // +0x34 bitfield group: uchar:1
    unsigned short                LastFrameRendered;         // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;             // +0x38
    void*                         RefPrimitive;              // +0x3c CEngineInternalPrimitiveBase * *
    CEngineInternalPrimitiveBase* NextLayerMask;             // +0x40
    void*                         RefLayerMask;              // +0x44 CEngineInternalPrimitiveBase * *
};
#pragma pack(pop)
#define FABLE_ENGINE_CENGINEPRIMITIVETYPEINFO_H
#pragma pack(push, 1)
class CEnginePrimitiveTypeInfo { public:
    void*                    __vftable;                      // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char            _pad_0x04[0x4];                 // +0x04
    unsigned char            _pad_0x08[0x4];                 // +0x08
    unsigned char            _pad_0x0c[0x4];                 // +0x0c
    CEnginePrimitiveManager* PrimitiveManagers[47];          // +0x10
};
#pragma pack(pop)
#endif
// CEnginePrimitiveChangerManager::RenderPrimitive -- if enabled render the selected enabled child with changer mesh effect and a fresh subprimitive chain; retail00bc5970 166bytes. Shared primitive/typeinfo headers, retail changer vector12byte shifts active index58/effect54. Dispatch effect slot25(+64) takes CIVCountedPointer const-reference; manager render slot7(+1c). Passed outer chain unused; child gets zeroed local chain.
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
enum ERendered { RENDERED_NOTHING=0, RENDERED_SUB_PRIMITIVES=2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT=0 };
class CEngineMeshEffect { public:
 virtual void v00();
 virtual void Release(); // counted object virtual release slot1
 unsigned long RefCount;
 unsigned long RemoveReference() { return --RefCount; }
};
struct CMeshEffectPointer {
 CEngineMeshEffect* Pointer;
 CMeshEffectPointer():Pointer(0){}
 CMeshEffectPointer(const CMeshEffectPointer& other):Pointer(other.Pointer) { if(Pointer) ++Pointer->RefCount; }
 ~CMeshEffectPointer() { if(Pointer) { CEngineMeshEffect* current=Pointer; if(!current->RemoveReference()) current->Release(); } }
};
struct CPrimitiveEffectInterface {
 virtual void v00();virtual void v01();virtual void v02();virtual void v03();virtual void v04();
 virtual void v05();virtual void v06();virtual void v07();virtual void v08();virtual void v09();
 virtual void v10();virtual void v11();virtual void v12();virtual void v13();virtual void v14();
 virtual void v15();virtual void v16();virtual void v17();virtual void v18();virtual void v19();
 virtual void v20();virtual void v21();virtual void v22();virtual void v23();virtual void v24();
 virtual void SetMeshEffect(const CMeshEffectPointer& effect);
};
class CEngineSubPrimitiveChain { public: void* LastOpaque;void* LastAlpha;CEngineSubPrimitiveChain():LastOpaque(0),LastAlpha(0){} };
class CEnginePrimitiveManager { public:
 virtual void v00();virtual void v01();virtual void v02();virtual void v03();virtual void v04();virtual void v05();virtual void v06();
 virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
};
class CEnginePrimitiveHandle { public: virtual ~CEnginePrimitiveHandle(); CEngineInternalPrimitiveBase* Data; operator bool() const { return Data!=0; } };
struct CPrimitiveArray { CEnginePrimitiveHandle *First,*Last,*Limit; CEnginePrimitiveHandle& operator[](unsigned long i) { return First[i]; } };
struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase { CPrimitiveArray Primitives; CMeshEffectPointer MeshEffect; long ActiveIndex;bool PreloadAll;unsigned long PrimitiveFlags; };
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo; //01436e84
class CEnginePrimitiveChangerManager { public:
 void* Vtable;void* SceneList;bool Enable;
 ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
};
ERendered CEnginePrimitiveChangerManager::RenderPrimitive(CEngineInternalPrimitiveBase& primitive,EEngineRenderLayer layer,CEngineSubPrimitiveChain& outerChain)
{
 (void)outerChain;
 if(!Enable) return RENDERED_NOTHING;
 CEngineInternalPrimitiveChanger& changer=static_cast<CEngineInternalPrimitiveChanger&>(primitive);
 if(changer.ActiveIndex!=-1) {
  CEnginePrimitiveHandle& handle=changer.Primitives[changer.ActiveIndex];
  CEngineInternalPrimitiveBase* child=handle.Data;
  if(handle && (child->PostRenderEnable[0]&1)) {
   reinterpret_cast<CPrimitiveEffectInterface*>(child)->SetMeshEffect(CMeshEffectPointer(changer.MeshEffect));
   unsigned int type=*reinterpret_cast<const unsigned int*>(child->_pad_0x08);
   CEnginePrimitiveManager* manager=g_pEnginePrimitiveTypeInfo->PrimitiveManagers[type];
   CEngineSubPrimitiveChain chain;
   return manager->RenderPrimitive(*child,layer,chain);
  }
 }
 return RENDERED_NOTHING;
}
#include <cstdio>
#include <cstring>
CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo=0;
static int effectCalls=0,renderCalls=0,releaseCalls=0,failures=0;
static bool dropOwner=false;
static unsigned long expectedDuringEffect=0,expectedDuringRender=0;
static CEngineMeshEffect* expectedEffect=0;
static CEngineInternalPrimitiveBase* expectedChild=0;
static CEngineInternalPrimitiveChanger* owner=0;
static EEngineRenderLayer expectedLayer=ENGINE_RENDER_LAYER_DEFAULT;
static void check(bool ok) { if(!ok) ++failures; }
void CEngineMeshEffect::v00(){}
void CEngineMeshEffect::Release(){check(RefCount==0);++releaseCalls;}
CEnginePrimitiveHandle::~CEnginePrimitiveHandle(){}
#ifdef _MSC_VER
#define STUB_CALL __fastcall
#define STUB_DUMMY , int ignoredEdx
#else
#define STUB_CALL
#define STUB_DUMMY
#endif
static void STUB_CALL ReceiveEffect(CEngineInternalPrimitiveBase* child STUB_DUMMY,const CMeshEffectPointer& effect)
{
 ++effectCalls;check(child==expectedChild && effect.Pointer==expectedEffect);
 if(effect.Pointer) check(effect.Pointer->RefCount==expectedDuringEffect);
 if(dropOwner) {check(owner->MeshEffect.Pointer==effect.Pointer);owner->MeshEffect.Pointer=0;--effect.Pointer->RefCount;}
}
void CEnginePrimitiveManager::v00(){} void CEnginePrimitiveManager::v01(){} void CEnginePrimitiveManager::v02(){} void CEnginePrimitiveManager::v03(){} void CEnginePrimitiveManager::v04(){} void CEnginePrimitiveManager::v05(){} void CEnginePrimitiveManager::v06(){}
ERendered CEnginePrimitiveManager::RenderPrimitive(CEngineInternalPrimitiveBase& child,EEngineRenderLayer layer,CEngineSubPrimitiveChain& chain)
{
 ++renderCalls;check(&child==expectedChild && layer==expectedLayer && !chain.LastOpaque && !chain.LastAlpha);
 if(expectedEffect)check(expectedEffect->RefCount==expectedDuringRender);
 chain.LastOpaque=&child;return RENDERED_SUB_PRIMITIVES;
}
int main()
{
 CEnginePrimitiveTypeInfo info;std::memset(&info,0,sizeof(info));g_pEnginePrimitiveTypeInfo=&info;
 CEnginePrimitiveManager childManager;info.PrimitiveManagers[3]=&childManager;
 CEnginePrimitiveChangerManager manager;manager.Enable=false;
 CEngineInternalPrimitiveChanger changer;owner=&changer;
 std::memset(static_cast<CEngineInternalPrimitiveBase*>(&changer),0,sizeof(CEngineInternalPrimitiveBase));
 CEnginePrimitiveHandle handles[2];handles[0].Data=0;handles[1].Data=0;
 changer.Primitives.First=handles;changer.Primitives.Last=handles+2;changer.Primitives.Limit=handles+2;changer.ActiveIndex=0;changer.PreloadAll=false;
 CEngineInternalPrimitiveBase child;std::memset(&child,0,sizeof(child));
 void* vtable[26];std::memset(vtable,0,sizeof(vtable));vtable[25]=reinterpret_cast<void*>(&ReceiveEffect);child.__vftable=vtable;
 const unsigned int type=3;std::memcpy(child._pad_0x08,&type,4);child.PostRenderEnable[0]=1;handles[0].Data=&child;expectedChild=&child;
 CEngineSubPrimitiveChain outer;outer.LastOpaque=&manager;outer.LastAlpha=&changer;
 check(manager.RenderPrimitive(changer,expectedLayer,outer)==RENDERED_NOTHING);check(!effectCalls && !renderCalls);
 manager.Enable=true;changer.ActiveIndex=-1;
 check(manager.RenderPrimitive(changer,expectedLayer,outer)==RENDERED_NOTHING);check(!effectCalls && !renderCalls);
 changer.ActiveIndex=1;
 check(manager.RenderPrimitive(changer,expectedLayer,outer)==RENDERED_NOTHING);check(!effectCalls && !renderCalls);
 changer.ActiveIndex=0;child.PostRenderEnable[0]=0;
 check(manager.RenderPrimitive(changer,expectedLayer,outer)==RENDERED_NOTHING);check(!effectCalls && !renderCalls);
 child.PostRenderEnable[0]=1;
 check(manager.RenderPrimitive(changer,expectedLayer,outer)==RENDERED_SUB_PRIMITIVES);check(effectCalls==1 && renderCalls==1 && !releaseCalls);
 CEngineMeshEffect effect;effect.RefCount=1;changer.MeshEffect.Pointer=&effect;expectedEffect=&effect;expectedDuringEffect=2;expectedDuringRender=1;
 check(manager.RenderPrimitive(changer,expectedLayer,outer)==RENDERED_SUB_PRIMITIVES);check(effectCalls==2 && renderCalls==2 && !releaseCalls && effect.RefCount==1);
 dropOwner=true;expectedDuringRender=0;
 check(manager.RenderPrimitive(changer,expectedLayer,outer)==RENDERED_SUB_PRIMITIVES);check(effectCalls==3 && renderCalls==3 && releaseCalls==1 && !changer.MeshEffect.Pointer && effect.RefCount==0);
 check(outer.LastOpaque==&manager && outer.LastAlpha==&changer);
#ifdef _MSC_VER
 check(sizeof(CEngineInternalPrimitiveBase)==0x48 && offsetof(CEngineInternalPrimitiveChanger,MeshEffect)==0x54 && offsetof(CEngineInternalPrimitiveChanger,ActiveIndex)==0x58);
#endif
 if(failures){std::printf("00bc5970_TEST FAIL %d\n",failures);return 1;}
 std::printf("00bc5970_TEST PASS\n");return 0;
}