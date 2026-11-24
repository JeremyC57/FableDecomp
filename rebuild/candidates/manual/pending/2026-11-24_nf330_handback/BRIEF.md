# Agent NF330 (Account A naming queue, 2026-11-24): NF324 Q1 remainder, the 95 thin-family flat-header rows NF329 did not reach

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, standing rulings included).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; unset TMPDIR TMP` in every Bash call (a TMPDIR inside the scratchpad makes Wine cl segfault).
- Your folder: $S/w22/agentNF330/ . Agent name for lock.sh: NF330. Locks: $S/w22/nflocks/<addr>/owner.
- Donor debug builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/". Evidence only; never copy or quote bytes.
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB for your folder.

## Scope
Your entries: $S/w22/agentNF330/ENTRIES.txt (95 addresses), NF329's `skipped-port` rows (thin remainder: CCreatureActionBase ~28, CSubDefClassBase ~18,
CCreatureAction_TrollWhackGroundBase, CAIStateGroupBase, CCreatureAction_BuildUp*Spell, SheatheItemToInventory, BlockBase, ...). Each still has its
nf324 T3 line (column 2): the coordinator sends you into those lines, and lands with `land_scripted.py --release-t3 nf324`.

## Route: exactly NF329's, which has now LANDED
- Read first: rebuild/candidates/manual/batches/2026-11-24_landing_nf329_b3/README.md (rulings D-a..D-g, all in force for you), the b1 / b2 / b4
  READMEs, rebuild/candidates/manual/pending/2026-11-24_nf329_handback/ (NOTES.md, LESSONS.md), and NF329's tools:
  tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf329/ (flatten_pilot.py v5, genmodel.py, probe / equiv / link tools). Copy what you use into your tools/.
- NF329's 118 headers are in the repo now, so models you chain onto (BuildUp*, BlockBase, ...) are landed opt-ins: use them; re-generate nothing that exists.
- Per class: `FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` opt-in derived model (add-only, flat model kept as `#else`), the emitted `??_7` probe against retail slot for
  slot, asserts pinned, hdrcheck default=same, equiv_check over the includer closure with a planted control. A base the model needs and lacks: NF322 Q2
  (part of the port when in struct_layouts_egor.tsv + rtti_bases.tsv); a class absent from the layout TSV, a `_PASS` switch base, or a vftable you cannot
  probe equal: STATUS skipped-port with the reason.
- Per row and across rows: NF329's proof set (objcheck ALL_EQ or + exactly libcp; MATCH / RELOCATION_MATCH with the unchanged landed test PASS; graded
  symbol = the PDB symbol; one link HEAD v new: 0 new duplicates, equal unresolved; new-name census; byte scan).
- Hand back in land_scripted run-dir batches (header blocks per batch, no class straddling batches), like NF329's batches/.

## Stay off
- Agent work held for the coordinator: the 9 NF328 rows (0x004AA840 0x004AB170 0x004ABE90 0x00CE0920 0x00CE0B00 0x00CE0CE0 0x00D151D0 0x00DDE230 0x00E6B750) and any row
  that defines CScriptThing's destructor / copy constructor; pending/2026-11-24_nf326_handback/rows.txt.
- B's claimed rows (COORDINATION_CLAIMS.tsv, newest line per item, B, not done/released) and pending/2026-11-24_acctB_b24/; Codex pass 26 / 27 / 28 RESERVED.tsv rows.
- Every T3_PORT_QUEUE line that is not your entries' nf324 line. Any address locked by another owner: skip it and note it.

## Hand-back
As in the standing brief, to $S/w22/agentNF330/, plus HEADERS_<batch>.tsv and per-header probe / equiv evidence. Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile (NF329 slipped on this).
