# e11 notes (egod lane, w23)

## 0x0065CB6B CTCVillage::ReportCrime -- RELOCATION_MATCH (747v747), test PASS, vland RELOCATION_MATCH PASS (9 grades)
- What made it match: the port onto the shared headers (include / opt-in set of the landed sibling UpdateMirroredCrimeData 0x0065C5BB
  plus FABLE_RETAIL_CTCVILLAGE_CRIMES / _WITNESSEDCRIME, FABLE_RETAIL_STLP_TREE_CONSTFIND, the two e10 opt-ins) with Ego_d's locals and
  scopes verbatim; it graded 746v747 at the first compile. Two residues then:
  * GetPPlayerCharacter was called out of line: its inline body in engine/CThingPlayerCreature.h is only defined when engine/CPlayer.h is
    included FIRST (guard `defined(FABLE_ENGINE_CPLAYER_H)`). Include order fixed; no CPlayer opt-in needed (retail `lea ecx,[eax+34h] ;
    call GetPItem` at 0x0065CC70).
  * set lookup order: `find(x) == end()` reads end() before the call; a named iterator (`it`) gives retail's find-then-`cmp eax,[esi]`.
    FITTED (Ego_d has no `it`; same as landed HasCrimeBeenReported). `end() == find(x)` gives `cmp [esi],eax` (1 hunk).
- FABLE_RETAIL_STLP_TREE_MFIND_DECL is not needed (matches with and without; kept out).
- Header opt-ins: see hdr/README.md (CTCHEROOPINIONDEEDLOG_CRIMES, CTCHEROSTATS_CRIMECOUNTS new; CTCVILLAGE_WITNESSEDCRIME reused).
- Names: all callees are Ego_r / FableWin decorated names (see the source's Callees line); no descriptive names.

## 0x0095B9E0 CAIStateGroup_HoverHornetQueen::DiveBomb -- RELOCATION_MATCH (782v782), test PASS, vland RELOCATION_MATCH PASS
- Port onto the landed same-TU sibling Attack 0x0095BCF0's include set (pruned of the combat-manager / wasp-def / world-map opt-ins the
  row does not use), Ego_d's locals (pTCNav / ret at function scope, { nav } inside the approach loop) and the class's own .cpp helpers
  GetNextCirclePoint / GetPreviousCirclePoint / GotoNextCirclePoint / GotoPreviousCirclePoint defined in the row in Ego_d object order
  (retail expands them; the catalog addresses listed for GetNextCirclePoint (0x00C1B460 ...) are label propagation: 0x00C1B460 is an unrelated ring-buffer count; the gap after GotoCirclePoint up to 0x0095A7D0 holds no copy -- nothing found to grade them against).
- What moved it: (1) FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE (inline body) instead of Attack's _OUTOFLINE: retail builds the
  CIntelligentPointer<const CThing> argument inline here (0x0095BA99..0x0095BAB2); (2) lineToTarget / directionToTarget copy-initialised
  where computed, lineOfSightPos assigned (814 -> 782 bytes); (3) FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D + _ORIGINAL_SUM_PASS23 (one
  expression; the two-statement body swaps the X*X / Y*Y frame words); (4) a 45-variant sweep of the existing no-code opt-ins
  (SUBTRACT_XY_GROUP / X_GROUP / none x SCALE none / NAMED_RESULT / X_PAREN x NEWTON GROUPED / SQUARE_PAREN / PAREN / STEP_PAREN / none):
  only the default subtraction + default Newton text matched (the draft's one-load interleave at 0x0095BC13..0x0095BC45 was this).
  NOTE for the coordinator: Attack (same TU, landed) uses SUBTRACT_XY_GROUP + NEWTON_GROUPED; one retail TU has one inline text, so
  one of the two rows carries a fitted no-code variant. Both match their bytes.
- Names: Ego_r / FableWin decorated names for every member; GetNext/PreviousCirclePoint addresses in the catalog (0x00C1B460 ...) are
  label propagation, not these functions.

## 0x007C81F0 CTCHitOnCollision::OnCollision -- PARKED DIFFER(768v768), 16 grades; differ/007c81f0.cpp (no test yet)
- Ported onto shared headers: NEW engine/CTCHitOnCollision.h (Ego_d class record), CHitParameters (HIT_CTOR + CTOR + SETWEAPON + new
  SETBLOCKABLE), CThing ORIGINAL_VTABLE / VIRTUALS / CTHINGPHYSICAL_VIRTUALS (OnHit is CThingGameObject's slot 65, +0x104), CTCPhysicsBase
  GetFacingVector slot 70, the twin row CTCShotDefinitionBase::OnCollision 0x007BF510's STLport find opt-ins (RANDOM_DISPATCH + NOINLINE_BODY).
- Facts found: the target test is `GetTypeFlags() & 8` (creature bit); the combat-action check is CCreatureActionBase slot 15 = IsStrikeAction
  (header order; IsPartOfComboChain gave +0x40); OnHit is called through targetGameObject; the blocked test is
  `PassThroughIfBlocked || (!params.IsBlocked() && !params.IsHitNegated())` (a materialised `blocked` bool gives 764/774 bytes); AddTC takes
  the implicit const char* -> CCharString conversion (retail builds the temporary before the two `push 0`, 0x007C8478..0x007C8493); Ego_d's
  `blocked` is then unused (left out; Ego_d declares it).
- Residue (frame only): the find tag byte is homed at [esp+27h] directly below hit_direction [esp+28h..33h]; retail has hit_direction at
  [esp+24h..2Fh] and the tag at [esp+33h] (0x007C8210). Knock-on: every hit_direction / hit_pos access offset by 4 and the left ->
  hit_direction copy is one block (retail splits Y,X into the Newton step 0x007C8325..0x007C8335 and Z after the compare 0x007C838E).
- Tried without effect: hit_pos / hit_direction declaration order and assignment forms, `bool blocked = false` / removed, copy placement
  before the normalise, Dot operand order, find without NOINLINE_BODY (19 hunks), the FORWARDER opt-in (763 bytes).
- Next idea: replay the frame packer (c2 pk tools in rebuild/candidates/manual/pending/2026-11-09_linux_hand892/tools/) to see the tag's
  density versus hit_direction; candidates are the C3DVector RotateXY receiver form (`hit_direction = left; hit_direction.RotateXY`) and the
  Normalise / Dot opt-in texts (retail sums the normalise as Z*Z, Y*Y, X*X and the dot as Z, Y, X).

## 0x00B45720 CEngineLightingManager::CalcSWLighting(pos, normal, light_array) -- RELOCATION_MATCH (787v787), test PASS
- Ego_d has no record for this overload (its CalcSWLighting record is the global-only 0x00B45510). Body = e08's matched twin
  CalcSWLightingNoClip 0x00B45CF0 (pending in rebuild/candidates/manual/pending/2026-11-17_egod_e08/) with the result returned:
  `if (col.X > 1.0f) col.X = 1.0f;` x3 and `return CRGBFloatColour(col.X, col.Y, col.Z, 1.0f);` (a named result costs 34 bytes).
  `-DiffuseVector.Dot(normal)` vs `-normal.Dot(DiffuseVector)` give the same bytes here.
- First compile was 787v787 with ONE x87 operand-order hunk (0x00B45735, the global dot's X term). A diagnostic dead-name sweep (k=1..7,
  never delivered) matched at k=6/7 = one or two names fewer (r3004 mod-8 symbol key): dropping `falloff` (chosen), `dist2` or both all
  match. FITTED, documented in the source.
- Name: Ego_r / FableWin decorated name (overload with the local lights); parameter names from the twin's Ego_d record.

## 0x008F3310 CCreatureAction_DragonFireBlastBase::PerformAction -- NOT ATTEMPTED (context budget; analysis only)
- Ego_d locals: rhs (CRightHandedSet), pobject (CThingObject*), pweapon_def (CDefPointer<const CWeaponDef>), pshot_def
  (CDefPointer<const CThingShotDef>), mouth_pos at function scope; { target_pos, fire_direction, acc (long), line, ptc_shot
  (NThingShot::CTCShotDefinitionBase*), norm }. The old draft is a local model (DIFFER 796v796, one `mov ecx,edi` slot at 0x008F3535)
  that names none of fire_direction / line / norm -- re-derive the direction block with them (line = target - mouth?, norm = normalised).
- Retail head: GetSubDef<CWeaponDef> 0x0042B1A6 on PCreature (+8) def +0x70, GetDef<CThingShotDef> 0x0078B230 via 0x00692940 world
  (+0x10 def manager) with the weapon def's shot +0xA0, GetPos 0x004C73D0, the CCharString("DRAGON_MOUTH_DUMMY") + 0x004C08C0 dummy
  lookup into rhs / mouth_pos, CreateThingObject 0x00703210 (fastcall, `""` string), the PShot (+0x114) assignment via a CIntelligentPointer
  temporary (0x00A01B10 / vptr 0x01238C6C / SetPItem / ~ 0x00A01C10). Same-TU landed twins (all local models): DragonLandFireball
  0x008F3100, DragonRearUpFull 0x008F3630, DragonFlight 0x008F3020 -- port them together onto shared headers in one pass.

## 0x00949580 CAIStateGroup_GoToPub::WalktoSeatAndSit(bool during_pub_time) -- NOT ATTEMPTED (context budget; analysis only)
- Ego_d locals (function scope): required_facing_dir, ptc_village, pos_to_look_at (C3DVector), psit_mode
  (CCountedPointer<NCreatureMode::CModeSitBase>), ret, anim_entry (const CAnimationEntry&), ptc_village_member, tc_tavern_table;
  { nav_route_calculated } in the walk loop.
- Retail outline: ptc_village_member = GetTC(TCI 0x23) on the creature (out-param form, uninitialised on a miss: 0x009495CB reads
  [esp+24h]); ptc_village = member->GetPVillage() 0x007EE540; tc_tavern_table = GetTC(0xB8) on PTable (+0x24) item; loop
  `do { if (!PPub alive || (!ptc_village->IsTavernTime() 0x00658C0A && during_pub_time) || !PTable alive || ret == 1) terminate;
  ret = AIAction_MoveToPos(2, Pos +0x40, nav_route_calculated, 3, 1, 1, 0, 1, 0.0) 0x0097EC00 } while (ret == 4)`; ret != 2 -> terminate;
  pos_to_look_at = RHSet.Forward (+0x58) + creature GetPos 0x006A4D00; the same alive checks around AIAction_LookAtPos(2, pos, 0x11,
  0.01f) 0x0097D580 (4 loops, 1 terminates); anim_entry = GetAnimFromName(CCharString(0x01293060), 0) 0x00662A00; psit_mode =
  new CModeSitTavernStool(creature) 0x0094B530 (operator new 0x38); AIAction_PerformActionAndWait(CCreatureAction_SitDown(creature,
  anim_entry, psit_mode copy) 0x008AB9D0, false) != 2 -> Terminate, return false; else tc_tavern_table->SetSeatTaken(SeatIndex +0x3C,
  true) 0x007B8530, Sitting (+0x70) = true, return true. Terminate path returns false (0x00949892).
- Plan: GetSeat 0x009482A0 / IsRunnable 0x00949AB0 include set (shared headers, landed), + CTHINGCREATUREBASE_ANIMATION, CCreatureAction_SitDown.h,
  a new NCreatureMode CModeSitTavernStool header (Ego_d tc object lists its ctor / GetType 0x0094B5B0 / GetAnimFilter 0x0094B5C0 right after
  this row; model on NCreatureMode_CModeSitFloor.h) and a CTCTavernTable SetSeatTaken opt-in.

## Final pass (after the advisor review)
- Pinned the asserts my overlay insertions moved (pin_asserts.py: HoverHornetQueen AT(41), WaspQueenDiveAttack AT(30)/AT(31)); row 1
  now carries an `// Inline-budget:` line for FABLE_RETAIL_STLP_BITSET_TEST_DECL and the OPINION_DEED_TYPE_CRIME_* enumerators; row 2
  dropped a redundant cast (PTargetThing is CIntelligentPointer<CThingCreatureBase>) and its test calls the four circle-point helpers
  directly; row 5 has a `// Macro evidence:` line for MIN / MAX. All three regraded RELOCATION_MATCH, tests PASS, addmatch re-run,
  vland 3 WINS, 0 RELOC_IDENTITY.
- Rows 4 / 6: differ/ holds the carried-over drafts unchanged (0 grades this session).
