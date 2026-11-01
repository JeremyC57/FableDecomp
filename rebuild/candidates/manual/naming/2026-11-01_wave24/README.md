# Naming pass wave 24 (2026-11-01): 60 landed rows respelled -- CDefinitionManager / CDefStringTable / NBankFileManager, CCRCSymbolMap, vector<CWideString>, the six wrong-filter rows, libpk CString, ~CBaseClassNonCopyable callers

Inputs: brief `naming/briefs/BRIEF_W24.md`, QUEUE_W24_input.tsv (42 rows: the wave 23 open rows, the 22 c691 rows, three DECISIONS_W23 items),
wave 23 DEFERRED.tsv. One Opus 5.5 agent authored 60 rows in `$S/naming/w24` (AGENT_README.md; scripts/ here; own recheck `logs/recheck_final.log`
60/60). The coordinator copied ov_final (65 files), applied the 13 RENAMES (git mv + build_candidates.ps1), BINDINGS (59 RETIRE / 18 ADD / 8 KEEP,
0 CONFLICT; the table equals the agent's after-table) and LABELS (18), and rechecked the 60 rows against HEAD (`logs/coordinator_recheck.log`:
60/60 RELOCATION_MATCH + PASS). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): defmgr 10 (CDefStringTable::SaveTable 0x009D4CE0 -- landed as SaveBinary;
CDefinitionManager::Compile; ClearSymbols re-authored with CGameDefinitionManager deriving from CDefinitionManager; SetGlobalScriptDef;
NBankFileManager::GetHeaderDir / SetHeaderDir and SymbolHeaderDir), crc 4 (CCRCSymbolMap Error / ParseEnum / ParseBuffer protected with PDB types,
the static CurrentFilename, CVectorMap::DeallocateMemory), vws 4 (vector<CWideString> with the protected _M_insert_overflow), wsop 2
(CMemoryFile::Open 0x009E90A0, WorldToLoad at +0x1E40), misc 2 (STLport list<CScriptThingInfo>; 0x007066A0 calls CWorld::GetFrame and
CTCBase::PeekWorld 0x00686A80 -- it had called it GetWorld, which is 0x00686A70: both PDBs place PeekWorld 16 bytes after GetWorld), filt6 8 (the six
wrong-filter rows re-identified from the filter address each calls, callers 0x007EE5A0 / 0x00774B40), cstr 5 (CString::operator= 0x00C0C0C0, the
static LoadBank 0x00C17260, the 0x00C309E0 dtor on a stand-in class, CLoadLog::AddError() << on CString), bcnc 25 (callers on the protected
virtual ~CBaseClassNonCopyable MAE, their ??_G following).

Queue outcomes (QUEUE_W24.tsv): 19 respelled, 3 partial, 13 deferred, 3 hold, 5 done. Decisions: `DECISIONS_W24_RESOLVED.tsv` -- LoadBank accepted;
0x0139C8FC goes to the MFC spelling `_afxDataNil` / `_afxInitData` in wave 25 (the libpk CString is MFC text); the seven junk 0x00686A80 binders
respelled to PeekWorld in wave 25; 0x00C2D9C0's owner decided in wave 25 from two sources. The agent's report that 0x00BC9C30 and 0x009CDF50 DIFFER was
checked by the coordinator on the tree before wave 23 (a scratch worktree at 9c00ccb86): same DIFFER there, so a pre-existing defect (neither source has
a Leaf line), routed to a fix lane. Deferred (DEFERRED.tsv, 461 lines) -> wave 25 with the c692 queue rows.
