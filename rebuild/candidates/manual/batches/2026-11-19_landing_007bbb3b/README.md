# Landing 007bbb3b (2026-11-19, hand2b): NThingShot::CTCShotDefinitionProjectile::InitMovement (268 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS, no warnings (12 cases vs a double-precision reference, 7/7 mutants killed).
- Shape: FableWin.pdb (tc_shot_definition.cpp lines 196..215) and Ego_r.pdb locals ptc_phys, fire_at_pos, line_to_target, up, `float speed`,
  applied_acceleration; statement order and the member calls CrossWithUp().Cross().
- Frame 58h: CRIGHTHANDEDSET_CTOR_INLINE (as same-TU sibling 0x007BBE34).
- Tail schedule at 0x007BBC21: no-code PAREN tuples -- SCALE_X_PAREN (sibling 0x007BC6FA's operator* text) and the new
  FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_XY_GROUP (member Cross grouped like the operator^ CROSS_XY_GROUP text 0x007BBE34 landed with).
  Disclosure: 3 tuples are needed ahead of the Z product and every 2-tuple member-Cross grouping matches; XY chosen for its same-TU precedent.
  A row-side `((GetInitialVelocity()))` also matched and was rejected as an unsourced steering spelling.
- Other new opt-ins (declarations): FABLE_RETAIL_CTCSHOTDEFINITIONBASE_GETINITIALVELOCITY (protected const, 0x007BEFB0),
  FABLE_RETAIL_CTCSHOTDEFINITIONPROJECTILE_INITMOVEMENT (virtual InitMovement).
