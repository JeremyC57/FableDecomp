# Naming pass wave 26 (2026-11-01): 62 landed rows respelled -- _NullString binders, NScript predicates, STLport vector<long> / vector<CWideString> callers, CClassTypeInfo AllocFunc __fastcall, the chunk 693 / 694 queue rows

Inputs: brief `naming/briefs/BRIEF_W26.md`, QUEUE_W26_input.tsv (44 rows: wave 25 open rows plus the c693 and c694 rows appended mid-wave), wave 25
DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w26` (AGENT_README.md; scripts/ here; own recheck `logs/recheck_final.log`, 62
rows, against the tree after chunks 693 / 694). The coordinator copied ov_final (62 files), applied the 3 RENAMES (git mv + build_candidates.ps1),
BINDINGS (51 RETIRE / 3 ADD / 11 KEEP, 0 CONFLICT; the table equals the agent's after-table) and LABELS (10), and rechecked the 62 rows against HEAD
(`logs/coordinator_recheck.log`: 62/62 RELOCATION_MATCH + PASS). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): nul 3 (_afxDataNil -> `_NullString`), tbvm 1 (0x00744580 is
CTCPerceiveThing::GetTimeBasedVisibilityMultiplier, was GetExtendedSightRadius), wcol 1 (CSurface::SetPixel with the colour and locked rect by
reference), pred 11 (SetPredicate / IsActiveThreadTerminating through the global CEntityScriptBase on NScript::CPredicateBase; 0x00D9F270 now nests
CMC_Minion in CQ_MinionCampScript so its Leaf matches -- it had been graded on its largest function), gossip 2 (ClearActiveGossipCategories /
ClearIsGossipForPlayer as STLport map<CCharString,bool>::clear), rbt 1 (_Rb_tree::erase(iterator,iterator)), vlong 16 (STLport vector<long,
allocator<long> > with the protected grow-insert; 0x0098A9A0 on the landed CArray<T> model so it names 0x00A5ECB0's own spelling), gst 2
(2DSprites / Text GetSupportedTypes by RTTI slot 2), vws 4 (vector<CWideString, allocator<CWideString> >), c693 4 (~CIsThingAlive 0x00E241B0;
private GetRecoveryFilePath; CFileDataOutputStream(CAFile&); 0x009CDA70 on CBankFile), fdos 3 (CDiskFileWin32 derives from CAFile per RTTI, passed by
reference), c694 5 (the ??_G stand-ins call the NCameraModes dtors chunk 694 proved), cti 9 (CClassTypeInfo AllocFunc `CClass* (__fastcall*)()`).

Queue outcomes (QUEUE_W26.tsv): 7 respelled, 7 partial, 10 done, 10 deferred, 9 hold, 1 other lane. Decisions: `DECISIONS_W26_RESOLVED.tsv` -- the
NScript entity Main rows go to PDB nesting + private virtual (EAE) on the vtable model; CFileDataOutputStream gets an opt-in retail layout with
the vptr; the CBankFile own-row respells stay (CBankFile is retail's name; header and binders remain on HOLD); CPixelShader's private ctor with
CShaderRenderManager as friend; access-only Leaf respells allowed; two-target vector re-models approved where every moved target is PDB-proven.
Deferred (DEFERRED.tsv) -> wave 27; the 14 ccsns rows, 009994a0 / 00999620 and scope 3 were not reached.
