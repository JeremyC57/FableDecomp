# NF154 overlay headers

## engine/CTCPhysicsStandard.h
- Base: rebuild/include/engine/CTCPhysicsStandard.h of the repo working tree at 2026-11-20 (copy in orig/engine/CTCPhysicsStandard.h).
- Added (one block, after `virtual void ResetInterpolation();`, inside FABLE_RETAIL_CTCPHYSICSSTANDARD):
  `#ifdef FABLE_RETAIL_CTCPHYSICSSTANDARD_UPDATEINTERPOLATION` / `protected: virtual void UpdateInterpolation();` / `public:` / `#endif`
  (Ego_r.pdb 0x0073A570 / FableWin.pdb 0x02526850 ?UpdateInterpolation@CTCPhysicsStandard@@MAEXXZ; slot 94 of vtable 0x01265F84 is
  0x00723730, an override of the existing CTCPhysicsBase slot: no new slot, no layout change). Used by row 0x00723730.
- Default path (macro undefined) is token-identical to the base header.
- pin_asserts.py: pinned 0 (the header's asserts are already FABLE_STATIC_ASSERT_AT with literal lines).
- Landed includers: 61 rows under rebuild/src/compiled include CTCPhysicsStandard.h (`grep -rlE 'CTCPhysicsStandard\.h' rebuild/src/compiled`);
  none defines FABLE_RETAIL_CTCPHYSICSSTANDARD_UPDATEINTERPOLATION, so the opt-in changes none of them.
- Sibling overlays read first: $S/w22/agentNF153/hdr and $S/w22/agentNF155/hdr are empty (no overlap).

Landed includers of engine/CTCPhysicsStandard.h (61):
- 00/6a/CTCPhysicsBase_UpdateOldPosition_006af740.cpp
- 00/72/CTCPhysicsControlled_AddImpulseVelocity_00725250.cpp
- 00/72/CTCPhysicsControlled_AdjustThingListForDebugHelpers_007254b0.cpp
- 00/72/CTCPhysicsControlled_GetPossibleCollidedWithArea_00725a30.cpp
- 00/72/CTCPhysicsControlled_GetRenderZOffset_00725e60.cpp
- 00/72/CTCPhysicsControlled_Move_00725c40.cpp
- 00/72/CTCPhysicsControlled_PerformImpulseBasedMovement_00725500.cpp
- 00/72/CTCPhysicsControlled_SetForcedMoveToPos_00725000.cpp
- 00/72/CTCPhysicsStandard_CTCPhysicsStandard_00723f90.cpp
- 00/72/CTCPhysicsStandard_CTCPhysicsStandard_00723fd0.cpp
- 00/72/CTCPhysicsStandard_CanCollideWithLandscape_00724cb0.cpp
- 00/72/CTCPhysicsStandard_CanCollideWithThingsOneWay_00724ca0.cpp
- 00/72/CTCPhysicsStandard_CanCollideWithThings_00724c90.cpp
- 00/72/CTCPhysicsStandard_CanInteractWithThings_00724cc0.cpp
- 00/72/CTCPhysicsStandard_ConstructFromParams_00723570.cpp
- 00/72/CTCPhysicsStandard_EnableGravity_007235e0.cpp
- 00/72/CTCPhysicsStandard_FillPhysicsDebugInfoForSelf_007236c0.cpp
- 00/72/CTCPhysicsStandard_GetFacingAngleXY_00723b30.cpp
- 00/72/CTCPhysicsStandard_GetFacingAngleXZ_00723750.cpp
- 00/72/CTCPhysicsStandard_GetFacingAngleYZ_00723b90.cpp
- 00/72/CTCPhysicsStandard_GetFacingVector_00723c00.cpp
- 00/72/CTCPhysicsStandard_GetFacingVector_00723c20.cpp
- 00/72/CTCPhysicsStandard_GetLastRHSet_00724be0.cpp
- 00/72/CTCPhysicsStandard_GetOffsetVector_00724d30.cpp
- 00/72/CTCPhysicsStandard_GetPhysicsMeshInfo_00724d20.cpp
- 00/72/CTCPhysicsStandard_GetRHSet_00723c80.cpp
- 00/72/CTCPhysicsStandard_GetRHSet_00724bd0.cpp
- 00/72/CTCPhysicsStandard_HasNavigationLayers_00724bc0.cpp
- 00/72/CTCPhysicsStandard_IsAffectedByAir_00724c10.cpp
- 00/72/CTCPhysicsStandard_IsAffectedByGravity_00724c00.cpp
- 00/72/CTCPhysicsStandard_IsAffectedByWind_00724c20.cpp
- 00/72/CTCPhysicsStandard_IsAlwaysFacingMovementDirection_00724bf0.cpp
- 00/72/CTCPhysicsStandard_IsAngledWithGround_00724c30.cpp
- 00/72/CTCPhysicsStandard_IsExcludingCreatures_00724cd0.cpp
- 00/72/CTCPhysicsStandard_IsGravityEnabled_00724bb0.cpp
- 00/72/CTCPhysicsStandard_IsImmoveable_00724c40.cpp
- 00/72/CTCPhysicsStandard_IsOnAnyStationarySurface_00723a60.cpp
- 00/72/CTCPhysicsStandard_IsOnAnySurface_00724c50.cpp
- 00/72/CTCPhysicsStandard_IsOnGroundSurface_00724c80.cpp
- 00/72/CTCPhysicsStandard_IsOnMeshSurface_00724c70.cpp
- 00/72/CTCPhysicsStandard_IsPhysicsEnabled_00724ba0.cpp
- 00/72/CTCPhysicsStandard_IsPhysicsMeshImpassable_00724ce0.cpp
- 00/72/CTCPhysicsStandard_IsPushableByHero_00724cf0.cpp
- 00/72/CTCPhysicsStandard_OnCreate_00724010.cpp
- 00/72/CTCPhysicsStandard_RemoveIntersectingCreaturesFromList_00724240.cpp
- 00/72/CTCPhysicsStandard_ResetInterpolation_007236e0.cpp
- 00/72/CTCPhysicsStandard_ResetRHSet_00723980.cpp
- 00/72/CTCPhysicsStandard_ReturnToDefaultInteractionFlags_00723a50.cpp
- 00/72/CTCPhysicsStandard_SetAsPushableByHero_00724d00.cpp
- 00/72/CTCPhysicsStandard_SetFacingAnglesFromRHSet_007238e0.cpp
- 00/72/CTCPhysicsStandard_SetFacingAnglesFromRHSet_007239c0.cpp
- 00/72/CTCPhysicsStandard_SetInteractionFlags_007234c0.cpp
- 00/72/CTCPhysicsStandard_SetPhysicsMeshBankIndex_00724630.cpp
- 00/72/CTCPhysicsStandard_SetRHSetAsFlushOnGround_00723a10.cpp
- 00/72/CTCPhysicsStandard_SetScaleFromOwner_007236a0.cpp
- 00/72/CTCPhysicsStandard_UpdateOldRHSet_00723930.cpp
- 00/72/CTCPhysicsStandard_UpdateVelocityForImpassability_007243c0.cpp
- 00/72/CTCPhysicsStandard_scalardeletingdestructor_00724de0.cpp
- 00/8e/CTCPhysicsStandard_GetPossibleCollidedWithArea_008e5370.cpp
- 00/8e/CTCPhysicsStandard_GetPossibleInteractedWithList_008e57d0.cpp
- 00/8e/CTCPhysicsStandard_Move_008e5d40.cpp
