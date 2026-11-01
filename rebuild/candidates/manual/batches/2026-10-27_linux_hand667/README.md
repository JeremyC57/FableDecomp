# Chunk 667 -- 10 of 10 landed (2026-11-01, NA012 of the never-attempted pool, authorable band)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA012). Landed via `coordinator/land_chunk.sh 667` (8 RELOCATION_MATCH + PASS)
and a second `vland --land` against `oracle_int3_n1.tsv` for the two int3-after-noreturn throw stubs (0x00A5AFB0 CCachedFile::Write,
0x00A5B190 CCachedFile::SetLength: retail's 96 bytes + the 0xCC VC7.1 emits after `call _CxxThrowException`, GOTCHAS 2026-10-22);
multi-check 0. Rows: 0x008553C0 CCreatureAction_BlockRespond::FrameUpdate, 0x008C3F20 CCreatureAction_WaspQueenDie::FrameUpdate (13 probes),
0x0091D8C0 CAIStateGroup_EnemyCreatureBase::OnActivate, 0x00A75B70 CNavNavigableLeafNode::SetUpNeighbours, 0x00B0A060
NParticleEngine::~CPSCSplineBase, 0x00A6B5B0 ~CManagedTextureData, 0x009AC7C0 NBankFileManager::~CRetailBank, 0x00B3AB10
~CEngineInternalAttachedPrimitiveInfo. No row parked.
15 NAME_FIXES queued (c667). Labels: ghidra_out/labels_chunk667_2026-10-27.tsv (24). Descriptive: CEnginePersistentPrimitiveBase slots 2/3,
NAV_NEIGHBOUR_0..3, CBBBFileException::NotOpenForWrite = 8, CAIStateGroup_EnemyCreatureBase slots 14-21 order (FableWin names, stub-inferred
order; slot 20 IsCombatStateGroup two-sourced). `__forceinline` on the CEnginePrimitiveHandle / CIVCountedPointer helpers (0x00B3AB10) per
the 2026-11-01 rule. Header debt: CNavNavigableLeafNode, CANavQuadTreeNode, CCreatureAction_BlockRespond, CAIStateGroup_EnemyCreatureBase,
CManagedTextureData headers and fable_retail_banks.h flattened; engine/CWorld.h + CAIBrain.h cannot hold their inline members.
Lessons in GOTCHAS (1 line) and LESSONS.md.
