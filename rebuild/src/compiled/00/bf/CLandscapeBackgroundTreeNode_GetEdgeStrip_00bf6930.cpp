// CLandscapeBackgroundTreeNode::GetEdgeStrip -- native landscape tree operation
// Leaf: ?GetEdgeStrip@Tree@Native
// Native projection: donor Tree data shifts -8, native sizeof e8, proven BF6E20.
// Actual donor headers retained; native Movable/SubNode methods preserve genuine virtual member ABI.
#include "engine/CLandscapeBackgroundTreeNode.h"
#include "engine/CLandscapeBackgroundSubNodeBase.h"
#include "engine/CLandscapeBackgroundSubNodeSplitBranch.h"
#include "engine/CLandscapeBackgroundPatch.h"
#include "engine/CMovableResource.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
#include "engine/CEngineOcclusionManager.h"
#include <math.h>
class CPatchTesselationEdgeStrip;
namespace Native {
struct Tree;struct Split;
enum LANDSCAPE_EDGE_STRIP_SIDE { Side0,Side1,Side2,Side3 };
struct Movable {
 virtual ~Movable();
 virtual void RelocateData(long,void*,void*,long)=0;
 virtual bool CanRelocateData(long) const;
 virtual unsigned long Verify() const;
};
struct SubNode:Movable {
 virtual bool IsPatch()=0; // native slot4; PatchVT012A803C, SplitVT012A8258
 virtual void UpdateTesselation();
 virtual void InvalidateEdge(long,long,long,LANDSCAPE_EDGE_STRIP_SIDE);
 virtual void OnPostRelocation();
 virtual void OnPostParentRelocation(Tree*);
};
struct Freeable:Movable { virtual bool FreeableResourceFreeBlock()=0; };
struct EngineMap { unsigned char State[0x60];unsigned short WorldPosX,WorldPosY; };
struct LandscapeMap { void* VTable;Tree* PatchTree;void* Patches;long PatchGridWidth,PatchGridHeight;EngineMap* Map; };
struct C3DVector { float X,Y,Z; };
struct C3DBoundingBox { C3DVector MinCoord,MaxCoord; };
struct C3DBoundingSphere { C3DVector Center;float Radius; };
struct CLODLoadInfo { long FileBlockPos,FileBlockSize,OffsetIntoFileBlock;unsigned char OptimizedBandRemap; };
struct Matrix {float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;};
struct OcclusionManager { unsigned long RenderOcclusionBox(const Matrix&); }; // B8D420; PDB QAEKABVCMatrix3x4
extern OcclusionManager* gOcclusionManager; //01436e5c
struct Patch:SubNode {

    CLandscapeBackgroundTreeNode*   Parent;                  // +0x04
    unsigned char                   EdgeStrips[0xb0];        // +0x08 CPatchTesselationEdgeStrip[4]
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
 Patch(Tree*); // genuine donor9C60B0 overload; native address unestablished (BE6090 is destructor)
 bool IsPatch();void RelocateData(long,void*,void*,long);
 CPatchTesselationEdgeStrip* GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE); }; //00BE5C60
struct Tree:Freeable {
 CLandscapeBackgroundDynamicBuildTemplate* DynamicBuildTemplate;CLandscapeGPUUpdateTask* GPUUpdateTask;
 CLODLoadInfo LODLoadInfo[8];long FileBlockPos,FileBlockSize,OffsetIntoFileBlock;
 CLandscapeBackgroundLoadTask* AsyncLoadTask;Tree* Parent;SubNode* SubNodeData;LandscapeMap* Map;
 C3DBoundingBox BoundingBox;C3DBoundingSphere CameraUpdateThreshold;long OcclusionTestHandle,CreationFlags;
 unsigned short MapX,MapY,Width,Height;unsigned char FirstBand,FirstNonSplitBand,LastBand;
 bool TesselationValid,VisibleLastFrame,VisibleLastFrameHasBeenUpdated,HasSubPatchLoadInfo;
 Tree(LandscapeMap*,Tree*,long); // genuine donor owner ctor A3E4F0 / native BF6E20
 void RelocateData(long,void*,void*,long);bool FreeableResourceFreeBlock();
 long PeekWorldX(){return MapX+Map->Map->WorldPosX;}
 long PeekWorldY(){return MapY+Map->Map->WorldPosY;}
 Patch* GetPatch(); // BF6130
 Split* GetSplitBranch(); // BF6160
 Patch* PeekPatch(){return SubNodeData && SubNodeData->IsPatch() ? (Patch*)SubNodeData : 0;}
 Split* PeekSplit(){return SubNodeData && !SubNodeData->IsPatch() ? (Split*)SubNodeData : 0;}
 void MergeCameraUpdateThresholdWithChildren();
 CPatchTesselationEdgeStrip* GetEdgeStrip(long,long,LANDSCAPE_EDGE_STRIP_SIDE);
 bool IsBackgroundAtFullDetailInArea(long,long,long,long);
 void RenderOcclusionBox(long&);
};
struct Split:SubNode {
 Tree LeftBranch,RightBranch;
 Split(LandscapeMap*,Tree*,long); // genuine PDB A41BD0
 bool IsPatch();void RelocateData(long,void*,void*,long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(Tree)==0xe8);
FABLE_STATIC_ASSERT(offsetof(Tree,Map)==0xa4);
FABLE_STATIC_ASSERT(offsetof(Tree,SubNodeData)==0xa0);
FABLE_STATIC_ASSERT(offsetof(Tree,MapX)==0xd8);
FABLE_STATIC_ASSERT(offsetof(Tree,CameraUpdateThreshold)==0xc0);
FABLE_STATIC_ASSERT(offsetof(Tree,BoundingBox)==0xa8);
FABLE_STATIC_ASSERT(offsetof(Split,RightBranch)==0xec);
FABLE_STATIC_ASSERT(sizeof(bool (SubNode::*)())==4);
#endif
CPatchTesselationEdgeStrip* Tree::GetEdgeStrip(long x,long y,LANDSCAPE_EDGE_STRIP_SIDE side){
 Patch* patch=PeekPatch();if(patch)return patch->GetEdgeStrip(side);
 Split* split=PeekSplit();if(!split)return 0;
 long testX=x,testY=y;
 if(side==Side1)--testX;else if(side==Side3)--testY;
 if(split->RightBranch.MapX>MapX){
  if(testX<split->RightBranch.PeekWorldX())return split->LeftBranch.GetEdgeStrip(x,y,side);
  return split->RightBranch.GetEdgeStrip(x,y,side);
 }else{
  if(testY<split->RightBranch.PeekWorldY())return split->LeftBranch.GetEdgeStrip(x,y,side);
  return split->RightBranch.GetEdgeStrip(x,y,side);
 }
}
}