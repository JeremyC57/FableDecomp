# Agent NF331 (Account A naming queue, 2026-11-24): the CScriptThing::operator= pick-any unit (T3 `nf328 0x008AB980 unit`)

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, standing rulings included).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call (a TMPDIR inside the scratchpad makes Wine cl segfault); `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF331/ . Agent name for lock.sh: NF331. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only).
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Pick-any checker: rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I). Disk budget: 3 GB.

## Scope
`??4CScriptThing@@QAEAAV0@ABV0@@Z` (CScriptThing::operator=, both PDB name sets; self row 0x008AB980, landed). NF328's census link of 1,017 objects still
showed 75 duplicate lines for it, so some landed objects define it strongly against pick-any copies. Goal, exactly NF328's (landed 2026-11-24): every
strong definer becomes pick-any (`inline`, plus at most one `?A0x` helper in the self if nothing else emits it out of line), every emitted copy equal
to Fable.exe at 0x008AB980, no LNK2005, no new vftable / ctor / `??_G` emission, with NF328's proof set.

## Required reading (in addition to the standing list)
- rebuild/candidates/manual/batches/2026-11-24_landing_nf328/README.md (the rulings, including the helper device: `#pragma inline_depth(0)` over the
  ONE helper function, restored right after, below the row's own code, with an `// Inline-budget: scaffolding, not retail ...` comment line) and
  batches/2026-11-24_landing_nf326/README.md.
- pending/2026-11-24_nf328_handback/ (UNIT.md, NOTES.md, LESSONS.md, evidence/) and its tools: tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf328/
  (srccensus2.py, hcensus.py, census2.py, census_report.py, uproof.py, pairgrade.py, link2.py). Copy and adapt them for the one member.

## Rulings that already apply (do not re-ask)
- Differing pre-existing copies are DEBT, not a hold (NF326 Q2 / NF328 D1): land the strong->pick-any change if it adds no differing copy and removes
  every strong definer; list each differing copy with its shape in DEBT.tsv. Stop and ask only if your change itself would add or alter a copy, a
  vftable, a ctor or a `??_G` / `??_E`.
- Codex-reserved rows may be compiled READ-ONLY for the census (no lock, no edit); record them as census-only.

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released) and pending/2026-11-24_acctB_b24/; any edit to a Codex pass
  26 / 27 / 28 RESERVED row.
- Agent NF330 runs at the same time on combat-action / creature-action flat-header rows (lock owner NF330): skip its locks.
- Every other T3_PORT_QUEUE line.

## Hand-back
As in the standing brief, to $S/w22/agentNF331/, plus UNIT.md, CENSUS.tsv / CENSUS_after.tsv and DEBT.tsv. Read COORDINATION.md, ASM_RULES,
EXTRA_RULES and GOTCHAS BEFORE your first compile. Keep files current as you go (container restarts happen).
