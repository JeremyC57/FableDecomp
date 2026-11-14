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
