// 0x0066C7A0 -- release the CSurface at +0x24 (CRegionMinimap::RegionMinimapSurface, Ego_r.pdb) if it holds a D3D surface
// Leaf: ?DetachTarget@@YIXPAUS@@@Z
// Naming (NF142, 2026-11-20, q6305): the dt_helper(void*) stand-in replaced by engine/CSurface.h: the +0x24 member of the row's stand-in S
//   is the CSurface whose PD3DSurface (+0x28) the body tests, and the callee 0x009F2E20 is ?Uninitialise@CSurface@@QAEXXZ (Ego_r.pdb
//   0x00B46AF0 + FableWin.pdb 0x030C99C0). The own leaf (a propagated CRenderTarget::DetachTarget label on a free stand-in) is unchanged:
//   its caller 0x00829D90 binds ?FreeData@CRegionMinimap@@QAEXXZ, a name in no PDB (NOTES).
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_GETD3DSURFACE
#include "engine/CSurface.h"
struct S { char pad[0x24]; CSurface f24; };
void __fastcall DetachTarget(S* self){ if(self->f24.GetD3DSurface()) self->f24.Uninitialise(); }
