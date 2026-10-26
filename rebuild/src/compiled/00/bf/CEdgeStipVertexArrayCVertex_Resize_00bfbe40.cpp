// CEdgeStipVertexArray<CVertex>::Resize(long, CMovableResource*) -- reallocate and preserve the common prefix
// Retail 00BFBE40, 186 bytes after proven overcapture trimming. Original chunk span retained.
// Native layout Data+0 Count+4; PDB vertex layout reproduced with fixed-width integral fields.
// AllocateVirtual 00BD9AF0, FreeVirtual 00BD9B90. Global01436EA8 renderer native MemoryManager+6B0.
// Leaf: Resize
#include <string.h>
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
// Donor renderer MemoryManager+F58 differs from native+6B0; use a documented native view below.
class CMovableResource;
struct CEngineLandscapeMemoryManager_Methods : CEngineLandscapeMemoryManager {
 void* AllocVirtual(int bytes, CMovableResource* owner); // 00BD9AF0
 void FreeVirtual(void* block); // 00BD9B90
};
struct NativeLandscapeRenderer {
 unsigned char RendererState[0x6b0];
 CEngineLandscapeMemoryManager_Methods* MemoryManager;
};
extern NativeLandscapeRenderer* g_LandscapeRenderer; //01436EA8
struct CVertex { float WorldZ; unsigned int Normal; unsigned short WorldX,WorldY,VertexBufferIndex,IndexBufferIndex,FanBaseIndex; unsigned char U,V; };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CVertex)==20);
#endif
struct EdgeArray_CVertex {
 CVertex* Data; int Count;
 bool Resize(int count,CMovableResource* resource);
};
bool EdgeArray_CVertex::Resize(int count,CMovableResource* resource) {
 if(Count!=count) {
  if(count) {
   int copied=Count; if(count<copied)copied=count;
   CEngineLandscapeMemoryManager_Methods* manager=g_LandscapeRenderer->MemoryManager;
   CVertex* data=static_cast<CVertex*>(manager->AllocVirtual(count*sizeof(CVertex),resource));
   if(!data)return false;
   if(copied>0)memcpy(data,Data,copied*sizeof(CVertex));
   if(Data)g_LandscapeRenderer->MemoryManager->FreeVirtual(Data);
   Count=count;Data=data;
  } else {
   g_LandscapeRenderer->MemoryManager->FreeVirtual(Data);
   Data=0;Count=0;
  }
 }
 return true;
}