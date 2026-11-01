# Naming pass wave 22 (2026-11-01): 190 landed rows respelled, all byte-identical (190/190 RELOCATION_MATCH + PASS)

Inputs: brief naming/briefs/BRIEF_W22.md, QUEUE_W22_input.tsv (28 rows: 520 x2, w20 2, c689 7, w21 3, c690 15 -- the c690 rows were appended
while the wave ran and are covered), DEFERRED_W21_INPUT.tsv (454).
Index: idx/full.jsonl = HEAD (wave 21 idx/after.jsonl + the chunk-690 rows 00767da0 00b3b8e0 00c059d0 00c05cb0 00ca1360 00cb0700 in
idx/delta690.jsonl + the coordinator's 00c04850 edit in idx/delta2.jsonl; re-checked against HEAD before hand-back: no later change under
rebuild/src|include|tests, build_candidates.ps1 or reloc_bindings.tsv), idx/after.jsonl = HEAD + ov_final.
Commands (from $S/naming/w22, lane env sourced): `python3 scripts/compose22.py ov_final` (every stage on fresh repo copies; writes
runs/<fam>.rows.txt and ov_<fam>/), `scripts/ovrows.py ov_final > runs/final.rows.txt`, `bash scripts/recheck.sh final ov_final
$PWD/runs/final.rows.txt 3`, `scripts/mkindex.py $PWD/idx/ovfinal.jsonl $PWD/ov_final runs/final.rows.txt` (merge into idx/after.jsonl),
then qscan22 (base + final) -> mkq22 (+ mkq22_notes) -> mkdef22 -> mkbind22 (runs/rk_final.pairs.jsonl idx/after.jsonl runs/final.rows.txt)
-> mkren22 idx/after.jsonl -> mklabels22. (mkindex needs an ABSOLUTE output path: a relative one breaks cl's /Fo.)

Families (commit order; runs/<fam>.rows.txt, cumulative ov_<fam>/):
- cstr 116 (scripts/fam_cstr22.py, rows file runs/base_cstr.rows.txt "<row> <stand-in classes>"): DECISIONS_W21 CString family. Every row reaching
  the libpk CString own rows 0x00C0DEC0 / 0x00C0DF00 / 0x00C0DF10 / 0x00C0BA40 or the break temporary 0x00C16750 under a stand-in (CDebugStream,
  CString_<addr>, CLogLine_<addr>, CMember_<addr>, CCharString_00c0aa40/ac30, CStringMember_00c0ba40, CAssertString, CDebugStream_00c3ab50/3b0e0;
  CTrace[_<addr>] / CAssertTrace) -> `class CString { public:` with operator<<(const char*) / (long) / (unsigned long) and ~CString, and CDebugBreak;
  Append chains rewritten as `<<` chains (string-aware parser; integer literals keep their type with L / UL); the callees that take / return the
  string follow (CSoundEvent::Describe 0x00C21040, GetStatus 0x00C27760, OpenSegment 0x00C2D9C0, FindByName 0x00C101D0, NewMessage 0x00C307E0,
  TraceOutput); operator=(const CString&) returns const CString& (the bound 0x00C0C0C0 spelling). Tests: identifiers only (standalone models).
- vec 1: 0x00B67EC0 on STLport std::vector<float> / <unsigned char> / <unsigned long> (hand CVector<T> retired); 0x00441020 / 0x00485F3A /
  0x00464ACE move together (DECISIONS_W21).
- emit 3: 0x00C44F80 = ~CAmbientEmitter (CleanupA/B -> the landed ReleaseSounds 0x00C44B80 / ClearEvents 0x00C44D30); its new/delete sites
  0x00C36180 / 0x00C36290 respelled from CResourceAllocation (DECISIONS_W22 item 1 for the remaining own rows). RENAMES: 0x00C44F80 file.
- c689 1: 0x00D34470 CQ_FireHeartScript ctor on the PDB (CScriptDataBase*, CGameScriptInterfaceBase&) signature (precedent 0x00D6EF20).
- hastc 7: the PDB ??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z through an inline forwarding pointer overload (DECISIONS_W22 item 2);
  0x004A9DA0 / 0x0093F8A0 / 0x006186F0 DIFFER with it (excluded), 0x004A9EF0 (Thing model) not driven.
- ccs 40 (scripts/fam_ccs22.py): ~CCharString mega family, CMember_0099eae0 x26 / CBase_0099eae0 x10 / CCharString_Retail x4 -> the PDB class.
- ccsns 16 (scripts/fam_ccsns22.py): namespaced CCharString models hoisted to the global PDB class (14 skipped: namespace-level string-data
  types, same-TU CCharString bodies, qualified uses -- DEFERRED).
- c690 1: 0x0076C750's nested comparators class-keyed V (FableWin.pdb) -> it binds the c690 sort finals 0x0076C000 / 0x0076C1A0.
- ws 3 / wsns 2 (fam_ws22 / fam_wsns22): the c690 ~CWideString 0x0099B510 stand-ins of 0x00BC7AB0 / 0x0056C820 / 0x004B8110 and the namespaced
  models of 0x00569450 / 0x006BE9D0.
Outputs: BINDINGS.tsv (273 RETIRE / 34 ADD / 4 KEEP, 0 CONFLICT; bindings_multi_check 0 on runs/reloc_bindings.after.tsv), LABELS.tsv 5,
RENAMES.tsv 1, QUEUE_W22.tsv 28 (respelled 4, respelled-partial 4, already-correct 8, deferred 11, hold 1), DEFERRED.tsv 459, DECISIONS_W22.tsv 4.
