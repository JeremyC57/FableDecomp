// C3DMeshStats::GetDummyObjectNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool sort) const -- UnpackNames(names, true, prefix, sort): collect the packed dummy-object names that start with the prefix
// Leaf: ?GetDummyObjectNamesStartingWith@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2F150, 25 bytes (+ int3 padding), `ret 0Ch`.
// Identity: Ego_r.pdb 0x00B8E980 / FableWin.pdb 0x031E4C70
//   `?GetDummyObjectNamesStartingWith@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@...@std@@ABVCCharString@@_N@Z`
//   (three arguments, `ret 0Ch`; the catalog's "ConstructString" label is a descriptive guess). The only caller is the
//   landed forwarder 0x00A35B50 (the mesh object passes its statistics object's GetDummyObjectNamesStartingWith(names,
//   prefix, false)). The sibling GetAllDummyObjectNames 0x00A2F0F0 passes an empty CCharString temporary instead of the prefix.
// Callee: C3DMeshStats::UnpackNames(vector<CCharString>&, bool dummy_objects, const CCharString& prefix, bool sort) const
//   @ 0x00A2ED10 (private; `true` selects the dummy-object name table).
// Shape: a plain __fastcall forwarder that cannot tail-jump (four arguments vs three): `sort` and `prefix` are loaded
//   first, the constant pushed, then `names` reloaded from its slot; `this` stays in ecx.
// Name fix (NF214, 2026-11-21, q12343): moved onto the shared engine/C3DMeshStats.h (FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES) and
//   fable_string.h; the local C3DMeshStats / CCharString models and the CCharStringVector stand-in are gone. Own leaf: the PDB
//   std::vector<CCharString>& form (was the stand-in ?GetDummyObjectNamesStartingWith@C3DMeshStats@@QBEXAAUCCharStringVector@@
//   ABVCCharString@@_N@Z); the UnpackNames call binds the PDB private const ?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@...
//   (was ?UnpackNames@C3DMeshStats@@QBEXAAUCCharStringVector@@_NABVCCharString@@1@Z).
#define FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES
#include "engine/C3DMeshStats.h"

void C3DMeshStats::GetDummyObjectNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool sort) const
{
    UnpackNames(names, true, prefix, sort);
}
