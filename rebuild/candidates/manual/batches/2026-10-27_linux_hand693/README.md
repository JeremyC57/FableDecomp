# Chunk 693 (NA036 + two repairs) -- 7 of 10 new rows landed, both repairs re-landed (2026-11-01)

Seven new rows landed (`verify.log`, every row RELOCATION_MATCH + PASS; bindings multi-check 0): zlib 1.2.1 `deflate_fast` 0x00C013C0 (compiled as C,
`// Flags: /TC` -- as C++ the same text is 922v909; configuration_table entries 1-3), the libpk DSP Process rows CVtable_012bed7c::Process 0x00C6CC60
(plain C++) and CVtable_012b8ac8::Process 0x00C6FE10 (grade `retail_asm`: its TU repeats the landed retail_asm MMX ramp kernel 0x00C6FC80 verbatim,
because VC7.1 passes that static's `initial` in ecx only to a same-TU caller -- the precedent of the SSE2 Process 0x00C6D630; Process itself is C++),
and four quest-script bodies: NScript::CQ_ArenaHoldingScriptScript::Main 0x00CF0790, CQ_HerosOldHouseScript::Main 0x00D89E60,
CQ_PrisonWardenGameScript::Main 0x00DDDE20 and CPersonalScript_EmptyScript::CVillager::Main 0x00EF8D30 (slot 2 / entity slot 1 of the vtables the
landed ctors store). 0x00EF8D30 reproduces retail's "~CScriptThing inline at the first return, out of line after" split with a plain inline header
dtor -- no `#pragma inline_depth`.

Repairs (landed sources that DIFFER on the current tree; the coordinator had confirmed both before wave 23): 0x00BC9C30 (the
CEngineSubPrimitiveSpriteTrail ctor: no Leaf line, so vland graded the out-of-line base ctor; parameter spelled CEngineInternalPrimitiveBase* where the
PDB has CEngineInternalPrimitiveSpriteTrail*; local base model -> engine/CBaseClassNonCopyable.h) and 0x009CDF50 (CBankFile::WriteRecoveryFile: no Leaf,
an invented CBankFileRuntime class, and the "BREC" signature copied from a global into a char[5] below the file object -- the 8-byte frame shift; now
`char id[5] = "BREC";`, which /GS places under the cookie as retail does). The coordinator removed both old landings (files, ps1 entries, 10 binding
rows) and re-landed them through vland --land: RELOCATION_MATCH + PASS, renamed to CEngineSubPrimitiveSpriteTrail_CEngineSubPrimitiveSpriteTrail_00bc9c30.

16 labels applied (`ghidra_out/labels_chunk693_2026-10-27.tsv`): CBankFile::GetRecoveryFilePath / InternalWriteEntryList, CFileDataOutputStream ctor /
Write, ErrorDialogProc, ~CIsThingAlive 0x00E241B0, WardenKeyMiniGame 0x00DDE230, the libpk Process rows. Decisions (coordinator, under the owner
delegation): g_pAudioMaster is `struct CAudioMaster` (most landed libpk rows and the SSE2 / MMX siblings; queued c693); 0x0099D8C0 stays parked --
`tagRECT rects[2]` fixes the frame but is not shown to be retail's shape (two named RECT locals are the Windows idiom); the landed CUndeadFellow::Main
0x00EF87D0 and CLookAtMe1::Main 0x00EF5CB0 use `#pragma inline_depth`, a device retail never had -> repair rows in chunk 694 with the 0x00EF8D30 shape.

Parked: CVtable_012bedb8::Process 0x00C6E8F0 (673v673, 12 probes) and CVtable_012bed90::Process 0x00C6DBA0 (689v689, 14) -- register choice only;
ErrorDialogProc 0x0099D8C0 (685v689, 19). Best sources in `../../pending/2026-10-27_linux_hand693/differ/`. NAME_FIXES.tsv: 14 rows (queued c693).
Header debt: engine/CBankFile.h / CDiskFileWin32.h / CDataOutputStream.h / CFileDataOutputStream.h are flat layouts without methods or virtuals;
engine/CEngineInternalPrimitiveBase.h includes <stddef.h> (hangs cl in the lane); no shared CPredicate_And / CEntityScriptBinding headers.
