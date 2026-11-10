# 2026-11-10 landing y -- chunk 1006, the leftovers of chunks 1004 / 1005 (4 rows)

Source: `pending/2026-11-10_linux_hand1006/`. vland --land: 1 MATCH + 3 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x0056EF2D CTCInventoryExperience::UpdateMenuScrollButtonsItem (MATCH): `long statCount = statIndicatorMax;` read at the top of the item
  branch puts the member load early, in edi (found with 23 c2prio probes, 1 grade).
- 0x00570332 AddAbilityToList / 0x00571280 AddAbilityToListPC: the blank CWideString in its own `{}` block frees textBank's slot (frame 0xD8).
  The coordinator then added FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA so the CComponent releases bind ?ReleaseData (0x004291DE) instead of
  ??1 (rechecked RELOCATION_MATCH, logs/vland_recheck_releasedata.log).
- 0x005B3E4E CTCInventoryClothing::GlobalUpdate: on shared headers -- new CAppearanceDef.h, CAnimFilterBase.h, CAnimateThingUpdate.h,
  CAnimationSet.h, CTCAnimationComplex.h; opt-ins in CAnimInit.h, CAnimSet.h, CAnimateThing.h, CAnimFilterNoGroup.h, CAnimationEntry.h,
  CTCHeroAttachableAppearanceModifiers.h, NInventory_CTCInventoryBase.h, CTCInventoryClothing.h (hdr/README.md). Levers: visible
  GetSubDef<T> under auto_inline(off), `anim = &defaultAnim; if (..) anim = &other;`, the CAnimInit argument ctor, a named
  CAnimateThingUpdate local.

Parked (differ/): 0x005CA68E SetupActiveWeaponsOnCarrier DIFFER(1538v1535) (`this` beats the 0x48 constant for ebx); 0x00577AC0
OpenInventoryInternalPC DIFFER(2157v2157) (4-term sum operand order, ~110 probes); 0x005B9641 DrawPreparePrimitivesForRendering (needs
CUIMiscThingsDef / CRightHandedSet / C3DOrientationInterpolator / morphed-mesh headers). Queued `land1011z`: 8 name fixes (GlobalUpdate
UAE, CAnimateThing::Update, CAnimInit::Init, CAnimationSet::GetAnim, CAnimationEntry::GetTransitionTime / GetAnimateThingFlags,
GetSubDef<CAppearanceDef>, CTCAnimationComplex::GetAnimation).
