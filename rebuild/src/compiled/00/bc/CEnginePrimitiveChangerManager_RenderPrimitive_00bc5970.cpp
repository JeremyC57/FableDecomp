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