# Advisor decisions for Account B, 2026-11-20 (owner delegation)

The owner said: "any decisions or review for me let the expert advisor decide." These are the advisor's decisions under that delegation.
They are not owner rulings.

## Decided (Account B's items)
1. **`inline __declspec(noinline)` IsValid in 0x00D3A4A0 and 0x00EB3BA0: accepted; the rows stay landed.**
   - The cause is an inline function that retail called while its body was visible to the compiler. That is an inline decision; the CSE is
     only the symptom.
   - The row is marked `// Inline-budget:` (relabelled 2026-11-20).
   - Follow-up: ladder step 2 was never shown, a plain `inline` body plus a `/d2inlT` / `/d2inlS` flag. The discriminator is the TU's IsValid
     COMDAT: it should equal retail 0x00CD23B9 (9 bytes, not 11). If a flag gives the row match and a 9/9 COMDAT, it replaces the device.
   - Portability: most landed inline-budget rows write the declspec bare, without `#ifdef _MSC_VER` (24 bare uses among the 371 marked rows).
     Wrapping them is backlog, not part of this decision.
2. **b14's message-less `throw CGenericException();` (0x009BE340): accepted as labelled `// Inferred:`.** Do not invent a message string.
3. **0x009BE3C0 CDisplayManager::IsAntiAliasingModeValid: catalogue it and land it as its own row,** through
   rebuild/manifest/boundary-starts.tsv, using the text of the same-TU copy in 0x009BF7E0. Check Account A's CDisplayManager claims first.
   Queued.
4. **The `#ifndef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER` guard round EEngine2DLayer (0x0041AFA0): accepted as an
   interim.** One definition of the enum is backlog, because it needs all 1,390 includers regraded.
5. **Model-only tests (0x00589A30, 0x00A4D920, the GetRootsNumerically owners): accepted as landed.** Real-object tests are backlog.

## Recommendations for Account A (items released to A at B's wind-down; A decides)
- **CVsyncCallbackTimer header contract (A's RP1):** option A (nested CPendingQuery opt-in plus method declarations), then option B (layout
  opt-in with fable_containers.h) after the calibration probe; never option C.
- **CSoundEventManager (libpk/LHAudioEventPosition.h v libpk/CSoundSystem.h):** the PDB layout decides, and one header owns the class.
- **hand3c3 b12's CRGBCOLOUR_INTERPOLATE_FORCEINLINE:** admissible under the owner's 2026-11-08 inline-budget decision, if marked
  `// Inline-budget:` and a flag attempt is recorded.
- **Navedge 0x00A78800:** the loop-step text wins on the ladder (plain C++ before a pragma), unless FableWin's line records put the step in
  the loop header.
- **reports b6 (0x00B0A140):** A's call.

## Not decided
The duplicate-symbol and b30 wording items need their facts pulled from the session log first.

## Process requirements from these decisions (p25 series)
- **Device minimality.** Every row that uses an inline control states a per-device ablation in its README: each device removed in turn, and
  the grade without it. Owner condition: "the smallest that matches".
- **Ladder.** A plain inline body plus a per-row flag is tried before a source-level `noinline` / `forceinline` / pragma.

## 0x00E72630 CDF_ThreeSuits::Main: a dead conditional temporary (2026-11-20)
- **Ruling: admissible as `// Inferred:`, never as `// Inline-budget:`.** It is a fitted statement, not an inline split.
- **Why:** retail's entry has `xor ebp,ebp ; mov [esp+3Ch],ebp`, the home and initialisation of a conditional-temporary flag. The live form of
  the same store is in landed 0x00E71F10 / 0x00E70BC0. Ego_d's 36-byte prolog shows the later source revision had no conditional temporary;
  that does not show retail had none. Landed 0x00DA0230 is the same case.
- **First, one probe:** a conditional temporary inside an inline body that Main expands. If its flag initialisation lands at function entry,
  check Main's expanded inline bodies for the real carrier.
- **Form:** the matching form that adds no line Ego_d lacks, uses only calls and literals already live in the function, and has the fewest
  tokens.
- **Required:** the comment states that the bytes prove a conditional temporary existed and constrain neither its content nor its placement.
  Mutants of the dead line are reported separately. No new relocation or binding.
