# 2026-10-27_linux_hand674 — chunk 674 (NA019, never-attempted pool: first 257-512 band + five gap-scan rows)

Landed 2026-11-01 via `land_chunk.sh 674`: 9/10 (2 MATCH, 7 RELOCATION_MATCH), bindings multi-check 0. Parked: PlayCutsceneSound 0x00CBF7FE
(`pending/2026-10-27_linux_hand674/differ/00cbf7fe.cpp` + test, 16 probes): the 480-byte catalog row holds PlayCutsceneSound (465) and
GetScriptActiveStatus 0x00CBF9CF (15, reproduced byte-exact); the own body differs only in the order of two independent prologue instructions.
Decisions (coordinator under owner delegation 2026-11-01): split the catalog row at 0x00CBF9CF (retail has two functions there); the CrimeText
anonymous-namespace symbol binds under the lane's TU hash per the established policy (no PDB data-symbol hash); the cutscene-music flag 0x0143E8FD
keeps a descriptive name until a data-symbol dump. NAME_FIXES.tsv (19 rows) queued as c674. Follow-ups: align the landed trees.c TU to zlib 1.2.1
(`data_type` in deflate_state), respell 0x01228ED8 / 0x00F29DA0 / 0x00CBFAB8 with their callers (naming wave 16), packed CParentDefClassBase / CEngineLandscapeMemoryManager header debt.
