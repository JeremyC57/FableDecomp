# e08 notes (egod lane)

## 0x00878430 -- RELOCATION_MATCH (14 grades + 4 GetMapwhoArea probe compiles)
- Identity corrected: not GetPBestThingInRoughArea<HasTrackName,...,CThingTrackNode> (4 args) but
  CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<const CThing*>, CFilter_Bind2<CThingFilter_IsOfType, CThingFilter_OverlappingSphere> >
  (reloc_bindings.tsv binding from 0x00877720; landed caller NCameraModes::CTools::GetStandardThings calls it twice; Ego_d record in
  cm_helper_functions.obj, the CTools TU). The egod_tu.txt "retail 00959380 0095c7c0" for the TrackNode name is a catalog propagation error too.
- Baseline (twin 0x008780D0's Q19 source, CThing instance): 673v673 with the twins' parked residue (post-node-loop reload order edx/edi/esi).
- What closed it: the Ego_d body as a new header opt-in (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD), with the empty test written
  `clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0` (Ego_d C2DBoxI has both accessors; new opt-in FABLE_RETAIL_C2DBOXI_GETWIDTH +
  existing GETHEIGHT) and the box from the inline GetMapwhoArea returning `C2DBoxI(tlx,tly,brx,bry)` (FABLE_RETAIL_C2DBOXI_CTORS).
  Named-result GetMapwhoArea bodies (Ego_d names `clipped_area` inside it) grade 677v673 (30 hunks): the return form is FITTED.
  The empty test alone with the hand-expanded shift (no GetMapwhoArea) was not separately graded with GetWidth; with the edge-difference
  test every spelling (nest/continue, pcell++ placement, flags scope) kept the reload residue.
- Out-of-line GetMapwhoArea copy 0x00436500 (landed with a local model, name g_mapwhoLevels) is the SAME inline's out-of-line copy: my header body does NOT reproduce it out of
  line (68v66, store order). Coordinator: one text for both is still open (untried: named clipped_area + C2DBOXI_COPY + the GetWidth test in the row).
- Test: 5 hits of 13 things over 2 maps / 3 layers; killed mutants: search-id `<=`, width test removed, `||`->`&&`, cx<=BRX, cy<=BRY,
  c1<=MaxLayers, area for local_area, BRX rounding, filter removed, stamp removed. Removing only the GetHeight()==0 term is an equivalent
  mutant (the y loop runs zero times anyway).
- Coordinator: the parked twins 0x008780D0 (CThingBuilding) and 0x006FE7C0 (CThingGameObject) are the same template body; they should be
  regraded with FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD + C2DBOXI_CTORS/GETWIDTH/GETHEIGHT (no oracle bytes for them in this lane).

## 0x00B80CE0 -- RELOCATION_MATCH (2 grades)
- Ego_d debug length 43 for GetDeltaTransform = exactly two thiscalls (7 prologue + 15 + 15 + 6 epilogue): SetInverse(CurrentTransform),
  PreMultiply(LastTransform) -- the draft's statements. Ego_d's PreMultiply has a named CMatrix3x4 local `m` (a copy of *this): the shared
  FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY + _SETMULTIPLY_PREMULTIPLY nesting (copy, then SetMultiply(matrix, copy)) instead of the draft's direct
  FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY text: matched at the first grade. (The header names the copy `copy`; Ego_d says `m` -- no codegen effect.)
- Ported onto a NEW engine/CEngineClothSimulationMeshBase.h (Ego_d class record) over e03's CEngineSimulationBase.h / CParticleContext.h.
- Test: three transform pairs element-by-element against a double-precision reference + Current==Last gives identity; a Pre->Post
  multiply mutant fails. No conditions in the row.

## 0x00698170 -- DIFFER(693v693), parked after 17 grades (differ/00698170.cpp)
- Ported onto the shared headers: CGameCameraManager FULL branch (Camera +0x04, CamTools +0x78, CurrentMode +0x98), NCameraModes::CBase
  CYLINDER branch with the inline GetPosition() / GetLookAtPhase() accessors, Ego_d's names (origin, cam_pos, cam_look_at, line, perp,
  other_perp, one_line_in, other_line_in, fov). Name fixes vs the draft: CCamera::GetFOV (0x00A0BEB0, not GetHorizontalFOV 0x00A0BE90) and
  C3DVector::AccurateNormalise (0x00A14440, not Normalise); new opt-in NCameraModes_CTools.h FABLE_RETAIL_NCAMERAMODES_CTOOLS_ISPOSINVIEWINGFRUSTRUM.
- Residue (unchanged from the draft): at the first IsPosInViewingFrustrum call retail schedules `fld [perp.X] ; fmul [radius] ; mov esi,[origin]
  ; fld [perp.Y] ; mov ebp,[fov] ; fmul ...` (0x00698295..), ours puts `mov esi,[origin]` between the fld and the fmul (one slot), and the
  following pushes / lea shift by one.
- Tried (no change): dead-local ruler k=1..7 (diagnostic only); named-result / Initialise vector operators (worse, 725/745 bytes);
  ADD_XY_GROUP, SCALE_X_PAREN, SUBTRACT_XY_GROUP paren tuples; `perp * radius + origin`; bools declared at the top; perp declared at the
  top; `if (..) one_line_in = true` (worse 691).
- Next idea: a scheduler-replay probe of the region after the second AccurateNormalise call (what ties `mov esi` with the fmul); or the
  original reading of the two camera positions through other accessors (Ego_d's TU has CCountedPointer<NCameraModes::CBase const>::operator->,
  i.e. something reads the mode through a const counted pointer).

## 0x00B45CF0 -- RELOCATION_MATCH (14 grades)
- Ported off the draft's local model onto C3DVector.h / CRGBFloatColour.h / CEngineLightingManager.h with Ego_d's locals. Steps: in-place
  `light_vec.Normalise()` (shared GFFastOneOverSqrt) with dist2 recomputed as `(light.WorldPos - pos).GetSquaredMagnitude()` (649 -> 697 bytes);
  `dp = -normal.Dot(DiffuseVector)` (normal.X loaded first); `C3DVector col = <CRGBFloatColour sum>` through FableWin's inline operator C3DVector
  (Ego_d types col as C3DVector; new opt-in); GetCount() / PeekLight(l) (Ego_d TU; neutral); and the last residue (loop pointer base +4, colour
  temporary frame words) closed by REUSING `dot` for the product (`dot *= falloff; GFLimit(dot, 0, 1); col += light.Colour * dot;`): Ego_d has
  no other local there, the draft's extra named `scale` was the residue.
- MIN / MAX are row-local macros exactly as the landed same-TU twin 0x00B45510 (19 landed rows do the same); coordinator: say if a shared
  header should carry them.
- Test: four configurations against a double reference (fast inverse sqrt table built in the test); killed mutants: front MAX, front MIN,
  back MAX, count `<=`, Lambert clamp removed, threshold -> 0, product clamp low / high. The exact `>` v `>=` threshold boundary cannot be hit
  through the fast normalise (dot is never exactly 1e-4); the threshold itself is covered by a 4e-5 grazing light.

## 0x007D75A0 -- DIFFER(268v465), first pass 3 grades (differ/007d75a0.cpp)
- The draft was a SRC_FAIL local model. The landed same-TU sibling ShuffleCards 0x007D71E0 uses card_sorting_types.h (its own CTCTavernGameCardBase
  with protected members) plus a LOCAL `class CTCTavernGameCardPairs` -- it conflicts with engine/CTCTavernGameCardPairs.h (two class definitions).
  I used the engine/ headers (e01's 0x007DA400 include set + CCardDef.h + CARDPACK) and a new opt-in FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_GETRANDOMCARD
  (declaration of the slot-58 override, Ego_r ?GetRandomCard@CTCTavernGameCardPairs@@UAEXAAJ0JJ@Z). Coordinator: the two card-game header families
  need reconciling.
- Retail shape (decoded): pdef (CDefPointer, NULL-initialised) + GetCardDef(&pdef) 0x008F0B20 first; then the CCardDef `card` local (inline
  CResource ctor 0x009FBEC0, field inits, vftable 0x01243A44); loop: card = pdef->CardPack[GFRandom(52, GetPWorldSeed())] (LCG + GFROR13
  0x00497890, `div 34h`, element stride 0x44, member-wise copy incl. CRetailVector<CSubDefInfo>::operator= 0x00431F90); if current_card >
  total_cards/2 (ebx = total/2 via cdq/sar): accept iff Cards[current - half - 1] has the same Value (+8) and another Name (+4); else accept iff
  no dealt card shares Name or Value (loop over Cards +0x128..+0x12C); index = CardName, value = CardVal; then ~CCardDef (free of the vector
  data, vftable 0x01230BA0, ~CResource 0x009FC550) and the pdef release (dec [eax+4]).
- Remaining: our CCardDef ctor / assignment / dtor are calls or differ (268 bytes): the def-class headers need the inline bodies retail expands.
  Ego_d: quit (bool), card, pdef at function scope; original_card in the if block, card_it in the else block.

## 0x007993F0 -- NOT ATTEMPTED (context budget); analysis for the next author
- The draft is a local model with wrong parameter names/types. Ego_d: `WitnessedCrime(const COpinionDeed& deed, long deed_handle)`; function
  scope ptc_village_member; block { ptc_village; block { ptc_deed_log (CTCHeroOpinionDeedLog*), tc_shop, creature, guard (CThingAICreature*),
  player (CThingPlayerCreature*), pdeed_effects_def (CDefPointer<const COpinionDeedEffectsDef>); { const float MAX_DISTANCE_FOR_ALARM }
  { CTCTalk* tc_talk } { CTCCreatureOpinionOfHero* ptc_village_opinion } } }. The draft's residue (the CDefPointer hidden-return slot) is the
  pdeed_effects_def local declared in that big block together with the other pointers. Retail 0x00799653: when the player character has no
  interface 0x51 the deed-log pointer is the uninitialised GetTC out pointer (`mov ecx,[esp+28h]`).

## 0x00837570 -- NOT ATTEMPTED (context budget)
- Ego_d records only `this` and `script` (CCharString&): no named locals at all, so every intermediate of the draft must be an unnamed
  temporary (negative evidence). The draft is a local model (no shared includes); port onto CCharString / CThingTrackNode headers first.
