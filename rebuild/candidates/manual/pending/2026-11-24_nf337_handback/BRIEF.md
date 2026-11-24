# Agent NF337 (Account A naming queue, 2026-11-24): the CDefClassBase-family `Copy` rows NF335 could not move alone

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, rulings included).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call; `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF337/ . Agent name for lock.sh: NF337. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only). Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Background
NF335 (hand-back: /home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-24_nf335_handback/, NOTES.md and STATUS.tsv first; tools
in tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf335/, incl. flatten_pilot v6 and the moved-test machinery) moved 22
NF324 Q2 rows (their tests call the flattened name) onto their PDB members and landed them (batches/2026-11-24_landing_nf335_b1/README.md:
the test-move form `((Class*)(&obj))->Class::Method(args)`, `<Class>Layout` test models, D1..D4). It skipped every `?Copy@<Def>@@UAEXPBVCDefClassBase@@@Z`
row: retail's Copy is (for most) a 5-byte `jmp` to the class's operator= (thiscall tail call: `virtual void Copy(const CDefClassBase* p) { *this = *(const T*)p; }`
with operator= out of line), but the landed flat self is `__fastcall` taking src in edx, so it cannot move by itself.

## Scope ($S/w22/agentNF337/ENTRIES.tsv: 48 rows, NF335's reason in column 4)
- Port each Copy row to the PDB member `?Copy@<Class>@@UAEXPBVCDefClassBase@@@Z` in its real form, landed as a UNIT with the class's operator= row where the body
  needs it out of line (the unit's operator= row keeps its own bytes; find it by the jmp target; if that row is landed under a made-up name, it respells in the same unit
  with every binder, NF304 Q6). Use shared headers (`engine/<Class>.h` + existing FABLE_RETAIL_* opt-ins; add-only opt-ins with probe / hdrcheck / equiv_check + planted
  control where a declaration is missing; NF330 D1: a base `_RTTI_DECLS` block needs the BASE's own `??_7` probe).
- Retail's own bytes decide each row's form: read every row's retail bytes first and group the 48 by shape (5-byte jmp; longer bodies such as the 4 that use edx).
- Tests: the row's landed test moves with it in NF335's form (D1: hand back tests/<addr>_test.cpp + orig/tests/), the PassPattern kept; control = HEAD source + moved
  test must LINK_FAIL.
- 0x0046F49F: its operator= row 0x0046F43E already defines the PDB Copy symbol itself; resolve which address really is Copy (two sources) and hand back the fix
  as one unit, or STATUS with evidence.
- Per row: HEAD v new at one work path, grade unchanged (MATCH / RELOCATION_MATCH, test PASS), graded symbol = PDB symbol, objsec ALL_EQ (or + exactly libcp, NF318 Q1),
  COMDAT sets equal or each renamed COMDAT in its PDB spelling and byte-equal to retail (pickany_vs_retail.py, python3 -I), no new `??_7`/`??_R` unless equal to retail
  slot for slot; link check of the HEAD and new objects (0 duplicates, equal unresolved sets).
- A unit that cannot move without changing bytes: STATUS skipped-port with the measured reason; no forcing devices.
- Hand back batches as land_scripted run dirs (rows/, RESULTS.tsv, head/payload.json, TABLE_CHANGES.tsv, tests/, orig/rows/, orig/tests/, UNITS.tsv for units; HEAD entries
  of rows whose landed source has a retail byte-dump comment are listed in head/HEAD_FROM_REPO.txt, D-f).

## Stay off
- Agent NF336 runs at the same time (CV_HauntedHouseScript object 0x00E93D50..0x00E96E10 and 0x00E95380, engine/CV_HauntedHouseScript.h, engine/TRetVoid.h; lock owner
  NF336): skip its locks and do not edit those headers.
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done/released); Codex pass 26/27/28 RESERVED rows; every T3_PORT_QUEUE line that is not your entry
  or your unit's operator= row (list any such T3 line you need in NOTES as a decision; do not take it).
- CScriptThing destructor / copy-ctor / operator= emission rules (NF328 / NF331).

## Hand-back
As in the standing brief, to $S/w22/agentNF337/ (batches/bN/, hdr/ + orig/, TABLE_CHANGES.tsv, RENAMES.tsv, STATUS.tsv for all 48, NOTES.md, LESSONS.md, evidence/,
rows.txt, tools/). Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile.
