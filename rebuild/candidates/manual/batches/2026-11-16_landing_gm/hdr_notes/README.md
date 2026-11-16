# landing gm header changes

| header | opt-in | what | rows |
|---|---|---|---|
| libpk/CFader.h (new) | none (new file) | CRampedParam (0x34) + CFader (0x50) layout as the landed CFader rows 0x00C25E20..0x00C26010 model it; GetOutput 0x00C25F00 declared | 0x00C11700, 0x00C271E0 |
| libpk/CSoundEventManager.h | FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_FADER (new) | includes libpk/CFader.h; after the +0x360 pool (ends +0x7B0): opaque +0x7B0..+0x7D8, `CFader m_fader` at +0x7D8, `float GetFaderOutput() const` (0x00C11700); asserts on both offsets | 0x00C11700, 0x00C271E0 |
| engine/CThingObject.h | FABLE_RETAIL_CTHINGOBJECT_GETALPHA (new) | protected `unsigned char GetAlpha() const;` (Ego_r.pdb ?GetAlpha@CThingObject@@IBEEXZ) | 0x00702E00 |
| engine/GFAngles.h | FABLE_RETAIL_GFANGLES_UNSIGNED_DIFFERENCE (new) | FableWin.pdb ?GFGetUnsignedAngleDifference@@YIMMM@Z as FABLE_FORCEINLINE (expanded in retail; plain `inline` stays a call): fabs of the two GET_ROUNDED_ANGLE values, folded above 0.5. Body INFERRED from 0x00702E00 | 0x00702E00 |

Pinned: the existing CSoundEventManager.h assert moved from line 12 and is FABLE_STATIC_ASSERT_AT(12, ...); pincheck 0 mismatches.
CThingObject.h's assert was already pinned AT(104). The libpk headers are outside the verifier's engine/ shared-header check, so the
landed rows with local CFader / CSoundEventManager models are unaffected.
