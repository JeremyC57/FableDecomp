# Chunk 691 notes

7/10 rows graded (1 MATCH, 6 RELOCATION_MATCH incl. one retail_asm), 2 parked DIFFER, 1 not authored.

| row | identity | grade | probes |
|---|---|---|---|
| 0x0084FDB0 | NCameraModes::CCameraPointModeBase::OnDie | MATCH 15v15 | 1 |
| 0x004BA810 | std::_List_base<CGameScriptThingManager::CScriptThingInfo>::clear | RELOCATION_MATCH 89v89 | 1 |
| 0x00A39010 | CCRCSymbolMap::LoadFromFile | RELOCATION_MATCH 378v378 | 1 |
| 0x00C0CE50 | libpk CString::Replace(const char*, const char*) | RELOCATION_MATCH 681v681 | 1 |
| 0x00C6D630 | CVtable_012b8a64::Process (retail_asm, /GS) | RELOCATION_MATCH 808v808 | 4 |
| 0x00CD3F50 | CScriptDefinitionManager::InitAndCompile | RELOCATION_MATCH 867v867 | 12 |
| 0x00E3A6B0 | NScript::CV_BordelloScript::AdjustTavernPrices | RELOCATION_MATCH 1013v1013 | 2 |
| 0x007877E0 | CTCBob::FrameUpdate | DIFFER 487v487 (6 lines of scheduling) | 22 |
| 0x00D89710 | NScript::CQ_HeroSoulsNostroScript::DealWithCreatureGeneration | DIFFER 903v960 | 12 |
| 0x00C08D40 | libpk CSoundSystem slot 10 (bank loader) | SKIP, not authored | 0 |

## Extents
All extents in the oracle are right (each row ends at its own ret / tail jump before int3 padding).

## Label corrections
See NAME_FIXES.tsv. Every catalog label except CCRCSymbolMap::LoadFromFile and CTCBob::FrameUpdate was wrong:
DefinitionManager_LoadDefinitions = CScriptDefinitionManager::InitAndCompile; HandleBordelloNPC = CV_BordelloScript::AdjustTavernPrices;
Script_Undead_Generator = CQ_HeroSoulsNostroScript::DealWithCreatureGeneration; CCharString_ReplaceAll = libpk CString::Replace;
remove_if<...> = _List_base<CScriptThingInfo>::clear; CAudioMixer_ProcessChannels = slot 2 of vtable 0x012B8A64 (descriptive).
The two quest threads are identified by the CSpawnedFunc construction (member-function pointer stored at +0x34 next to the
PDB-spelled thread name string), plus Ego_r and FableWin.

## Callee names
- 0x009D4CE0 is CDefStringTable::SaveTable(bool) (the landed leaf says SaveBinary; the PDB has no SaveBinary).
- 0x009A76A0 NBankFileManager::GetHeaderDir, 0x009ACB10 CDefinitionManager::ClearSymbols, 0x009B08C0 CDefinitionManager::Compile,
  0x00CBF647 SetGlobalScriptDef: bound spellings differ (NAME_FIXES).
- 0x0138E188 g_bSaveBinaryDefinitions and 0x0143E8E0 g_ZeroVector stay descriptive (no PDB name found; as already landed).

## Header debt
- engine/CDefPointer.h / CParentDefClassBase.h `#include <stddef.h>`, which hangs cl in this lane (q.py timeout, probe 1 of 0xE3A6B0); the
  row uses a local CDefPointer<const CScriptDef> model with the same mangling. A lane include path for <stddef.h> or a guard would fix it.
- engine/CDiskFileWin32.h, CTCBase.h, CShipDef.h are flat pack(1) layouts without virtuals: 0xA39010 and 0x7877E0 use local models.
- 0xCD3F50 uses an in-TU STLport vector<CWideString> (no shared STLport vector header exists); _M_insert_overflow / operator= are
  same-TU auto_inline(off) stand-in bodies, graded only through the caller.

- vland warns SHARED_HEADER_AVAILABLE for CDefinitionManager (0xCD3F50) and CBaseClassNonCopyable (0xA39010): the 0xCD3F50 model needs the
  retail 12-byte STLport vector members at +0x40 / +0x4c, which the generated (Ego_r, 16-byte vector) header does not give.

## Decisions for the coordinator
1. 0x00D89710: retail expands ~CScriptThing inline at exactly one site (the loop-2 creature temporary) and calls 0x004AA840 elsewhere.
   GOTCHAS b18 says in-body `#pragma inline_depth(0)` regions reach this split; the chunk brief bans inline_depth. Allow it for this row?
2. 0x00C08D40 was not authored (budget): a 964-byte /EHsc /GS libpk factory with ~10 unnamed libpk helpers
   (PKCProtectionInstance 0x00C0FFC0, CDebugBreak, PKCAutoDelete 0x00C099F0, the .lug/.lut name builders). It needs a fresh row.
3. 0x00CD3F50: "Data\\Defs\\" must be a CWideString temporary written in place; a call to the inline
   NResourceDirectoryNames::GetDefDir builds it before the right operand is pushed (probes 5-12). Retail may have used a macro.
