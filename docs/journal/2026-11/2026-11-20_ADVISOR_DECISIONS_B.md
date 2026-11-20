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

## 0x00E97070 CV_HeroDollsScript::CBS_Teacher::Main: a new dead statement (2026-11-20)
- **Ruling: a new dead statement that Ego_d lacks may land as `// Inferred:`.** Landed 0x00DA0230 is already such a statement.
- **Cheaper forms come first:** the probe and the inline-body check; every existing dead position (Ego_d lines with /Od bytes but no retail
  bytes); placement at an inner endless-loop tail before "after a break"; and operands already live in the function, with an empty body.
- **Label:** all four dead-conditional-temporary rows (0x00DA0230, 0x00E72630, 0x00E54E90, 0x00E97070) start their label with
  `// Inferred: dead conditional temporary --`.
- **Lead:** FableWin's callee list (debug build) could name the real construct. It is not in pending/2026-11-19_fablewin_callees_for_B.tsv;
  Account A was asked.

## 0x00E9E420 CV_IntroductionToTrophiesScript::CBriarRose::Main: a redundant second read (2026-11-20)
- **Evidence:** the trophy test is `mov al,[eax+49h]` at 0x00E9F3FE, and `mov ecx,esi` is hoisted above the intro test's jne at 0x00E9F234.
  Ours gives `mov cl` with no hoist; about 40 spellings of the condition keep cl. A byte read once is a code-gen temporary and takes cl,
  because the base pointer is live at the load. A byte read twice, with a merge that survives to c2, becomes a global candidate and takes al.
  Only a second read of DoneBriarTrophySpeech at the blather arm's first T-check, whose two outcomes cross-jump, matches.
- **Ruling: it may land as `// Inferred: redundant second read --`, on the 0x00E97070 terms.** It is plain C++ that uses only a member and
  a call the function already has: no flag, pragma, device or invented type, which separates it from 0x00D01630.
- **Required:**
  - the smallest matching spelling (bare ternary, else `==true`), with every spelling in the ablation;
  - an Ego_d line-size check of that T-check line, citing the line on a surplus, else the label says "retail-only, not in Ego_d";
  - the label cites both retail sites, says the bytes fix the read's position and merge but not its spelling, and gives DIFFER(5923v5915)
    without it;
  - arm-mutants that are unreachable at run time are listed as such, not as killed, and must still fail the byte grade.

## 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn: a header forceinline against a respelled call (2026-11-20)
- **Case:** retail expands the one-argument `vector<long>::resize(n)` into its call to resize(n, const long&) 0x004428F0. `resize(n, 0)`
  matches with no device; `resize(n)` needs a new FABLE_FORCEINLINE contract in fable_containers.h. Ego_d line 240 is 13 B (one argument).
- **Ruling:** the ladder ranks devices for a given source. It does not license respelling a call against the evidence to avoid a device.
  Keep `resize(n)` if three checks hold:
  1. at /Od, resize(n) gives Ego_d's 13 B and resize(n, 0) doesn't (if they don't discriminate, land resize(n, 0));
  2. a per-row /d2inlS flag doesn't reach the match (if it does, the flag replaces the header device);
  3. retail has a real one-argument resize function somewhere (if none, forceinline is the wrong model; land resize(n, 0)).
- **Required:** line 2 names every device, including the CITEM_TRANSFERIN opt-in that makes ~CItem implicit. If the device lands, the
  README notes that resize(n, 0) is byte-equal with no device.

## 0x00B24850 CEngine::SetDefaultRenderStates: a dead named scalar and fitted assert arms (2026-11-20)
- **Case:** the last residue is the bookmark code in the 38 inline SetRenderState expansions.
  - The test order (s/a) moves only with one unused function-level named scalar, such as a discarded `GetVertexShaderVersion()` result.
  - The load order (C/B) moves only with the count of dead FABLE_RETAIL_ASSERT arms: 5..13 match.
  - About 150 natural spellings fail.
  - This contradicts the fable_assert.h note from 0x00BB3E80, which says arms without an inline call move the test order only.
- **Ruling:** both devices may land as `// Inferred:`. They are plain C++ with no flag or inline control, and fitted arm counts are already
  landed (0x00BB3E80). Because the fit is loose, two checks block the batch first:
  1. Identify Ego_d line 664 (34 B, no local): grade no-local forms of that size, e.g. the call inside an assert condition or an `if` with a
     compiled-out body. A form that matches replaces the local. Also diff the /Od locals against Ego_d's.
  2. The final strict vland runs with no header overlay.
- **Labels if the local stays:**
  - The local's label cites the discarded call 0x009BEDE0 and the DIFFER grade without it. It says the bytes prove one more named scalar
    but not its name, type or initialiser, that it is not in Ego_d, and that the name is descriptive.
  - Five arms, the bottom of the window. The label says the count is fitted to 5..13 and the conditions are unconstrained, and cites
    0x00B3B030 / 0x00B33B50 for 1100.
  - Byte-neutral mutants are listed separately.
- **Also:** the 0x00BB3E80 mechanism is unverified; a probe and a FINDINGS entry follow. The CRenderStateManager.h EARLY_RETURN brace bug
  was landed as its own fix commit.
