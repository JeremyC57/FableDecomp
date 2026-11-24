// CEdgeStipVertexArray<CFanBaseVertex>::Resize(long, CMovableResource*) -- reallocate and preserve the common prefix
// Retail 00BFBF40, 185 bytes after proven overcapture trimming. Original chunk span retained.
// Native layout Data+0 Count+4; PDB vertex layout reproduced with fixed-width integral fields.
// AllocateVirtual 00BD9AF0, FreeVirtual 00BD9B90. Global01436EA8 renderer native MemoryManager+6B0.
// Leaf: Resize
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER                // AllocVirtual 0x00BD9AF0 / FreeVirtual 0x00BD9B90 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include <string.h>
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
// NF262: the renderer is the shared CEngineLandscapeRenderer (MemoryManager +0x6B0) through NEngineInternal::GPLandscapeRenderer
// (both PDBs, 0x01436EA8); was the NativeLandscapeRenderer / CEngineLandscapeMemoryManager_Methods stand-ins.
class CMovableResource;
struct CFanBaseVertex { float WorldZ; unsigned int Normal; unsigned short WorldX,WorldY,VertexBufferIndex; unsigned char U,V; };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CFanBaseVertex)==16);
#endif
struct EdgeArray_CFanBaseVertex {
 CFanBaseVertex* Data; int Count;
 bool Resize(int count,CMovableResource* resource);
};
bool EdgeArray_CFanBaseVertex::Resize(int count,CMovableResource* resource) {
 if(Count!=count) {
  if(count) {
   int copied=count<Count?count:Count;
   CEngineLandscapeMemoryManager* manager=NEngineInternal::GPLandscapeRenderer->MemoryManager;
   CFanBaseVertex* data=static_cast<CFanBaseVertex*>(manager->AllocVirtual(count*sizeof(CFanBaseVertex),resource));
   if(!data)return false;
   if(copied>0)memcpy(data,Data,copied*sizeof(CFanBaseVertex));
   if(Data)NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(Data);
   Count=count;Data=data;
  } else {
   NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(Data);
   Data=0;Count=0;
  }
 }
 return true;
}