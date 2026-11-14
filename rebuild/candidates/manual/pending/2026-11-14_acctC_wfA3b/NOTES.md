# wfA3 notes (acctC lane)

## 0x00DADF80 NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main -- RELOCATION_MATCH, test PASS, addmatch done, vland WIN
- What made it match (from the parked 2790v2787 draft): (1) the generator loop shape `for (;; Game->NewScriptFrame()) { if (T) return
  _Fail_Val.Return(); ... }` with `TRetVoid _Fail_Val;` returns (the landed same-TU sibling CNOVI_Guard::Main's shape) -- the old
  `while (!T)` gave c2 a loop-exit frequency model (exits f 0.0179) in which the loop-exit ~seh_me pad is never expanded;
  (2) the eight speech picks as `{ long count = ParentClass.X.size(); speech = ParentClass.X[GetVillagerSpeechIndex(count)]; }`
  (a named count makes the size a global candidate coloured first into eax, so the parent takes ecx with the dead `add ecx,begin`,
  exactly retail; plain size() gives parent eax / size ecx, end()-begin() folds the begin load); (3) row-local CTimer::GetTime /
  operator=(long) FABLE_FORCEINLINE (76 landed script rows do this; operator= scores 187 here); (4) `// Flags: /d2inlT65`.
- Flags / inline: /d2inlT65 (EXTRA_RULES 2026-11-08 preference 2). The hit block's entry T-check return (0x00DAE103) has ~seh_me at
  score 65 (c1 60, f 0.625): expanded at T74, retail calls the pad. Works for T51..T65 (T50 fails, T66+ fails). T65 chosen = nearest the
  default and inside Guard::Main's working range T63..T72 (a single TU threshold in 63..65 serves both landed/matched rows).
  Natural alternatives tried first: while(!T) / for(;;) top return / while(true) break / do-while with guard (all wrong pad layout).
- Headers: no overlay change; the row's own opt-in list (FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR, CCHARSTRING,
  CCHARSTRING_CONCAT_STRING, NSCRIPT, BASE_OBJECT, CLEVELSCRIPTBASE, CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR) + TRetVoid.h.
- FITTED: `long count` named local (chosen by grading; natural), the T value. EVIDENCED: loop shape (Guard twin + retail layout),
  forced CTimer (landed precedent + inline trace), size() (retail's dead add).
- Test: state model (landed Guard test style), PASS under vt.py.

## 0x004A0D90 CWorld::LoadQuestsFromFile -- RELOCATION_MATCH, test PASS, addmatch done, vland WIN
- What made it match (from the parked 2738v2733 draft, unchanged statements): the two EXISTING shared opt-ins
  FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE + FABLE_RETAIL_CBASICSTRING_COMPARE_BODY. With CBasicString<char>::operator==(const char*)
  AND Compare visible (operator== still called out of line, 0x004115A0), VC7.1 knows the call writes no memory and keeps the token's
  PStringData in esi across it, threading the AddTestQuest test on both paths. TEXT_EQ_NOINLINE alone changes nothing (Compare
  is still an opaque call). No flags, no overlay change.
- vland WARN RELOC_CONFLICT: vector<char>(unsigned int) -> 0x00410F30 here, 0x00411910 in the landed 0x009EC890. The two retail copies
  are byte-identical (checked the first 14 instructions; separate COMDAT copies kept by two TUs): coordinator decides the binding
  (one name, two addresses -- probably both legitimately ??0?$vector@D...@QAE@I@Z in different TUs).
- Test: state model of the parse (AddQuest TRUE case-insensitive, clear flag, AddTestQuest fields, unknown command skipped).

## 0x00E2F170 NScript::CV_AmbushScamScript::SpringAmbush -- RELOCATION_MATCH, test PASS, addmatch done, vland WIN
- What made it match (from the parked 2163v2781 draft): (1) generator shape: `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` and both
  loops `for (;; Game->NewScriptFrame()) { if (T) return ...; ... }` -- the inline trace then expands ~CScriptThing at exactly retail's
  sites (first-loop top exit f 5.0, second-loop top exit f 0.39, post-first-loop path f 1.875) and calls it elsewhere; GetPos expands
  at f 0.039 (2679 bytes); (2) the spawn argument written `CCharString("")` explicitly, as the 341 landed spawn sites do (the implicit
  "" conversion was built before the new-expression; the explicit temporary after it; 2781v2781); (3) the trigger distance is from
  run_away, not the trickster (retail 0x00E2F218 `lea ecx,[esp+24h]` = run_away's slot) -- a BEHAVIOUR fix of the draft.
- No flags, no overlay change. Opt-ins as the landed same-TU sibling TricksterApproach (adds FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT)
  + engine/TRetVoid.h; IsAlive text as the sibling (`return PImp.Data && PImp.Data->IsAlive();`).
- EVIDENCED: loop / return shape (generator twins + inline trace), CCharString("") (landed idiom + retail order), run_away (retail slot).

## 0x00ACDB80 QuadricOptimiserInternals::COptimisedPrimitive::COptimisedPrimitive -- RELOCATION_MATCH, test PASS, addmatch done, vland WIN (successor)
- src/00acdb80.cpp, `// Flags: /Gr` (unchanged from the near-miss). What closed the last SIB-order instruction (0x00ACDF19):
  replacing the row-local STL tree models (CRbTree / CIndexList / CGroupMap / CContractionMultimap) by the SHARED STLport 4.6
  std::set<unsigned long> / std::map<unsigned long,float> / std::multimap<float,CCountedPointer<CVertexPairContraction> > of
  stlport/fable_map.h with the existing opt-in FABLE_RETAIL_STLP_MAP (map::operator[] and _Rb_tree::insert_unique visible; VC7.1
  keeps both out of line here exactly as retail: calls 0x00AD6420 / 0x00514C10). Measured mechanism first: the residue was a
  SYMBOL-COUNT threshold (12 extra initialised dead locals anywhere -- top, middle or after the last loop -- flipped it; 12
  uninitialised locals or one local assigned 30 times did not; symbols inside an inlined helper count too). The real STLport texts
  (allocator / _STLP_alloc_proxy / iterator / pair temporaries) supply those symbols. The dead-local ruler was diagnostic only.
- Then ported the chunk classes onto their shared headers (retail branches) with 7 new additive opt-ins (hdr/README.md): FableWin's
  always-inlined accessors C3DMeshFilePrimitiveChunk::GetMaterialIndex, C3DMeshFileVerticesChunk::PeekVertex,
  C3DMeshFileUniqueVerticesChunk::GetUniqueVertexIndex, C3DMeshFileTrianglesChunk::PeekTriangle, C3DVertexBlend2 getters (fwsize SAME
  each) and the Ego_r out-of-line getters as declarations; C3DVertex2::GetPosition / GetNormal / GetU / GetV and
  C3DTriangle2::GetVertexIndex (existing shared header). Still RELOCATION_MATCH. Also uses the FableWin inline members
  CIndexList::AddIndex(unsigned long), COptimisedVertex::AddGroup(unsigned long, float) and COptimisedPrimitive::GetVertex(unsigned long)
  (FableWin sizes SAME for AddGroup / GetVertex; AddIndex +2i = the later Dinkumware pair). Row-local: the QuadricOptimiserInternals
  classes only (as the landed same-TU CUniqueVertex 0x00AD4C40; engine/COptimisedPrimitive.h is opaque and namespace-less).
- EVIDENCED: shared STLport text, accessor names/texts (PDB + fwsize), long bone (PDB). FITTED: statement spellings of the earlier
  near-miss (named weight-loop locals, declaration order in the vertex / unique loops), the GetVertex use sites (insert + weight loops).
- vland: WIN; RELOC_NAME warns only (catalog labels CShaderPreParser::GetConstantCount / CWound::Kill / GetMember_dword0x2c on the
  callees -> NAME_FIXES.tsv; reloc_bindings' ?GetVertexCount@C3DMeshFileGroupChunk for 0x00ABA350 contradicts Ego_r GetCount).
- Test: the state model of the earlier draft (vertices, unique ids with / without UNIV, faces, weights, XMIF then CLTH), PASS.
- FIX-UP (2026-11-14, verifier rejection SHARED_HEADER_AVAILABLE COptimisedPrimitive): the row-local QuadricOptimiserInternals
  classes are gone. The row now uses engine/QuadricOptimiserTypes.h + engine/COptimisedPrimitive.h (FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE,
  _COPTIMISEDFACE, _COPTIMISEDEDGE as the landed 0x00AC8AE0) with 4 new additive opt-ins (hdr/README.md): _CUNIQUEVERTEX, _COPTIMISEDVERTEX_GROUPS,
  _COPTIMISEDFACE_CTOR, _COPTIMISEDPRIMITIVE_CTOR. Still RELOCATION_MATCH (/Gr), test PASS, addmatch redone, strict-header vland
  (LANDVERIFY_STRICT_HEADERS=1) WIN 4/4 with no HEADER_POLICY / SHARED_HEADER_AVAILABLE. 13 landed COptimisedPrimitive siblings regrade
  RELOCATION_MATCH with the overlay. EVIDENCED: all four opt-in texts (Ego_r layouts + FableWin inline members); the COptimisedVertex
  member line is an #ifdef alternative to the default line (default untouched) -- coordinator to accept that form.

## 0x00981410 CAIStateGroupBase::AIAction_InteractWith -- PARKED (graded, not worked further), DIFFER(2491v2703)
- Name proven: Ego_r + FableWin ?AIAction_InteractWith@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingAICreature@@@@VCCharString@@H_N@Z
  (`ret 18h` = 6 argument dwords); the catalog `_global::` module is wrong (NAME_FIXES.tsv).
- The draft (differ/00981410.cpp = ref draft as is) is NOT landable as written: ~150 lines of row-local models (CThing, CThingCreatureBase,
  CAIBrain, NEntityEvents::CEventVillagerInteraction, CWorld, CCreatureAction_PlayAnimation ...) where shared headers exist. First job for
  a successor: port it onto the shared headers with the opt-in set of the landed same-class sibling
  CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage 0x009810B0 (FABLE_RETAIL_CTHING / CTHINGCREATUREBASE / CTHINGAICREATURE /
  CAISTATEGROUPBASE(_MEMBERS/_PEEK/_PERFORMACTION/_GETCREATURE) / CAIBRAIN_GETCREATURE / CCREATUREACTIONBASE(_ANIMCTOR) ...).
- Measured today: 712 v 782 instructions, 121 structural hunks, 98 v 91 direct calls; the draft's own residue list (four separate 0xB4
  PlayAnimation temporaries, the non-const TC LowerBound expanded in the loop / called 0x0040F020 in the cleanup, def cached in edi
  across HasAnim, frame 0x360 v 0x364) still applies. Not attempted beyond the grade this session (time went to the four closable rows).

- SUCCESSOR (measured, differ/00981410.cpp now = this rework, 2714v2703, 144 hunks; still row-local models):
  (1) 0x006650E0 = ?IsPosInViewAngle@CThingCreatureBase@@QBE_NABVC3DVector@@M_N1@Z (reloc_bindings; the draft's IsLookingAtPos was wrong);
  (2) retail has THREE look-at blocks (IsPosInViewAngle + GetFocalPos + LookAtThing at 0x00981B3D.. and 0x00981D47.. plus the
      my_event==0 one): the draft's common tail (look-at, `ImReady = true; ReadyOnFrame = GetFrame() + 1`) belongs at the end of EACH
      ImComingToYou arm (both arms jump to the idle check 0x00981817; the PlayAnimation `continue`s jump to the loop test 0x00981827)
      -> 2599v2703, 56 hunks, calls equal except LowerBound / conversions;
  (3) FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND: the two in-loop CTCEntityEvents lookups expand and the cleanup one calls 0x0040F020
      (retail exactly) -> 2714v2703 (11 bytes long now);
  (4) remaining call difference: the four PlayAnimation-site CDefString->CCharString conversions are out-of-line ??BCDefString calls
      here, expanded (CDefStringTable::GetString 0x009D49B0 direct, 6 total) in retail; /d2inlT120 expands them (diagnostic only):
      ours spends the inline budget elsewhere. Frame 0x36C v 0x364.
  Next: find what eats the budget (row-local __forceinline helpers IS_THING_ALIVE / GetInterfaceR -> shared CThing GetTC + the
  sibling's IS_THING_ALIVE spelling), then the shared-header port (CTCEntityEvents::GetEvent(E**, const F&) const with the
  NEntityEvents::CIsEventOfType filter of the landed 0x00982610; CEventVillagerInteraction ctor landed 0x00982320 -- retail expands it).

- FIX-UP SESSION (2026-11-14): SHARED-HEADER PORT DONE -> differ/00981410.cpp = DIFFER(2698v2703), 110 structural hunks (was 2714v2703 /
  144 on row-local models). Row-local now ONLY the two classes without an engine header: CVillagerInteractionsDef : CParentDefClassBase
  (as the landed 0x00456F09 / 0x0045E620) and NEntityEvents::CEventVillagerInteraction (Ego_r layout; FableWin inline members GetPOther /
  GetPInteractionDef / IsReady(long) / SetImReady(long) / StillWaiting(long) -- all used, EVIDENCED by name sets + retail bytes).
  Everything else on shared headers: CAIStateGroupBase (+ new opt-in _INTERACTWITH), CAIBrain (SUSPENDABLE_BASE: the idle yield is
  PBrain->SuspendSuspendableProcess(), slot 2), CThing GETTC_EXPANDED + CVECTORMAP_INLINE_LOWERBOUND (in-loop GetTC expand LowerBound,
  cleanup calls 0x0040F020: retail exactly), IS_THING_ALIVE + COMPARE_NULL_INT, CThingCreatureBase ISPOSINVIEWANGLE / ANIMATION /
  ANIMQUERY, PeekDefinitionManager().GetDef(interaction, &def), CCreatureAction_PlayAnimation (+IMPLICIT_DTOR: temporaries destroyed by
  ~CCreatureActionBase), CTCEntityEvents (+ADDEVENT, new _REMOVEEVENTS: cleanup = RemoveEvents(CIsEventOfType(EVENT_VILLAGER_INTERACTION))),
  new NEntityEvents _CISEVENTOFTYPE and _CEVENTBASE_CTOR (ctor WorldFrame(0) + GetWorldFrame). Enum spellings from fable_ai_action_enums.h
  (AI_ACTION_TERMINATE_PROCESS / FINISHED / FAILED / IN_PROGRESS, ACTION_REPEAT_UNTIL_FINISHED / CALLBACK, AI_MOVEMENT_WALKING).
  With the shared texts all 6 CDefString conversions expand (GetString 0x009D49B0 direct) -- the old inline-budget residue is gone; calls
  equal retail (98 v 98, same order) except that two look-at+SetImReady tails are tail-merged here.
  Fixed: `idle` is declared INSIDE the loop (retail re-stores 1 at the loop head 0x00981579); operand order of the owner check
  `other.Get() != my_event->GetPOther()` (retail calls my_event's GetPItem first; 2691 -> 2698, 121 -> 110 hunks).
  RESIDUE (measured, retail order): (a) 0x0098176C retail loads my_event into edi BEFORE the StillWaiting store and keeps it there
  (push edi as the two false args, [edi+8], IsReady): ours keeps it in memory and reloads. Probe w3/pr9.cpp (a visible, non-inlined
  GetEvent<E,F> body = retail 0x00982610's text, plus inline CIsEventOfType::operator() and E::StatGetType) reproduces that block
  byte-for-byte, so the retail TU saw GetEvent's body (mod/ref). But then BOTH arms get identical registers and VC7.1 cross-jumps the
  arm-2 look-at into arm 1 (2561 bytes, 78 hunks). (b) Retail arm 1 reloads event into edi AFTER my_event->IsReady (my_event dead there,
  0x009819FE), arm 2 keeps event in ebx and my_event in edi (their_def = my_event->GetPInteractionDef()), so arm 1 SetImReady uses
  [edi+18h] and arm 2 reloads ecx: the tails differ and are not merged. pr9 hoists the event load above the first GetFrame (ebx) in arm 1.
  (c) frame 0x368 v 0x364: retail shares my_event's word with the GetEvent filter / frame temporaries (0x24) and `moving` with the
  materialised HasTC byte (0x1E); ours keeps them apart; a dead store of the has_anims && conditional-destruction temporary
  ($T, listing) is hoisted to the prologue and makes ebp the zero register there (retail zeroes ebp at the loop head 0x00981577).
  (d) loop head aligned here (do-while), unaligned in retail.
  NEXT: put the GetEvent body into CTCEntityEvents.h as an opt-in (needs CEventBase::GetType -- VSLOTS3 conflicts with
  NEntityEvents_EEventType.h's enum: needs a header-side fix -- and CEventVillagerInteraction::StatGetType, FableWin
  ?StatGetType@CEventVillagerInteraction@NEntityEvents@@SI?AW4EEventType@2@XZ), then work (b) on top of pr9 (arm-1 event load order).
  Ideas tried without effect: declaring event / my_event at loop top or before other_events, idle outside the loop (all byte-identical).
  Grades this session ~25 (+ ~12 probes). Tools: tools/cseq.py (call-sequence diff vs retail), tools/od.sh (built range), /FAs listing.

## Session summary (wfA3)
- Matched: 0x00DADF80, 0x004A0D90, 0x00E2F170 (all RELOCATION_MATCH also with NO overlay, behaviour tests PASS, vland WINS 3).
- Near miss: 0x00ACDB80 (one SIB order). Parked: 0x00981410 (needs a shared-header port first).
- Coordinator decisions: (1) /d2inlT65 for 0x00DADF80 (range T51..T65; Guard::Main in the same TU carries /d2inlT72 and works T63..T72:
  a common TU value 63..65 would serve both -- consider respelling Guard to T65 or accept per-row values); (2) vland RELOC_CONFLICT for
  0x004A0D90: vector<char>(unsigned int) bound to 0x00410F30 here and 0x00411910 in the landed 0x009EC890 (byte-identical duplicate
  COMDAT copies); (3) name fixes in NAME_FIXES.tsv (0x00981410 class, 0x00ACDB80 namespace, four C3DMeshFile* getters from the earlier chunk).
- 0x00ACDB80 update (end of session): differ/00acdb80.cpp now also carries the STLport 4.6 _Rb_tree ctor text (_M_empty_initialize through
  _S_color/_M_root/_M_leftmost/_M_rightmost) and the STLport set::insert pair text -- code unchanged (still 1 SIB instruction) but the
  dead-local ruler (diagnostic only, never delivered) dropped from >=22 to 9..12 units needed: the residue is "too little intermediate code"
  in the row-local STL models; continue giving them their real STLport texts and re-measure.
- SUCCESSOR session summary: 0x00ACDB80 MATCHED (RELOCATION_MATCH, test PASS, addmatch, vland WIN 4/4 with the new overlay; the three
  earlier matched rows regraded RELOCATION_MATCH inside that vland run). 0x00981410 reworked to 2714v2703 (calls equal retail except
  four inline conversions), still parked. Inline-budget probe on 0x00981410: /d2inlT90 expands 2 of the 4 conversions, T100 all 4 but
  then VC7.1 tail-merges the duplicated look-at blocks (2593 bytes) -- not a flag answer; un-forceinlining the row-local helpers changes
  nothing for the conversions. Work dirs: w2/ (0x00ACDB80 variants, ruler.sh), w3/ (0x00981410 variants, cnt.sh call-target counter).
- FIX-UP SESSION summary (2026-11-14): Task 1 0x00ACDB80 respelled on engine/QuadricOptimiserTypes.h + engine/COptimisedPrimitive.h
  (4 new opt-ins: QUADRIC_CUNIQUEVERTEX, QUADRIC_COPTIMISEDVERTEX_GROUPS, QUADRIC_COPTIMISEDFACE_CTOR, QUADRIC_COPTIMISEDPRIMITIVE_CTOR),
  still RELOCATION_MATCH /Gr, test PASS, addmatch redone; 13 landed COptimisedPrimitive siblings regrade RELOCATION_MATCH on the overlay.
  Task 2 0x00981410 ported to shared headers (4 more opt-ins: CAISTATEGROUPBASE_INTERACTWITH, CTCENTITYEVENTS_REMOVEEVENTS,
  NENTITYEVENTS_CISEVENTOFTYPE, NENTITYEVENTS_CEVENTBASE_CTOR), DIFFER(2698v2703), parked with a measured residue (above).
  Strict-header read-only vland (LANDVERIFY_STRICT_HEADERS=1) WINS 4/4, no HEADER_POLICY / SHARED_HEADER_AVAILABLE. Style rows
  0x0072CF40 / 0x00945060 / 0x00944940 (include the touched AI / event headers) regrade RELOCATION_MATCH on the overlay.

- SUCCESSOR-2 SESSION (2026-11-14, w4/): differ/00981410.cpp = DIFFER(2735v2703), 37 structural hunks (was 2698 / 110). What moved it:
  (1) arm 2 has no named their_def: `my_event->GetPInteractionDef()->Animation1/2` as a temporary in EACH has_anims branch (retail
      0x00981BF6 hoists `mov edi,[edi+14h]`, then `inc [edi+4]` / `dec [edi+4]` per branch) -> 73 hunks;
  (2) GetEvent<E,F> body visible (w3/pr9 text) + FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY (existing opt-in: the GetDef body
      0x009824E0 visible -> def kept in edi across HasAnim, ebp = CAnimFilterAnyGroup vtable, prologue / final release exact) -> 56;
  (3) Anniversary 360 PDB locals (llvm-pdbutil on anniv_dbg/...4k.pdb): interaction_def, my_pevent, their_ptc_ent, my_ptc_ent,
      their_pevent, dir, nav_route_calculated x2, their_ptc_ent (2nd = cleanup): cleanup uses its OWN uninitialised TC pointer
      (retail shares its word with the LowerBound key at 0x2C) and my_events is loop-local uninitialised;
  (4) then-block (my_event == 0) ends `if (idle) yield; continue;` and the arms follow without else: the two idle checks cross-jump
      and retail's layout (idle check + loop test right after the then-block, arms jump back) appears;
  (5) `C3DVector offset = GetCreature().GetPos() - other->GetPos(); offset.Z = 0.0f;` (retail loads GetCreature into ebx before
      other->GetPos, 0x00981874): event no longer held in ebx through arm 1 -> the two arm tails differ, no tail merge -> 37 hunks.
  Remaining: see line 2 of differ/00981410.cpp.
  UPDATE: dropping the then-block `if (idle) yield; continue;` again (single idle check, original if/else) on top of (5) gives
  DIFFER(2703v2703) EQUAL LENGTH, 36 hunks (w4/g2.cpp = differ/00981410.cpp). The y2-style then-block copy produces retail's layout but
  puts idle in bl inside the then-block (retail keeps it in memory: one textual check, shared by all paths, retail's arms carry
  ebx=result into 0x00981817), so retail's IL is g2's and only the block placement differs.
  Layout probes (tools/lay.sh: idle-call line vs then-block jl): the join follows the then-block (retail) whenever arm 2's tail has no
  conditional block (pa: arm 2 = one store; pd/pr: arm-2 look-at removed / unconditional; ps: arm-2 look-at as if/else with the
  SetImReady duplicated) and follows arm 2 otherwise (g2, pf, ph, pj, pk, pc, pq); condition spellings (!p, 0==p, bool local,
  compound), else-if chain, loop-condition spellings, size cuts elsewhere (pl, pn, po, pm) do not move it. `if (my_event != 0)
  {arms} else {then}` (q2) = 31 hunks with layout [arms][then][join][latch] (a rotation of retail's [then][join][latch][arms]).
  UPDATE 2: DIFFER(2700v2703), 782 v 782 instructions, 15 structural hunks (w4/m1.cpp = differ/00981410.cpp):
  (6) owner check `if (!(my_event->POther == other))` with the FableWin same-type ??$?8VCThingAICreature@@@@YI_NABV?$CIntelligentPointer
      @VCThingAICreature@@@@0@Z (first.GetPItem() == second.GetPItem(); existing opt-in FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED,
      same text) -> retail's `cmp ebp,eax` (0x00981859); FableWin has no two-pointer operator!=;
  (7) `if (my_event != 0) { arms } else { my_event==0 block }` (arms first in IL): the cleanup register picks (0x00981E23..) become
      retail's -- the layout is then [arms][then][idle+test] (retail: [then][idle+test][arms]);
  (8) MoveToPos target as a named local `C3DVector pos = other->GetPos() + offset * distance;` (retail calls GetPos before pushing
      the arguments, 0x00981988) -> 15 hunks; this also fixed the arm-1 PlayAnimation register rotation and the def hoist.
  FRAME FOUND (probe/vis1.cpp, diagnostic only): with VISIBLE noinline bodies of AIAction_MoveToThingProximity (0x0097E380) and
  AIAction_MoveToPos (0x0097EC00) in the TU the frame becomes 0x364 and every slot equals retail (moving + HasTC byte share 0x1E,
  my_event 0x24, def 0x28, other_events 0x30): c2 then knows `moving`'s address does not escape. Retail's TU (CAIStateGroupBase.cpp)
  defines both above this row. Delivering it needs their REAL bodies in the row source: 0x0097EC00 is landed (row-local models, 252
  lines, needs a shared-header port), 0x0097E380 (~0x4E8 bytes) is NOT landed -> needs authoring first. Stand-in bodies (vis1) are
  not deliverable. Same probe for LookAtThing / PerformActionAndWait / AddEvent / IsStopping bodies: no effect.
  Left after vis1: event in ebx v edi in the new-event path (0x0098169C..0x009816E7, 0x00981772) and the block layout.
  Still open after vis1 (12 hunks, 782 v 782): (a) event in ebx (copied from the new-expression temp edi, reloaded at the join
  0x009816E7, kept to StillWaiting) where retail keeps the new path in edi and reloads event into ebx only at 0x00981772. Tried
  without effect: event / my_event / other_events / my_events at function scope or loop top in every order (360-PDB order too),
  a named StillWaiting frame, `event->POther != &GetCreature()` (FableWin operator!=(pointer, item)), `initiator == false`;
  `new_event` local (e5/e12) collapses the arm tails (2548). (b) layout: arms-first IL gives retail's cleanup registers but lays
  the arms first; then-first IL (z1) loses the cleanup registers; then-block `if (idle) yield; continue;` gives the layout but idle
  in bl (z2: 18 hunks); duplicated SetImReady tails (t1/t2, k3, pu) cross-jump the arms (2610..2683); `if (my_event != 0 &&
  !(POther == other))` pre-check (L1/L2), arm tails with own idle checks (z4/z5): no.
  LANDING PREREQUISITES (independent of the residue): the GetEvent<E,F> template body, CIsEventOfType::operator() and
  CEventVillagerInteraction::StatGetType are written in the row source (probe text of w3/pr9) -- they belong in header opt-ins
  (CTCEntityEvents.h GETEVENT body; CIsEventOfType::operator() needs CEventBase::GetType, whose VSLOTS3 declaration conflicts with
  NEntityEvents_EEventType.h's enum); and the two same-TU MoveTo bodies above.
  Grades this session: ~150 (fg/sh + lay/fm probes), see ATTEMPTS.log.
