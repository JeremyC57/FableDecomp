# Agent NF363 (Account A naming queue, 2026-11-24): finish NF360 b1 (0x0042AF3C) and build NF359's option A unit

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full. Lock name: NF363.
Folder: $S/w22/agentNF363/. Rulings below are advisor decisions, owner delegated (2026-11-24).

## Part 1: 0x0042AF3C GetDef<CThingBaseDef> self respell (NF360 b1)
NF360's hand-back: rebuild/candidates/manual/pending/2026-11-24_nf360_handback/ (NOTES.md, batches/b1/, evidence/); its working copy is
$S/w22/agentNF360/ (read only; copy what you need). Ruling R1: the strong in-place form lands, because the different-body definers are
already landed under the PDB name. Ruling R2: the `reinterpret_cast<CDefPointer<const CDefClassBase>&>(*out) = def` is accepted only as
marked scaffolding.
1. Edit b1's row text: put `// Fidelity: scaffolding, not retail` on the cast with the reason (the only measured text that keeps every COMDAT
   name; goes away with the nf349 header port of 0x0042AF3C, T3 line nf349 0x0042AF3C). Fix the stale "the def type is opaque" comment in
   the same edit. Nothing else changes.
2. Regrade from scratch and rerun every NF360 gate on the edited text: vland RELOCATION_MATCH PASS, objsec (only the self name changes v
   HEAD), both checkers HEAD v new with the reversed and planted controls, reach flips, drop test, link with the owner binder 0x004CA010.
3. Read-only: compile from HEAD the ten landed strong copiers NF360 checked only by source (0x005B43A2 0x005B4795 0x005B4EBA 0x005B5BE6
   0x0071DA70 0x007744A0 0x00801A50 0x00811470 0x008115A0 0x008ED1F0; 0x00659164 is done). For each: does the object define
   ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z, with what selection, size, and is
   it EQ to retail 0x0042AF3C (pk_all --leaf / retcmp)? This decides the T3 line text, not land or hold.
Hand back b1 as a land_scripted run dir (ACCEPT if every gate passes). The coordinator parks T3 line nf349 0x0042AF3C during landing and
restores it (do not release it).

## Part 2: NF359 option A (Alloc form) for 0x00455EF0, 0x0044C1BB, 0x0045C5DA
NF359's hand-back: rebuild/candidates/manual/pending/2026-11-24_nf359_handback/ (NOTES.md, held/h1_ctor_ports_unit/options/A_allocform
with README, evidence/definers.txt); working copy $S/w22/agentNF359/ (read only). Ruling: option A, with the 0x004304E0 precedent; its
only duplicates are identical-byte strong Alloc ones (the nf184 LNK2005 pattern, as at 0x00432C40 / 0x004317D0 / 0x004304E0). **No
0x0045C5C1 row:** it is already emitted EQ by 0x0045C5F6's object; leave it alone. NF359 measured option A with parity_of only, so build a
fresh land_scripted run dir with full gates:
- vland RELOCATION_MATCH (or MATCH) PASS per row, HEAD v new;
- tests rewritten on the shared headers (engine/CThingNoiseDef.h, engine/CTCNoiseDef.h; no local models of those classes);
- both checkers HEAD v new with reversed and planted controls; reach flips 0; nothing turns NEQ;
- drop test on every symbol HEAD defines that option A no longer does: `??0CMemberBase_0044c0c0@@QAE@XZ`,
  `?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z`, the 1-slot (4 B) `??_7CThingNoiseDef@@6B@` / `??_7CTCNoiseDef@@6B@` bodies,
  and any other: reloc_bindings lines witnessed by these rows, relocations in other landed objects that need them, pk_all EQ copies lost;
- TABLE_CHANGES: NF359's proposed `delete ??0CMemberBase_0044c0c0@@QAE@XZ 0044c0c0 00455ef0` only if your drop test confirms it;
- a link of the unit (the three new objects + 0x0045C5F6's landed object, plus 0x00432C40 / 0x004317D0 / 0x004304E0 for the Alloc
  duplicates): report every LNK2005 and whether its bodies are byte-identical. A duplicate with different bodies holds the row.
- self names: 0x0045C5DA's row graded on ??_GCThingNoiseDef (the Alloc text graded on ??_G, NF359 measured RELOCATION_MATCH); the self /
  leaf / catalog spelling must be the PDB one; RENAMES for any file rename (proposal only).

Other agents running (stay off their rows): NF361: 0047a9f0 005fb7e0 005fc2d0 005fc8a0 005fcbe0 005fddd0 005fe090 005ff190 006001e0
00600b60 00603000 00605570 00607330 006096e0 0060c900 0060eeb0 0061b610 0061e6d0 00622a00 00868e50, then 0x0066C340 census users.
NF362: 00427216 00582ace 0058315b 0058a0c7 0058e208 005907ab 0059cb90 0059d270 0059ed42 0059f51e 0059fee3 005a29f4 005a8c66 005a9cbe
005aaf28 005ad571 005cdd86 005d64dd 005d6748 005d7081 0041e6d3 00584b86 0058578b 005a04f7 005a8a5d 005b3aef 005b76d1 005b8743 005c6b26
005c7452 005c8459 005c90fc 005c9a95 005cacb9.

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad. In every Bash call run
  `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`. Run `mkdir -p /run/user/0` if it is missing.
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.
- Checkers: run both, HEAD against new, on every row you change.
  - rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py, with python3 -I.
  - tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py. It needs env FABLE_EXE=<retail exe> and
    FABLE_ROOT=/home/user/FableDecomp, plus --row-length=<catalog length> and --leaf=<the row's // Leaf: symbol>.
  - Diff the reach column HEAD v new: an already-NEQ copy whose reach flips n -> y holds the row (NF352 rule). `$S/tools/reachflip.py <dir>`
    pairs H_<addr>.pkall.tsv / N_<addr>.pkall.tsv in one folder.
- awk gotcha: compare addresses as strings (`$1""==a""`); `00e07640`-shaped values compare numerically otherwise.

## Hand-back
- land_scripted run dirs under batches/bN/: rows/, RESULTS.tsv (ACCEPT only when every gate passes), head/payload.json, TABLE_CHANGES.tsv,
  orig/ (rows + tests), tests/ if changed, hdr/engine/ plus orig/hdr_<name> and HEADERS.tsv for any header edit. Put prose notes in NOTES.md,
  never in head/HEAD_FROM_REPO.txt (land_scripted reads that file as an address list).
- NOTES.md (decisions, measurements), STATUS.tsv, evidence/, tools/ (every helper you wrote).

Rules:
- Read docs/COORDINATION.md, docs/pipeline/ASM_RULES.md, tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md and
  docs/pipeline/GOTCHAS.md before the first compile. Byte purity: no __asm / naked / _emit; `volatile` only where retail used it.
- Locks: $S/w22/nflocks/<addr>/owner holds the owner name; claim only a lock that is missing or reads `released`, release by writing
  `released`. Precheck a row's test before claiming its lock, and its source after claiming and before compiling.
- Stay off: Account B's claimed or staged rows (rebuild/candidates/manual/COORDINATION_CLAIMS.tsv, pending/*acctB*), Codex reservations,
  and the other running agents' rows (listed above).
- Never write into the repo and never run git. Run long jobs in the background; no sleep loops.
- Loop prevention: never repeat a failing call more than twice with the same arguments.
