// CBankFile::SetBankUpdateStatus -- forward to the bank's progress callback (CBankFile::CProgressCallback, +0xEC) when one is set
// Leaf: ?SetBankUpdateStatus@CBankFile@@IAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBD30, thiscall; `mov eax,[ecx+0ECh] ; test eax,eax ; je R ; mov ecx,eax ;
//   mov eax,[ecx] ; jmp [eax+4] ; R: ret 4` (the callback's virtual call is a tail jump).
// Name fix (NF243, 2026-11-22, CODEXP27 L108): was the stand-in `CScriptedThingOwner_Retail::func_009cbd30(long)` on a row-local
//   CScriptedThingOwner_Retail model (the manifest class CScriptGameResourceObjectScriptedThingBase was propagated). Two sources:
//   Ego_r.pdb and FableWin.pdb S_PUB32 ?SetBankUpdateStatus@CBankFile@@IAEXABVCCharString@@@Z; the body is the PDB's `if (ProgressCallback) ProgressCallback->...`
//   wrapper over CProgressCallback's slots (SetProgress 0, SetStatus 1, WriteLog 2: engine/CBankFile.h), and the three wrappers
//   sit together at 0x009CBD30 / 0x009CBD50 / 0x009CBD70 (SetBankUpdateStatus is called from CBankFile::Update at 0x009CF27B).
//   The engine/CBankFile.h inline bodies (FABLE_RETAIL_CBANKFILE_UPDATE_PASS27) are declared only here
//   (FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL) so the row defines the out-of-line function.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_UPDATE_PASS27
#define FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL
#include "engine/CBankFile.h"
void CBankFile::SetBankUpdateStatus(const CCharString& text)
{
    if (ProgressCallback)
        ProgressCallback->SetStatus(text);
}
