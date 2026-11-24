# Landing auth24 (2026-11-22): 0x00839420 NThingHelpers::CreateEffectOnHit (Fable 5.2 authoring trial)

Agent AUTH24 (Fable 5.2), continuing AUTH23's draft. Agent files: `pending/2026-11-22_auth24/` (NOTES, LOG with every pre-registration,
HANDOFF for the next agent).
- **Grade:** RELOCATION_MATCH (1308 B, stock flags), test PASS, strict headers (`logs/vland_land.log`). One RELOC_NAME warning:
  0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB` labelled `?g_NormaliseEpsilon@@3MB` (label debt already queued by c2_symbol_order).
- **Inferred:** the release dead arm of GFDebugPDynamicCast's debug assert (existing opt-ins FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT +
  FABLE_RETAIL_ASSERT_DEAD_ARM; the header marks it Inferred). The cast itself is Ego_d's (line 129 = 13 B); the arm moves c2's commutative
  operand sort to retail's X, Z, Y. Admissible as `// Inferred:` (the 0x00E72630 precedent); the row says so on line 31.
- **Headers (add-only, off by default, `logs/hdr.diff`):** CMaterialDef.h `FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS` (three PDB
  declarations); CDefinitionManager.h nested `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCREATUREDEF_SIZE_BODY` (the 0x0066B370 pick-any device for
  T = CCreatureDef; scaffolding with all three conditions: leaf identical without it, only that COMDAT changes 182 -> 162 B, owner 0x00485815
  sets the same pragmas). Base headers unchanged since the brief.
- **Equivalence:** the agent's run flagged 6 rows (`__FILE__` artefacts of a copied tree); all 6, plus owners 0x00485815 and 0x0066B370,
  recheck RELOCATION_MATCH + PASS on the landed tree (`logs/recheck.log`).
- **Pick-any** (`logs/final.pk.full.txt`): 11 owned copies EQ; one NEQ (`_M_insert_overflow<pair<ulong,C3DVector>>` v 0x0076ADA0), not
  leaf-bound and identical in landed 0x0076B120's HEAD object: debt.
- **reloc_bindings:** 3 first witnesses (the CMaterialDef PDB spellings at 0x008EE790 / 0x008EE720 / 0x008EE600); no second spelling.
- **Queued (land1122auth24):** respell the self rows 0x008EE790 / 0x008EE720 onto the const PDB forms; 0x0122DEE0 split
  (`__real@bf800000` v `?g_fMinusOne@@3MA`); the 0x0129BA3C label. Open Ego_d deviations (lines 207 / 208) and the CDefPointer `throw()`
  observation are in NOTES.
