# Agent NF336 (Account A naming queue, 2026-11-24): CV_HauntedHouseScript family onto the shared headers (+ TRetVoid::Return 0x00E95380)

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, rulings included).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call; `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF336/ . Agent name for lock.sh: NF336. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only). Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Why
Account B's Gate 0 for the owner-approved object-order build mode (rebuild/candidates/manual/pending/2026-11-24_acctB_b24/progress/GATE0.md; COORDINATION_LOG 2026-11-24
"Gate 0 ... FAIL") could not run: 24 landed rows of the retail object 0x00E93D50..0x00E96E10 define their OWN local model of NScript::CV_HauntedHouseScript (several also of
CScriptDataBase, CCharString, CScriptThing, CSpawnedFuncBase, CScriptProcessBase), so the object-mates cannot share one TU (C2011 / C2118), and TRetVoid::Return 0x00E95380 is
landed under the stand-in class `C00e95380` (NAME_FIXES_QUEUE `acctB_b24 PRIORITY 0x00E95380 ?Return@TRetVoid@@QAEXXZ`). Removing that local-model debt is ordinary naming-queue
work; it is also the prerequisite B named for a rerun.

## Scope ($S/w22/agentNF336/ENTRIES.txt: 25 rows)
- The 24 local-model rows: re-base each onto `engine/CV_HauntedHouseScript.h` and the other shared headers (+ their existing FABLE_RETAIL_* opt-ins; add-only opt-ins where a member
  the row needs is missing, with probe / hdrcheck / equiv_check + planted control), dropping the row-local class models. Names: the real PDB spellings (two sources).
- 0x00E95380: the self moves from `C00e95380::M` / `Nop_ret0_00e95380` onto `?Return@TRetVoid@@QAEXXZ` (engine/TRetVoid.h), with every landed binder of the old spelling moving in
  the same landing (NF304 Q6) and its table lines respelled.
- Per row: HEAD v new at one work path: grade unchanged (MATCH / RELOCATION_MATCH, test PASS), graded symbol = PDB symbol, objsec ALL_EQ (or + exactly libcp, NF318 Q1 — the move onto
  a shared header under an existing class-model switch), COMDAT sets equal or every renamed COMDAT now in its PDB spelling and byte-equal to retail (pickany_vs_retail.py,
  rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py, python3 -I), no new `??_7` / `??_R` unless it equals retail's slot for slot. Link check HEAD v new.
- A row that cannot move without changing its bytes or its own function: STATUS skipped-port with the measured reason (do not force it with a device).
- Finally, as evidence only (not a deliverable): try `cl /Zs` of ONE TU that #includes your 24 new row sources + 0x00E95380 in address order (scratch only) and record whether it
  composes; list any remaining conflict.

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released), especially 0x00E93EB0 and 0x00E20600 (B's open rows in this object); pending/2026-11-24_acctB_b24/.
- Codex pass 26 / 27 / 28 RESERVED rows. Agent NF335 runs at the same time (NF324 Q2 combat rows, lock owner NF335): skip its locks.
- CScriptThing destructor / copy-ctor / operator= emission: a re-based row that newly emits or changes one of those COMDATs must keep it pick-any and BYTES_EQUAL to retail (NF328 / NF331).
- Every T3_PORT_QUEUE line that is not one of your entries.

## Hand-back
As in the standing brief, to $S/w22/agentNF336/ (rows/, tests if changed, hdr/ + orig/, TABLE_CHANGES.tsv, RENAMES.tsv, STATUS.tsv, NOTES.md, evidence/, recheck_final.log,
rows.txt, tools/), plus COMPOSE.md (the one-TU /Zs result). Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile.
