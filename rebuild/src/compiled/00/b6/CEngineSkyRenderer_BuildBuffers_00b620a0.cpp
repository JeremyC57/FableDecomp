// CEngineSkyRenderer::BuildBuffers() -- allocates, fills and unlocks sky geometry buffers.
// Retail00b620a0 length242. Actual SkyRenderer header matches native buffers+58/+8c.
// Vertex buffer base is opaque in its actual header; byte length is native+8,
// established by00a63150 storing vertexCount*stride there. No virtual slots invented.
#include "engine/CEngineSkyRenderer.h"
#include <string.h>
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); its out-of-line buffer getters from the retail branch of
// engine/CEngineResourceManager.h (Ego_r.pdb GetVertexBufferMemPool 0x00B3B090 (_VBPOOL), GetVertexBufferUsage 0x00B3B030,
// GetIndexBufferMemPool 0x00B3B0A0, GetIndexBufferUsage 0x00B3B060); was a CSkyBufferManager / gSkyBufferManager stand-in.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_VBPOOL
#include "engine/CEngineResourceManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
struct CSkyVertexBufferCalls {
 bool Create(unsigned format,unsigned count,unsigned stride,unsigned usage,unsigned pool); //00a63150
 void* Lock(unsigned offset,unsigned flags,unsigned bytes); //00ac20a0
 void Unlock(); //00ac20b0
};
struct CSkyIndexBufferCalls {
 void Create(unsigned bytes,unsigned usage,unsigned format,unsigned pool,unsigned primitiveType); //00ac1f70
 unsigned GetByteLength(); //00ac1f10
 void* Lock(unsigned offset,unsigned flags,unsigned bytes); //00ac1fb0
 void Unlock(); //00ac1f00
};
struct CSkyRendererMethods:CEngineSkyRenderer {
 void BuildBuffers();
 void BuildSky(void*& vertices,void*& indices,unsigned indexBase,unsigned vertexBase); //00b61b30
 void BuildClouds(void*& vertices,void*& indices,unsigned indexBase,unsigned vertexBase); //00b61dd0
 void BuildBand(void*& vertices,void*& indices,unsigned indexBase,unsigned vertexBase); //00b61cd0
 void BuildSolidBaseBand(); //00b61ff0
};
void CSkyRendererMethods::BuildBuffers() {
 CSkyVertexBufferCalls* vb=reinterpret_cast<CSkyVertexBufferCalls*>(VertexBuffer);
 vb->Create(0x142,0x1bb,24,NEngineInternal::GPResourceManager->GetVertexBufferUsage(),NEngineInternal::GPResourceManager->GetVertexBufferMemPool());
 CSkyIndexBufferCalls* ib=reinterpret_cast<CSkyIndexBufferCalls*>(IndexBuffer);
 ib->Create(0x10e0,NEngineInternal::GPResourceManager->GetIndexBufferUsage(),101,NEngineInternal::GPResourceManager->GetIndexBufferMemPool(),4);
 unsigned byteLength; memcpy(&byteLength,VertexBuffer+8,4);
 void* vertices=vb->Lock(0,0,byteLength);
 void* indices=ib->Lock(0,0,ib->GetByteLength());
 BuildSky(vertices,indices,0,0);
 BuildClouds(vertices,indices,0x144,0x49);
 BuildBand(vertices,indices,0x804,0x196);
 vb->Unlock();ib->Unlock();BuildSolidBaseBand();
}