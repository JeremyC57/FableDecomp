// CLandscapeBackgroundTreeNode::InvalidateEdge(long,long,long,LANDSCAPE_EDGE_STRIP_SIDE) -- reject disjoint segments then invalidate child and cache.
// Leaf: InvalidateEdge
// Native0x00bf67f0 length309. Tree RTTI12a8240->FreeableResource->Movable; child callback SubNodeBase slot6.
#include "engine/CLandscapeBackgroundPatch.h"
#include "engine/CMovableResource.h"
#include "engine/CLandscapeBackgroundSubNodeBase.h"
#include "engine/CLandscapeBackgroundTreeNode.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
namespace Native {
struct Movable {
 virtual ~Movable() {} // native base VT0129b860
 virtual void RelocateData(long,void*,void*,long)=0; //slot1
 virtual bool CanRelocateData(long) const; //slot2
 virtual unsigned long Verify() const; //slot3
};
enum LANDSCAPE_EDGE_STRIP_SIDE { Side0=0,Side1=1,Side2=2,Side3=3 };
struct SubNodeBase:Movable {
 virtual bool IsPatch()=0;
 virtual void UpdateTesselation();
 virtual void InvalidateEdge(long,long,long,LANDSCAPE_EDGE_STRIP_SIDE);
 virtual void OnPostRelocation();
 virtual void OnPostParentRelocation(CLandscapeBackgroundTreeNode*);
};
struct EngineMap { unsigned char State[0x60];unsigned short WorldPosX,WorldPosY; }; // donor +64/66, retail release-list shrink4
struct LandscapeMap { unsigned char State[0x14];EngineMap* Map; };
struct FreeableResource:Movable { FreeableResource();virtual ~FreeableResource();virtual bool FreeableResourceFreeBlock()=0; };
struct Tree:FreeableResource {
 // Retail BF6E20 constructor places these at +4/+8; donor base LRU links are absent.
 void* DynamicBuildTemplate;void* GPUTemplate;
 unsigned char LODLoadInfo[0x80]; // native CLODLoadInfo region +0x0c..+0x8c
 long FileBlockPos,FileBlockSize,OffsetIntoFileBlock;void* AsyncLoadTask;Tree* Parent;
 SubNodeBase* SubNode;LandscapeMap* Map;
 unsigned char BoundsAndState[0x30];unsigned short MapX,MapY,Width,Height;
 unsigned char FirstBand,FirstNonSplitBand,LastBand;bool TesselationValid;
 Tree(LandscapeMap*,Tree*,long); // donorA3E4F0 actual owner constructor signature
 void RelocateData(long,void*,void*,long);bool FreeableResourceFreeBlock();
 private:virtual bool CanRelocateData(long)const; //native BF6B50, PDBA3DD50
 public:
 void InvalidateEdge(long,long,long,LANDSCAPE_EDGE_STRIP_SIDE);
};
void Tree::InvalidateEdge(long x,long y,long length,LANDSCAPE_EDGE_STRIP_SIDE side) {
 if(side==Side0 || side==Side1) {
  if(x<MapX+Map->Map->WorldPosX || x>Width+MapX+Map->Map->WorldPosX)return;
  if(y+length<=MapY+Map->Map->WorldPosY || y>=Height+MapY+Map->Map->WorldPosY)return;
  if(SubNode)SubNode->InvalidateEdge(x,y,length,side);
  TesselationValid=false;
 } else {
  if(y<MapY+Map->Map->WorldPosY || y>Height+MapY+Map->Map->WorldPosY)return;
  if(x+length<=MapX+Map->Map->WorldPosX || x>=Width+MapX+Map->Map->WorldPosX)return;
  if(SubNode)SubNode->InvalidateEdge(x,y,length,side);
  TesselationValid=false;
 }
}
}