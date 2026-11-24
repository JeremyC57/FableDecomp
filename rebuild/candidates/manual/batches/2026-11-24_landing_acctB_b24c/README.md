# Landing acctB_b24c (Account A, 2026-11-24): 0x00975BB0 CAIStateGroup_JackOfBladesStage1::Update, staged by Account B

Owner ruling (2026-11-24): B verifies and stages; A lands. B's README section is in `pending/2026-11-24_acctB_b24/README.md`, with
B24A7's notes and ESCALATE.md under `notes/00975bb0/`. A re-graded the row on its own tree: RELOCATION_MATCH PASS before landing
(`logs/verify.log`) and again on the landed tree (`logs/recheck.log`). Pick-any: all 13 copies are EQ to retail (`logs/pickany.log`).

## Decisions (advisor decision, owner delegated)
- **The nine dead release ASSERTs land with no owner flag.** The owner decision of 2026-11-15 on device d (acctC WF_RULES.md) accepts dead
  asserts at any count, provided the row carries an `// Inferred:` note with the count, the residue it closes and the evidence. EXTRA_RULES
  2026-11-19 keeps fitted tie-breakers as `// Inferred:` and reports them here.
- **The debug build settles the count.** FableWin.exe's copy of this function (0x02BE6F30, 8620 bytes) holds exactly nine
  `CDebugManager::DoErrorMessage` call sites, so the count of nine is evidenced. B's note had estimated "ten to seventeen" from size.
  The expression strings recover the conditions, which B's note had called unrecoverable:
  `GetCreature().PeekBaseDef().GetSubDef(&p_jack_def)`, `GetTC(&ptc_boss)`, `GetTC(&ptc_combat)`, `GetTC(&p_TC_hit_locations)`,
  `p_hit_location_final` ("final hit location jack not found"), `GetTC(&ptc_targeted)`, `PeekTC(&ptc_perceive_thing)`,
  `ptc_perceive_thing->GetPerceivedThing() == PTargetThing.Get()` and `p_obstruction_thing->GetTCDriver(&ptcd_obstruction)`.
  Seven are "Failed success assert on" forms.
- **The row's placements are partly fitted.** Eight of the row's placements follow those results. The ninth, after `ptc_solid_to_thing`,
  stands where the debug build asserts the perceived-thing compare. The conditions are spelled on the pointer results, not as the
  recovered success forms. The coordinator rewrote the row's `// Inferred:` comment to state this; it was a comment-only change and the
  row re-graded identically. The retext to the recovered forms is queued to Account B, and that retext must stay inside the graded window.
- **Window dependency (B's ESCALATE section 3).** The matching window of 9 to 14 asserts rests on helper texts that still differ from
  their FableWin sizes: GetMagnitude, the C3DVector operators, GFSqr and Normalise. A retext of any of them must recheck this row.
- **Headers.** Two add-only opt-ins are applied to the current headers. `hdrcheck2` prints default=same and every existing macro the
  same; only the new macros differ.
  - `C3DVector.h`: `FABLE_RETAIL_C3DVECTOR_ISNONZERO_YZX_BRACKET`, a bracket under the 2026-11-19 rule.
  - `CThing.h`: `FABLE_RETAIL_CTHING_ISDEAD_DIRECT` (FableWin size SAME) and
    `FABLE_RETAIL_CTHING_GETTCDRIVER_PHYSOBSTRUCTION_SIZE_OWNER`. The second is size-pragma scaffolding under the 2026-11-20 rule; all three
    of that rule's conditions were checked:
    - the row grades the same without it, and only that COMDAT changes;
    - the owner 0x007890EA sets `optimize("s", on)` itself, and the opt-in cites it;
    - the block ends with `optimize("", on)`, and the row has no row-head optimize pragma.
- `bindings_multi_check.py` reports one multi-target line: the existing `?NullCoord@CScriptThing` line. The row adds 5 bindings.
- **Name fixes.** B's two label spellings are queued in NAME_FIXES_QUEUE as `acctB_b24c`: 0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB`, and the
  0x0091F990 template converting constructor.
