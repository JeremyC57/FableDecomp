// C3DMeshObject::GetAllDummyObjectNames(std::vector<CCharString>& names) const -- append the mesh statistics' dummy-object names (unsorted)
// Leaf: ?GetAllDummyObjectNames@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35B30, 18 bytes, `ret 4`.
// Identity: Ego_r.pdb 0x00B8C740 + FableWin.pdb 0x031F89B0 ?GetAllDummyObjectNames@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@
//   V?$allocator@VCCharString@@@std@@@std@@@Z; called by the landed CTCGraphicAppearance::GetAllDummyObjectNames 0x004BDB40 as
//   `object.GetAllDummyObjectNames(names)` (it binds this spelling).
// Callee: C3DMeshStats::GetAllDummyObjectNames(std::vector<CCharString>&, bool) const 0x00A2F0F0 (landed).
// Shape: `mov eax,[esp+4]; mov ecx,[ecx+8]` (the counted pointer's Data, Mesh +8), push false, push names, call, `ret 4`.
// Name fix (NF217, 2026-11-21, q12368): moved onto the shared engine/C3DMeshObject.h (FABLE_RETAIL_C3DMESHOBJECT_GETNAMES) and
//   engine/C3DMeshStats.h (FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES); the July stand-in ?fn_00a35b30@@YIXPAUBL@@HH@Z (local N / BL models)
//   and its callee stand-in ?h@N@@QAEXHH@Z are gone; the call binds the PDB ?GetAllDummyObjectNames@C3DMeshStats@@QBE...@std@@_N@Z.
#define FABLE_RETAIL_C3DMESHOBJECT_GETNAMES
#define FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES
#include "engine/C3DMeshObject.h"
#include "engine/C3DMeshStats.h"

void C3DMeshObject::GetAllDummyObjectNames(std::vector<CCharString>& names) const
{
    Mesh->GetAllDummyObjectNames(names, false);
}
