# NF255 header overlay (2026-11-22)

Base copies: `../orig/` (repo working tree at the start of NF255, byte-equal to rebuild/include).

## engine/CCreatureAction_DragonFireBeamInto.h (q12720, L92)
- Copies rebuild/include/engine/CCreatureAction_DragonFireBeamInto.h.
- Adds ONE add-only, member-specific opt-in inside the class (after the Clone declaration):
  `FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE` -> `virtual void Initialise();` (public virtual override; Ego_r.pdb 0x00919B50 +
  FableWin.pdb 0x02AFC3E0 `?Initialise@CCreatureAction_DragonFireBeamInto@@UAEXXZ`; retail vftable 0x01293E54 slot 12 = 0x008F41D0, the
  landed row with the same Leaf). Declaration only: no layout change (sizeof 0x118 still asserted).
- Default path: token-identical apart from the static assert, which the six added lines would have moved from line 13 to 19:
  `pin_asserts.py` pinned 1 (`FABLE_STATIC_ASSERT_AT(13, ...)`, the old typedef name `fable_static_assert_13`, so the preprocessed text is
  identical).
- Includers (grep of rebuild/src, rebuild/include, rebuild/tests): 0x00951FB0, 0x009539A0, 0x00953A00. None defines the new macro; no other
  header includes this file. The opt-in changes none of them: equiv_check (see ../evidence/equiv_l92.log) and their vland recheck
  (../recheck_final.log) say so.
- Users of the opt-in: rows/009535b0.cpp, rows/00953cf0.cpp, rows/009539c0.cpp.

## engine/CCreatureAction_NymphSummonOutOf.h (q12721, L74)
- Copies rebuild/include/engine/CCreatureAction_NymphSummonOutOf.h.
- Adds ONE add-only, member-specific opt-in inside the FABLE_RETAIL_NYMPH_ATTACK_PASS27 class (after the ctor declaration):
  `FABLE_RETAIL_CCREATUREACTION_NYMPHSUMMONOUTOF_OVERRIDES` -> `virtual CCreatureActionBase* Clone() const;` + `virtual void FinishAction();`
  (public virtual overrides; Ego_r.pdb 0x00902060 / 0x009014A0 + FableWin.pdb 0x02A2D5D0 / 0x02A2D6F0; retail vftable 0x01282354 slots 11 / 4
  = 0x008CAA70 / 0x008C9E40). Declarations only: no layout change.
- Default path token-identical; every assert of this header was already `FABLE_STATIC_ASSERT_AT` (pin_asserts: pinned 0).
- Includers: 0x008C9E40 (its HEAD source, default branch) and 0x0091C6A0 (+ its test). No header includes this file (rebuild/include hit:
  INDEX.tsv only). Neither defines the new macro: equiv_check over both (../evidence/equiv_l74.log) and their recheck say so.
- Users of the opt-in: rows/008ca600.cpp, rows/008caa70.cpp, rows/008c9e40.cpp, rows/008ca9a0.cpp.
