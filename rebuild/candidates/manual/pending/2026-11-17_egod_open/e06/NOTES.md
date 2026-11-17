# e06 egod lane notes

## 0x0070E2A0 CAnimateThing::CalcCachedAnims() const -- OPEN, DIFFER(618v618) frame slots only (19 grades), differ/0070e2a0.cpp
- Ported onto shared headers (no local models): new engine/CTCSkeletalMorph.h (Ego_d class record + landed 0x00786870 / 0x00786700
  offsets), CAnimateThing.h opt-in CALCCACHEDANIMS (private const CalcCachedAnims / CalcInterpolatedMeshAnim / CalcMeshAnims
  declarations, inline GetParentThing, the four cached blend states `mutable` -- Ego_r ABEXXZ = const and the method writes them),
  C3DAnimationBlendState.h opt-in SETSKELETALMORPH, fable_containers.h opt-in STLP_LIST_CONST_FRONT (const list::front()).
- First compile: 11 hunks; the CIVCountedPointer release form (`mov edi,[eax+4] ; mov ecx,eax`, 0x0070E2F5) closed with any of the
  existing RELEASE_MEMBER / RELEASE_INNER / RELEASE_ISNULL(+RELEASE_COUNT) texts; kept ISNULL+COUNT (fwsize: 17i = FableWin 17i).
- Residue (7 hunks, all frame offsets): retail slots A=[esp+0Ch] {PeekTC key, blend temp 1, both const-morph temps},
  B=[esp+14h] {blend temp 2, IsNull temp, both GetPSkeletalMorph temps}; ours puts blend temp 2 and the IsNull temp in A
  (0x0070E327, 0x0070E35F..0x0070E381, 0x0070E3C6..0x0070E3F1).
- Measured with the c2 frame-packer probe (hand885 tools/pk/pk.py, copied to tmp/pk): node order key(4), const1 w8, const2 w8, then
  w4 ties in tnum order blend1, blend2, has, skel1, skel2; skel1 conflicts with const1 and opens slot B. Retail's partition needs
  skel1 sorted BEFORE blend2 and the IsNull temp but AFTER blend1: i.e. blend2 / has carry one IL reference less than blend1 / skel
  (or blend1 / skel one more). No natural spelling found that does that.
- Tried (no change): IsNull v Get() != NULL (Get() alone 613 bytes), early returns (9 hunks), && in one if (640 bytes: destruction flag),
  ParentThing direct v GetParentThing(), ptc declaration late, CCOUNTEDPOINTER_RELEASEDATA dtor (FableWin's 8i dtor = ReleaseData()).
  Ego_d's same-TU CalcMeshAnims(anim, delayed_anims, mesh_anim1, mesh_anim2) (debug 216, no retail copy): plain = called (423 bytes),
  FORCEINLINE = front() hoisted into ebp (612 bytes) -- retail re-reads [esi+60h] per call, so the row calls CalcInterpolatedMeshAnim directly.
- FIRST pass-2 hypothesis (more likely than a weight difference: retail's instructions equal ours, so its weights should too): blend
  temp 2 CONFLICTS with blend temp 1 / slot A in retail (overlapping lifetimes or interference), so it opens slot B and the IsNull / skel
  temps join it. Probe the interference sets with `pk.py --iset` (also on the forced CalcMeshAnims variant probe/r1/h.cpp).
- Next idea: find which source construct gives blend temp 1 / the skel temps an extra IL reference (packer probe gives weights in
  one compile: `python3 tmp/pk/pk.py <src> out.json --func CalcCachedAnims`); the Anniversary 360 line table (third source) for statement order.

## 0x008BF280 CCreatureAction_GenericFireProjectileWeapon::PerformAction -- RELOCATION_MATCH (1 grade + 4 compile fixes), test PASS, vland PASS
- What matched: the port onto the shared headers with the landed same-TU FinishAction 0x008BF500's include / opt-in set and Ego_d's
  locals and scopes verbatim (an `if (IS_THING_ALIVE(PCreature)) { ... }` block holding ptc_weapon / pcarried_thing / ptc_carry; named
  float shot_accuracy_percentage and shot_damage_multiplier = 1.0f; PeekTC(&ptc_carry) as a statement, its bool unused). The old draft's
  register / slot residue is gone at the first compile.
- Headers: hdr/ opt-in FABLE_RETAIL_CTCWEAPON_GETCREATEDSHOTDEFGLOBALINDEX, new hdr/engine/NEntityEvents_CEventFiredShot.h; existing
  opt-ins listed in the row header. NThingShot::CreateShot is declared in the row (no shared header declares it); its default arguments
  are FITTED (same call either way). No flags.

## 0x00805600 CTCTargetingPlayer::FrameUpdate -- RELOCATION_MATCH (4 grades), test PASS, vland RELOCATION_MATCH PASS (3 RELOC_NAME label warnings)
- What matched: the shared-header port (landed UpdateProjectileTargeting 0x00803D60's opt-in set; e05's "header wrong past +0x68" is stale:
  the UPDATE_LIST layout asserts +0x84 .. +0xF4), Ego_d's locals, and three source facts: (1) PostProcessTargetedThing (same TU, Ego_d right
  after FrameUpdate) defined in the row and expanded by VC7.1 like retail (0x00805851..0x00805877; also RELOCATION_MATCH at its own address
  0x00803B20); (2) CTCTargeted::IsZTargetable is an out-of-line Ego_r member (called at 0x008056F0; new opt-in _ISZTARGETABLE_DECL);
  (3) test_slong = GetFrame() + the 64-bit UniqueID (the high word's dead spill at 0x00805796).
- Last residue was the PostProcess compare operand order (`cmp eax,edi`): `GetPTargetedThing() != PLastThreatenedThing.Get()` (FITTED).
- Headers: hdr/ opt-ins CTCTARGETINGPLAYER_FRAMEUPDATE, CTCTARGETED_ISZTARGETABLE_DECL, CMAINGAMECOMPONENT_GETPPLAYERINTERFACE,
  CPLAYERGUI_NPCNAMEDISPLAY. No flags (the sibling's /d2inlT68 is not needed).
- vland RELOC_NAME warnings (label debt; our spellings are PDB-proven): 0x00416359 labelled GetGamePlayerInterface (Ego_r / FableWin
  GetPPlayerInterface), 0x00803540 labelled CViewBase_UpdateSeaLevelParticles (shared header + Ego_r UpdateTargetedPositionRandomWalk),
  0x004379D0 labelled Forward__Cons_val... (Ego_r / FableWin SetNPCNameDisplay).
- Descriptive name used: IsCreatureButNotHero (0x0085EE10, the landed self row's name; no PDB name found). The landed 0x00803B20 row spells
  that callee CThing::IsControlledCreature and returns bool (NAME_FIXES).

## 0x00A1BA10 CNavigatorAStar::GetRandomNavigablePos(pos, distance, direction, spread) const -- OPEN, DIFFER(642v642) 4 hunks x87 order (12 grades), differ/00a1ba10.cpp
- Now on the shared headers (no local models): CNavigatorBase (PInterface +0x14, NavigationManager +0x1C), CNavigatorManager::GetNodeAt
  (GETNODEAT opt-in), CANavQuadTreeNode (Blocked, operator== 0x00A763F0, slot 4 = IsNeighbourOf), CNavigatorExternalInterface slot 16 =
  GetNavigationLevelAt and slot 4 = FindPosZAt (the draft's "GetLayerAt" / "GetGroundHeight"), C2DVector Normalise / Rotate; new hdr/ opt-in
  FABLE_RETAIL_CNAVIGATORASTAR_RANDOMPOS (both GetRandomNavigablePos overrides + private GetPosInDirection 0x00A1AD40). Identity fixes: the
  blocked-node call (+0x1C) is the bool overload GetRandomNavigablePos(pos, distance, true), not "GetNearestNavigablePosTo"; 0x00A1AD40 is
  GetPosInDirection (Ego_r), not the draft's GetNavigablePosInDirection.
- The global 0x013D27D0 is Ego_d's `anonymous namespace'::ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM (navigator TU file static with a dynamic
  initializer; only Ego_d names it -- one source; its value / initializer was not located). Declared extern in the differ; defining it
  TU-local (any value) grades the same.
- Frame: the draft's block scoping or an `else { }` around the fallback part is needed (without it the frame is 0x44 v 0x2C); kept the else.
- Residue: the C3DVector sum's operand order (r3010-type record-id order). Tried: operand swap (normalised by c2), C3DVECTOR_ADD_NAMED_RESULT /
  INITIALISE_ADD (FableWin operator+ is 34i: those texts give +10 bytes elsewhere), C2DVECTOR_NAMED_RESULT_MUL + SQUAREDMAGNITUDE_YX (same-TU
  CullWayPoints uses YX; same 4 hunks), TU-local definition of the global, dead-local ruler k=1,2,4 (no movement). Ego_d has no record for
  this overload (its one record is the bool overload), so no locals evidence.
- Next: the c2 record-id model (r3010 tools) or the Anniversary 360 line table for the statement shape; GetPosInDirection's real body
  (Ego_d 310 debug bytes, same TU, defined right after) made visible in the row.

## 0x00681E60 CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool) -- NOT GRADED in e06 (context budget), plan only
- e05's local-model source (ref/00681e60/e05_localmodel_match.cpp) is the structure oracle. Port needs: CTCCreatureOpinionOfHero.h
  (SERIALISE branch types AttitudeMask / TransientOffsets, existing SETPERSONALITYOVERRIDE_INDEX opt-in), COpinionPersonalityDef.h opt-in
  typing AttitudeEnabled as std::map<EOpinionAttitudeType,bool>, CDefString != CDefString, stlport map / vector / list / fill.
- Its `FillTrue` helper is invented (not allowed): the else branch is `std::fill(AttitudeMask.begin(), AttitudeMask.end(), true)` that
  VC7.1 expands (rep stos) where the then branch's variable-value fill calls std::fill<bool*,bool> 0x00685430 -- check the c2 inline trace
  for that per-site decision first (a constant argument lowers the expanded cost).

## 0x00B47390 CEngineLightingManager::SetupShaderSpotLight(const CMatrix3x4&) -- RELOCATION_MATCH (2 grades), test PASS, vland PASS (1 RELOC_NAME label warning)
- Identity: the CMatrix3x4 overload (Ego_r / FableWin). Ego_d's single record (egod_locals.txt) belongs to the no-argument twin 0x00B47310
  (landed, local model); its names (scene_data const&, light_attenuation_squared / _constant) were borrowed (FITTED for this row).
- What matched: the shared headers (CEngineLightingManager MEMBERS: CurrentShadowedSpotLightParams +0x4698; CEngineShadowRenderer
  PeekSceneData returning a const value bound to a const reference; CMatrix3x4 SetMultiply + PostMultiply on a named copy of the
  transform -- retail's two `rep movsd`); GFSqr for the squared ratio (existing C3DVECTOR_NORMALISE_SQUARE_HELPER). The draft's
  "operator*= term order" residue was its hand-written product body.
- Header: hdr/ opt-in FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETUPSHADERSPOTLIGHT. No flags.
- vland RELOC_NAME: 0x01436E60 labelled g_pEngineShadowRenderer (bound ?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A).
- NAME_FIXES: 0x009899B0 is ?SetShadowSpotLightFallloffParameters@CShaderRenderManager@@QAEXABVCMatrix3x4@@MMABVCRGBFloatColour@@@Z (both
  PDBs; references) -- the landed twin 0x00B47310 binds a pointer-form "SetShadowSpotLightFalloffParameters".

## Untouched rows (context budget): 0x00B2BD80, 0x0066A9E0, 0x00911300, 0x00AB3870 -- pass-1 starting points
- 0x00911300 / 0x0066A9E0 / 0x00B2BD80 drafts are local models; port first (the lane's lesson: three of four rows here matched at the
  first compiling grade of a shared-header port with Ego_d's locals). 0x00AB3870 has no CVertexShaderInput header (new header from the
  Ego_d class record) and Ego_d gives its full loop / block structure (egod_locals.txt).

## Summary (e06)
- Matched (RELOCATION_MATCH, test PASS under vt.py, vland PASS): 0x008BF280, 0x00805600, 0x00B47390 (payload.json / oracle.tsv).
- Open with differ/: 0x0070E2A0 (frame-slot packer tie, 7 hunks), 0x00A1BA10 (x87 operand order of one sum, 4 hunks).
- Not graded: 0x00681E60 (plan), 0x00B2BD80, 0x0066A9E0, 0x00911300, 0x00AB3870.
