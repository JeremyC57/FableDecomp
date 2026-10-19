// CLocalDetailPrimitiveMeshZSpriteBatch::~CLocalDetailPrimitiveMeshZSpriteBatch() -- destroy the batch's objects and free their block, free the z-sprite entry block, release the z-sprite primitive handle
// Leaf: ??1CLocalDetailPrimitiveMeshZSpriteBatch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFB590, length 181 bytes (`ret`), vtable 0x012A82D8, base vtable 0x0129B860.
// Callees: CLocalDetailPrimitiveMeshZSpriteBatch::CObject::~CObject @ 0x00BFBBF0 (per object, 0x4C bytes each),
// CEngineLandscapeMemoryManager::FreeVirtual @ 0x00BD9B90 (renderer 0x01436EA8 -> +0x6B0), CBaseClass::~CBaseClass @ 0x0099A2E0
// (the handle's base), the primitive's NotifyReleased (slot 3) / Delete (slot 1).
// Model: shared verbatim with the landed sibling dtor CLocalDetailPrimitiveRepeatedMesh 0x00BFAAE0 (same TU): Objects +0x38,
// ZSpriteEntries +0x3C, ObjectCount +0x40, ZSpriteHandle +0x48 (engine headers carry the donor layout, renderer +0xF58 vs retail +0x6B0).
// Shape: Objects and ObjectCount are read into locals (the Objects load is scheduled above the vtable store); the manager local is kept in
// the frame word across the object loop; the object array is destroyed by an
#include "engine/CEnginePrimitiveRenderer.h"
#define CMovableResource CMovableResourceDonor
#include "engine/CMovableResource.h"
#undef CMovableResource
#define CIVCountedPointeeBase CIVCountedPointeeBaseDonor
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CEnginePersistentPrimitiveBase CEnginePersistentPrimitiveBaseDonor
#include "engine/CEnginePersistentPrimitiveBase.h"
#undef CEnginePersistentPrimitiveBase
namespace std { template<class T> struct less; template<class T> class allocator; template<class A,class B> struct pair; template<class K,class V,class C,class A> class map; }
enum EEnginePrimitiveStatIndex { PrimitiveStatCount = 0 };
typedef std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > > PrimitiveStatsMap;
struct CEnginePrimitiveHandle;
struct CBaseClass { CBaseClass(); virtual ~CBaseClass(); }; // 0x0099A2E0
struct CIVCountedPointeeBase {
 virtual ~CIVCountedPointeeBase();
 virtual void Delete() const; // 0x0042D930
 unsigned long IVRefCount;
 void DecRef(){--IVRefCount;}
 long GetRefCount()const{return IVRefCount;}
 __forceinline void RemoveReference(){DecRef();if(GetRefCount()==0)Delete();}
};
struct CEnginePersistentPrimitiveBase : CIVCountedPointeeBase {
 long Type;
 virtual void CollectStats(PrimitiveStatsMap&,EEnginePrimitiveStatIndex,unsigned long) const = 0; // genuine pure slot2 (fablewin PDB)
 virtual void NotifyReleased(const CEnginePrimitiveHandle&); // slot3 0x00B8FFC0
 virtual bool WasRendered() const; // slot4 0x00B8FFD0
};
struct CIVPrimitivePointer {
 CEnginePersistentPrimitiveBase* Object;
 CIVPrimitivePointer():Object(0){} // native handle default ctor0041BE50
 CIVPrimitivePointer(const CIVPrimitivePointer& r):Object(r.Object){if(Object)++Object->IVRefCount;} // PDB counted-pointer copy

 unsigned long GetRefCount()const{return Object?Object->GetRefCount():0;}
 CEnginePersistentPrimitiveBase*operator->()const{return Object;}
 __forceinline void Clear() { if(Object) { Object->RemoveReference(); Object=0; } }
 __forceinline ~CIVPrimitivePointer() { Clear(); }
 __forceinline CIVPrimitivePointer& operator=(const CIVPrimitivePointer& r) { if(Object!=r.Object) { Clear(); Object=r.Object; if(Object) ++Object->IVRefCount; } return *this; }
};
struct CEnginePrimitiveHandle : CBaseClass {
 CIVPrimitivePointer Data;
 __forceinline void Clear() { if(Data.GetRefCount()==1) Data->NotifyReleased(*this); Data.Clear(); Data.Object=0; }
 __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& r) { Clear(); Data=r.Data; return *this; }
 __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
}; // actual vtable 0x0122F598
struct CMovableResource {
 virtual ~CMovableResource() {} // 0x0129B860
 virtual void RelocateData(long,void*,void*,long)=0;
 virtual bool CanRelocateData(long) const;
 virtual unsigned long Verify() const;
};
struct CEngineCamera; struct CLocalDetailObjectCollectionType; struct CDataInputStream;
enum LOCAL_DETAIL_PRIMITIVE_TYPE { LocalDetailPrimitiveUnspecified = 0 };
// Native VTs contain ten slots; donor's extra virtual APIs are not projected.
struct __declspec(novtable) CLocalDetailPrimitiveBase : CMovableResource {
 virtual void RenderUpdate(const CEngineCamera&,bool,unsigned long,const CLocalDetailObjectCollectionType*,float);
 virtual void AddObjectsToPrimitiveRenderer()=0;
 virtual bool NeedsRenderUpdate();
 virtual void OnPostRelocation();
 virtual bool Load(CDataInputStream&)=0;
 virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType()const=0;
 virtual bool CanRelocateData(long) const; // native 0x00BF9A80

 CLocalDetailPrimitiveBase* Next;
 float BoundingBox[6]; float BoundingSphere[4];
 void* Type; void* Map;
};
struct C4DVector { float X,Y,Z,W; };
struct CSubsectionElement;
struct CAddMeshDesc {
 void* MeshInfo; float MaxScale; float* VectorNormals; long ObjectCount;
 CSubsectionElement* SubSections; float BoundingBox[6], BoundingSphere[4];
 float FadeDist[2]; C4DVector *ObjectOffsets,*ObjectMatricies;
 unsigned long EnginePrimitiveFlags; long AlphaRef; float AlphaMipBias;
 bool Persistent; unsigned long EPFFlags; unsigned char* WindDelayArray; float WindScaleFactor;
};
struct CAddPrimitiveDesc {
 float BoundingSphere[4], BoundingBox[6]; float MaxScale; void* Mesh;
 float FadeDist[2],FadeInStart,FadeInEnd; void* ObjectEnts; long ObjectCount,AlphaRef; bool Persistent;
};
struct CLocalDetailRenderer : CEnginePrimitiveRenderer {
 CEnginePrimitiveHandle AddRepeatedStaticMesh(const CAddMeshDesc&); // 0x00B32F70
 CEnginePrimitiveHandle AddRepeatedZSprite(const CAddPrimitiveDesc&); // 0x00B32FF0
 void UpdateRepeatedStaticMesh(CEnginePrimitiveHandle&,const CAddMeshDesc&); // 0x00B31FE0
};
extern CLocalDetailRenderer* g_PrimitiveRenderer; // 0x01436E80
struct CLocalDetailPrimitiveRepeatedMesh : CLocalDetailPrimitiveBase {
 float MaxObjectScale; long ObjectCount,SubSectionsCount;
 CSubsectionElement* SubSections; C4DVector *ObjectMatricies,*ObjectOffsets;
 unsigned char* WindDelayArray; float* LandscapeNormalArray; CEnginePrimitiveHandle PrimitiveHandle;
 virtual ~CLocalDetailPrimitiveRepeatedMesh();
 CAddMeshDesc SetupPrimitiveDesc(); // 0x00BF9BC0
 virtual void AddObjectsToPrimitiveRenderer();
 virtual void RelocateData(long unused,void* oldBase,void* newBase,long size);
 virtual void OnPostRelocation(); // 0x00BF9B40
 virtual bool Load(CDataInputStream&); // 0x00BFA2A0
 virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType()const; // 0x00BFBB90

};
struct CLocalDetailPrimitiveMeshZSpriteBatch : CLocalDetailPrimitiveBase {
 struct CObject { float Matrix[12],Scale,BoundingSphere[4]; CEnginePrimitiveHandle PrimitiveHandle; ~CObject(); }; // 0x00BFBBF0
 CObject* Objects; void* ZSpriteEntries; long ObjectCount; bool HasMeshPrimitiveHandles;
 CEnginePrimitiveHandle ZSpriteHandle;
 CAddPrimitiveDesc SetupPrimitiveDesc(); // 0x00BF9FA0
 virtual void AddObjectsToPrimitiveRenderer();
 virtual ~CLocalDetailPrimitiveMeshZSpriteBatch();
 virtual void RelocateData(long,void*,void*,long); // 0x00BFA8A0
 virtual void RenderUpdate(const CEngineCamera&,bool,unsigned long,const CLocalDetailObjectCollectionType*,float); // BFAD90
 virtual bool NeedsRenderUpdate(); // 0x00BFBBA0
 virtual void OnPostRelocation(); // 0x00BF9B90
 virtual bool Load(CDataInputStream&); // 0x00BFB210
 virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType()const; // 0x00BFBBB0

};

// Native assertions retained in self-contained fixtures; host uses full-width members.
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CMovableResource)==sizeof(CMovableResourceDonor));
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase)==sizeof(CIVCountedPointeeBaseDonor));
FABLE_STATIC_ASSERT(sizeof(CEnginePersistentPrimitiveBase)==sizeof(CEnginePersistentPrimitiveBaseDonor));
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase,IVRefCount)==4);
FABLE_STATIC_ASSERT(offsetof(CEnginePersistentPrimitiveBase,Type)==8);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle)==8);
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveBase)==0x38);
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveRepeatedMesh)==0x60);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailPrimitiveRepeatedMesh,PrimitiveHandle)==0x58);
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveMeshZSpriteBatch)==0x50);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailPrimitiveMeshZSpriteBatch,ZSpriteHandle)==0x48);
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveMeshZSpriteBatch::CObject)==0x4c);
FABLE_STATIC_ASSERT(sizeof(CAddMeshDesc)==104);
FABLE_STATIC_ASSERT(sizeof(CAddPrimitiveDesc)==80);
typedef void (CLocalDetailPrimitiveRepeatedMesh::*LocalDetailAddPMF)();
FABLE_STATIC_ASSERT(sizeof(LocalDetailAddPMF)==4);
#endif

#include "engine/CEngineLandscapeMemoryManager.h"
// Methods projection inherits the complete actual layout; no additional data.
struct LandscapeMemoryMethods : CEngineLandscapeMemoryManager { void FreeVirtual(void*); void FreeAlignedVirtual(void*); }; // 0x00BD9B90
// Actual complete donor header audited before grade. Native renderer projection follows
// the retail manager access: 6B0, versus donor F58 after its larger MeshBuilder.
#include "engine/CEngineLandscapeRenderer.h"
struct LocalDetailEngine { unsigned char BeforeLandscapeMemory[0x6b0]; LandscapeMemoryMethods* LandscapeMemory; };
extern LocalDetailEngine* g_LocalDetailEngine; // 0x01436EA8
CLocalDetailPrimitiveMeshZSpriteBatch::~CLocalDetailPrimitiveMeshZSpriteBatch()
{
 CObject* objects=Objects;
 LandscapeMemoryMethods* manager=g_LocalDetailEngine->LandscapeMemory;
 long count=ObjectCount;
 if(objects){
  for(long i=0;i<count;++i) objects[i].~CObject();
  manager->FreeVirtual(objects);
 }
 if(ZSpriteEntries) manager->FreeVirtual(ZSpriteEntries);
}