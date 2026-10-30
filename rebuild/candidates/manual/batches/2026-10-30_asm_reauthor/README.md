# asm_bake re-author pass (2026-10-30): 9 of 27 asm rows re-authored, 8 as plain C++ and 1 as retail_asm

Input: the 27 landed rows graded `asm_bake` (whole bodies transcribed as `__declspec(naked)` / `__asm`; list in migration wave 6
`logs/asm_bake.tsv`, copied here as rows.tsv). Owner question: were they asm because retail was? Answer: 22 were ordinary C++
in retail (group `reauthor`), 5 are CRT / cpuid / import thunks (group `crt_or_cpuid`). Brief BRIEF.md; per-row STATUS.tsv
(status, grades used, best residue); NOTES.md; LESSONS.md; NAME_FIXES.tsv (28 proven spellings); differ/ (best C++ for the 13
rows that still differ); payload.json / retail_asm_payload.json; oracle.tsv (6 rows added for the uncatalogued files, 00551340
length corrected 428 -> 431).

Landed as C++ (RELOCATION_MATCH + PASS; the asm file replaced in place, the catalog `Grade = 'asm_bake'` line dropped):
00c8db10 libogg `ogg_stream_packetout` (/Gr; the catalog label `std::_Dest_val<..CFlash>` was propagated; file renamed),
0099bf30 / 0099be70 `operator+(const CWideString&, ...)` (new catalog entries; the asm files had none), 0042f722
CNewFrontendGameComponent::InitialiseDefs, 0042e204 InitialiseEngine (same-TU GetDef<CEngineDef> body), 00536b4f
NUISystem::CList::InitialiseOffsets (`1.0f - GetAlphaOffset()*half` gives retail's fsubr), 00413c50 InitialiseConsoleVariables
(4,158 bytes, was a raw `_emit` blob: 25 AddInput registrations through the inlined CTBaseSingleton<CConsole>::Get()),
009c85a0 NGameText::CDataBank ctor (already landed as C++; the asm twin file was deleted).
Landed as retail_asm: 00a5b850 NVectorMath::GFInitVectorMath (`cpuid`, no VC7.1 intrinsic; `// Retail-asm: 0x00A5B856 cpuid`).
Coordinator: landed the 3 uncatalogued rows with `vland.py --land` (verify_land.log, 3 wins, bindings written by the lander);
rechecked the 5 in-place rows plus the 3 RunScript callers (verify_recheck.log, 7/7; 00c8db10 needed `CompilerFlags = '/Gr'`
in its catalog entry, verify_recheck_00c8db10.log). Respelled 0x009EC890 `ExecuteConfigFile` -> `?RunScript@CConsole@@QAEXABVCWideString@@@Z`
(both PDBs) in rows 009ecb70 / 0061ab30 / 0061ac60 and the binding. Labels ghidra_out/labels_asm_reauthor_2026-10-30.tsv.
22 NAME_FIXES rows queued for naming wave 11 (prefix asmre).
Still asm (the asm file stays as the oracle, grade asm_bake, 16 catalog entries + 2 uncatalogued): 11 differ (0042df9e Draw
478v478 one spill order, a retail_asm candidate for its fnstcw/fldcw; 00597bf2 517v517; 0066ff20 175v175 register picks;
00551340 431v431 frame 0x10 v 0x0c; 007022c0 22v22 exit order; 004167a7 33v33 x87 pop placement, RTM and QFE alike;
009bef80 478v479; 00899ef0 154v150 `push ecx` frame; 005bd993 30v31; 005dacf0 19v22 setne v two exits; 0053c332 1119v1111),
3 not attempted (009a8840 OpenRetailBank 1.5 KB, 009a8170 OpenIniFile 1.7 KB, 00c8c730 `_ov_d_seek_lap` whose callees take vf
in esi with no save: a lane gap), 4 CRT / import library (00bfe9f9 `__security_check_cookie`, 004013c7 `__SEH_epilog`,
0040138c `__SEH_prolog`, 00401284 the `jmp [__imp__exit]` thunk): these should come from the VC7.1 CRT and import library,
not reconstruction; they stay asm_bake oracles until the owner decides.
Flagged, not changed (NOTES.md): bound globals 0x013B8646 g_bLoadFromAnyDirectory ("UseRetailSaveGameSystem"), 0x013B8616
g_bRuntimeLevelFiles ("UseRetailBanks"), 0x01375459 g_WADBuildEnabled ("AllowDataGeneration") disagree with their console
names but are not provably wrong. Header debt: engine/CDefPointer.h has no null constructor (00536b4f, 00413c50 use local models).
