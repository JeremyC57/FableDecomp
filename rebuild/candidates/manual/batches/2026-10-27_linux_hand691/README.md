# Chunk 691 (NA034, 15-1013 band) -- 7 of 10 landed (2026-11-01)

Seven rows landed (`verify.log`, every row MATCH / RELOCATION_MATCH + PASS; bindings multi-check 0): NCameraModes::CCameraPointModeBase::OnDie
0x0084FDB0 (MATCH; vtable 0x0124DDDC slot 22), the STLport `_List_base<CGameScriptThingManager::CScriptThingInfo>::clear` 0x004BA810 (the label
said remove_if), CCRCSymbolMap::LoadFromFile 0x00A39010, the libpk CString::Replace 0x00C0CE50 (MFC strex.cpp text; the label said
CCharString_ReplaceAll), the libpk multi-tap delay `CVtable_012b8a64::Process` 0x00C6D630 as grade `retail_asm` (a hand SSE2 movdqa copy on esi / edi
reloaded from the frame slots the C code had just written -- the inline-asm signature; siblings 0x00C6D370 / 0x00C6D410 / 0x00C6D560 landed the same
way; it reached the payload through the chunk 690 finish.py fix), CScriptDefinitionManager::InitAndCompile 0x00CD3F50 (vtable 0x012C2648 slot 2) and
NScript::CV_BordelloScript::AdjustTavernPrices 0x00E3A6B0 (CSpawnedFunc name string). 22 labels applied (`ghidra_out/labels_chunk691_2026-10-27.tsv`):
CDefStringTable::SaveTable 0x009D4CE0 (landed as SaveBinary), CDefinitionManager::Compile / ClearSymbols, SetGlobalScriptDef, the NBankFileManager
header-dir accessors and global, the vector<CWideString> members, the CCRCSymbolMap helpers, CWideString::operator= 0x0099B800 (labelled
CCharString::AssignFromWide).

Decisions (coordinator, under the owner delegation): the chunk 690 queue spelling of 0x0099A300 was wrong -- the destructor is protected virtual
`??1CBaseClassNonCopyable@@MAE@XZ` (engine/CBaseClassNonCopyable.h cites both PDBs; three landed rows already bind it); corrected in the queue.
0x00D89710 stays parked rather than lifting the chunk brief's ban on `#pragma inline_depth`: retail's one-inline-site split of ~CScriptThing is a
compiler inlining decision, and a pragma region retail's source never had is a modelling device (wave 22 rule). 0x00C08D40 (the 964-byte libpk
.lug/.lut bank loader, CSoundSystem slot 10) was not attempted (agent budget) and is re-queued in the next chunk.

Parked: CTCBob::FrameUpdate 0x007877E0 (487v487, 22 probes: fsin / lea and store scheduling), NScript::CQ_HeroSoulsNostroScript::
DealWithCreatureGeneration 0x00D89710 (903v960, 12 probes: one of six ~CScriptThing sites inline in retail). Best sources in
`../../pending/2026-10-27_linux_hand691/differ/`. NAME_FIXES.tsv: 21 rows (queued c691, + 0x0099B800). Header debt: engine/CDefPointer.h and
CParentDefClassBase.h include <stddef.h>, which hangs cl under q.py; the CDefinitionManager header has Ego_r 16-byte vectors (retail 12); the
CDiskFileWin32 / CTCBase / CShipDef headers are flat pack(1) layouts -> migration wave. Lessons in LESSONS.md.
