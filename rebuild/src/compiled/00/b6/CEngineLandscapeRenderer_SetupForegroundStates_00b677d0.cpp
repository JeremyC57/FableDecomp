// CEngineLandscapeRenderer::SetupForegroundStates() -- the foreground landscape pass's texture-stage-1 sampler states: U and V address modes and the minification filter, each written through the render-state manager's cached SetRenderState (skip an unchanged value; otherwise bookmark the old value on the state stack once per capture block, store the new one and queue the state for the device update)
// Leaf: ?SetupForegroundStates@CEngineLandscapeRenderer@@AAEXXZ
// Grade: RELOCATION_MATCH (retry wave 2) -- the three SetTexture* inline helpers take the D3D value by const reference,
//   so the value stays a memory operand (as retail's table lookup would be) and the core-pointer copy precedes the value
//   copy in blocks 2 and 3. NOTE: Ego_r.pdb spells these helpers SetTextureUVModeU/V(long, TEXTURE_UV_MODE) and
//   SetTextureFilterModeMin(long, TEXTURE_FILTER_MODE) by value; the retail helpers most likely index a D3D-value table
//   by the enum (0x01396DC8 / 0x01396E60 are table entries), which this const& models without knowing the table bases
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B677D0, length 333 bytes, plain `ret`; `this` is never read. Single caller
//   0x00B6B17E (CEngineLandscapeRenderer::Render).
// Name: Ego_r.pdb `?SetupForegroundStates@CEngineLandscapeRenderer@@AAEXXZ` (private; the catalog binding spells QAE).
// Callees: none (everything inline).
// Globals: g_pRenderManagerCore 0x01436E18 (re-read after every state, as retail does); the two D3D values come from the
//   engine's enum-to-D3D lookup tables -- 0x01396DC8 (texture address table entry 0 = 1, D3DTADDRESS_WRAP; existing
//   descriptive binding g_renderStateValue_01396dc8) and 0x01396E60 (filter table entry = 3, D3DTEXF_ANISOTROPIC; descriptive).
// Layout: CRenderManagerCore derives from CRenderStateManager (Ego_r.pdb sizes: the manager is 0x3A3C bytes and the core's
//   own DisplayManager starts at +0x3A3C). CRenderStateManager (Ego_r.pdb names): StateStack +0x8, StateUpdateList +0x2008,
//   StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, DeviceState +0x2818; CDeviceState
//   TextureUVModeU[8] +0x6CC, TextureUVModeV[8] +0x76C, TextureFilterModeMin[8] +0x8AC; CStateInfo (0x14): DesiredState +4,
//   BookmarkMask +8, DirtyListFlag +0x10. Stage 1 entries: +0x2EF8 / +0x2F98 / +0x30D8, as retail addresses them.
//   engine/CRenderStateManager.h keeps StateStack / DeviceState opaque, so the nested types are added on the donor layout.
// Shape: the PDB's public setters SetTextureUVModeU / SetTextureUVModeV / SetTextureFilterModeMin (stage, mode) map the mode
//   through a lookup table and call the private SetRenderState(value, state) (modelled here with the looked-up value); the
//   bookmark test reads the manager mask first, and the dirty flag is tested before the new value is stored.
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderStateManager at offset 0), and the three
//   states are the shared inline CRenderStateManager setters with their engine enums: stage 1 TextureUVModeU / TextureUVModeV
//   [TEXTURE_UV_WRAP] through D3DTextureUVModeTable 0x01396DC8 and TextureFilterModeMin [TEXTURE_FILTER_ANISOTROPIC] through
//   D3DTextureFilterModeTable 0x01396E58 (+8). Was a CRenderStateManager model derived from the header under a renaming macro, an
//   empty local CRenderManagerCore, g_pRenderManagerCore and the address-named g_renderStateValue_01396dc8 / _01396e60.
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

#define CEngineLandscapeRenderer CEngineLandscapeRendererLayout
#include "engine/CEngineLandscapeRenderer.h"
#undef CEngineLandscapeRenderer
class CEngineLandscapeRenderer : public CEngineLandscapeRendererLayout {
private:
    void SetupForegroundStates();
};

void CEngineLandscapeRenderer::SetupForegroundStates()
{
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_ANISOTROPIC);
}