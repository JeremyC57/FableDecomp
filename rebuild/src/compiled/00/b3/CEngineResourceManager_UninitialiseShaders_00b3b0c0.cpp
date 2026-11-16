// CEngineResourceManagerNative::UninitialiseShaders -- unbind stream/index/textures, close the populated vertex shader resources and their banks. Retail00b3b0c0 complete189bytes+3int3. Shared CEngineResourceManager/CShaderDataBank differ materially: donor bank1a8/resourcesc versus nativebank128/resources8, nativeVShaderBanks960/resources29c0 versusdonora94/38f4. Localnativeview required. Shared CRenderManagerCore direct exact fields. COM slots65 SetTexture,100 SetStreamSource,104 SetIndices; no virtual COM destructor. ResourceCloseShaderBank00a5d750; bankClose virtualslot5=00a62110 genuine inheriteddtor slot0.
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0, members
//   TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0, CurrentlySelectedTextures +0x3D00) on the shared engine/IDirect3DDevice9.h (was
//   g_pRenderManagerCore through the flattened header, a local IDirect3DDevice9 model and a non-MSVC fallback branch). The unbind
//   helper keeps its descriptive name: no PDB inline matches it (SelectIndexBuffer also stores PSelectedIndexBuffer).
#include "engine/CRenderManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderDataBank.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
class CShaderResource { public:void CloseShaderBank();}; //00a5d750
struct CShaderDataBankNative {
 virtual ~CShaderDataBankNative();
 virtual void ReservedSlot1();virtual void ReservedSlot2();virtual void ReservedSlot3();virtual void ReservedSlot4();virtual void Close(); //00a62110
 unsigned char NativeMembers[0x124];
};
struct CShaderResourcePointer {CShaderResource* Data;void* Info;};
struct CEngineResourceManagerNative {
 unsigned char EarlierMembers[0x960];CShaderDataBankNative VShaderBanks[27];unsigned char PixelShaderAndOtherData[0x128];CShaderResourcePointer VShaderResources[27];
 void UninitialiseShaders();
};
static FABLE_FORCEINLINE void UnbindBuffersAndTextures(CRenderManagerCore* core)
{
 IDirect3DDevice9*& device=core->PD3DDevice;
 device->SetStreamSource(0,0,0,0);
 device->SetIndices(0);
 for(long stage=0;stage<core->TextureStagesInUse;++stage){if(core->CurrentlySelectedTextures[stage]){core->CurrentlySelectedTextures[stage]=0;device->SetTexture(stage,0);}}
 core->TextureStagesInUse=0;
}
void CEngineResourceManagerNative::UninitialiseShaders()
{
 UnbindBuffersAndTextures(NEngineInternal::GPRenderManager);
 for(long i=0;i<27;++i){if(VShaderResources[i].Data){VShaderResources[i].Data->CloseShaderBank();VShaderBanks[i].Close();}}
}