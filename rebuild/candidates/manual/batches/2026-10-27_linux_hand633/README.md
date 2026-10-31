# Chunk 633 -- 1 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 633` (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0):
CNavigatorAStar::IsNavLineOfSightClear 0x00A1AF00 -- what matched was the PDB signature set: the const
CNavigatorManager::GetNodeAt(const C2DVector&, unsigned char) 0x00A14F70 reached through the private inline
CNavigatorAStar::GetNodeAt(const C3DVector&), the level narrowed from the interface slot 16 GetNavigationLevelAt (returns long);
the earlier non-const (C2DVector, bool) direct calls scheduled the second `lea ecx` ahead of `push eax`. New binding
IsRouteClear 0x00A1A260 (private const). Nine rows stay DIFFER in `pending/2026-10-27_linux_hand633/differ/` (00752f30
CTCLook::UpdateHeadCreatureAnim callee-saved swap, 004c10e0 GetStaticFocalPos zero-offset arm, 008da7a0 CModeDrained::OnDeactivate
ebx/edi swap, 009fe460 CFrameData::FreeTextureMemory eax/ecx/edx rotation, 008af500 CombatDialogueManager::AttemptToSpeak 236v235
esi/edi + fps register -- the forbidden `#pragma pack(push,1)` of the earlier source is removed --, 00a3f250 CSoundManagerLH::vfunc_39
scratch rotation, 0091ea70 IsAbleToFire scratch rotation, 00c11230 CSoundEventManager::CanStartEvent return-0 epilogue placement,
0074e140 GeneratePopoutVectorFromVars one mov after the fstp). New residue class "scratch rotation" (one consistent eax/ecx/edx step
over the whole function; QFE 4035 gives the same bytes as RTM 3077 on four rows). 5 NAME_FIXES (GetNodeAt 0x00A14F70 const uchar --
nine spellings in reloc_bindings, callers to respell --, GetFirstCombinationAnimFromType 0x0070C4E0, CAnimInit::Init 0x0070C050,
GetNavigationLevelAt 0x006CDA00 slot 16 returns long, GetPVillage 0x007EE540) + 3 review rows queued (prefix c633).
Labels: ghidra_out/labels_chunk633_2026-10-27.tsv.
