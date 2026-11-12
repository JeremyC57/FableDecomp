# acctB hand-back, row 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction: matched

| address | result | notes |
|---|---|---|
| 0x0087F3A0 `?PerformAction@CCombatAction_KickThingOnGround@@UAEXXZ` | RELOCATION_MATCH 2422v2422, test PASS (g++ and VC7.1) | no flags, no asm sites. 66 grades over three sessions (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no RELOC_CONFLICT / RELOC_IDENTITY / NOT_GENUINE; one `WARN RELOC_NAME` (0x005DCC00 is
`CAnimationSet::GetRandomAnim`, stale label `AnimDef_FilterAnimationList`).

## What closed it

Visible `GetSubDef` / `GetDef` bodies (existing opt-ins), the shot class name as a named block-scoped `CCharString`,
`ToC2DVector().GetSquaredMagnitude()` for the flat distance, a named `effect_pos` passed to
`CTCDParticleEmitter::Create`, and two `C3DVector` helper texts for the x87 order in the projection block:

- `Dot`: `float dot=(X*other.X+Y*other.Y);dot+=Z*other.Z;return dot;` (opt-in `FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z`)
- `GetMagnitude`: `float squared=(X*X+Y*Y)+Z*Z;return (float)sqrt(squared);` (opt-in `FABLE_RETAIL_C3DVECTOR_MAGNITUDE_XY_THEN_Z`)

Both are needed (each alone grades DIFFER). The match holds with 1, 2 and 4 extra dead locals: not on an edge.

## Review items

- **GetMagnitude text: PROVEN** by size (26 instructions / 66 bytes at `/Od`, exactly FableWin's debug copy; the repo
  default is 13 / 31). **Dot text: FITTED** (60 bytes against FableWin's 59; the only exactly-59-byte text of 26
  tried grades DIFFER).
- **FITTED row spellings:** `shot_class_name`, `effect_pos`, `movement` copied from the member and reassigned, the
  chained `base_speed_scale = speed_scale = ...`, the `is_kickable` flag.
- **`NAME_FIXES.tsv`** (4): 0x006624F0 `CThingCreatureBase::SetAttackedByCreature` (landed on CThing); 0x007BF790
  the `const C3DVector&` overload of `NThingShot::CTCShotDefinitionBase::Init`; 0x0087E760
  `NEntityEvents::CEventKicked::GetType` (landed placeholder `gapscan_sub0087e760`); 0x0087FEA0 the CEventKicked
  scalar deleting destructor. The event is CEventKicked (RTTI vtable 0x01277B5C), not CEventKillLocked as chunk
  2051's outline said.
- **Headers:** 13 engine headers, opt-ins only; new `engine/CTCRotate.h` and `engine/NEntityEvents_CEventKicked.h`.
  None of the new macros occurs in landed code. Header debt noted, not touched: `C3DVector` `+` / `-` / `*(float)`
  are 6 instructions short of FableWin.
- `OPINION_POST_DEED_TYPE_ALL = 7` is a landed spelling, not PDB-proven. Locals and the two opt-in names are descriptive.
