// CEngineSceneGrid::PreloadPrimitives -- native 00b92d30, C3DBoundingSphere overload.
// Leaf: PreloadPrimitives
#ifndef CHUNK236_SCENE_NATIVE_RESUME_H
#define CHUNK236_SCENE_NATIVE_RESUME_H
// Native interfaces audited against table 012A3D7C (28 entries), RTTI hierarchy,
// FableWin PDB named signatures and native field loads. Donor record headers
// remain included; actual virtual classes replace their data-only projections.
#define CIVCountedPointeeBase CIVCountedPointeeBase_PDB
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CEnginePersistentPrimitiveBase CEnginePersistentPrimitiveBase_PDB
#include "engine/CEnginePersistentPrimitiveBase.h"
#undef CEnginePersistentPrimitiveBase
#define CEngineInternalPrimitiveBase CEngineInternalPrimitiveBase_PDB
#include "engine/CEngineInternalPrimitiveBase.h"
#undef CEngineInternalPrimitiveBase
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#ifndef _MSC_VER
#define __declspec(x)
#endif
class C3DVector { public:float X,Y,Z;};
class C3DBoundingSphere { public:C3DVector Center;float Radius;};
class C3DBoundingBox { public:C3DVector MinCoord,MaxCoord;bool IntersectsSolid(const C3DBoundingSphere&) const;}; //00ADECC0
class CMatrix3x4 { public:float Values[12];};
class CEngineFadeDistance { public:float FadeBeginDistance,MaxDrawDistance;};
class C2DBoxF { public:float TLX,TLY,BRX,BRY;};
struct CEngineCameraDesc {C2DBoxF Window;CMatrix3x4 RotationMatrix;C3DVector WorldPosition;float HorizontalFOV,VerticalFOV;bool Use2DFOV;float NearZPlane,FarZPlane,ZBufferStart,ZBufferEnd;bool StateValid;};
struct CEngineCameraMethods:CEngineCamera {
 bool ClipWorldSpaceBoundingBox(const C3DBoundingBox&,float,bool&) const; //00B308E0
 const C3DVector& Position()const{return reinterpret_cast<const CEngineCameraDesc*>(CameraDesc)->WorldPosition;}
};
class CEnginePrimitiveHandle;class CEnginePrimitive;class CEnginePrimitiveAnimation;class CEnginePrimitivePosition;class CEngineMeshEffect;class CEngineInternalPrimitiveOcclusionBase;
template<class T>class CIVCountedPointer;
namespace std {template<class T>struct less;template<class T>class allocator;template<class K,class V>struct pair;template<class K,class V,class C,class A>class map;}
typedef std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > > PrimitiveStats;
enum EEnginePrimitiveStatIndex {PrimitiveStatDefault=0};
class CIVCountedPointeeBase { public:
 mutable unsigned long IVRefCount;
 CIVCountedPointeeBase();virtual ~CIVCountedPointeeBase();virtual void Delete()const;
};
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase { public:
 long Type;
 CEnginePersistentPrimitiveBase(long);virtual ~CEnginePersistentPrimitiveBase();
 virtual void CollectStats(PrimitiveStats&,EEnginePrimitiveStatIndex,unsigned long)const=0;
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);
 virtual bool WasRendered()const;
};
class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase { public:
 CEngineInternalPrimitiveBase(long,unsigned long);virtual ~CEngineInternalPrimitiveBase();
 virtual void CollectStats(PrimitiveStats&,EEnginePrimitiveStatIndex,unsigned long)const;
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);virtual bool WasRendered()const;
 virtual void Update(const CEnginePrimitive&,unsigned long);
 virtual unsigned long AddChildPrimitive(const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(unsigned long,const CEnginePrimitive&,unsigned long);
 virtual void GetChildPrimitiveHandle(unsigned long,CEnginePrimitiveHandle&);
 virtual void ClearChildPrimitive(unsigned long);virtual void ClearAllChildPrimitives();
 virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long);
 virtual CEngineFadeDistance GetFadeDistance()const;virtual void PostRender();
 virtual bool GetWorldPosition(C3DVector&);virtual bool GetBoundingBoxTransform(CMatrix3x4&);
 virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox&);virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere&);
 virtual bool GetBoundingSphereRadiusWorldSpace(float&);virtual bool IsInsideFrustum(const CEngineCamera&,bool);
 virtual bool GetObjectTransform(CMatrix3x4&)const;virtual bool SetObjectTransform(const CMatrix3x4&);
 virtual void SetAnimation(const CEnginePrimitiveAnimation&);virtual void SetPosition(const CEnginePrimitivePosition&);
 virtual void SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>&);virtual bool GetMeshEffect(CIVCountedPointer<CEngineMeshEffect>&)const;
 virtual CEngineInternalPrimitiveOcclusionBase* GetAsOcclusionPrimitive();
 unsigned long EnginePrimitiveFlags;float AlphaMipBias;CEngineFadeDistance FadeDistance;unsigned long OcclusionLayerBitMask;
 CEngineSceneGridCell* GridCell;signed char GridLevel;unsigned char GridX,GridY,Alpha,AlphaRef,PreloadStat;
 int InterpolationSet;unsigned long RenderLayerMask;
 unsigned char Enable:1;unsigned char NonPersistent:1;unsigned char OverrideFadeDistance:1;unsigned char PostRenderEnable:1;
 unsigned short LastFrameRendered;CEngineInternalPrimitiveBase* NextPrimitive;CEngineInternalPrimitiveBase** RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;CEngineInternalPrimitiveBase** RefLayerMask;
};
class CEngineScene;template<class T>class CArray;
class CEngineSubPrimitiveBase;class CEngineSubPrimitiveChain;class CEnginePrimitiveTypeList;
namespace NEngineShadowRenderer {class CShadowSceneContext;}
enum EEnginePrimitiveType{PrimitiveTypeDefault=0};enum EEngineRenderLayer{LayerDefault=0};enum ERendered{RenderedDefault=0};enum ERenderPassType{RenderPassDefault=0};enum ESubPrimitiveType{SubPrimitiveDefault=0};
class CBaseClass { public:CBaseClass();virtual ~CBaseClass();};
struct CBaseClassNonCopyable:CBaseClass {protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
struct CEnginePrimitiveRenderUtil:CBaseClassNonCopyable {CEnginePrimitiveRenderUtil();virtual ~CEnginePrimitiveRenderUtil();};
class CEnginePrimitiveManager:public CEnginePrimitiveRenderUtil { public:
 CEnginePrimitiveManager(CArray<CEngineScene*>&);virtual ~CEnginePrimitiveManager();
 virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType)=0;
 virtual void GetSupportedTypes(CEnginePrimitiveTypeList&)const=0;virtual void FrameStart();virtual void FrameEnd();
 virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&);
 virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&)=0;
 virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&);
 virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase&,const C3DVector&,unsigned long);
 virtual void Render2DPrimitive(CEngineInternalPrimitiveBase&,CEngineSubPrimitiveChain&,ESubPrimitiveType);
 virtual void RenderNonsortedPrimitives();virtual void AddStaticShadowData(CEngineInternalPrimitiveBase&,NEngineShadowRenderer::CShadowSceneContext&);
 CArray<CEngineScene*>* Scenes;bool Enable;
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo; //01436E84
class CEnginePrimitiveHandle { public:void* NativeVTable;CEnginePersistentPrimitiveBase* Data;};
class CEngineSceneGridCell { public:
 CEngineSceneGridCell* Parent;CEngineInternalPrimitiveBase* FirstPrimitive;C3DBoundingBox BoundingBox;float DrawDistance;
 CEnginePrimitiveHandle GroupPrimitiveHandle[2];long LocalCount;unsigned long TotalCount,RenderMask,OccludedLayers;
 unsigned char BoundingBoxValid:1;unsigned char TotalCountValid:1;unsigned char RenderMaskValid:1;unsigned char WasRendered:1;unsigned char HasOcclusionResults:1;
 unsigned char OccludedFrames,PreloadFrames;
};
class CEngineSceneRenderStats { public:unsigned long StaticPrimitivesRendered,MovingPrimitivesRendered,StaticPrimitivesTouched,MovingPrimitivesTouched,VisibleCells,BoundingBoxTests,BoundingSphereTests;};
class CShadowClipVolume { public:bool ClipBoundingBox(const C3DBoundingBox&,bool&)const;}; //00B4CED0
class CEngineSceneGrid { public:
 void* NativeVTable;unsigned long Level;CEngineSceneGrid* LevelUp;CEngineSceneGrid* LevelDown;unsigned long Size;
 CEngineSceneGridCell** GridCells;CEngineSceneGridCell** LastCell;CEngineSceneGridCell** EndCell;
 bool GetCellBoundingBoxAndDrawDistance(C3DBoundingBox&,float&,unsigned long,unsigned long); //00B919C0
 unsigned long GetCellRenderMask(unsigned long,unsigned long); //00B91DC0
 bool PreloadPrimitives(unsigned long,unsigned long,const CEngineCamera&,unsigned long); //00B91F50
 bool PreloadPrimitives(unsigned long,unsigned long,const C3DBoundingSphere&,unsigned long); //00B92D30
 void RenderCellClippedShadowBuffer(unsigned long,unsigned long,EEngineRenderLayer,const CShadowClipVolume&,bool,CEngineSceneRenderStats&); //00B92660
};
struct TreeNodeBase {unsigned int Colour;TreeNodeBase* Parent;TreeNodeBase* Left;TreeNodeBase* Right;};
inline TreeNodeBase* TreeNext(TreeNodeBase* node){if(node->Right){node=node->Right;while(node->Left)node=node->Left;}else{TreeNodeBase* parent=node->Parent;while(node==parent->Right){node=parent;parent=parent->Parent;}if(node->Right!=parent)node=parent;}return node;}
template<class V>struct TreeNode:TreeNodeBase{int Key;V Value;};
template<class V>struct RetailTree{TreeNodeBase* Header;unsigned long Count;unsigned char Comparator;struct iterator{TreeNodeBase* Node;iterator(TreeNodeBase*n):Node(n){}iterator&operator++(){Node=TreeNext(Node);return *this;}bool operator!=(const iterator&o)const{return Node!=o.Node;}V&value()const{return static_cast<TreeNode<V>*>(Node)->Value;}};iterator begin(){return iterator(Header->Left);}iterator end(){return iterator(Header);}};
struct CCountedGrid{CEngineSceneGrid* Data;void* Info;};struct CGridArray{CCountedGrid* First;CCountedGrid* Last;CCountedGrid* End;};
class CEngineSceneMap{ public:void* NativeVTable;C2DBoxF Area;unsigned long MaxLevels;CGridArray GridLevels;CEngineScene* Scene;};
struct CCountedSceneMap{CEngineSceneMap* Data;void* Info;};
class CEngineScene{ public:
 void* NativeVTable;RetailTree<RetailTree<CCountedSceneMap> > MapList;float MapSize,OneOverMapSize;unsigned long MaxMapLevels,MapsRenderLayerMask;
 CEngineInternalPrimitiveBase* FirstMovingPrimitive;CEngineInternalPrimitiveBase* FirstNonPersistentPrimitive;
 bool PreloadPrimitives(const CEngineCamera&,unsigned long);bool PreloadPrimitives(const C3DBoundingSphere&,unsigned long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveBase)==0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase,Type)==8);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalPrimitiveBase,NextPrimitive)==0x38);
FABLE_STATIC_ASSERT(sizeof(CEngineSceneGridCell)==0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineScene,FirstMovingPrimitive)==0x20);
FABLE_STATIC_ASSERT(sizeof(CEngineSceneGrid)==0x20);
#endif
#endif

bool CEngineSceneGrid::PreloadPrimitives(unsigned long x,unsigned long y,const C3DBoundingSphere& volume,unsigned long flags){
 CEngineSceneGridCell* cell=GridCells[x+y*Size];bool result=false;
 C3DBoundingBox bounds;float distance;
 if(cell&&GetCellBoundingBoxAndDrawDistance(bounds,distance,x,y)){
  if(bounds.IntersectsSolid(volume)){
   for(CEngineInternalPrimitiveBase* primitive=cell->FirstPrimitive;primitive;primitive=primitive->NextPrimitive){
    C3DBoundingBox primitiveBounds;
  if(primitive->GetBoundingBoxWorldSpace(primitiveBounds)&&bounds.IntersectsSolid(volume)){
   bool loaded=g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type]->PreloadPrimitive(*primitive,volume.Center,flags);
   primitive->PreloadStat=loaded?2:1;result|=loaded;
  }
   }
   if(LevelDown)for(unsigned long yy=0;yy<2;++yy)for(unsigned long xx=0;xx<2;++xx)result|=LevelDown->PreloadPrimitives(x*2+xx,y*2+yy,volume,flags);
  }
 }
 return result;
}