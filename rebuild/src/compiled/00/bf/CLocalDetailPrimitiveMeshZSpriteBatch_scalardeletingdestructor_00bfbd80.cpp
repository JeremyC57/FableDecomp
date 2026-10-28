// CLocalDetailPrimitiveMeshZSpriteBatch::scalar deleting destructor(flags) -- destroy the object and release its landscape virtual allocation when requested.
// Leaf: ??_GCLocalDetailPrimitiveMeshZSpriteBatch@@UAEPAXI@Z
// Identity: proven; own RTTI scalar deletingdtor slot0 and exact original destructor API; original CLocalDetailPrimitiveMeshZSpriteBatch : CLocalDetailPrimitiveBase : CMovableResource hierarchy; target invokes actual dtor00BFB590 then conditionalbit1 classdelete; global01436EA8 actual CEngineLandscapeRenderer* with MemoryManager+6B0 and original CEngineLandscapeMemoryManager::FreeVirtual(void*)00BD9B90 exactPDB. Legacy vector-deleting label rejected; ret4 flagsbit1 and single dtor prove scalar form.
// Layout: compiler generates the deleting destructor from real virtual destructor declaration and actual class delete. Original base chains and typed owner members retained; PC full object size checked. Shared headers included unchanged as evidence or passive renderer layout; flattened active types reconstructed. CPatch edge strips restore original CBaseClassNonCopyable hierarchy and typed arrays. ForceNew only makes the actual vtable/deleting helper materialize. No hand-written destructor-flags wrapper.
#include "engine/CEngineLandscapeRenderer.h"
#define CEngineLandscapeMemoryManager CEngineLandscapeMemoryManager_HeaderLayout
#include "engine/CEngineLandscapeMemoryManager.h"
#undef CEngineLandscapeMemoryManager
#define CMovableResource CMovableResource_HeaderLayout
#include "engine/CMovableResource.h"
#undef CMovableResource
#define CLandscapeBackgroundSubNodeBase CLandscapeBackgroundSubNodeBase_HeaderLayout
#include "engine/CLandscapeBackgroundSubNodeBase.h"
#undef CLandscapeBackgroundSubNodeBase
#define CLandscapeBackgroundPatch CLandscapeBackgroundPatch_HeaderLayout
#include "engine/CLandscapeBackgroundPatch.h"
#undef CLandscapeBackgroundPatch
#define CLandscapeLayerMesh CLandscapeLayerMesh_HeaderLayout
#include "engine/_quarantine/CLandscapeLayerMesh.h"
#undef CLandscapeLayerMesh
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};class CMovableResource{public:virtual ~CMovableResource(){}virtual unsigned long Verify()const{return 0;}virtual bool CanRelocateData(long)const{return true;}};class CFailedAllocationHandler{public:virtual ~CFailedAllocationHandler(){} };class CEngineLandscapeMemoryManager:public CFailedAllocationHandler,public CMovableResource{public:void FreeVirtual(void*);};extern CEngineLandscapeRenderer* g_LandscapeRenderer_01436EA8;
class C3DVector{public:float X,Y,Z;};class C3DBoundingBox{public:C3DVector MinCoord,MaxCoord;};class C3DBoundingSphere{public:C3DVector Center;float Radius;};template<class T>class CEdgeStipVertexArray{public:T* Data;long Count;};class CPatchTesselationEdgeStrip:public CBaseClassNonCopyable{public:struct CVertex;struct CFanBaseVertex;struct CWaterVertex;struct CWaterFanBaseVertex;unsigned short StartPos,Size;CEdgeStipVertexArray<CVertex> Verts;CEdgeStipVertexArray<CFanBaseVertex> FanBaseVerts;CEdgeStipVertexArray<CWaterVertex> WaterVerts;CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;bool Tesselated,Vertical;};
class CLandscapeBackgroundSubNodeBase:public CMovableResource{};class CLocalDetailCacheMap;class CLocalDetailObjectCollectionType;class CLocalDetailPrimitiveBase:public CMovableResource{public:CLocalDetailPrimitiveBase* Next;C3DBoundingBox BoundingBox;C3DBoundingSphere BoundingSphere;CLocalDetailObjectCollectionType* Type;CLocalDetailCacheMap* Map;};class CEnginePersistentPrimitiveBase;class CIVCountedPointeeBase;template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;};class CEnginePrimitiveHandle:public CBaseClass{public:CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;};
enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION{LANDSCAPE_MAPPING_UNNAMED=0};
class CLocalDetailPrimitiveMeshZSpriteBatch:public CLocalDetailPrimitiveBase{public:
class CObject;class CObjectEntry;CObject* Objects;CObjectEntry* ZSpriteEntries;long ObjectCount;bool HasMeshPrimitiveHandles;CEnginePrimitiveHandle ZSpriteHandle;
virtual ~CLocalDetailPrimitiveMeshZSpriteBatch();static void operator delete(void* p){g_LandscapeRenderer_01436EA8->MemoryManager->FreeVirtual(p);}};
#ifdef _MSC_VER
typedef char ExactOwnerSize[(sizeof(CLocalDetailPrimitiveMeshZSpriteBatch)==80)?1:-1];
#endif
CLocalDetailPrimitiveMeshZSpriteBatch* ForceNew_00bfbd80(){return ::new CLocalDetailPrimitiveMeshZSpriteBatch;}