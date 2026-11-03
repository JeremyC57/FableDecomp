// CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch() -- dispose owned resources, unregister and destroy edge strips.
// Leaf: ??1Patch@
// Native0x00be6090 length284; VT012a803c slots0..8 independently mapped; edge ctor00bfc370.
#include "engine/CLandscapeBackgroundPatch.h"
#include "engine/CMovableResource.h"
#include "engine/CLandscapeBackgroundSubNodeBase.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineWaterBackgroundSubPatch.h"
namespace Native {
struct Movable {
 virtual ~Movable() {} // native base VT0129b860
 virtual void RelocateData(long,void*,void*,long)=0; //slot1
 virtual bool CanRelocateData(long) const; //slot2
 virtual unsigned long Verify() const; //slot3
};
struct EdgeStrip { void* vtable; unsigned short StartPos,Size; void* Verts;long VertCount;void* Fans;long FanCount;void* Water;long WaterCount;void* WaterFans;long WaterFanCount;bool Tesselated,Vertical;
 EdgeStrip(); //0x00bfc370
 ~EdgeStrip(); //0x00bfc410
};
enum LANDSCAPE_EDGE_STRIP_SIDE { Side0=0,Side1=1,Side2=2,Side3=3 };
struct SubNodeBase:Movable {
 virtual bool IsPatch()=0;
 virtual void UpdateTesselation();
 virtual void InvalidateEdge(long,long,long,LANDSCAPE_EDGE_STRIP_SIDE);
 virtual void OnPostRelocation();
 virtual void OnPostParentRelocation(CLandscapeBackgroundTreeNode*);
};
struct Water:Movable { virtual ~Water(); Water(const CEngineLandscapeMap*); void RelocateData(long,void*,void*,long);virtual bool CanRelocateData(long)const; //nativeBFD390
 };
struct LandscapeMemory:CEngineLandscapeMemoryManager {void DeleteIndexBuffer(CIndexBuffer*);void DeleteVertexBuffer(CVertexBufferWin32*);void DeleteTexture(CTexture*);};
struct Renderer { unsigned char State[0x6b0]; LandscapeMemory* MemoryManager;void UnregisterSubPatchForZSorting(long&); };
extern Renderer* gRenderer; //0x01436ea8, donor renderer memory+f58 vs retail+6b0
struct Patch:SubNodeBase {
 CLandscapeBackgroundTreeNode* Parent;EdgeStrip EdgeStrips[4];
 CTexture* Texture;CIndexBuffer* IndexBuffer;CVertexBufferWin32* VertexBuffer;
 CIndexBuffer* BridgeIndexBuffers[4];CVertexBufferWin32* BridgeVertexBuffers[4];unsigned short BridgePolyCount[4];
 const CSubPatchTreeLODLockElement* UVLockTreeU;const CSubPatchTreeLODLockElement* UVLockTreeV;
 Water* WaterSubPatch;float MinZ,MaxZ;long ZSorterHandle;
 unsigned short VertexCount,PolyCountIncDegenerates,TextureWidthNeeded,TextureHeightNeeded,MapX,MapY,Width,Height,LockedGridSpacingX,LockedGridSpacingY;
 unsigned char LODBand;bool SharedIndexBuffer,WaterOnlyPatch,IsDefaultSection;
 virtual ~Patch();
 virtual void RelocateData(long,void*,void*,long);
 virtual bool CanRelocateData(long) const;
 virtual bool IsPatch(); //slot4
 virtual void UpdateTesselation(); //slot5
 virtual void InvalidateEdge(long,long,long,LANDSCAPE_EDGE_STRIP_SIDE); //slot6 enum LANDSCAPE_EDGE_STRIP_SIDE
 virtual void OnPostRelocation(); //slot7
 virtual void OnPostParentRelocation(CLandscapeBackgroundTreeNode*); //slot8
 Patch(CLandscapeBackgroundTreeNode*,float,float,long,long,long,long,long,long,long,const CSubPatchTreeLODLockElement*,const CSubPatchTreeLODLockElement*,long,long,long);
};
Patch::~Patch() {
 for(int i=0;i<4;++i){
  if(BridgeVertexBuffers[i]){gRenderer->MemoryManager->DeleteVertexBuffer(BridgeVertexBuffers[i]);BridgeVertexBuffers[i]=0;}
  if(BridgeIndexBuffers[i]){gRenderer->MemoryManager->DeleteIndexBuffer(BridgeIndexBuffers[i]);BridgeIndexBuffers[i]=0;}
 }
 if(Texture){gRenderer->MemoryManager->DeleteTexture(Texture);Texture=0;}
 if(!SharedIndexBuffer && IndexBuffer){gRenderer->MemoryManager->DeleteIndexBuffer(IndexBuffer);IndexBuffer=0;}
 if(WaterSubPatch){delete WaterSubPatch;WaterSubPatch=0;}
 if(VertexBuffer){gRenderer->MemoryManager->DeleteVertexBuffer(VertexBuffer);VertexBuffer=0;}
 if(ZSorterHandle>=0)gRenderer->UnregisterSubPatchForZSorting(ZSorterHandle);
}
}