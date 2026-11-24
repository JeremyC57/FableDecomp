# Agent NF328 (Account A naming queue, 2026-11-24): the ~CScriptThing / copy-constructor pick-any unit (NF326 Q4)

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, standing rulings included).

## Paths (this lane; the standing brief's Paths section names another session)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh` in every Bash call.
- Your folder: $S/w22/agentNF328/ . Agent name for lock.sh: NF328. Locks: $S/w22/nflocks/<addr>/owner.
- Donor debug builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (FableWin.exe/.pdb, Ego_d.exe/.pdb, Ego_r.pdb). Evidence only; never copy or quote bytes.
- Retail exe: $FABLE_EXE (after sourcing env.sh). Pick-any checker: rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (run it with python3 -I; table rebuild/oracles/reloc_bindings.tsv).
- Disk budget: 3 GB for your folder at most (the box has ~9 GB free and another agent runs). Delete build dirs with clean.py as you go.

## Scope (the coordinator sends you into these T3_PORT_QUEUE.tsv lines; they are yours)
- `nf326 0x004AA840 unit` and `nf326 0x004ABE90 unit` (and the matching `port` lines): CScriptThing::~CScriptThing (??1CScriptThing@@UAE@XZ, retail 0x004AA840) and the copy constructor (??0CScriptThing@@QAE@ABV0@@Z, retail 0x004ABE90).
  The queue lines say: ~CScriptThing is strong at 0x004AA840, 0x00DDE230, 0x00E6B750 (LNK2005 shape) and 3 local-model destructor copies differ from retail; the copy ctor is strong against 25 pick-any copies.
- Secondary, only if the unit is done or blocked: `nf326 0x00DF6010 debt` (GetDataString copy 45 bytes v retail 42).
- Goal: one unit in which every landed row that emits either member emits it under the PDB spelling with bytes equal to Fable.exe at the self address, no LNK2005, and each binder binds the PDB spelling, with NF326's full proof set (selfproof, copyproof, COMDAT census before/after, full link of the census objects HEAD v new: equal duplicate and unresolved sets, pickany_vs_retail EQ for both members in every changed object).

## Required reading (in addition to the standing list)
- rebuild/candidates/manual/batches/2026-11-24_landing_nf326/README.md (the ruling: decision (a), the pick-any whole-unit form, for that unit; this unit is its follow-up "with the same proof set").
- rebuild/candidates/manual/pending/2026-11-24_nf326_handback/ (NOTES.md, LESSONS.md, CENSUS.tsv, STATUS.tsv, landing_proof/).
- tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf326/ (selfproof.py, copyproof.py, objcensus.py, census.py, link2.py, mkpick.py, pairgrade.py ...). Copy what you use into your tools/.
- The 2026-11-23 deleting-destructor ruling in the standing brief (no forcing construct; do not copy the MakeOne pattern).

## Stop conditions (hand back a decision instead of going on)
- You cannot take the address of a constructor or destructor, so NF326's `ReferenceScriptThing<M>` helper form does not carry over as is. Find the smallest retail-faithful emission. If a self's object gains ANYTHING beyond one helper relative to HEAD (a default ctor, a vftable / RTTI COMDAT `??_7` / `??_R`, a `??_G` / `??_E`, or the other member with bytes that differ from retail at its address), stop and hand it back as a decision with the measurements.
- ~CScriptThing is virtual (UAE): any change that makes a TU newly emit or change CScriptThing's vftable is a decision (standing brief: "If a vtable appears or changes, stop").
- Any unit member that you cannot fix holds the whole unit: hand back the census and the blocker, land nothing partial.

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv lines by B with status claimed: 0x00E20600 0x00D09010 0x00E93EB0 0x00D01630 0x00975BB0 0x00D11930 0x00F1EED0 0x00D0C410 0x00839940 0x00D0D910 0x00DC93F0 0x00DB3FA0 0x012133C0 0x00407A70 0x00407BA0 0x00402510 0x00BE4FE0 0x0070D940 0x00BE3970 0x00BE4660) and the staged rows in pending/2026-11-24_acctB_b24/.
- Codex pass 27 / 28 reserved rows (pending/*codex_pass27*/RESERVED.tsv, pending/2026-11-22_codex_pass28/RESERVED.tsv) and pass 26 (pending/2026-11-21_codex_pass26/RESERVED.tsv).
- Any address locked by another owner (agent NF329 runs at the same time on combat-action rows; the coordinator's scripted unit waves may also lock rows). If a row your unit needs is locked by someone else, record it and hand back: do not wait in a loop.
- Every other T3_PORT_QUEUE line.

## Hand-back
As in the standing brief, to $S/w22/agentNF328/. Add UNIT.md: the member list, the census before / after, the proof summary, and the decisions you need. Keep files current as you go.
