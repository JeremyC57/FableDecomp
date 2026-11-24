// CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch() -- dispose owned resources, unregister and destroy edge strips.
// Leaf: ??1Patch@
// Native0x00be6090 length284; VT012a803c slots0..8 independently mapped; edge ctor00bfc370.
// NF265 (2026-11-22): the renderer global, its memory manager and their four calls are the shared classes now (two sources each):
//   0x01436EA8 ?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A (nf256 / nf260 proof; was the Native renderer stand-in);
//   MemoryManager +0x6B0 is the retail CEngineLandscapeMemoryManager, whose public DeleteIndexBuffer 0x00BDA090 / DeleteVertexBuffer
//   0x00BDA0D0 / DeleteTexture 0x00BDA110 (Ego_r.pdb + FableWin.pdb; retail order IB / VB / Tex = the PDBs' public order) were the
//   Native::LandscapeMemory stand-ins; UnregisterSubPatchForZSorting 0x00B677A0 = ?UnregisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJ@Z
//   (Ego_r.pdb + FableWin.pdb; was Native::Renderer). The overlaid headers come first (declaration-only opt-ins).
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETETEXTURE
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_UNREGISTERSUBPATCHFORZSORTING
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CLandscapeBackgroundPatch.h"
#include "engine/CMovableResource.h"
#include "engine/CLandscapeBackgroundSubNodeBase.h"
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
  if(BridgeVertexBuffers[i]){NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(BridgeVertexBuffers[i]);BridgeVertexBuffers[i]=0;}
  if(BridgeIndexBuffers[i]){NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteIndexBuffer(BridgeIndexBuffers[i]);BridgeIndexBuffers[i]=0;}
 }
 if(Texture){NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteTexture(Texture);Texture=0;}
 if(!SharedIndexBuffer && IndexBuffer){NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteIndexBuffer(IndexBuffer);IndexBuffer=0;}
 if(WaterSubPatch){delete WaterSubPatch;WaterSubPatch=0;}
 if(VertexBuffer){NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(VertexBuffer);VertexBuffer=0;}
 if(ZSorterHandle>=0)NEngineInternal::GPLandscapeRenderer->UnregisterSubPatchForZSorting(ZSorterHandle);
}
}