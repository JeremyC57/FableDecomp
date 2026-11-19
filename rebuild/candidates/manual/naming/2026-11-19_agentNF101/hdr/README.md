# NF101 header overlay

Every overlay header is a copy of the current repo header (base copy in ../orig/engine/) plus `#ifdef` opt-ins only. Without the opt-in
macros each header is token-identical to the repo copy except for FABLE_STATIC_ASSERT lines that pin_asserts.py respelled
FABLE_STATIC_ASSERT_AT(<old line>, ...) (same typedef names, so the default path is unchanged).

## engine/CTCWeapon.h
- Added `FABLE_RETAIL_CTCWEAPON_GETMISSILELOADCARRYSLOT` (row 0x0078B070): declaration `long GetMissileLoadCarrySlot() const;` 0x005DA920
  (Ego_r.pdb / FableWin.pdb ?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ). Placed before the READYPROJECTILE block. pin_asserts: pinned 0.

## engine/CCreatureAction_HeroLoadRangedWeapon.h
- Added `FABLE_RETAIL_CCREATUREACTION_HEROLOADRANGEDWEAPON_CLASS` (row 0x0078B070): the polymorphic class on CCreatureActionBase with
  `virtual void PerformAction();` and PRangedWeapon (+0xA8) / PAmmo (+0xB0); the generated flattened struct and its asserts sit in the
  `#else` branch. pin_asserts: pinned 4 (the four generated asserts, now FABLE_STATIC_ASSERT_AT(54..57)).
- Landed includers (grep of rebuild/src/compiled): CTCWeapon.h 68, CCreatureAction_HeroLoadRangedWeapon.h 2. None defines the new NF101
  macros, so their compile is unchanged (default path token-identical; the pinned asserts keep their typedef names).

## engine/CThingSearchTools.h
- Added `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCDSINROUGHAREA_EGOD` (row 0x005185F0): the GetAllTCDsInRoughArea<T,Filter,Container> body defined
  out of class, appended just before the final `#endif` (after the TCSINROUGHAREA_EGOD and FOREACHINROUGHAREA_EGOD blocks), so no line
  before it moves. Guarded like TCSINROUGHAREA_EGOD (needs FABLE_RETAIL_CTHINGSEARCHTOOLS, TCSINROUGHAREA, ROUGHAREA_EGOD, not TCSINROUGHAREA_BODY).
  pin_asserts: pinned 0.
- Landed includers of CThingSearchTools.h: 329; none defines the new macro, so the default path and every other opt-in path are unchanged (the block is appended at the end under its own guard).

## engine/CThingCreatureDef.h
- Added `FABLE_RETAIL_CTHINGCREATUREDEF_VALIDATE` (row 0x00670D50): `public: virtual void Validate(const class CValidateDefinitionInitBase&,
  const class CCharString&) const;` (Ego_r.pdb / FableWin.pdb ?Validate@CThingCreatureDef@@UBEXABVCValidateDefinitionInitBase@@ABVCCharString@@@Z),
  placed just before the ALLOC opt-in at the end of the class. Declaration only, no layout change. pin_asserts: pinned 0 (the asserts
  after it are already FABLE_STATIC_ASSERT_AT).
- Landed includers: 49; none defines the new macro, so their compile is unchanged.
