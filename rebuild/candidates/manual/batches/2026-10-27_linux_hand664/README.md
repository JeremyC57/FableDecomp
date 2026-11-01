# Chunk 664 -- 10 of 10 landed (2026-11-01, NA009 of the never-attempted pool, authorable band)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA009). Landed via `coordinator/land_chunk.sh 664` (vland --land 10/10
RELOCATION_MATCH + PASS, multi-check 0; 1-3 probes each): 0x0075A480 ~CTCPhysicsRigidBody, 0x00A1CB40 ~CNavigatorAStar, 0x00D005E0
NScript::~CQ_AwakeningTheOracleScript (implicit; tail ~CScriptBase), 0x00523440 ~CThingManager, 0x009FC690 ~CResourceBank, 0x00B33AE0
CEnginePrimitiveRenderer::AddPrimitiveManager, 0x0055B760 NUISystem::~CClickable, 0x00CA5570 CBaseVideoRenderer::OnDirectRender
(DirectShow strmbase), 0x00442610 ~CMiniMapDisplay, 0x00F37570 ~CGPUScratchBufferTextureManager. No row parked.
21 NAME_FIXES queued (c664; 0x00440730 HOLD until the other TU is confirmed). Labels: ghidra_out/labels_chunk664_2026-10-27.tsv (29).
Decision at landing (owner delegation 2026-11-01): engine/CLevelScriptBase.h loses its explicit virtual dtor declaration in migration
wave 10 (retail's ~CLevelScriptBase<T> is implicit; 1,000+ includers but only derived-dtor TUs change) -- 0x00D005E0 keeps a local
copy until then. Descriptive: g_pEnginePrimitiveTypeInfo 0x01436E84 (no PDB data symbol). Header debt per row in NOTES.md.
Lessons in GOTCHAS (1 line) and LESSONS.md.
