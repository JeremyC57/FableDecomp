# hand4d batch 3 (2026-11-19)
- 0x00727220 CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThing*) const -- RELOCATION_MATCH + test PASS (strict vland). No flags.
  Overlay: engine/NCreatureMode.h FABLE_RETAIL_NCREATUREMODE_PUSHABLEBYSELFAVOIDANCE (new, declaration only; hdr/README.md).
  Names: NAME_FIXES.tsv (0x00727220 spelling: const + CThing*; 0x0137574A NGlobalConsole::AllowCreatureActionRepulsion; 0x01381198 the
  GetToleranceBand local static tolerance_band). The landed callers 0x00727500 / 0x00726BE0 declare operator()(CThingCreatureBase*) non-const.
