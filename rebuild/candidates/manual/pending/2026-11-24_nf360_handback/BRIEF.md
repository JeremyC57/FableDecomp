# Agent NF360 (Account A, 2026-11-24): two PRIORITY in-place self respells (NAME_FIXES nf356 lines)

Lock name NF360. Folder $S/w22/agentNF360/.
1. **0x0042AF3C** CDefinitionManager::GetDef<CThingBaseDef>: the landed owner's self is the made-up ?GetDef_0042af3c@...; respell it IN PLACE
   onto ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z on its local model, in the strong
   form 0x0042B040 uses at HEAD. NOT a port onto engine/CDefinitionManager.h (NF356 measured that; it changes the COMDAT set). Find every
   landed binder of the stand-in (NF356 found none in objects; three CTCShop sources mention it in comments) and the table / self_symbols
   lines. Then check the link: does the strong definition collide with pick-any copies elsewhere (as 0x0042B040's does)? Report it.
2. **0x00428B10** DeleteData<NUISystem::CComponent>: the landed row defines @DeleteIfSet@4 while its callers (Reset 0x00429C6F, the pick-any
   Reset copies) reference ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z. Respell its self onto the PDB name (two
   sources: Ego_r / FableWin publics, the Reset store `mov [eax+4], addr`) and fix the table line; list every binder of @DeleteIfSet@4.
Gates per row: grade not worse HEAD v new; objsec (only the self name changes); both checkers with --leaf; drop test; a /FORCE link of the
owner with its binders HEAD v new (unresolved sets and duplicates). Catalog rename if the file / function name changes (RENAMES.tsv).

## Paths and rules (all NF agents)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; every Bash call: `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`; `mkdir -p /run/user/0` if missing.
- Retail exe "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget 3 GB.
- Checkers: rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I; args: exe rebuild/oracles/reloc_bindings.tsv obj addr) and tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py (env FABLE_EXE, FABLE_ROOT=/home/user/FableDecomp; ALWAYS --row-length and --leaf=<row // Leaf:>). Neither follows vftables; a copy with no table line reads NOADDR: compare such copies with retail directly (tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf355/retcmp_coordinator.py + objcmp.py). In awk compare addresses as strings (`$1""==a""`).
- vland payloads: set each entry's `leaf` to the row's // Leaf: symbol (recheck.py reads it from the source).
- Read COORDINATION.md, ASM_RULES, EXTRA_RULES, GOTCHAS and BRIEF_NF_BATCH.md (rulings at its end, incl. NF352 reach flips and the NF356 rule: an owner port onto a shared header is a MOVE, gated on the COMDAT set staying the same) before the first compile.
- Lock each row you compile ($S/w22/nflocks/<addr>/owner = your lock name), release all before hand-back. Stay off Account B claims / staged rows, Codex reservations, other agents' locks. Never write into the repo, never run git (not even read-only). Long jobs in the background. Keep oracle files with retail bytes out of the hand-back.
- Hand-back: land_scripted run dirs under batches/bN/ (rows/, RESULTS.tsv with ACCEPT only when every gate passes, head/payload.json, orig/rows + orig/tests, tests/ if changed, hdr/engine + orig/hdr_<name> + HEADERS.tsv if a header changes, TABLE_CHANGES.tsv, RENAMES.tsv, NAME_FIXES.tsv, pk_gate.txt + reversed control); do NOT write a prose head/HEAD_FROM_REPO.txt (addresses only, or omit). Plus NOTES.md, STATUS.tsv, evidence/.
