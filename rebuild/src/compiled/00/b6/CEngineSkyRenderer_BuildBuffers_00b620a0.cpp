// CEngineSkyRenderer::BuildBuffers() -- allocates, fills and unlocks sky geometry buffers.
// Retail00b620a0 length242. Actual SkyRenderer header matches native buffers+58/+8c.
// Vertex buffer base is opaque in its actual header; byte length is native+8,
// established by00a63150 storing vertexCount*stride there. No virtual slots invented.
#define FABLE_RETAIL_CENGINESKYRENDERER
#include "engine/CEngineSkyRenderer.h"
// NF149 (q4552 / q4555..q4560): the sky header's retail branch (FABLE_RETAIL_CENGINESKYRENDERER) types the buffers as the shared retail classes
// (engine/CVertexBufferWin32.h, engine/CIndexBuffer.h; Ego_r.pdb + FableWin.pdb CVertexBufferWin32::Initialise 0x00A63150,
// CVertexBufferBase::Lock / Unlock 0x00AC20A0 / 0x00AC20B0 and the inline GetSize (Length +0x08), CIndexBuffer::Initialise / GetSize /
// Lock / Unlock 0x00AC1F70 / 0x00AC1F10 / 0x00AC1FB0 / 0x00AC1F00) with the shared buffer enums; were the stand-ins
// CSkyVertexBufferCalls::Create / Lock / Unlock and CSkyIndexBufferCalls::Create / GetByteLength / Lock / Unlock.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); its out-of-line buffer getters from the retail branch of
// engine/CEngineResourceManager.h (Ego_r.pdb GetVertexBufferMemPool 0x00B3B090 (_VBPOOL), GetVertexBufferUsage 0x00B3B030,
// GetIndexBufferMemPool 0x00B3B0A0, GetIndexBufferUsage 0x00B3B060); was a CSkyBufferManager / gSkyBufferManager stand-in.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_VBPOOL
#include "engine/CEngineResourceManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
struct CSkyRendererMethods:CEngineSkyRenderer {
 void BuildBuffers();
 void BuildSky(void*& vertices,void*& indices,unsigned indexBase,unsigned vertexBase); //00b61b30
 void BuildClouds(void*& vertices,void*& indices,unsigned indexBase,unsigned vertexBase); //00b61dd0
 void BuildBand(void*& vertices,void*& indices,unsigned indexBase,unsigned vertexBase); //00b61cd0
 void BuildSolidBaseBand(); //00b61ff0
};
void CSkyRendererMethods::BuildBuffers() {
 VertexBuffer.Initialise(VERTEX_COL_TEX1,0x1bb,24,NEngineInternal::GPResourceManager->GetVertexBufferUsage(),NEngineInternal::GPResourceManager->GetVertexBufferMemPool());
 IndexBuffer.Initialise(0x10e0,NEngineInternal::GPResourceManager->GetIndexBufferUsage(),IB_FORMAT_16,NEngineInternal::GPResourceManager->GetIndexBufferMemPool(),PT_TRIANGLE_LIST);
 void* vertices=VertexBuffer.Lock(0,0,VertexBuffer.GetSize());
 void* indices=IndexBuffer.Lock(0,0,IndexBuffer.GetSize());
 BuildSky(vertices,indices,0,0);
 BuildClouds(vertices,indices,0x144,0x49);
 BuildBand(vertices,indices,0x804,0x196);
 VertexBuffer.Unlock();IndexBuffer.Unlock();BuildSolidBaseBand();
}