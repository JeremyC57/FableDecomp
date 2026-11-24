# NF295 overlay headers

## engine/CAIStateGroupBase.h (base: orig/hdr/engine/CAIStateGroupBase.h = repo HEAD copy, 2026-11-23; still equal to the repo at hand-back)
- One new add-only opt-in, `FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_MOVETOPOS`, in the flattened (default) branch. It mirrors NF287's
  `FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_LOOKATPOS` block and has two parts:
  - forward declarations before the class (`class C3DVector; class CThing;` and MS forward enums `EAIActionReturn`, `EAIActionRepeat`,
    `EMovementType`), with a comment;
  - inside the class, just after the LOOKATPOS block, `protected: EAIActionReturn AIAction_MoveToPos(EAIActionRepeat, C3DVector, bool&,
    EMovementType, bool, bool, const CThing*, bool, float);` for 0x0097EC00. This is a declaration only.
- Spelling: `?AIAction_MoveToPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@AA_NW4EMovementType@@_N4PBVCThing@@4M@Z`.
  - It is in both PDB name sets (evidence/pdbq.txt).
  - The self row 0x0097EC00 defines it (work/obj/H_0097ec00.obj), and landed 0x0093B1D0 binds it.
  - The retail branch already declares it under MEALACTIONS.
- User: binder 0x0094A6D0 only.
- No existing line was edited. Nothing changes for a row that does not define the macro, and the layout is unchanged (a non-virtual
  member function). Every assert in the header is already pinned `FABLE_STATIC_ASSERT_AT`, so the added lines move no assert name.
- equiv_check, old repo tree against the new tree, over the header's landed includers plus every relative-include row: 1,970 rows,
  0 differ (evidence/eq_aisg.*).
- Planted control: a `typedef int NF295PlantedControl;` member at the top of the flat class, outside any opt-in. It flags 82 rows
  (evidence/eq_aisgctl.*).
- Other agents' overlays:
  - NF292's hdr/ holds CMemoryDataOutputStream.h, CTCBuyableHouse.h, CTCDoor.h and CWorldMap.h.
  - NF293's hdr/ holds fable_user_profile.h, CThreadedFileManager.h and CWorldMap.h.
  - Neither holds CAIStateGroupBase.h.
