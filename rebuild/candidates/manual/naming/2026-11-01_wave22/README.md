# Naming pass wave 22 (2026-11-01): 183 landed rows respelled -- the libpk CString stream, ~CCharString, ~CWideString, CAmbientEmitter, the c689 / c690 rows

Inputs: brief `naming/briefs/BRIEF_W22.md`, QUEUE_W22_input.tsv (28 rows: the wave 21 open rows, the DECISIONS_W21 families, the 15 c690 rows
appended when chunk 690 landed), wave 21 DEFERRED.tsv. One Opus 5.5 agent authored 190 rows in `$S/naming/w22` (AGENT_README.md; scripts/ here;
its own recheck `logs/recheck_final.log` 190/190 RELOCATION_MATCH + PASS). The coordinator copied ov_final (212 files), applied the 1 RENAME (git mv +
build_candidates.ps1), BINDINGS (274 RETIRE / 34 ADD / 4 KEEP, 0 CONFLICT; the table equals the agent's runs/reloc_bindings.after.tsv) and LABELS
(5), then HELD OUT the 7-row hastc family (sources restored to HEAD; DECISIONS_W22_RESOLVED item 2), and rechecked the 183 changed rows against
HEAD (`logs/coordinator_recheck.log`). `bindings_multi_check.py` 0. Name-only except the re-authored own rows, all byte-identical.

Families, committed in this order (`runs/<fam>.rows.txt`): cstr 116 (every stand-in for the libpk string -- CDebugStream x59, CString_<addr>,
CLogLine_, CMember_, CAssertString, CStringMember ... -- is `class CString` with operator<< for text / long / unsigned long and ~CString; CTrace /
CAssertTrace are CDebugBreak; Append chains as << chains; the callees taking or returning the string follow), vec 1 (0x00B67EC0 on STLport
std::vector<float> / <unsigned char> / <unsigned long>; 0x00441020 / 0x00485F3A / 0x00464ACE together), emit 3 (0x00C44F80 = ~CAmbientEmitter,
renamed; its new / delete sites 0x00C36180 / 0x00C36290), c689 1 (the 0x00D34470 CQ_FireHeartScript ctor on the PDB signature), ccs 40
(CMember_0099eae0 / CBase_0099eae0 / CCharString_Retail -> the PDB CCharString), ccsns 16 (namespace-nested CCharString classes moved to the
global PDB class), c690 1 (0x0076C750's comparators declared `class`, binding the c690 sort names), ws 3 + wsns 2 (the ~CWideString 0x0099B510
stand-ins).

Queue outcomes (QUEUE_W22.tsv, 28 rows): 4 respelled, 4 partial (00c0dec0 -- 8 rows left: four own rows + four views; the two sort rows; 0x0099B510),
8 already correct (the c690 own rows), 11 deferred (0x00C28690; the GDefStringTable leftovers; the 0x007ACC50 callers and the 23
vector<long>::_M_insert_overflow rows -- per-row STLport re-models; the ~tree family callers of 0x00613AE0 / 0x006196D0; 0x009CEC60 / 0x009CBD30 /
0x009CBD50 behind the CBankFile header; 0x0099A300), 1 hold (0x00AD3A10). Decisions: `DECISIONS_W22_RESOLVED.tsv` -- CAmbientEmitter accepted as
one class; the HasTC forwarding overload REJECTED as non-retail text (template-wide filter(*p) re-model in wave 23; 10 rows in DEFERRED.tsv); the
libpk CString own rows re-authored in wave 23 with int Mid / Right (the existing binder's spelling); the CBankFile members wait for the engine-header
migration. Deferred (DEFERRED.tsv, 460 rows) -> wave 23 with the c691 queue rows.
