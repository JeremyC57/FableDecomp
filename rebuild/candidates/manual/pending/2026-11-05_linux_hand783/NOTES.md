# Chunk 783 notes

Result: 1 matched (0x004321F0 RELOCATION_MATCH), 3 parked with a named residue (0x009FA780 2028v2028 4 real lines,
0x00DDB720 2150v2158, 0x00D07490 2089v2182), 2 not attempted (0x00A15890, 0x007EBEA0: identity proven and analysed
below, no source). All names are proven PDB spellings (two sources each, LABELS.tsv); none is descriptive.

## 0x004321F0 NUISystem::CUIMiscThingsDef::operator= (matched, src/)
- Shape: the implicit copy assignment of the def class, with Copy (vtable 0x01230EEC slot 19, 0x004321E0) a `jmp` here.
  Expanded: CDefClassBase op= (CResource op= 0x00431F60 stays a call; PDefManager, GlobalIndex, the three flag bits as
  xor/and/xor), CParentDefClassBase op= (InstantiationName, SubDefInfoMap vector op=), then every CUIMiscThingsDef member in
  declaration order (CWideString / CCharString op=, the two maps through their _Rb_tree op=, scalars, C2DVectors).
- Levers: three opt-ins (hdr/README.md): implicit CDefClassBase op=, declared CResource op=, __forceinline PDCB op=.
- Labels: catalog CSpecialAbilitiesBerserkDef::operator= is wrong (BerserkDef op= landed at 0x0045F854).
- Name fixes: 0x00431F60 (CResource op=, row-local models say CDefPointeeBase), 0x00432B38 (_Rb_tree op=, bound as
  map<CCharString,long>::operator=); NAME_FIXES.tsv.

## 0x009FA780 CTexture::BltToRenderTargetWithGPU (parked, differ/)
- Shape: capture block + CScopedShadersEnable(false,false); display manager; copy of the viewport; quad size from the
  viewport extents, or (full / scaleHalf) the render-target dimensions with the viewport reset to them; four RHW vertices
  (column-major init), UV offsets (scaleHalf: 1/(w*0.5); pointSample: 0.5/w); state setters; DrawAsIndexedTris; restore.
- Key spellings: (float)width / (float)height at each use (no float locals), column-major vertex init, loop RHW before Z,
  FABLE_RETAIL_C2DBOXI_COPY.
- Residue: the AlphaBlendEnable block (0x009FAA34) of the inline SetRenderState loads CurrentBookmarkMask into ecx and the
  state's mask into edx; VC7.1 picks the other order for this block (the documented CRenderStateManager lane gap, GOTCHAS
  2026-11-03; 0x00B70230 / 0x00B761D0 / 0x00B751F0 are parked on it).
- Name fix: 0x00A0AEA0 DrawAsIndexedTris parameter types (NAME_FIXES.tsv).

## 0x00D07490 NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Main (parked, differ/)
- Shape: alive filter; scripted control (priority 4); flags chatted / commented; a CTimer; SetQuestCardObjective(
  "Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_06", "", "BanditCampMain"); loop: hero hit (MsgIsHitBy ||
  (AnySpecial && !Special(14))) -> GiveThingBestEnemyTarget, Release, ClearThingHasInformation, idle forever; while
  !Gate3Open: timer clear && within 8 units -> conversation with COMMENT_FIRST / _SECOND, timer = 10; talked to ->
  re-take control, movie + RAII pause, FIRST_CHAT once (chatted set inside the movie scope), then SECOND_CHAT.
- The calls, frame and data flow match; the residue is VC7.1's per-site choice to expand ~CTimer / the movie dtor
  (STATUS.tsv lists the ten spellings). Next: per-site frequency measurement with marker calls.

## 0x00DDB720 NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main (parked, differ/)
- Shape: scripted control; Pause(1); GuardPosition thing; sheathe; teleport; face its angle twice; taunt timer; loop:
  within DistanceToPlayAnim and the timer at 0 -> RACE_TAUNT line + PlayAnimation(Me.GetDataString(), ...,
  GDefaultToUsingPhysicsInCutsceneAnimations, ...) + wait + timer = 10; hero hit -> Release, enemy, a 20 s fight timer
  renewed on each further hit, ClearThingBestEnemyTarget, re-take control, walk back to the copied GuardPosition position
  (MoveToPosition(pos, 0, 1, false, true)) while more than 2 units away, face its angle, Pause(2).
- Best (f): loop-head macro + __forceinline GetPos. Residue: ~CTimer expanded (ours) vs called (retail 0x00CD4470) at the
  returns after the taunt speak loop, inside / after the post-fight StartScriptingEntity wait and after the walk-back loop.

## 0x00A15890 CNavigatorManager::OutputWorld(const C2DVector&, unsigned char) (not attempted)
- Identity: Ego_r ?OutputWorld@CNavigatorManager@@QAEXABVC2DVector@@E@Z in donor order; `ret 8`; aligned ebp frame.
- Body: fopen("C:\...\FablePathVisualiser\nav.data", "wt"), fallback fopen("nav.data", ...); fprintf "0.0, 0.0, 0.0, 0.0\n"
  then per node "%f, %f, %f, %f, %f, %f, %f, %f, %d\n"; fclose. Traversal: GetNodeAt 0x00A14F70, GetAreaCovering 0x00A763B0,
  GetNextNeighbour 0x00A15140, CNavBlockedQuadTreeNode ctor 0x00A76900 (new 0x10), vectors (_M_insert_overflow 0x004416E0 /
  0x00A17270 / 0x00A17190, push_back helpers 0x00A17400 / 0x00A17440), __find 0x00A169B0, __find_if<IsSameNavQuadTreeNode>
  0x00A16550; CRT fopen 0x00BFEDFA, fprintf 0x00BFEDF4, fclose 0x00BFEA20, free / delete 0x00BFEA14. 21 landed
  CNavigatorManager siblings give the class model.
- Label: catalog InitializeNavigationData is wrong.

## 0x007EBEA0 CTCActionUseBed::OnActionUse(CThing&) (not attempted)
- Identity: Ego_r ?OnActionUse@CTCActionUseBed@@UAEXAAVCThing@@@Z right after OnSerialise (retail 0x007EBE50, landed as
  vfunc_1: NAME_FIXES.tsv); `ret 4`; /GS cookie frame.
- Body start: PThingUser (+0x2b) = user through a CIntelligentPointer temporary (0x00A01B10 ctor / 0x00A01B90 assign /
  0x00A01B50 get / 0x00A01C10 dtor); the user's TC map (+0x44, find 0x0040F020, keys 0x33 and 0x23) checks; GetPBuilding
  0x00686D40 / 0x00449970; flags +0x14 / +0x15; 0x007EAB90(user); `movsx` of the user's +0x90 into +0x18; PBuilding (+0x23)
  set to null; 0x00686A80 then a message box through 0x00492FB4 with the 0x01271244 / 0x0127122C texts when +0xD9 is clear.

## Header debt
None beyond hdr/: all overlay changes are opt-in or new. Flipping the CDefClassBase / CParentDefClassBase defaults
would let the five row-local def-TU models (0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, ...) use the shared headers;
not attempted here.
