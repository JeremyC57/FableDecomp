# e09 notes (egod lane)

## 0x007993F0 CTCShopKeeper::WitnessedCrime -- DIFFER(698v698), one x87 scheduling hunk; COORDINATOR DECISION
- Ported onto shared headers (include / opt-in set of landed 0x00682810 InformOfDeed + CTCShop / CTCTalk / CThingSearchTools /
  CThingBuilding) with Ego_d's locals and scopes: ptc_village_member at function scope; `if (GetTC(..)) { ptc_village; if (ptc_village)
  { tc_shop, creature, guard, ptc_talk-block, opinion-block, player, ptc_deed_log, pdeed_effects_def } }`; MAX_DISTANCE_FOR_ALARM in the
  stall block. The first compiling grade had 5 hunks (stall distance call order, string temporary, slots); the parked CDefPointer slot
  residue went with GFGetWorld2DDistance + the implicit const char* argument (copy-initialised pdeed_effects_def at its use).
- Inline controls: none needed -- the same RELOCATION_MATCH (dx-first exhibit) holds WITHOUT CTHINGSEARCHTOOLS_FIRSTTHING_BODY /
  ALLTHINGSBODY, CTHINGFILTER_HASTC_OUTOFLINE and CTHINGAICREATURE_STATTYPEFLAGS (GetPFirstThing declared only, called out of line
  at 0x00799C40); differ/ carries the smaller set.
- 2D distance = FableWin's inline ?GFGetWorld2DDistance@@YIMABVC3DVector@@0@Z (engine/GFWorldDistance.h, opt-in
  FABLE_RETAIL_GFGETWORLD2DDISTANCE) on (work building pos, creature pos): gives retail's call order (GetPItem, GetPos(creature),
  GetPos(building)) and mult order. Only residue: retail subtracts X first (0x007994A3 fld [eax] / fsub [esi], then +4); the shared body
  computes dy first.
- Evidence the shared body is wrong: fwsize (/Od) of the dx-first text `dx=pos1.X-pos2.X; dy=pos1.Y-pos2.Y; sqr_dist=dx*dx+dy*dy;
  return (float)sqrt(sqr_dist);` = 28i/73b = FableWin 0x025B03E0 exactly; today's dy-first text = 27i/70b (also measured inside the row).
  (A dx-first text without sqr_dist but with a named `dist` also sizes SAME; only the sqr_dist one was retail-graded.)
- With the dx-first body (probe/hdr2/engine/GFWorldDistance.h, NOT in hdr/) this row is RELOCATION_MATCH (probe/b.cpp), but landed
  0x009711D0 CAIStateGroup_CreatureUseCoverBase::MoveIntoCover goes DIFFER(516v516) (its retail 0x0097131E does Y first). Tried on that
  row (probes only): Ego_d's `float dis_to_camera` named result in the else arm, and a k=1..7 dead-reference sweep: no change. Its Ego_d
  record differs from the landed text (hidden_cover_pos, is_strafing, ret, nav, ptc_cmm; loop block { dis_to_dest, vector_to_dest
  { dis_to_camera } }); a full Ego_d re-port of 0x009711D0 under the dx-first body is the next idea. Note in 0x009711D0 pos2 is a
  by-value virtual return (PeekCurrentPos into a frame temporary), here a GetPos reference.
- Decide: (a) change the shared GFGetWorld2DDistance body to the FableWin-sized dx-first text and re-fit 0x009711D0, or (b) keep it and
  park this row. Not in hdr/: no second body, no non-Ego_d locals.
- Still to do before landing (pass 2): behaviour test, vland, addmatch. FITTED: implicit const char* speech argument; early returns.
- Names: Ego_d enum NSpeechGainManager::EDialogueLayer = DIALOGUE_LAYER_FOREGROUND/MIDGROUND/BACKGROUND/LAST (0..3); landed rows use
  descriptive DIALOGUE_LAYER_0/1/2/NONE spellings (rename candidates). THING_TYPE_AI_CREATURE = 1 (Ego_d enum; row keeps `GetType() != 1`
  as landed siblings do). Callee 0x00659C32 is StartSecuritySweep(bool) (NAME_FIXES).

## 0x00837570 CThingTrackNode::Save -- DIFFER(697v697), frame/schedule residue unchanged by the port
- Ported onto shared headers with two new opt-ins (CThingPhysical.h FABLE_RETAIL_CTHINGPHYSICAL_SAVE protected virtual Save 0x006A4BA0;
  CThingTrackNode.h FABLE_RETAIL_CTHINGTRACKNODE_SAVE public virtual Save), the landed twins' line shape (CThingAICreature::Save 0x008304A0,
  CThingSwitch::Save 0x0083BF90), GFUHUGEToCharString (Ego_r/FableWin ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z), no named locals
  (Ego_d has only this / script). All instructions equal; residue: (1) retail stores the conditional-temporary flag (`mov [esp+18h],0`,
  F+0xC) as the first entry instruction, ours after `mov esi,ecx` -- the matched twins show both orders in retail (CThingSwitch first,
  CThingAICreature late), so it is a scheduler tie; (2) last line: retail NULL temporary F+8 and the `"ScriptName " + x` result in the
  flag's word F+0xC (after `mov bl,[esp+1Ch]`), ours F+4 / F+8.
- Tried (no change or worse): PeekTrackName() / IsTrackStart() / IsTrackEnd() accessors; swapped ternary arms (layout worse); copy arm
  `CCharString(TrackName)` (748 bytes); implicit "NULL" (does not compile); CCharString(IsStart ? ..) (739 bytes); default operator==
  text instead of FORCEINLINE_TEXT_EQ (same). fwsize: FableWin 339i/1607b v ours 317i/1356b -- the twins differ by the same order
  (+36..38i), so FableWin's extra code is not a spelling hint.
- Next ideas: a frame-packer / scheduler replay (no tool on this machine); a pair probe with CThingPhysical::Save's body visible.

## 0x00680050 CThingObjectDef::Validate -- DIFFER(439v439), one load-order swap (unchanged)
- The draft already carried Ego_d's names; ported its reinterpret_cast world read onto a NEW shared header engine/CValidateDefinitionInit.h
  (member `World`, const CWorld&, +8: Fable Anniversary Xbox 360 Test PDB class record TPI 0x7D39 -- third-tier source -- plus Ego_d's
  `validation_class` / `world` local types). Same bytes as before.
- Residue: after HasDummyObject (0x00680191) retail reloads edi = pappearance_def.Object (0x00680196) before the cleanup-flag word
  (0x0068019A); ours the reverse. All else identical.
- Tried (no change): `!info || ...` (twin 0x00670D50 spelling), `(Property..) && info && ...` single expression, empty `if (...) { }`
  validation checks (one or both statements), declaration-order permutations of the locals (object_name first: same; strings before
  validation_class: 7 hunks; info first: 9 hunks), k = 1..7 dead named references (diagnostic: no change), default CDefPointer release
  (same), GetSubDef body hidden (447 bytes, worse: the visible same-TU body is required).
- Next ideas: the original validation macro shape (Ego_d / FableWin of a sibling Validate with debug code: the release statements
  are what is left of `VALIDATE(...)`-style checks); scheduler replay of the final block.

## 0x00837220 CThingTrackNode::UpdateFacingDirection -- DIFFER(716v716), 2 scheduling residues (draft had 3)
- Ego_d: function scope `forward` (C3DVector), block { right, up }: the draft's `next` / `physics` / `side` / `direction` locals dropped
  (direction -> forward, side -> right). Without `next` a raw NextNode is reloaded after GetPos (44 hunks); the FableWin-emitted
  accessors GetNextNode() / GetPreviousNode() (new opt-in GET_NODES) plus CThing::GetPhysicsTC() give retail's ebx-held node and
  close the PhysicsTC vtable-load hoist (0x008374DD). PeekNextNode() alone: 14 hunks.
- Left: (1) third Normalise: retail `mov edx,17Ch` before `fmul [0x0122DEDC]` (0x008373EC/F1), ours after; (2) up = right ^ forward,
  Z component: retail `fld forward.Y ; fmul right.X` (0x00837419), ours right.X first.
- Tried: cross opt-ins CROSS_NAMED_RESULT (13 hunks), CROSS_XZ_GROUP / CROSS_XYZ_GROUP / ORIGINAL_GROUPED_CROSS_PASS22 /
  CROSS_ORIGINAL_FORMAL (same 7); k = 1..7 dead named references (diagnostic, no change). fwsize: ours 99i/292b v FableWin 105i/304b
  (FableWin's helpers carry assert code, so no text hint).
- Next ideas: named-symbol count lever with USED forwarded names is not available (Ego_d has exactly 3); c2 record-id tools (r3005).

## 0x00B30610 CEngineCamera::ClipBox -- DIFFER, first pass only (x87 corner-sum CSE)
- Ego_d: unit_to_world; points (C3DVector[8]) at function scope; { c_plane { outside, plane (const CPlane&) { c_point } } } -- no
  index for a transform loop, so the eight corners are explicit statements. Loop / early-out part: the draft's shape is already exact
  (retail `test ah,5 ; jnp` next plane at 0x00B308A8).
- Port onto engine/CEngineCamera.h (FABLE_RETAIL_CENGINECAMERA_POSITION + _CLIPBOX), CMatrix3x4.h, CPlane.h with Ego_d names:
  two-argument TransformPoint(C3DVector(+-1..), points[i]) = 434 bytes; `points[i] = C3DVector(..); TransformPoint(points[i])` =
  588 bytes (probe/r5port.cpp; both: c2 folds / shares many more of the 24 sums than retail). CMatrix3x4::operator*(C3DVector) 0x00A56530 is the
  3x3 product without translation, so it is not the corner helper. fwsize: FableWin ClipBox 187i/565b; in-place port 175i/612b;
  two-arg 135i/484b.
- Retail keeps `fmul [-1.0f]` per negated term and copies the +1 terms (0x00B3061D..), i.e. a +-1 constant vector times the matrix
  rows with the 1.0 products folded; the draft's free `C3DVector * CMatrix3x4` (E41 + (x + y + z)) reproduces the 710-byte size.
- Next: a shared-header spelling of the draft's corner expression (the translation-first sum) -- needs an evidenced helper (no
  C3DVector*CMatrix3x4 operator exists in FableWin / Ego_r); then the 81-tuple region split ideas of the draft notes.

## 0x006C14D0 CTCBuyableHouse::OnInitialActivate -- DIFFER(678v734), first port only (context budget)
- Ego_d: ptc_thing_owner (CTCThingOwner*), main_signpost_def_index, main_owned_def_index, main_rent_def_index,
  dressing_signpost_def_index (long) at function scope; { pdef (CDefPointer<const CThingBaseDef>) }; { end_it, it (list const
  iterators) { pthing (CThing*) } }. So TC 0x21 is TCI_THING_OWNER (CTCThingOwner::ObjectList at +0x0C), not the draft's
  "CTCBuildingContents"; the four indices compare against pthing->GetDefGlobalIndex() (MAIN / OWNED / RENT -> MainSignPost,
  DRESSING -> DressingSignPost). Globals are the anonymous-namespace CCharStrings as landed 0x006C0F00 declares them.
- Port (src in differ/): FrameUpdate 0x006C1250's opt-in set + CTCThingOwner.h + CGameDefinitionManager + new opt-in
  CTCBUYABLEHOUSE_ONINITIALACTIVATE; 678 bytes (probe/r6port.cpp; the draft 734v734 stays best). Ego_d puts end_it AND it in the same
  block (both declared before the loop); the port declares `it` in the for header -- fix in the next pass. The pdef release differs (ours `mov / dec / store` form): the same-TU GetDef /
  GetSubDef bodies (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY) need
  CBuyableHouseDef::StatGetTypeName -- only engine/_quarantine/CBuyableHouseDef.h (generated pad model) exists; landed OnCreate /
  FrameUpdate use local stand-ins. Next: a CBuyableHouseDef header from the Ego_d class record, then the CIntelligentPointer
  temporary-assign forms (retail: base ctor 0x00A01B10, vptr store, SetPItem, GetPItem, SetPItem, dtor per sign post).

## Pass 2
- 0x007993F0: behaviour test written (test/007993f0.cpp, vt.py PASS on the differ/ source): 22 cases over every early return, the
  exact 10.0 distance threshold (6/8 offset) and 10.5, stock state 1 v 2, shop v stall, talk TC present / absent, reported / opinion /
  known-deed branches, severity 2 / 3 / 4; the deed-effects ref count is checked released. Mutants killed: `>` -> `>=` on the
  distance, dropped `== 3`, dropped same-thing test, KnowsOfDeed forced true. If the coordinator adopts the dx-first
  GFGetWorld2DDistance body, run: q.py on differ/007993f0.cpp (expect RELOCATION_MATCH as probe/b.cpp), vland, addmatch.py.
