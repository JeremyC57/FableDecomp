# Chunk 649 -- 2 of 10 landed + 1 bonus + 1 re-author (2026-10-31)

Cut from the fixable bucket of the re-ranked pool. Landed via `coordinator/land_chunk.sh 649` (vland --land 2/2 RELOCATION_MATCH + PASS,
multi-check 0; new bindings CCombatManager::IsCreaturePerformingNonInterruptableSequence 0x006E97A0 and the Ego_r data symbols
VIRTUAL_SCREEN_WIDTH 0x013961E8 / VIRTUAL_SCREEN_HEIGHT 0x013961F0): CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending
0x0093DAE0 -- the Ego_d.pdb inline accessors (CAIStateGroupBase::GetCreature -> CAIBrain::GetCreature; GetCombatManager -> GetWorld().
GetCombatManager()) plus a named `CCombatManager& manager` reference local; raw member chains gave the wrong registers; GFACoordToVCoord
0x009E21D0 -- the Y half written `(float)((float)VIRTUAL_SCREEN_HEIGHT * y) / dims.Y`: the outer truncation makes the product spill into
y's argument slot as retail; GetRenderTargetDimensions returns `const C2DExtentsI` by value (user ctors + copy ctor). Bonus: GFAYToVY
0x009E1DD0 (not a chunk row, found while matching 0x009E21D0; landed separately from `bonus/`, 1/1 RELOCATION_MATCH + PASS). Re-author:
the landed GFAXToVX 0x009E1DA0 carried a `volatile` on the width global (FIDELITY NOTE, 2026-10-23); the same truncation lever
`(float)((float)VIRTUAL_SCREEN_WIDTH * x) / dims.X` matches volatile-free (`verify_gfaxtovx_reauthor.log`; new test, pass pattern updated
in build_candidates.ps1; binding `g_FableVirtualWidth_013961E8` -> `?VIRTUAL_SCREEN_WIDTH@@3NA`). Eight rows stay DIFFER in
`pending/2026-10-27_linux_hand649/differ/` (00587bff frame 0x20 v 0x1c loop-counter slot, 00563480 drifted copy -- re-derive from its earlier
grade first --, 005f45b5 items in ebx v reloaded, 0086bfb0 one jump target in the erase arm, 00bf01e0 / 00b5e0f0 / 00b6fa40 SetState test/or
operand roles, 00d39960 post-loop cleanup placement). `pending/.../lever_probe_not_reconstruction/` holds 00bf01e0 and 00b5e0f0 variants that
match ONLY through dead member loads: lever probes, never to be landed without an owner ruling. Lessons: the PDB inline accessors fix
register roles that raw member chains get wrong; `(float)((float)a*b)` forces an early product spill into the dead argument slot (and
replaces the GFAXToVX volatile); VC7.1 picks SetState test/or operand roles by a tie-break on the function's expression-temp count (dead
loads through a parameter or `this` flip it, globals/constants do not); the real PDB model CRenderStateManager::CStateInfo / SetRenderState /
AddStateToUpdateList / CDeviceState fixes the roles without dummy loads but moves `mov ecx,eax`; a reloc-masked diff must not mask jump
targets. 9 NAME_FIXES + 5 review rows queued (prefix c649): CAIStateGroupBase::GetWorld protected const (GetPWorld in ~45 bindings and four
landed siblings 0x0093E240 / 0x0093DDE0 / 0x0093DB70 / 0x0093DCD0), IsPerformingAnyAction const, 0x006E97A0 label, GFGetSystemManager,
GetRenderTargetDimensions const by-value, GFACoordToVCoord reference, the two VIRTUAL_SCREEN data names (11 and 9 descriptive spellings
bound), SetUserVertexShaderConstant(C4DVector) (bytes cannot tell C4DVector from CRGBFloatColour); CDeviceState member mislabel in landed
0x00BF1030 (StencilOpPass +0x2BC4 / StencilRef +0x2BEC swapped); the dead-load lever probes (owner ruling). Labels:
ghidra_out/labels_chunk649_2026-10-27.tsv (+ GFAYToVY 0x009E1DD0).
