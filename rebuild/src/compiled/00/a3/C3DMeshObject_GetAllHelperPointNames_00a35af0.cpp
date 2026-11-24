// C3DMeshObject::GetAllHelperPointNames(std::vector<CCharString>& names) const -- append the mesh statistics' helper-point names (unsorted)
// Leaf: ?GetAllHelperPointNames@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35AF0, 18 bytes, `ret 4`.
// Identity: Ego_r.pdb 0x00B8C700 + FableWin.pdb 0x031F77D0 ?GetAllHelperPointNames@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@
//   V?$allocator@VCCharString@@@std@@@std@@@Z; called by the landed CTCGraphicAppearance::GetAllHelperPointNames 0x004BDA30 as
//   `object.GetAllHelperPointNames(names)` (respelled onto this spelling in the same hand-back; 0x004BDA30 bound the stand-in ?GetAllHelperPointNames@C3DMeshObject@@QBEXAAU?$CStdVector@VCCharString@@@@@Z).
// Callee: C3DMeshStats::GetAllHelperPointNames(std::vector<CCharString>&, bool) const 0x00A2F120 (landed).
// Shape: `mov eax,[esp+4]; mov ecx,[ecx+8]` (the counted pointer's Data, Mesh +8), push false, push names, call, `ret 4`.
// Name fix (NF217, 2026-11-21, q12369): moved onto the shared engine/C3DMeshObject.h (FABLE_RETAIL_C3DMESHOBJECT_GETNAMES) and
//   engine/C3DMeshStats.h (FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES); the July stand-in ?fn_00a35b30@@YIXPAUBL@@HH@Z (local N / BL models; the same symbol 0x00A35B30 defined)
//   and its callee stand-in ?h_00a2f120@N@@QAEXHH@Z are gone; the call binds the PDB ?GetAllHelperPointNames@C3DMeshStats@@QBE...@std@@_N@Z.
#define FABLE_RETAIL_C3DMESHOBJECT_GETNAMES
#define FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES
#include "engine/C3DMeshObject.h"
#include "engine/C3DMeshStats.h"

void C3DMeshObject::GetAllHelperPointNames(std::vector<CCharString>& names) const
{
    Mesh->GetAllHelperPointNames(names, false);
}
