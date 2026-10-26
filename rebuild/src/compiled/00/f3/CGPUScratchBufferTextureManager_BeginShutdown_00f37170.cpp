// CGPUScratchBufferTextureManager::BeginShutdown() -- detach GPU textures, dispose resources, and release non-restorable pages once.
// Retail00f37170 length305. Manager header unavailable; native arrays12 vs donor16,
// resource entries44, pages28 vs donor32, ShutdownFlag40 vs donor54.
#include "engine/CTexture.h"
#define CSurface CSurfaceDonorLayout
#include "engine/CSurface.h"
#undef CSurface
#include "engine/CRenderManagerCore.h"
#include "engine/CPixelFormat.h"
#include <new>
struct CScratchTexture:CTexture {void Uninitialise();~CScratchTexture(){Uninitialise();}}; //009f9f70
// Actual surface layout with its real virtual destructor restored. Retail VT122F84C
// slot0 scalar destructor41C9C0 calls Uninitialise9F2E20 and optionally scalar delete.
struct CScratchSurface {virtual ~CScratchSurface();IDirect3DSurface9* PD3DSurface;unsigned AllocationSource;void* PAllocatedMemory;};
struct CScratchRenderMethods:CRenderManagerCore {
 void PrepareShutdown(); //00a0af80 true no-op member
 void AttachTextureToStage(int,CTexture*); //009a0cf0
};
// System/display headers unavailable; PDB System.PDisplayManager96, Display.PRenderManager8.
struct CScratchDisplay {void* VTable;void* SystemManager;CScratchRenderMethods* PRenderManager;};
class CSystemManager { public:unsigned char Prefix[0x60];CScratchDisplay* PDisplayManager;};
CSystemManager* __fastcall GFGetSystemManager(); //009a4ec0
struct CScratchResource {
 int ResourceType;CScratchTexture* Texture;CScratchSurface* Surface;int PageIndex;
 int Width,Height,NoLevels;CPixelFormat PixelFormat;int MultiSampleType;unsigned UsageFlags;int MemoryPool;
};
template<class T>struct CScratchArray {
 T* Begin;T* End;T* Capacity;
 unsigned Size()const{return unsigned(End-Begin);}
 T& operator[](unsigned i){return Begin[i];}
};
struct CScratchPage {int ScratchPool;CScratchArray<int> ResourceList;unsigned char* NextFreeBufferAddress;int FreeSpaceSize;bool EnableAutoRestore;};
class CGPUScratchBufferTextureManager { public:
 void* VTable;CScratchArray<CScratchResource> Resources;CScratchArray<CScratchPage> MemoryPages;
 CScratchArray<int> ScratchPools,FreeTexturePageSlots,FreeTextureSlots;bool ShutdownFlag;
 bool BeginShutdown();
public:
    void FreeMemoryPage(long);   // 0x00F37030
};
bool CGPUScratchBufferTextureManager::BeginShutdown(){
 if(!ShutdownFlag){
 CScratchRenderMethods* renderer=GFGetSystemManager()->PDisplayManager->PRenderManager;
 renderer->PrepareShutdown();
 for(int i=0;i<4;++i)renderer->AttachTextureToStage(i,0);
 for(unsigned i=0;i<Resources.Size();++i){
  CScratchResource& r=Resources[i];delete r.Texture;delete r.Surface;r.Texture=0;r.Surface=0;
 }
 for(unsigned i=0;i<MemoryPages.Size();++i){
  if(MemoryPages[i].ScratchPool>-1&&!MemoryPages[i].EnableAutoRestore)FreeMemoryPage(i);
 }
 ShutdownFlag=true;return true;
 }return false;
}