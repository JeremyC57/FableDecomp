# 2026-11-12 landing eb -- chunks Q32 (6 of 6) and Q31 (2 of 4): eight rows matched under the one-row-at-a-time rule

Sources: `pending/2026-11-12_linux_handQ32/`, `pending/2026-11-12_linux_handQ31/`.
Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
- Q32: 0x00EB3BA0 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero, 0x00BD2A90
  CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings, 0x00958B20 CAIStateGroup_MinionReactToCrime::OnReplace, 0x00465C54
  CCombatAbilityBlockDefBase::Transfer (`/d2inlS39`), 0x009FC950 CGraphicFrameData::InitialiseAsSurface, 0x00722740
  CTCSmokeGenerator::FrameUpdate.
- Q31: 0x008F70F0 CMap::AddThemeDefIndexToPalette, 0x00A69B70 CTextureManager::AllocateTexture.

Headers (only those the rows use; additions only, hdrcheck2 default=same, no existing opt-in changed):
- Q32 (9): stlport/fable_map.h STLP_TREE_CTOR_DTOR_OUTOFLINE, C3DMesh2.h BONES, new retail CAIStateGroup_MinionReactToCrime class,
  CGraphicFrame.h, CThingBuilding.h FORCEOPENCOUNT, CTexture.h GETWIDTH, CTCCreatureOpinionOfHero.h SETDEEDMASK,
  CEngineInternalPrimitiveAnimatedMesh.h BONESCALINGS, C3DAnimationBlendState.h SKELETALMORPH.
- Q31 (6): CTextureManager.h, CManagedTextureData.h, CManagedTexture.h (ALLOCATETEXTURE), CIVCountedPointer.h RESET_FORCEINLINE,
  CMap.h THEMEPALETTE_GETINDEX_NONCONST, CHeightMap.h GETCELL.

Real callee bodies carried inside row files (keep each identical to its own row): IsValid 0x00CD23B9 in 0x00EB3BA0;
GetSubDef<CSmokeGeneratorDef> 0x00722D30 in 0x00722740; ZeroAllUnusedThemePaletteEntries 0x008F6FC0 in 0x008F70F0.
Coordinator action: 0x00722740's first hand-back defined a local CDefinitionManager stub; it was sent back and fixed before landing.
Checks: reserved_check 0; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
RELOC_NAME notes (queued land1011eb): 0x00465C54 calls 0x00451ADD / 0x00451B07 as the set<EWeaponClass> tree ctor / dtor (landed
`CList_Initialize` / `std::list<T_00451b07>::~list`; same-shape bodies, unproven) and 0x00463F3C as
CPersistContext::TransferVector<EWeaponClass> (landed `TransferBinary_00463f3c`).
