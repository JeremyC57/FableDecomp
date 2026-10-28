// CLandscapeBackgroundPatch::scalar deleting destructor(flags) -- destroy the object and release its landscape virtual allocation when requested.
// Leaf: ??_GCLandscapeBackgroundPatch@@UAEPAXI@Z
// Identity: proven; own RTTI scalar deletingdtor slot0 and exact original destructor API; original CLandscapeBackgroundPatch : CLandscapeBackgroundSubNodeBase : CMovableResource hierarchy; target invokes actual dtor00BE6090 then conditionalbit1 classdelete; global01436EA8 actual CEngineLandscapeRenderer* with MemoryManager+6B0 and original CEngineLandscapeMemoryManager::FreeVirtual(void*)00BD9B90 exactPDB. Legacy vector-deleting label rejected; ret4 flagsbit1 and single dtor prove scalar form.
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
class CLandscapeBackgroundPatch:public CLandscapeBackgroundSubNodeBase{public:

    CLandscapeBackgroundTreeNode*   Parent;                  // +0x04
    CPatchTesselationEdgeStrip        EdgeStrips[4];        // +0x08 CPatchTesselationEdgeStrip[4]
    CTexture*                       Texture;                 // +0xb8
    CIndexBuffer*                   IndexBuffer;             // +0xbc
    CVertexBufferWin32*             VertexBuffer;            // +0xc0
    CIndexBuffer*                   BridgeIndexBuffers[4];   // +0xc4
    CVertexBufferWin32*             BridgeVertexBuffers[4];  // +0xd4
    unsigned short                  BridgePolyCount[4];      // +0xe4
    CSubPatchTreeLODLockElement*    UVLockTreeU;             // +0xec
    CSubPatchTreeLODLockElement*    UVLockTreeV;             // +0xf0
    CEngineWaterBackgroundSubPatch* WaterSubPatch;           // +0xf4
    float                           MinZ;                    // +0xf8
    float                           MaxZ;                    // +0xfc
    long                            ZSorterHandle;           // +0x100
    unsigned short                  VertexCount;             // +0x104
    unsigned short                  PolyCountIncDegenerates; // +0x106
    unsigned short                  TextureWidthNeeded;      // +0x108
    unsigned short                  TextureHeightNeeded;     // +0x10a
    unsigned short                  MapX;                    // +0x10c
    unsigned short                  MapY;                    // +0x10e
    unsigned short                  Width;                   // +0x110
    unsigned short                  Height;                  // +0x112
    unsigned short                  LockedGridSpacingX;      // +0x114
    unsigned short                  LockedGridSpacingY;      // +0x116
    unsigned char                   LODBand;                 // +0x118
    bool                            SharedIndexBuffer;       // +0x119
    bool                            WaterOnlyPatch;          // +0x11a
    bool                            IsDefaultSection;        // +0x11b
virtual ~CLandscapeBackgroundPatch();static void operator delete(void* p){g_LandscapeRenderer_01436EA8->MemoryManager->FreeVirtual(p);}};
#ifdef _MSC_VER
typedef char ExactOwnerSize[(sizeof(CLandscapeBackgroundPatch)==284)?1:-1];
#endif
CLandscapeBackgroundPatch* ForceNew_00be8e60(){return ::new CLandscapeBackgroundPatch;}