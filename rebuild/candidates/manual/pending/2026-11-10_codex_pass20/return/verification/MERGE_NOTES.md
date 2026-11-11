# Pass20 combined header overlay

Completed merged_hdr for all40 selected rows, including34 verified bodies and6 parks. The overlay contains169 files:168 C++ headers plus combined README.md. It is an overlay over the unchanged input/include, not a replacement input tree. No row source, row metadata or selected row header was edited. No Git command was used.

Every selected hdr/ directory was inventoried from OWNERSHIP.json. MERGE_INVENTORY.json maps each path to source rows; MERGE_INPUT_HASHES.json records all40 row overlay hashes, checked unchanged after validation. MERGED_HDR_HASHES.json records the final output. There were24 paths with distinct selected versions, including the per-row README notes. Initial baseline-relative union resolved non-overlapping edits;12 overlapping edit groups were inspected and resolved manually. New headers were coalesced only when one full selected file was proven to contain all others as exact ordered-line subsets, or when the models were identical apart from their explicit enabling guard.

# Conflict resolutions

| Path | Resolution |
|---|---|
|README.md|All row notes retained under their source addresses.|
|engine/CTCVillage.h|Manually inspected independent complete #ifdef insertion blocks at the same baseline anchor; unioned all row-specific declarations/accessors and remaining disjoint edits.|
|engine/CThingAICreature.h|Union of distinct non-overlapping baseline-relative edits.|
|engine/CTCHeroStats.h|Manually inspected independent complete #ifdef insertion blocks at the same baseline anchor; unioned all row-specific declarations/accessors and remaining disjoint edits.|
|engine/CTCSoundPlayer.h|Manually inspected independent complete #ifdef insertion blocks at the same baseline anchor; unioned all row-specific declarations/accessors and remaining disjoint edits.|
|engine/C2DBoxI.h|Union of distinct non-overlapping baseline-relative edits.|
|engine/CMainGameComponent.h|Union of distinct non-overlapping baseline-relative edits.|
|engine/CTCVolumeContainmentTracker.h|Same complete owner; preserve native declaration-only setter for volume update and genuine visible setter only under existing DIVINE_FINALCLEANUP policy.|
|engine/NGraphicAppearance_CTCGraphicAppearance.h|Unioned volume layout/static type/changed flag, flare declarations and static focal declaration. Manually nested existing MOVEMENT include policies so TRAIL and STATIC_FOCAL use genuine CEngineGraphic while original default still uses its unchanged quarantine include. Independent later fields retained.|
|engine/CTCTargetingBase.h|Coalesced identical full56-byte FIRE_RANGED/UPDATE_NEAREST owner using the portable enum guard; retained ATTACK-only visible GetTargetingDef in its original default-owner branch. Original input default members preserved.|
|engine/NCreatureMode.h|Union of distinct non-overlapping baseline-relative edits.|
|engine/CThingCreatureBase.h|Manually inspected independent complete #ifdef insertion blocks at the same baseline anchor; unioned all row-specific declarations/accessors and remaining disjoint edits.|
|engine/CTCPerceiveThing.h|Union of distinct non-overlapping baseline-relative edits.|
|engine/C3DVector.h|Preserved both deliberate GetMagnitude2D source bodies at their original positions; ATTACK and UPDATE_NEAREST row guards are mutually exclusive for these definitions to avoid duplicate helpers if combined. No selected TU enables both. ATTACK copy-constructor body retained, input default unchanged.|
|engine/CThing.h|Union of distinct non-overlapping baseline-relative edits.|
|engine/CMessageEventFilters.h|Unioned payment/original-guard/yell unary_function conditions; preserved original null early return and range-filter base only under ORIGINAL_GUARD; added YELL-specific original filters/constructor without changing other branches.|
|engine/CCodaContext.h|Selected 00c3db80 only after proving every other complete selected version is an ordered exact-line subset; this retains all original and additional guarded members. Narrowed the factory-only CCodaSource forward declaration to its existing CREATE3D opt-in, preserving the Process and original default token stream.|
|engine/CCoda3DSource.h|Existing new-header versions are exact ordered subsets of 00c3db80, which retains all additions.|
|engine/CCodaSource.h|Union of distinct non-overlapping baseline-relative edits.|
|engine/CTCSpecialAbilities.h|Manually inspected independent complete #ifdef insertion blocks at the same baseline anchor; unioned all row-specific declarations/accessors and remaining disjoint edits.|
|engine/CQuestManager.h|Selected 004b2280 only after proving every other complete selected version is an ordered exact-line subset; this retains all original and additional guarded members.|
|engine/CEnginePrimitivePrepareInfo.h|Identical complete definition/default branch; union enabling condition STEALTH_DRAW || CENGINEPRIMITIVEPREPAREINFO preserves both existing row options.|
|engine/CEnginePrimitive.h|Inspected nested union: ID_CTOR public original three-argument body is retained; otherwise STEALTH_DRAW exposes its original public three-argument body and protected legacy two-argument ctor; with both off the original default protected ctor remains exact. No constructor access or body was flattened across rows.|
|engine/CCombatActionBase.h|The three full276-byte models are byte-identical except their enabling guard; coalesced the three existing row opt-ins. Exact original default branch retained.|

CTCWeaponTrail.h also received one narrow default-preservation adjustment: its newly introduced CThingCreatureBase / CEnginePrimitivePrepareInfo forward declarations are guarded by the existing CTCWEAPONTRAIL_UPDATE option. CCodaContext's factory-only CCodaSource forward declaration is similarly guarded by CREATE3D. These retain the selected token streams and restore the exact original default graphs. No shared default behavior/layout was altered; no new packing, qualifier or inline policy was added by integration.

The C3DVector source policies remain separate under existing row guards, at their original positions. ATTACK takes precedence if both row-specific options are defined; no selected row defines both. This avoids duplicate GetMagnitude2D/GFFastSqrt definitions while retaining each selected build. The two getter source forms were not flattened into a shared implementation. Likewise, the volume setter visibility, primitive constructor access/overloads, original introducing virtuals and graphics-owner include choices retain their selected guarded policies.

All other169-path resolutions, including identical copies and independent disjoint additions, are recorded in MERGE_RESOLUTIONS.json. All per-row README text is preserved in merged_hdr/README.md under its address. New shared headers absent from input are included intact.

# Validation completed

All40 current selected translation units preprocess successfully against the combined overlay. Each complete preprocessed token stream matches that row's own selected overlay after ignoring whitespace, physical include-root path differences and the numeric suffix of generated fable_static_assert_LINE typedef names. No function/type/member/statement token was otherwise normalized. The generated names change only because the shared header line numbers change. MERGE_PREPROCESS_RESULTS.json records all40 passes.

A separate default audit removes all55 newly introduced FABLE_RETAIL opt-ins and preprocesses each row's existing input-header include graph using its remaining original prerequisites. All40 graphs agree with the original input tokens under the same narrow normalization. MERGE_NEW_OPTINS.json and MERGE_DEFAULT_RESULTS.json record this audit. No baseline or include-cache file was edited; the temporary validation tree uses hardlinks for unchanged input files and unlinks each overlay target before copying it, then is read-only during checks.

Final RTM regrading remains with main as requested. No final byte-match or behavior-test claim is made by this integration pass; the preprocessing/default checks show that the selected code/type tokens are preserved. Main's prior per-row audits and tests remain the baseline for the pending combined-header grade.
