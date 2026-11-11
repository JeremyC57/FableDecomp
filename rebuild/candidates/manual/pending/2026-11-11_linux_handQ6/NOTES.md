# agentQ6 (chunk Q6, 40 parked near misses) -- notes for the coordinator

Grade every row with `export FABLE_HDR_OVERLAY=$A/hdr` (the overlay opt-ins are below). 4 RELOCATION_MATCH (src/ + test/,
behaviour tests PASS under g++ and VC7.1 /Od via tall.py); 36 DIFFER at retail length in differ/.

## Rework on shared headers (coordinator request, 2026-11-11)
Clean matches (src/ + test/, no local class models, grade with FABLE_HDR_OVERLAY=$A/hdr):
- 0x00A66FD0 CMouse::ProcessMaintainedEventsFromEvent(const CInputEvent*): engine/CMouse.h (FABLE_RETAIL_CMOUSE, the 4-byte
  STLport list at +0x340c), engine/CMaintainedEvent.h, engine/CInputEvent.h (+_COPY, new _MOUSE_SETTERS), stlport/fable_containers.h
  std::list with the new FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE / _ERASE_NOINLINE (visible, out-of-line insert(iterator) /
  push_back() / erase(iterator), retail 0x009E4A20 / 0x009E4AA0 / 0x009E47E0). Only an anonymous enum of EInputEventType values
  remains in the row (the PDB enumerator names are not in the repo). Hand-check the 3-entry jump table pairing before landing.
- 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath: engine/CAIStateGroupBase.h retail form (+_MEMBERS,
  _AIACTIONS, _SPEECH for PeekWorldMap, _GETCREATURE + CAIBrain _GETCREATURE), CThingAICreature/CThingCreatureBase retail (+ new
  _HEIGHTZBELOW), CWorldMap retail (GetMapNumberAt slot 16), GFFloatRandom.h, NEW engine/CBezierCurve.h and
  engine/CAIStateGroup_FlyingThingBase.h (overlay), stlport/fable_deque.h FABLE_STLPORT_DEQUE_PUSH_BACK + new _AUX_NOINLINE (the
  _M_push_back_aux_v body moved out of the row). The row keeps only `extern const C3DVector HeightProbeOffset; // 0x0138C500`.

Parked in differ/ (still RELOCATION_MATCH, but not landable under the shared-headers rule):
- 0x00542780 NUISystem::CIconText::AddIcon: now on engine/NUISystem_CIconText.h (the header already has retail's Icons +0x18c, no
  layout opt-in needed), std::vector (new FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE; _M_insert_overflow visible by default),
  NUISystem::CManager::GetInstance()->GetGraphicsBank() (the callee 0x0041E5F2 is CManager::GetInstance, not CFrontEndManager as my
  first draft and some landed comments say) and CCountedPointer. Still local: the graphics-bank stack -- CResource/CResourceList
  (inline TouchResource = ListRemove + ListInsert), CGraphicFrame/CGraphicFrameData refcounting, CManagedTexture touch,
  CGraphicInfoFrame::CFrameData, the inline CGraphicInfoFrame::GetFrame, CGraphicInfo's frame size, CGraphicDataBank's
  PeekGraphicInfoFrame. The shared retail forms (FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) have no inline bodies for these and keep
  the members private; the landed 0x0041C260 GetFrame / 0x0041AC20 CSprite::GetDataFromDef carry the same local models. The
  CEmbeddedIcon (graphic, charIndex, flags) inline ctor is an opt-in with no PDB symbol (the PDB default ctor + assignments gives
  a different prologue, tried).
- 0x007DDA30 CTCCreatureFlyInCircles::QueueSwoop: not reworked (all local models). Needs new engine/CCardinalSpline.h and
  engine/CTCCreatureFlyInCircles.h, a C3DVector X-paren operator- opt-in, a decision on vector<C3DVector> traits (retail's
  overflow 0x0066E370 copies inline and calls only malloc, while clear() calls the fastcall random-access __copy 0x0074CA20; the
  shared header's single __type_traits model can't give both), and shared CThing::GetInterface / CIntelligentPointer /
  CTCPhysicsBase slot 31. Its differ copy still has a bare __forceinline.

## Overlay (hdr/, documented in hdr/README.md)
- FABLE_STLPORT_DEQUE_PUSH_BACK (stlport/fable_deque.h): needed by 0x00912840.
- FABLE_RETAIL_CM_UNDEADHERE_MAIN (engine/CV_HauntedHouseScript.h): CM_UndeadHere ParentClass +0x14, PMasterData +0x18,
  `long Counter` +0x1c, private virtual Main(). Used only by the unmatched 0x00E95770 shared-header attempt (not in differ/).

## Unmatched rows (differ/<addr>.cpp, line 2 = residue + next idea)
- 0x00972F30 differ source is the compile-fixed draft (header pad names `_pad_0x34 + 0xc`, PCutsceneLookAtMarker); it was
  SRC_FAIL as parked. It still contains a made-up `FindFirstInArea` stand-in (not a retail symbol) inside the
  GetPFirstThingInRoughArea stand-in: replace with the faithful 0x007FD110 body before landing.
- 0x0087A7C0 differ source swaps the prior fake `_M_insert_overflow` stand-in for the real STLport POD body (0x0049C370);
  same grade (one reload swap at the loop exit).
- 0x008DD7E0: a stub GetAllThingsInRoughArea body moved the box stores, so the area's escape drives the residue; the
  faithful template body (0x008E3C50) is the next step.
- Drafts in differ/ still carry bare `__forceinline` in local models (unchanged from the parked drafts); convert to
  FABLE_FORCEINLINE when they are reworked.

## Names
- LABELS.tsv: the 4 rows plus GetRandomFlightPos, GFFloatRandom, CInputEvent::GetType / GetMousePos (manifest labels for the
  last two are propagated and wrong).
- NAME_FIXES.tsv: 0x00912700 GetRandomFlightPos is protected const with const C3DVector& / C3DVector& (landed 0x00912700
  file and 0x0093D030 spell it non-const with pointers).
