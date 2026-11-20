# b12 (hand3c3, Account B, 2026-11-20): CTCBerserk::FrameUpdate -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x0078D250 | CTCBerserk::FrameUpdate() | 1729 | RELOCATION_MATCH (no flags) | PASS (60,000 cases) | 267 / 268 (1 equivalent) |

Strict vland (LANDVERIFY_STRICT_HEADERS=1): `RELOCATION_MATCH PASS`; one RELOC_NAME warning (the folded list destructor
0x006FE660, see NAME_FIXES.tsv). Purity: retail_asm through the shared engine/GFFloatToLongNear.h only (inside the two
CRGBColour::Interpolate expansions; `// Retail-asm:` line; no asm in the row). Grades: 3 (first port 1668v1729; 1724v1729, 94
instructions off; the match). No local class model: the row is the typed CTCBerserk of engine/CTCBerserk.h.

## What the parked diagnosis missed
The parked draft (1614v1729, "lane gap: bare fistp ROUND, inverse-sqrt table, twice-expanded ramp block") modelled everything
locally. On the shared headers the first compile was 1668 bytes with the right structure; the bare fistp is GFFloatToLongNear
inside CRGBColour::Interpolate, the table form is C3DVector::Normalise, the rest were opt-in choices.

## Source shape
Ego_d.pdb locals and line table (lines 196..463; egod_lines/). Release half: SetCurrentAction(CCreatureAction_ReleaseStrengthBerserk
(*pcreature)); once accepted: MakeNoise("SIMPLE_NOISE"), kill StartUpEffect, AttachOpinionSource("OPINION_SOURCE_BERSERK") ->
SetReactNonstop(true), and from level 0 a knock-back: C2DBoxI(center.ToC2DCoordI(), (long)search_radius),
GetAllThingsInRoughArea<CThingGameObject>(.., GFFilter_Bind2(CThingFilter_In2DRange(center, search_radius),
CThingFilter_CanBeHitByEpicSpell()), things), one CHitParameters / OnHit per thing that is not the creature and not a friendly
creature; then the mode manager's mode 0x24. Other half: AnimTimer ramp, the Timer countdown, the radial blur centre, cleanup or
the fade-out ramp. The two ramp blocks are the same twelve statements.
- The decisive reading: `berserk_fraction` is the SCALE local. Ego_d's last two lines of each ramp block are 33 bytes (an
  expression stored to a frame local) and 21 bytes (a call with that local): `float berserk_fraction = (BerserkScale - 1.0f) *
  fraction + 1.0f; SetScale(berserk_fraction);`. With it as a copy of `fraction` the first block's SetScale argument is built in
  the outgoing slot (94 instructions off); with this text the row matches.
- `// Inferred:` (1) FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE_FORCEINLINE (new): retail expands Interpolate at both tint sites;
  plain inline is called (1683 bytes) and /d2inlT has no window (T76..T160 first expand the list destructor retail calls: 1705;
  T200 1728; T300 1751). (2) FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY: without the visible constructor body 1715 bytes / 126
  off. (3) the mode manager is fetched from GetParentThing() and not tested (Ego_d line 282 is one 491-byte debug-checked
  statement); mode 0x24 has no enum header. (4) `AnimTimer < ReleasedAnimationTimer` (`test ah,5 ; jp`).
- Not needed and left out: C3DVECTOR_NORMALISE_UNPAREN, CTHING_GETTC_ORIGINAL_CHAIN, CINTELLIGENTPOINTER_COPY / _DEREFERENCE
  (the row matches with or without each).

## Same-TU COMDATs (`// Fidelity:`; tools/gother.py from src/0078d250.cpp)
- GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind2<CThingFilter_In2DRange,
  CThingFilter_CanBeHitByEpicSpell>> vs 0x0078DB20 (596 bytes, compiled right after this row): RELOCATION_MATCH (the define set
  of the landed row of that address; the row needs the visible body: the list head stays in ebp across the call).
- the std::list destructor vs 0x006FE660 (62 bytes, folded): RELOCATION_MATCH.
- the nine-argument CHitParameters constructor: 242 bytes here, retail 0x00789238 is 178 bytes at an address that is not
  16-aligned -- the copy the linker kept comes from a size-optimised TU, so this TU's /O2 copy is not the retail one.

## Header overlay (hdr/, hdr.diff; strictly additive: 0 removed lines; copied from today's rebuild/include)
- engine/CTCBerserk.h: `FABLE_RETAIL_CTCBERSERK_FRAMEUPDATE` -- `virtual void FrameUpdate()` (slot 9) and
  `void CleanupAbility(bool)` (0x0078CD80), in the typed branch.
- engine/CCreatureAction_ReleaseStrengthBerserk.h: `FABLE_RETAIL_CCREATUREACTION_RELEASESTRENGTHBERSERK` -- the polymorphic
  class (: CCombatActionBase, constructor by reference 0x008E68E0, sizeof 0x114) as `#ifdef .. #else <generated layout> #endif`.
- engine/CTCOpinionSource.h: `FABLE_RETAIL_CTCOPINIONSOURCE_SETREACTNONSTOP` -- inline SetReactNonstop(bool) (Ego_d name).
- engine/CTCHeroMorph.h: `FABLE_RETAIL_CTCHEROMORPH_SETBERSERKSKIN` -- SetBerserkSkin(float) 0x0071BEE0 (declaration; the class
  block needs one of its three existing gates, the row uses FABLE_RETAIL_CTCHEROMORPH_TATTOOTHINGS).
- engine/CRGBColour.h: `FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE_FORCEINLINE` -- three added lines before Interpolate.
- NEW engine/CThingFilter_CanBeHitByEpicSpell.h -- the member-less filter class (operator() 0x006A8D30).
  NEWLY FLAGGED by the strict shared-header check (they define that class locally): 0x0078DB20, 0x007A5190, 0x007A63D0,
  0x007A9300, 0x00792E50, 0x007930B0 (GetAllThingsInRoughArea instances) and 0x006A8D30 (the operator itself).

## Test (test/0078d250.cpp)
Links the real row object (with its real rough-area search and CHitParameters constructor). No class models: 33 out-of-line
callees bound through `/alternatename`, OnHit through a hand-built vtable (slot 65); raw creature / def / component / world /
thing maps (two maps, two layers, cells with node chains, things filed in several cells, some already stamped). The model
predicts the complete event list -- action, noise, kill, opinion source, the search area, every filter call with the filter's
centre and radius, every enemy query, every hit (flags, hitter, weapon, position, direction), the mode calls, skin / tint /
scale, the blur centre, cleanup -- and the final component, opinion-source and search-stamp state. 60,000 seeded cases; path
counters printed (release 20146, refused 4904, found 7241 of which self 505 / friend 1755 / hit 4981, ramp up 19045, ramp
down 15022, cleanup 6228, timer held 13257, ...); the test fails if one is zero.

## Mutants (mutants/)
268 one-decision mutants (264 of the row, 4 of the new SetReactNonstop body): 267 KILLED, 1 equivalent
(`something->GetPos() - center` for `- pcreature->GetPos()`: center is an unmodified copy of that position).
The full list ran against test rev 1; rev 2 (MakeNoise also logs the release flag; thing type flags get more random bits) was
written for two survivors and those, the survivor above and the 14 scale-statement mutants (whose occurrence index had pointed
at the header comment) were rerun on rev 2 -- lines marked `[rev 2]` in the log. Rev 2 only logs more.
