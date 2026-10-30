# Naming pass wave 10 (2026-10-30): 543 rows respelled (584 row-edits), plus 154 funclet rows regraded

Inputs (brief `../briefs/BRIEF_W10.md`): QUEUE_575_585_mig.tsv (NAME_FIXES_QUEUE chunks 575-585, migration items mig1-mig5),
the coordinator's chunk-586 lock family (sub-wave 586), 0x0143E8F8, wave 9's open decisions. Every spelling was checked
against both PDB name lists (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv).

## Sub-waves (scripts/, composed on fresh copies of the CURRENT repo by
`scripts/compose10.py ov_final sw1 sw1b sw586 sw586b sw2 sw3 sw1c`, stages in scripts/stages.py; each stage had its own recheck)
| stage | rows | what |
|---|---|---|
| sw1 | 283 | item 1: fam_isv 139 (IsScripting -> `bool IsValid() const`, 0x00CD23B9), fam_gmin 72 + h10_gwf 2 (0x006E7510 GetMinWorldFrameForMessages / 0x006E7530 Max, protected const IBE), fam_gmp 71 + h10_gmp 4 (0x00449970 `CPlayer& CPlayerManager::GetMainPlayer()`) |
| sw1c | 38 | item 1 follow-up for 0x00449970: fam_gmp2 36 (free `__fastcall` stand-ins and row-local accessor classes -> the real member call on the same ecx) + h10_gmp2h 2 (cast chains) + h10_gmp2t (7 tests that link the row) |
| sw1b | 38 (36 also in sw1) | item 1: STB slots QAE -> UAE (fam_stb: the header's virtual declarations, enum EScriptEntityMoveType / ETextGroupSelectionMethod) |
| sw586 | 174 | coordinator chunk 586: fam_pkc 170 (guard -> PKCProtectionInstance 0x00C0FFC0/0x00C0FFE0, 0x0143BAFC -> g_soundLock), h10_pkc (static-init rows 01224b00/01224b20), h10_coda (0x00C35CA0 `_CODA_IsSourcePlaying@4`; 00c5a1d0 = CVtable_012B5C60::Initialise tail-calling `?Initialise@CVtable_012B5C10@@UAEJPAUCAudioSound@@@Z`, test updated) |
| sw586b | 3 (in sw586) | the three cast-route rows whose 0x0143BAFC global kept a stand-in name -> g_soundLock |
| sw2 | 20 | item 2 (rest of the queue): h10_q2 16 (PlayCutscene, CutsceneMazeDeath, CRandomPopulationCreature, _M_insert_overflow protected, CalcSectionPolyMask private, CConversation / CHeroSoulsCreatureDef, CDefPointeeBase::operator=, GFIntToCharString, CalcObjectMatrix static), h10_ns 4 (NScript wrapping) |
| sw3 | 28 | item 3, 0x0143E8F8: fam_gsi 27 + h10_gsi 1 |

The 154 EH-unwind funclet stubs (0121xxxx) of edited parents carry no text of their own; they were regraded against the
overlay parents (`scripts/recheck_fx.sh` / `mkpayload_fx.py`: the stub's include is pointed at ov_final, `funclet_offset`
as in coordinator recheck.py) and follow their parents at landing. They are why the lock-family stand-ins could be retired.

## 0x0143E8F8 decision
`?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A` (`CGameScriptInterface* g_pGameScriptInterface`). No PDB data symbol;
every landed use dispatches CGameScriptInterface vtable slots (SetTimer slot 89 = 0x006E76E0, GetWorldFrame slot 653 =
0x0088E070; the stand-in views carry 653+ slots); 152 rows already spelled it so and 00cbe716 binds it. 28 rows respelled;
9 rows that include the flattened CGameScriptInterface header without methods are DEFERRED (cast plan in DEFERRED.tsv).

## Recheck (vland --recheck-landed, LANDVERIFY_WORK=w10/lv, deleted after each batch)
Final, from ov_final: 543 rows = 3 MATCH + 540 RELOCATION_MATCH, all PASS (logs/recheck_final.log); 154 funclets
RELOCATION_MATCH PASS (logs/recheck_funclets.log). Sub-wave rechecks: sw1 283, sw1b 38, sw586 174 (+00c5a1d0 retest),
sw23 48, sw586b 3, sw1c 38 - all PASS. One RELOC_CONFLICT warning (00e383a0 `??BCDefString@@QBE?AVCCharString@@XZ` ->
00415d70 here, 00415dd0 bound by 0071c7c0) is pre-existing: wave 9's recheck.log line 1438 carries the same line.
An inline-forwarder variant of fam_gmp2 was rejected: VC7.1 emitted the forwarders out of line (COMDAT), the rows still
called the stand-in, and the recheck passed only because that stand-in is bound to 0x00449970. Every edited row was then
checked for new defined symbols (only legitimate leaf respellings remain).

## Bindings (BINDINGS.tsv, scripts/mkbind10.py)
178 RETIRE (no row names them after wave 10, incl. 14 stale catalog-wide ones for queue targets), 9 ADD (exact PDB
spellings), 13 KEEP (still named), 1 NO-BIND (`??0PKCCriticalSection@@QAE@XZ`, no PDB name). Applied to a copy of
rebuild/oracles/reloc_bindings.tsv (runs/reloc_bindings.after.tsv): bindings_multi_check = 0.

## Other hand-back files
LABELS.tsv (7 proven names), RENAMES.tsv (4 git-mv proposals, src + test), DECISIONS_W10.tsv (120 targets: 16 applied,
11 partly applied, 47 no change needed, 46 open, each with its reason and before/after emitter counts), DEFERRED.tsv
(26 items with plans). Brief item 4 (wave 9's 321 open decisions) was not reached within the ~600-row budget.
