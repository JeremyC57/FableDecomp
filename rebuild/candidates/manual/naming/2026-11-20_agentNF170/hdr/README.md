# NF170 header overlay (hdr/) -- opt-ins only; base copies in ../orig/ (byte-equal to the repo working tree at copy time)

Every opt-in is `#ifdef`-guarded with a member-specific macro; without the macro each header is token-identical to the repo copy,
apart from the static-assert pins pin_asserts.py wrote (FABLE_STATIC_ASSERT(x) -> FABLE_STATIC_ASSERT_AT(<repo line>, x), the same
typedef names). Sibling overlays read first (NF168: CTCInventoryAbilities.h, CTCInventoryTrade.h, CTCPerceiveThing.h; NF169:
CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h): none touches these headers.

## engine/CCombatAbilityStrafeDef.h
- FABLE_RETAIL_CCOMBATABILITYSTRAFEDEF_STATGETTYPENAME: inline `static CCharString __fastcall StatGetTypeName()` (FableWin.pdb
  ?StatGetTypeName@CCombatAbilityStrafeDef@@SI?AVCCharString@@XZ 0x02433CA0; GetSubDef<CCombatAbilityStrafeDef> 0x006F0F60 pushes the
  literal). Added inside both retail class variants (ALLOC and plain). Users: rows 0x006EA2C0, 0x006F0F60.
- pin_asserts.py: pinned 0 (the asserts already carry explicit lines).
- Landed includers (0044c72b 0044c599 0044c57d 007355f0 ): none defines the macro, so none changes.

## engine/CCombatAbilityUseProjectileWeaponDef.h
- FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_STATGETTYPENAME: inline StatGetTypeName (FableWin.pdb 0x01DA9C60; the literal in
  GetSubDef<CCombatAbilityUseProjectileWeaponDef> 0x006696B0). Both class variants. Users: rows 0x0097EF60, 0x0097F3A0, 0x006696B0.
- pin_asserts.py: pinned 0.
- Landed includers (0044c72b 007355f0 ): none defines the macro, so none changes.

## engine/CTavernGameDef.h
- FABLE_RETAIL_CTAVERNGAMEDEF_PRIZESCORES_VECTOR: types PrizeScores (+0x174) as Ego_r.pdb has it, std::vector<CPrizeScoreDef> (STLport,
  12 bytes; element stride 0x30), and includes stlport/fable_containers.h + engine/CPrizeScoreDef.h for it. Same layout (the typed-member
  opt-in pattern of CDefinitionManager.h ErrorCallback). User: row 0x005EA010 (walks the prize scores).
- pin_asserts.py: pinned 16 (the CheckRetailLayout asserts below the inserted lines).
- Landed includers (007cf860 007cf7e0 007cff40 007cf9a0 007cf530 008ef910 008f10b0 008f0b80 008f09b0 008f09d0 008f2280 008f0eb0 008f0b20 007d3510 007d60c0 007d7c40 007d3820 007d5560 007d71e0 007da760 007da120 007d4ed0 007d5890 007d6870 007da640 007d5a00 007da3a0 007d7f10 007d7ec0 007db6a0 007d5070 007d6260 007d8e20 007d3700 007d6150 007d80c0 007d8290 007da400 007d8160 007daef0 007d8570 007da520 007d83c0 007d62d0 007d4e30 007d3560 007d4f40 007d7fd0 007d70e0 005ec270 005ea650 005eb590 005eb9e0 ): none defines the macro; the pins keep every typedef name, so none changes.
