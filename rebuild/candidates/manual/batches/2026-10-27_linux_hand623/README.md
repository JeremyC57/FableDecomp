# Chunk 623 -- 2 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 623` (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0):
NUISystem::CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D (optimize("s",on)/("y",off); one whole
`component = CreateComponent(defs.GetDefGlobalIndexFromName(name));` statement per arm lets VC7.1 merge the tails and keep
retail's this-before-argument order; CDefinitionManager from engine/CDefinitionManager.h) and CTCHeroExperience::GetRunningNumHits
0x006D63A0 (catalog said CEngine::GetEditorAnimation; two divisions by one named float local give retail's `fdivr 1.0` reciprocal).
Eight rows stay DIFFER in `pending/2026-10-27_linux_hand623/differ/` (00bc9b30 esi/edx swap, 00c73830 CRegistryKey::QueryDWord
block order vs ebp frame, 009e1960/009e1a80 GFStringToInt guard-jump threading, 00c52c40 CCpuInfo::GetCyrixModelName ecx/eax,
00be1e20 CEnginePrimitive2DViewportManager ctor edx/eax, 007447e0 epilogue order, 00521a90 operator[] argument registers).
6 NAME_FIXES (CEnginePrimitiveManager ctor 0x00BACDB0 by reference, DoesNPCNameEntryExistForThing 0x0051F010, map operator[]
0x0052B8B0, 0x00527E00, CThing::GetPos 0x004C73D0, CBaseIntelligentPointer::GetPItem 0x00A01B50) + 6 review rows queued (prefix
c623). Labels: ghidra_out/labels_chunk623_2026-10-27.tsv.
