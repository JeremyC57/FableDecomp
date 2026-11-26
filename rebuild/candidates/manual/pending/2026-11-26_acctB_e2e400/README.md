# 0x00E2E400 NScript::CV_AmbushScamScript::ResetRandomSpeechTime

Landed 2026-11-26 on FullDecomp-B under VC7.1 RTM cl 13.10.3077. Grade RELOCATION_MATCH, behaviour test `00e2e400_TEST PASS`.

PDB spelling, both tables: `?ResetRandomSpeechTime@CV_AmbushScamScript@NScript@@QAEXXZ` (Ego_r 0x00E77A70, FableWin 0x0392E420). The manifest already used that name, so this landing does not queue a NAME_FIXES row.

Retail Fable.exe at 0x00E2E400 is 57 bytes (int3 at 0x00E2E439). The unmatched length estimate was 64. pGlobalScriptDef is 0x0143E90C, pDSTGame is 0x0143E8F8, the MSVCR71 rand thunk is 0x00BFEB16, and SetTimer is vtable slot 89. The timer index is at +0x7C and the def fields are AmbushScamRandomSpeechTimerMax/Var at +0x2B8/+0x2BC.

The 2026-10-28 draft stays at `pending/2026-10-28_chatgpt_pass8/differ/00e2e400.cpp`. That spelling loaded the SetTimer vtable into ebx and was 59 bytes.
