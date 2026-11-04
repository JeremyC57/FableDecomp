// NScript::CV_ParanoidWhispersScript::OnPersist(CPersistContext&) -- empty override (the level script persists nothing of its own)
// Leaf: ?OnPersist@CV_ParanoidWhispersScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBEED0, 3 bytes: ret 4.
// Identity (naming wave 35, family lop): outer script vtable 012f0f38 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ebef80 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CV_ParanoidWhispersScript; PDB ?OnPersist@CV_ParanoidWhispersScript@NScript@@; refs=1; was sub_00ebeed0.
//   ?OnPersist@CV_ParanoidWhispersScript@NScript@@UAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the shared header engine/CV_ParanoidWhispersScript.h (includes as the landed includer rebuild/src/compiled/00/eb/NScriptCVParanoidWhispersScriptCSecretWhisperingMan_Main_00ebfd10.cpp); only this member is graded.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_ParanoidWhispersScript.h"

void NScript::CV_ParanoidWhispersScript::OnPersist(CPersistContext&)
{
}
