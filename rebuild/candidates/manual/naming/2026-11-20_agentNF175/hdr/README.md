# NF175 overlay headers

Each header is a copy of the repo working-tree header (base copy in ../orig/ at the same path) with add-only `#ifdef` opt-ins.
Without the opt-in macros every header is token-identical to its orig/ copy (no default-path change). pin_asserts.py was run after
each edit.

## engine/CScriptedMapBrush.h (base: rebuild/include/engine/CScriptedMapBrush.h)
- `FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSTOMOVE` (needs FABLE_RETAIL_CSCRIPTEDMAPBRUSH + _THINGSETS; row 0x0082E4E0, q4390): the nested
  Ego_r.pdb class `CThingPosAndOrientation` (C3DVector Position +0, C3DAngles FacingAngles +0x0c, 24 bytes), `ThingsToMove` typed as
  `std::map<unsigned __int64, CThingPosAndOrientation>` (12 bytes, same +0x20 offset), and the declaration of
  `GetThingsMovement(unsigned __int64) const`.
- pin_asserts.py: pinned 0 (the asserts already carry their AT line numbers; none moved).
- Landed includers (grep -rlE 'CScriptedMapBrush\.h' rebuild/src/compiled rebuild/tests): none defines the new macro, so the opt-in changes
  none of them.

## engine/CCombatActionBase.h (base: rebuild/include/engine/CCombatActionBase.h)
- `FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS` (MELEE branch; rows 0x00956720 / 0x00956730, q11895 / q11896): the eleven virtuals
  CCombatActionBase adds to CCreatureActionBase's seventy (same names / order as the other branch's existing `_OWN_VIRTUALS`), declared after
  every other virtual of the MELEE class (slots 70..80; retail ThrowShotObject vftable 0x012944D4 has 81 slots, NF148 NOTES). Access is
  restored to `protected:` after the block. Declarations only.
- pin_asserts.py: pinned 0. Landed includers: 213 files under rebuild/src/compiled + rebuild/tests; none defines the new macro, so none changes.
- Slot check (scratch compile work/slotprobe.cpp with both NF175 macros, virtual calls through a pointer): GetIsToFaceTargetCone `call [eax+138h]`
  (slot 78), IsToFaceTarget `jmp [edx+124h]` (slot 73), as retail vftable 0x012944D4.
- Not checked by a vtable link: a TU that emits ??_7CCreatureAction_ThrowShotObject@@6B@ (0x009551A0 / 0x009566C0 / 0x00956960) must define
  this macro and _FACETARGET together or not at all (they define neither; my two rows emit no vtable).

## engine/CCreatureAction_ThrowShotObject.h (base: rebuild/include/engine/CCreatureAction_ThrowShotObject.h)
- `FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_FACETARGET` (_CLASS mode; needs _MELEE_OWN_VIRTUALS): the FableWin.pdb overrides
  `IsToFaceTarget() const` (slot 73, 0x00956720) and `GetIsToFaceTargetCone() const` (slot 78, 0x00956730). Declarations only.
- pin_asserts.py: pinned 0. Landed includers: 4; none defines the new macro.

## engine/CCreatureActionBase.h (base: rebuild/include/engine/CCreatureActionBase.h)
- `FABLE_RETAIL_CCREATUREACTIONBASE_UPDATE` (retail mode; row 0x006929A0, q11935): Ego_r.pdb / FableWin.pdb `void Update();` (public),
  placed after WaitForMeshToLoad (protected) as `public: ... protected:` so the following members keep their access. Declaration only.
- pin_asserts.py: pinned 0. Landed includers: 352 files under rebuild/src/compiled + rebuild/tests; none defines the new macro, so none changes.
