# b25 (hand3c chunk 1, Account B, 2026-11-19): 0x00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable -- READY

- Grade: RELOCATION_MATCH, strict vland `RELOCATION_MATCH PASS` (LANDVERIFY_STRICT_HEADERS=1, overlay = hdr/ of this batch), no warnings.
  20 grades in ATTEMPTS.log (14 by the earlier author + 4 bracket probes + rebaseline + source). Flags: none.
  Leaf ?CallingChildrenIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ (private), 461 bytes.

## Identity
Ego_r.pdb / FableWin.pdb ?CallingChildrenIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ (FableWin 0x02C76EB0 gives the statement
shape); Ego_d.pdb locals ptc_perception, ret_pcreature, scan_area (no others written). Name unchanged from the catalog.

## What closed it
1. FableWin statement shape on the shared headers + the same-TU template body GetPFirstThingInRoughArea<CFilter_Bind4<..>, const
   CThingAICreature> (retail 0x009871C0, the landed row's text) defined in the row TU: frame 0x5c (0x68 without it).
2. Bracket (owner rule 2026-11-19), one `// Bracket:` line in the source: `(ptc_perception->GetSightRadius())` as the second argument of
   the first In2DRange closes the last three instructions 0x00986374..0x0098637A (retail loads the spilled sight radius between the
   position's Y load and Y store). Measured and rejected: brackets on the GetPos() argument, on GetCreature(), on the whole temporary.

## Overlay header (hdr/engine; copy of TODAY's repo header + one new opt-in; diff alongside)
- CAIStateGroup_OpinionReactionRoot.h: FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_CALLINGCHILDREN, the private declaration
  `bool CallingChildrenIsRunnable();` (nested in the _WIFEUPDATE block). Declaration only; pin_asserts.py: 0 to pin (the header's asserts
  are already FABLE_STATIC_ASSERT_AT).

## Inferred / fitted lines
- Retail-asm: 0x0098628A `fadd` + 0x0098628D `fistp dword ptr [ebp-4]` = the shared engine/GFFloatToLongCeil.h helper (asm + FABLE_PORTABLE
  twin there); the row source has no asm.
- Bracket: as above (FITTED by measurement; the owner's accepted lever).
- Same-TU template definition: written as the landed 0x009871C0 text (a COMDAT template instance: no symbol clash with the landed row).

## Test (test/00986210.cpp)
Links the REAL row object: the row, its expanded GetTC / LowerBound, GFFloatToLongCeil, the filter constructions and the same-TU template
all run as compiled, over raw native-layout fixtures whose every offset is cited to the instruction that reads it. The row's externals
(GetCreatureType, GetSightRadius, GetPos, GetPItem, PeekThingSearchTools, GetAllThingMapsInArea, ClipAndConvertWorldMapAreaToLocalArea,
IsNot / In2DRange operator(), the search statics, NThingMap::LayerInfo / MaxLayers) are recording doubles with the shared headers'
signatures. Checks: result v an independent model; receivers (perception TC, +0x6B pointer, this); scan box = truncated creature position
+- ceil(sight); IsNot holds the creature; third filter = creature position + raw sight radius; fourth = reacted-to position + 3-D distance;
filter call order; child creature calls nothing else; a stack pre-dirtied with 0x5A so an uninitialised ret_pcreature cannot read null.
40 targeted + 1500 random worlds (found 278 / not 1262).
Mutation check (tools/mut.py, VC7.1 build + link + run): 16 one-decision mutants over the row (type gate == / other constant, ret_pcreature
init, scan centre, ceil v truncation, ceil + 1, IsNot argument, creature-type constant, sight-filter centre, sight v its ceiling,
distance-filter centre, distance scaled, distance zero, `!= NULL` -> `== NULL`, final `return false` -> true, the two In2DRange filters
swapped): 16 KILLED, 0 SURVIVED (mutants_round1.log; the IsNot mutant re-spelled compilably in mutants_round2.log).

## NAME_FIXES
None.

## For the coordinator
- The row TU defines the 0x009871C0 template instance (COMDAT), as the landed 0x009871C0 row's TU does; the linker picks one, no clash.
