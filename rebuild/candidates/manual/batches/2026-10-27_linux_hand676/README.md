# 2026-10-27_linux_hand676 — chunk 676 (NA021, never-attempted pool: 257-512 band, descriptive `_global` labels)

Landed 2026-11-01 via `land_chunk.sh 676`: 9/10 RELOCATION_MATCH, bindings multi-check 0. Parked: SetupStaticBlocksNoMorph 0x00BA48F0
(`pending/2026-10-27_linux_hand676/differ/00ba48f0.cpp`, 22 probes; one extra `mov edi,[ebp+44h]`, every removal flips the register allocation).
8 of 10 labels wrong. NAME_FIXES.tsv (17 rows) queued as c676 (wave 17: wave 16 is running). The unlisted 10 B 0x00ADD5C0 was added as a boundary start.
Decisions (coordinator under owner delegation 2026-11-01): the two CLHFile ctor spellings (`PAVCLHFileManager@@PBD@Z` / `PBG@Z`) are accepted as the
evidence-based best (class in neither PDB; RTTI + CLHFileManager slots 0/1 + the bytes); the landed 0x00D25410 file / vtable binding respell to
CQ_DragonBossFightScript goes to the next naming wave (file rename + ps1). Header debt (NOTES.md): engine/CThingGameObject.h flattens the vptr; the
quest-script pack(1) C3DVector belongs in a shared NScript header; engine/CLevelScriptBase.h still declares a dtor the quest scripts lack (migration wave 10).
