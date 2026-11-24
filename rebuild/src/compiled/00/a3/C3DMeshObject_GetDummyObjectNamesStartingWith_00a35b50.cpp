// C3DMeshObject::GetDummyObjectNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix) const -- append the mesh statistics' dummy-object names that start with the prefix (unsorted)
// Leaf: ?GetDummyObjectNamesStartingWith@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35B50, 23 bytes, `ret 8`.
// Identity: Ego_r.pdb 0x00B8C760 + FableWin.pdb 0x031F89E0 ?GetDummyObjectNamesStartingWith@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@
//   V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@@Z; called by the landed CTCGraphicAppearance::GetDummyObjectNamesStartingWith
//   0x004BDD70 as `object.GetDummyObjectNamesStartingWith(names, prefix)` (respelled onto this spelling in the same hand-back; 0x004BDD70
//   bound the stand-in ?GetDummyObjectNamesStartingWith@C3DMeshObject@@QBEXAAU?$CStdVector@VCCharString@@@@ABVCCharString@@@Z).
// Callee: C3DMeshStats::GetDummyObjectNamesStartingWith(std::vector<CCharString>&, const CCharString&, bool) const 0x00A2F150 (landed).
// Shape: `mov eax,[esp+8]; mov edx,[esp+4]; mov ecx,[ecx+8]` (the counted pointer's Data, Mesh +8), push false, prefix, names, call,
//   `ret 8`.
// Name fix (NF217, 2026-11-21, q12370): moved onto the shared engine/C3DMeshObject.h (FABLE_RETAIL_C3DMESHOBJECT_GETNAMES) and
//   engine/C3DMeshStats.h (FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES); the July stand-in (a free __fastcall ConstructString over local
//   CWideString / Inner models; catalog name NGameText::ConstructString is wrong) and its callee stand-in ?Build@Inner@@QAE_NPAXJH@Z are
//   gone; the call binds the PDB ?GetDummyObjectNamesStartingWith@C3DMeshStats@@QBE...@std@@ABVCCharString@@_N@Z.
#define FABLE_RETAIL_C3DMESHOBJECT_GETNAMES
#define FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES
#include "engine/C3DMeshObject.h"
#include "engine/C3DMeshStats.h"

void C3DMeshObject::GetDummyObjectNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix) const
{
    Mesh->GetDummyObjectNamesStartingWith(names, prefix, false);
}
