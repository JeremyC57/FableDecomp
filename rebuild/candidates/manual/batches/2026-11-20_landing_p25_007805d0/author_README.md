# 0x007805D0 NFire::CFirePoint::SetAsSmoldering() -- READY (p25Q, Account B, 2026-11-21)

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x007805D0 | NFire::CFirePoint::SetAsSmoldering() | 299 | RELOCATION_MATCH (no flags) | PASS (links the real row; ~4,100 scenes incl. all 144 combinations) | 21 / 21 KILLED |

Strict check: `LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=hdr vland.py payload.json oracle.tsv` -> RELOCATION_MATCH, behaviour PASS, no warnings (vland.log).

## Text
Shared headers only (the same-TU TU-mate 0x00780D80 FrameUpdate's model set: CThing, CIntelligentPointer, CDefPointer, fable_string)
plus engine/CSpecialEffectsDef.h, engine/CDefClassBase_GetSubDef.h, engine/CTCDParticleEmitter.h (FULL), engine/ThingAlive.h. Ego_d
locals by name (pspecial_fx, fx_index, peffect, tcd_particle_emitter) and statement for statement on Ego_d's lines 59..77 (the source
comment decodes each line's size; lines 67/68/72 equal the landed twin SetAsBurning's 46/47/51).
What closed the old 299v299 / 28-line draft (pending/2026-10-27_linux_hand611): GetSubDef<CSpecialEffectsDef> and GetEffectFromString
with their bodies visible in the TU (existing opt-ins FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY,
the 0x0066BC10 / 0x007941D0 idiom): c2 then knows the callees only write through the out pointer and keeps pspecial_fx.Object in a
register across GetEffectFromString and the string's destructor, reusing it for the release (retail `mov edi,[esp+10h]` 0x00780629).
FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN gives the GetPItem/SetPItem assignment (ASSIGN_DIRECT grades the same).

## Labels
- `// Inferred:` Ego_d line 71 is 358 B (SetAsBurning line 50: 361 B) against ~12 B for a plain GetTCDriver call: the debug line carries a
  debug-only check around the call. Release bytes are the plain call (0x0078068C), written as the landed 0x00836260 / 0x008C4810 / 0x00750DD0 do.
- No Fidelity copies, no brackets, no flags.

## Header changes (hdr/, hdr.diff) -- strictly add-only
- engine/NFire_CFirePoint.h: new opt-in FABLE_RETAIL_NFIRE_CFIREPOINT_SETASSMOLDERING declaring the private
  `void SetAsSmoldering();` (Ego_r AAEXXZ). The size assert pinned with FABLE_STATIC_ASSERT_AT(29, ...) (pin_asserts.py).

## Names (NAME_FIXES.tsv)
UNMATCHED_ROWS.tsv lists the row as `_gapscan::sub_007805d0`; it is NFire::CFirePoint::SetAsSmoldering (Ego_r.pdb + Ego_d.pdb; the landed
caller AddFireDamage 0x00780800 already binds `?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ`).

## Newly flagged rows
None. The landed TU-mate SetAsBurning 0x00780540 still carries local class models (pre-dates the shared headers); not touched.
