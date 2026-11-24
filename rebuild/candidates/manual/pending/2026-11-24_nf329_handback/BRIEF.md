# Agent NF329 (Account A naming queue, 2026-11-24): NF324 Q1, the flat-header combat families on generated opt-in derived models

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, standing rulings included).

## Paths (this lane; the standing brief's Paths section names another session)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh` in every Bash call.
- Your folder: $S/w22/agentNF329/ . Agent name for lock.sh: NF329. Locks: $S/w22/nflocks/<addr>/owner.
- Donor debug builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (FableWin.exe/.pdb, Ego_d.exe/.pdb, Ego_r.pdb). Evidence only; never copy or quote bytes.
- Retail exe: $FABLE_EXE (after sourcing env.sh).
- Disk budget: 3 GB for your folder at most (the box has ~9 GB free and another agent runs). Delete build dirs with clean.py as you go.

## Ruling you work under (coordinator, advisor decision, owner delegated, 2026-11-24)
NF324 Q1 is wanted, as its own pass: the 396 NF324 entries skipped as "flat-header ports" (engine/<Class>.h holds only the generated flat model:
`void* __vftable`, no base, no `virtual`) move onto a generated opt-in derived model per class, in NF325's genmodel.py style, under the
NF322 Q1 / Q2 rules:
- per class `#ifdef FABLE_RETAIL_<CLASS>` (add-only; default path unchanged) `class X : public <Base> { <Ego_r members>; <RTTI-named overrides> };`
  generated from ghidra_out/struct_layouts_egor.tsv + the RTTI tables; FABLE_STATIC_ASSERT on size / offsets;
- model proof = the compiler-emitted `??_7` probe against retail, slot for slot (declaration order alone is not proof);
- equiv_check over every landed row and test with a planted control (S pointing into your folder);
- then the rows move through the switch / header route (flatten_pilot v4: tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf324/),
  with NF324's per-row proof (objcheck ALL_EQ, or ALL_EQ plus exactly `/DEFAULTLIB:"libcp"` per NF318 Q1; MATCH / RELOCATION_MATCH with the
  unchanged landed test passing; graded symbol = the PDB symbol), one link of all HEAD v new objects (0 new duplicates, equal unresolved sets),
  and the new-name census.
- A base header the derived model needs and lacks: NF322 Q2 (part of the port when the class is in struct_layouts_egor.tsv and rtti_bases.tsv); a class absent from the layout TSV is out of scope (STATUS skipped-port).

## Scope and order
1. Re-triage first: rebuild the Q1 list from rebuild/candidates/manual/pending/2026-11-24_nf324_handback/ (TRIAGE.tsv, evidence/flatport_bases.txt,
   NOTES.md "Questions" Q1) against the CURRENT tree and tracker (`naming_status.py`): NF325 already landed 25 CCombatSequence rows on generated models
   and the coordinator's unit waves landed more since; drop anything now done.
2. CCombatActionBase family (~156 entries; engine/CCombatActionBase.h already has a polymorphic model under `#if defined(FABLE_RETAIL_LEADER_INITIALISE) || ...`
   with landed definers).
3. CCombatSequenceBase family (~143) only on the base model NF325 landed (read batches/2026-11-24_landing_nf325/README.md: if the base is still only
   polymorphic under a `_PASS27` switch, the NF322 Q1 widening comes first, with its own probe and planted control; record it).
4. The thin remainder (CCreatureActionBase 28, CSubDefClassBase 18, ...) only if 2 and 3 are done.
Hand back after about 60-80 rows per batch, as NF324 did; keep rows/ batches separately landable (header blocks first).

## Required reading (in addition to the standing list)
- batches/2026-11-24_landing_nf324_b1/README.md (rulings Q1..Q8), pending/2026-11-24_nf324_handback/ (NOTES.md, LESSONS.md), batches/2026-11-24_landing_nf322/README.md,
  batches/2026-11-24_landing_nf325/README.md and pending/2026-11-24_nf325_handback/; agent_tools/nf325/genmodel.py and agent_tools/nf324/.

## Stay off
- B's claimed rows (0x00E20600 0x00D09010 0x00E93EB0 0x00D01630 0x00975BB0 0x00D11930 0x00F1EED0 0x00D0C410 0x00839940 0x00D0D910 0x00DC93F0
  0x00DB3FA0 0x012133C0 0x00407A70 0x00407BA0 0x00402510 0x00BE4FE0 0x0070D940 0x00BE3970 0x00BE4660) and pending/2026-11-24_acctB_b24/.
- Codex pass 26 / 27 / 28 reserved rows (pending/*codex_pass2[678]*/RESERVED.tsv).
- Agent NF328 runs at the same time on the CScriptThing destructor / copy-ctor unit: never lock or edit a row that emits ~CScriptThing or
  CScriptThing's copy constructor (??1CScriptThing@@UAE@XZ / ??0CScriptThing@@QAE@ABV0@@Z), nor any row in
  pending/2026-11-24_nf326_handback/rows.txt. Any address locked by another owner: skip it and note it.
- NF324 Q2 (the 79 own-test rows), Q6 holds (0x008601B0 / 0x008601C0), and every other T3_PORT_QUEUE line unless it is a Q1 flat-header line.

## Hand-back
As in the standing brief, to $S/w22/agentNF329/, plus HEADERS_<batch>.tsv (header, macro, rows) and the probe / equiv_check evidence per header.
