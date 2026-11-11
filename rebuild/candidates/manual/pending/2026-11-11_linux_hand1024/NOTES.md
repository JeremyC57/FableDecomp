# Chunk 1024: hand-back notes

**Grading:** every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (g.sh / gall.sh / q.py / gsplit.py). The overlay opt-ins are
listed in `hdr/README.md`. 0x007CD390 is graded at its SPLITS.tsv length (796) with `tools/gsplit.py`. `gall.sh` / `q.py` compare
against the catalog's 832, which also covers two unreferenced stubs after the int3 padding (see SPLITS.tsv). `tools/d.py` is a
diagnostic-only compile with a normalised side-by-side; it is not a grade and is not logged in ATTEMPTS.log.

## Matched (5): RELOCATION_MATCH, one official grade each, tests PASS
- **0x00966DA0 `CAIStateGroup_TavernServer::GetServingPoint()`**
  - Identity: Ego_r donor order IsCarryingJug / GetServingPoint / FreeServingPoint.
  - Body: list the pub's live tavern tables, then pick tables at random with GFRandom until one gives a serving point.
  - Lever: the random index is a signed `long`.
- **0x00967D40 `CAIStateGroup_TavernServer::Update()`**
  - Identity: vtable slot 12 at 0x0128CD44.
  - Lever: same-TU bodies of WanderWithinTavern, FreeServingPoint, WalkToBar and WalkToServingPoint.
  - INLINE_LOWERBOUND expands LowerBound only inside the loops.
- **0x006B3B80 `CEnvironment::LightingFlash()`**
  - The catalog's "CEnvironment_UpdateThunder" is a placeholder.
  - Identity: FableWin order puts it directly before Update; it is Update's third call (0x006B42B2).
  - Levers: the `!(freq > 0 || start >= 0)` entry test, a named CCharString for the thunder sound, the `if (0.0 > chance)` clamp, and FABLE_RETAIL_CARRAY.
- **0x006E2ED0 `CTCTalk::GetCurrentLipSyncFrameData(CLipSyncFrameData&)`**
  - The catalog's "CViewBase_ProcessLipSync" is a placeholder.
  - Identity: private, `ret 4`; the caller is 0x006E3C65.
  - Lever: an early `return false` after `empty()` keeps the vector-dtor block inline.
- **0x007CD390 `CTCMultiStrike::CTCMultiStrike(CThing&)`**
  - Identity: vtable 0x0126EE0C and the def literal; Ego_r 0x007CBA10, FableWin 0x0272FCE0.
  - Lever 1: GetEffectFromString's same-TU noinline body (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY). It lets the effect index share [esp+14h] with the five interface keys and the CCharString temporaries, so the frame is 8 (it was 0xC without the body).
  - Lever 2: an explicit `case 0` beside an identical `default`, which VC7.1 tail-merges into one block; this gives the table from 0.
  - Weapon, carrying and abilities lookups are the const PeekTC (0x004365B0); the emitter and sound-player lookups are the non-const GetTC.

## Parked (3): structural drafts in differ/, not graded
These are 825–876-byte bodies whose types are missing from the shared headers. Remaining budget went to the five matches.
The drafts give the decoded logic, constants and callees; the names marked as placeholders in each draft are descriptive and unproven.

- **0x0080CD40 `CTCDExplosion::UpdateHitThings(const C3DVector&, const C3DVector&)`** (Ego_r 0x00808AF0)
  - What it does:
    - Builds GetLineThingSearchArea 0x008F58C0 (box over the line, DamageRadius).
    - Copies HitThings and ImmuneThings into an ignore `vector<CThing*>` and SpecifiedThingsToHit into another (0x0080EC50, three calls).
    - Runs two `std::list<CThing*>` searches through GetAllThingsInRoughArea instantiations 0x0080E670 (damage radius) and 0x0080E8E0 (physics radius).
    - Specified-things test: std::find 0x0080D970 / 0x0080D9E0.
    - First list: GiveDamageToThing 0x0080CA20.
    - Second list, with interface 0xDE: ApplyForFrames 0x00750BA0 on `(dist / DamageRadius * 0.75 + 0.25) * ConstantFPS`, where dist comes from GetPerpendicularDistanceFromFiniteLine 0x008F5BA0 against StartPos / EndPos. Interface 0xDE is looked up with an inlined binary-search LowerBound (no call).
    - Second list, else with interface 0x4E: GiveDamageToThing if bit 2 of +0x6C is set, else Kill(true).
  - Needs:
    - CTCDExplosion.h members past +0x3C (HitThings +0x54, ImmuneThings +0x60, SpecifiedThingsToHit +0x6C; all `vector<CIntelligentPointer<CThing> >`).
    - The two filter types (Ego_r ??$GetAllThingsInRoughArea instantiations of CTCDExplosion's TU).
    - The owner of 0x0080EC50.
- **0x006CBAC0 `CTCCreatureNavigation::GetNextNavPosOnRoute(const CNavigationInput&, bool x5)`** (Ego_r 0x00701580; `ret 1Ch`, hidden CCreatureNavOutput)
  - Movement-type switch, 11-entry table 0x006CBDFC:
    - 0 → input speed (+0x14)
    - 1, 2 → slot 79 × 0.5
    - 3 → slot 79
    - 5 → slot 81
    - 6 → slot 81 ± a random 40%
    - 7–10 → slots 83–86
    - 4 and out of range → 0
  - Then the navigator slots 3, 4 or 5 (+0x0C / +0x10 / +0x14), chosen on the proximity sign and CanMoveThroughSolidObjects, with the 3D distance to the target.
  - Needs: CThingCreatureBase virtual slots 79–86 in engine/CThing.h or CThingCreatureBase.h (the old note in ROWS.tsv), the CNavigatorBase GetNextNavPos slots, and the CNavigationInput / CCreatureNavOutput layouts.
- **0x006CBE80 `CTCCreatureNavigation::GetNearestFleePositionBehindCover(const C3DVector&, C3DVector&) const`** (Ego_r 0x00701A60)
  - PeekWorldMap slot 15 by C2DVectorI of the creature's physics position (ftol Y then X).
  - GetPBestThing 0x006CD3B0 with a cover filter (radius 4.0f, positions copied into the functor; compare functor 0x006CCF50).
  - Direction from threat to cover:
    - When the 2D length is below 1e-4: a random heading through GFFloatRandom(1.0) × 2π.
    - Otherwise: the inline table fast inverse sqrt 0x013CE558 with one Newton step.
  - Scaled by the sum of both physics radii (+0x60 → +0x34) + 0.5, added to the cover position.
  - Clamped to the region bounds +0x1D0C / +0x1D24 and +0x1D10 / +0x1D28 (strict < and <= tests).
  - Z from 0x006A4DD0 (its catalog label is wrong; name unproven).

## Header debt
- New headers: engine/CTCTavernTable.h, CTCTavernJug.h, CLipSyncSampleData.h, CActiveSoundInfo.h, CSpecialAbilitiesMultiStrikeDef.h and CTCDParticleEmitter.h.
- The rest are opt-ins in existing headers; see hdr/README.md.
- CTCTavernTable ServingMode enumerators (SERVING_MODE_0..2) and the CActiveSoundInfo member names are descriptive (no PDB member names were found).
- The CThingSoundEmitter / CTCSoundPlayer `friend class CTCTalk` lines model access that is unproven.

## Names
- LABELS.tsv lists the eight row identities and the callees proven here.
- NAME_FIXES.tsv lists the corrections, with evidence, to six landed spellings (0x007B9090, 0x00A42830, 0x006E46F0, 0x006E4DD0, 0x0065D55F, 0x0049DBE0).
- It also has one catalog-label debunk (0x006A4DD0, owner unproven).
