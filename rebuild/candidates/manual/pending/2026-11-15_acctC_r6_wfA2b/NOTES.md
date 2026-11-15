# wfA2 NOTES (acctC lane, w22)

## 0x00DCE230 NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::Main -- RELOCATION_MATCH, test PASS, addmatch + read-only vland PASS (src/, test/)
- CLOSED (successor 2, 45 grades): the 6-hunk eax/edx residue was NOT a candidate question. c2's expression temporaries take eax/ecx/edx from a
  ROTATING pointer (docs/pipeline/C2_ALLOCATOR_PROBE.md section 4; `c2prio_f.py --rotation` prints it per source line). Retail's DEFENDING block is
  exactly the direct-argument statement `Game->EntityFollowThing(Me,MyTeam->TeamCrateCarrier,1.0f,true)` (no `carrier` local) entered with the pointer
  at eax; ours entered it at edx because the inline `CScriptThing::operator==` of the preceding condition was spelled `PImp.Data->IsEqualTo(*other.PImp.Data)`.
  The landed-majority text `PImp->IsEqualTo(*other.PImp)` (CCountedPointer operator-> and operator*: two more expanded inlines) leaves the pointer one
  step further = retail. EVIDENCED spelling (15 landed rows use it; Ego_r ??8CScriptThing@@QBE_NABV0@@Z); nothing FITTED in the row.
- Measured dead ends on the way: k = 1..7 unused named locals (plain or initialised) byte-identical output, on both the carrier and the direct variant.
- Devices carried over from the earlier lane (both landed idioms): FABLE_FORCEINLINE on the row-local IsBeingCarriedBy / GetPos wrappers (19 landed
  script rows) and `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor + CPREDICATEBASE_NOVTABLE + CSCRIPTTHING_IMPLICIT_COPY (CBeggarBully::Main).
- Test: log model of the member state machine (style of the matched script Mains). vland WARN RELOC_NAME: 0x00DCEC50 is SetState (catalog label wrong, NAME_FIXES).
- History of the earlier agents (kept for the record):
- Identity: Ego_r.pdb + FableWin.pdb ?Main@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ; entity vtable slot 1 (prior lane LABELS).
- Rewritten on the shared engine/CQ_OrchardFarmRaidScript.h with a new opt-in FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CCRATETEAMMEMBER_MAIN
  (nested class defined after the enclosing class; no assert moved). MakeTeamMemberComment's third arg is the header's ERaidComment (push ebp = 0).
- What closed the old prologue residue (82 -> 6 hunks): the landed CBeggarBully::Main 0x00E5B3B0 inline-budget idiom,
  `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE + FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY.
- Residue: only the DEFENDING block 0x00DCE7F2..0x00DCE82F: retail EntityStopFollowing vtable eax / NewScriptFrame edx / EntityFollowThing
  MyTeam edx + vtable eax; ours edx / eax / eax + edx. c2prio trace (tools/c2prio_f.py): the `carrier` reference is the only global
  candidate in the block and takes eax (costs eax 0, ecx 1); the vtable temps are picked afterwards bottom-up around it, so the whole
  residue is carrier=eax (ours) v edx (retail).
- Tried (all 6 hunks unless noted): carrier non-const ref / pointer / team pointer / team ref / nested scope / `!(a==b)` / operator!=
  bodies / bool flag before the if / T-check form; direct argument (no carrier) 8 hunks (comment lea also moves); carrier declared
  before SetState / before NSF / before the T check: 2563 bytes (callee-saved).

## 0x0068CB90 CInputProcessControlFreeCamera::ProcessInput -- RELOCATION_MATCH, test PASS (src/, test/, payload.json)
- Identity: Ego_r/FableWin ?ProcessInput@CInputProcessControlFreeCamera@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z (`ret 8`, two args).
  The batch task text called it the constructor: wrong, the ctor is inline (header comment) and takes one argument.
- Rewritten on shared headers (stale draft had local C2DVector / CInputEvent / CCamera / class models). Opt-ins: CCamera MOVEFORWARD_MOVE,
  CInputEvent GETMOUSEMOVEMENT, CInputProcessControlFreeCamera PROCESSINPUT, GFAngles GET_ROUNDED_ANGLE_FORCEINLINE (new, hdr/README.md);
  existing C3DVector ANGLE_BODIES + INITFROMANGLES_GFSIN + CROSS_XYZ_GROUP, CCamera ANGLES / UP_DIRECTION / RENDER_INTERPOLATION (PointAt) /
  MOVESIDEWAYS, CInputEvent ACCESSORS, CProcessedInput PERFORMED, INPUTPROCESS.
- What closed it: (1) GET_ROUNDED_ANGLE force-inlined (1926 -> 2339 bytes); (2) the shared GFSin/GFCos InitialiseFromAngles body
  (chunk 2041 opt-in) removed the old 27-line x87 kill-placement residue at once; (3) CROSS_XYZ_GROUP (or XY) fixed the copy of
  temp.X into right.X (0x0068CBFF); (4) `processed.SetAsPerformedEvent()` at the end of each handled arm (cross-jumped to the single
  retail call) moved the right-stick y from the dead event slot [esp+78h] to the dead processed slot [esp+7Ch] (0x0068D2E5).
  Tried before (4): y/x decl orders, function-scope x/y / angleXY (worse), C2DVector stick(...) (frame 0x68).
- Test links the row object (stub callees, raw camera / processor storage); mutant without the stick normalisation FAILs.
- vland WARN RELOC_NAME: 0x00A0D390 is catalog-labelled NScript::CV_SickChildScript::CWomanToAttract::Init; the header's
  CProcessedInput::SetAsPerformedEvent (Ego_r ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ, agent1022) is the binding used.

## 0x0048E78E CSubtitleRenderer::DrawButtonGraphics -- parked (differ/0048e78e.cpp = the acctB draft, unchanged)
- Baseline: the acctB draft grades DIFFER(2662v2662) 14 hunks, but only with ref/0048e78e/hdr_STALE as the overlay.
- On today's rebuild/include + this hdr/ it fails to compile: 12 opt-ins the draft defines are absent from the repo:
  CRGBCOLOUR_CTOR_INITIALISE, CIVCOUNTEDPOINTER_FORCEINLINE_RELEASE, CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR, CFONTBANK_VSTRINGWIDTH,
  CFADEINFADEOUTBASE_FADE, CFADEINFADEOUTBASE_ISOPEN_OUTOFLINE, CSUBTITLERENDERER_BUTTONGRAPHICS, CSUBTITLERENDERER_ISFLAGSET,
  CPLAYERGUIDEF_TEXTBOX, CDRAWQUESTINFO_MINIGAMETYPE, CMAINGAMECOMPONENT_SCOPED_ACCESSORS, CPLAYER_RESPAWNDELAY.
- A mechanical re-apply (tools/reapply.py + tools/dedup.py + pin_asserts.py) works for CPlayerGuiDef (the repo has since named the
  text-box members on the default path: the opt-in's #else arm equals today's default text) but NOT for CSubtitleRenderer (asserts
  block), CMainGameComponent (scoped_ptr members now typed by default + SMART_POINTER_ACCESSORS), CRGBColour, CFadeInFadeOutBase
  (FADE vs today's OPEN opt-in; IsOpen) -- these need a hand re-base against today's text. Reverted (hdr/ holds none of them).
- DONE later in the session: re-based by hand (hdr/README.md wfA2 re-base section): the 14-hunk baseline reproduces on today's headers
  (differ/0048e78e.cpp). CMAINGAMECOMPONENT_SCOPED_ACCESSORS turned out unused (define dropped). Arm-A probes on it: `const C2DVector& offset`
  TLX,TLY,BRX,BRY 33 hunks (2664), float refs 27 (2666), TLX,BRX,TLY,BRY 26.
- (old) Next: re-base those headers by hand (or reuse today's equivalents: CFADEINFADEOUTBASE_OPEN, CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS),
  regrade to confirm the 14-hunk baseline, then the residue in ref/0048e78e/notes/NOTES.md (single-block flags/InCutscene regs + arm-A X-address CSE).

## 0x00435530 CDisplayEngine::DoRender / 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine -- not worked (baseline grades only)
- Stale drafts graded once under ref/<addr>/hdr_STALE: 0x00435530 DIFFER(2571v2617) 102 hunks (frame 0xa4 v retail 0xcc);
  0x00439070 DIFFER(2611v2613) 49 hunks. Both drafts are local-model sources (local CCharString / CRGBColour / CIEngine / CCamera /
  CWorld ... stand-ins, 286 and 870 lines): landing needs a rewrite on engine/*.h first. Breadth-first budget went to rows that could close.
- 0x0048E78E probes on the re-based base (probe/r5/): single block (layer if after box3, retail's shape) s1 23 hunks 2659 bytes:
  the 0x800 test is hoisted only to 0x0048EA0C in eax (retail: right after the GetFadeInOutAlpha call, 0x0048E9C6, ecx, with the
  0x45 push/pop next to it) -- a scheduling-region question; raw `DisplayFlags & 0x800` s3 same; bool before box3 s2 24 (2664);
  extra no-code parens on half / box3 extents / pulse (t1..t3) no effect; `bool behind = IsFlagSet(0x800)` right after pulse u1 24
  (2664); layer declared right after pulse u2/u3 ~160 hunks. Best stays differ/ (14).
- Ideas not yet tried: c2prio on s1 (which tuple ends the region before the test), C2DBoxF::Move (FableWin, FORCEINLINE) on s1 together
  with the single block, the 81-tuple cut moved by a tiny inline store helper (lesson 2005/2006).

## 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine -- re-authored on shared headers (successor), best 17 hunks, equal length
- Name: Ego_r/FableWin ?DrawPreparePrimitivesForEngine@CPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z. Extent 0x00439070..`ret 4` 2613 bytes confirmed.
- Source differ/00439070.cpp: engine/*.h only, illustrators without a header declared locally in NPlayerGui (Ego_r names, retail addresses). New opt-ins
  in hdr/README.md (wfA2 successor block). PeekGuiDef = the landed ctor's same-TU text (expanded 4x). CThing GETTC_EXPANDED (GetTC/PeekTC), plain inline
  bitset test (BITSET_TEST_DECL off: retail tests the word in place). Illustrator loop with `end` hoisted (retail reads +0x2C4 first).
- What closed most: CRGBFLOATCOLOUR_INITIALISE_CTOR (A,B,G,R right to left); the three colours as named locals in their own { } block (frame 0x94
  = retail; v3 temps-in-call 0xa0, v5 no block 0x9c); `C2DVector pos(...)` named before the sprite ctor (retail stores the zero uv vectors last).
- Residue (6 non-offset hunks, 0x0043912C..0x004392E9): colour slots backlight 0x20 / ambient 0x30 / diffuse 0x40 vs retail diffuse 0x20 / backlight 0x30
  / ambient 0x40 -- invariant under all 6 declaration orders, renames, copy-init, const-ref, one-declaration; and retail's 4th (R) int temp uses
  [esp+18h] (the dead CCharString slot) so its store is hoisted above the 3rd fild; ours reuses [esp+10h].
- fwsize: FableWin ??0CRGBFloatColour@@QAE@ABVCRGBColour@@@Z 46i/141b; ours 38i/117b; GetR() accessor forms 42i/134b (and grade worse, 29 hunks);
  named r,g,b,a locals 46i/146b. FableWin x87 sizes are unreliable (WF_RULES), so not pursued further.
- Ideas not tried: the colour block as a CPlayerGui-private inline helper (e.g. a lighting setter taking the engine), the block's locals as
  CRGBFloatColour temporaries bound to const refs inside the call, c2 slot-order trace (wfcommon/tools C26 fa.sh / C15 efm.py) to read the packing rule.

## 0x00435530 CDisplayEngine::DoRender -- still only the stale-draft baseline (102 hunks); successor did not re-author it (budget went to 0x00439070)
- Re-author on engine/*.h is needed (the draft has ~25 local stand-ins). Callee map built for it (target: landed file | leaf | reloc_bindings majority):
  0x00405F00 GFIntToWideString (bindings say ?LongToWideString@@YI..: check PDB); 0x00413520 CCountedPointer<CConsole>::Reset; 0x0041D03C
  NUISystem::CManager::RenderErrorMessage; 0x0041E5F2 NUISystem::CManager::GetInstance; 0x00434870 CFadeInFadeOutBase::Update; 0x004348D0
  GetFadeInOutAlpha; 0x00434C90 EndScreenFadeOut; 0x00434CD0 SetAboutToRun; 0x00435000 (landed as CGameScriptInterface::GiveHeroGold, the
  draft calls it CDisplayEngine::RenderHeroFuckShit -- identity unproven, PROVE FIRST); 0x00435070 CDisplayEngine::RenderHeroDrunkenness;
  0x00436D60 CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CThingFilter_OverlappingCircle>; 0x00436FB0
  CTBaseSingleton<CEngineManager>::Get; 0x00487570 CPlayer::DrawFreeCamDebug(const C2DBoxF*, const CFontBank&) const; 0x0048DB46
  CSubtitleRenderer::IsWaitingForClickPast; 0x0049D9D0 CWorld::PeekMyPlayer() const; 0x00633BE0 CPlayer::GetCurrentMode; 0x00637C80
  NThingMap::CThingMapManager::DebugDraw(const C2DCoordI&, long) const; 0x00640500 CEngineManager::OnRender; CWideString 0x0099AED0 ctor /
  0x0099B220 GetLength / 0x0099B510 dtor / 0x0099B6B0 ctor(const unsigned short*) / 0x0099B720 copy / 0x0099B8D0 += CWideString / 0x0099B940
  += wchar* / 0x0099BA70 Sprintf / 0x0099BFF0 operator+(const unsigned short*, const CWideString&); 0x0099E480 CCharString::ToWideString;
  0x009A4EC0 GFGetSystemManager; CDisplayManager 0x009BEAA0 SetGamma(float,bool) / 0x009BECE0 GetVVisibleTelevisionScreenArea / 0x009BEEB0
  SwapScreens / 0x009BEF20 BeginRender? (bindings: ToggleRendering) / 0x009BEF50 EndRender; 0x009D49B0 CDefStringTable::GetString;
  CRenderManager2D 0x009D8CF0 ClearRenderTarget / 0x009D9C80 DrawBufferedPolys / 0x009DA9F0 Render2DDrawList / 0x009DBFF0 Draw3DScaledText /
  0x009DD8F0 Draw2DText; 0x009E1DD0 GFAYToVY; 0x009EA550 and 0x009F8D60 unnamed (E2_ files: the console Render and the system-manager debug
  draw in the draft -- PROVE); 0x009ECD80 CConsole ctor; 0x009F4590 CSurface::SaveAsTGA; 0x00A0B560 CRenderManagerCore::SetVWindow;
  0x00A0BF20 CCamera ctor (ABVC3DVector@@0M_N vs 0MJ: last param bool or long, check PDB); operator new / free / __ftol2.
- Globals in the draft named g_013BA85C / g_013B8A0C / g_013B8A10 / g_RenderThrottle* / g_Screenshot* are descriptive placeholders: prove or
  name per the 2026-11-10 made-up-name rule before landing.
- (0x00439070, later) Slot-order diagnosis (probe/a70): slots follow packer weight -- a diagnostic second call using diffuse 3x (d4) moved
  diffuse to 0x20; `CRGBFloatColour diffuse; diffuse = CRGBFloatColour(PeekGuiDef().DiffuseColour);` (w6) gives EXACTLY retail's slot map
  (diffuse 0x20 / backlight 0x30 / ambient 0x40) but costs a 4-word copy (2645 bytes, frame 0xa4). So retail's source gives diffuse more IL
  references than the other two with no extra code. Not it: diffuse copy-init / `= CRGBFloatColour(...)` / const-ref-bound temp, ambient as a
  temp in the call (x2), ambient + diffuse as temps (x3, frame 0x9c), dead self-assignments (removed before packing).
- Regrades after the overlay changes: 0x0068CB90 still RELOCATION_MATCH, 0x0048E78E 14 hunks, 0x00DCE230 6 hunks (unchanged).
- Also not it: FableWin ??4CRGBFloatColour@@QAEAAV0@ABVCRGBColour@@@Z (operator= from CRGBColour, probe opt-in, removed again) with all three
  default-constructed then assigned (either declaration order) or only diffuse assigned: slots unchanged (y1..y3).
- Next ideas: the 4th-channel int temp and the slot order may share one cause (retail's colour temps packed with the PeekGuiDef CCharString
  slot 0x18): try the PeekGuiDef CCharString as a NAMED block-scoped local in the same-TU PeekGuiDef text only if a sibling shows it; or read
  c2's packer weights with a /d2 trace tool from wfcommon/tools (C26 slots.py / smap.py) on v4 vs w6.

## 0x00439070 (wfA2-3, opus successor) -- 17 -> 9 hunks
- Packer probe (tools/pk/pk.py --iset + tools/psim.py, a latest-slot-first simulator that reproduces c2's pass-2 joins; tools/pki.sh
  wraps both): retail's 4th-channel int temp at 0x18 = the R temps joining the hero/CCharString slot, which needs the drawHud slot
  CREATED FIRST (list = size asc, weight desc). drawHud weight 3 -> 8 with the six disjuncts written as an `if (...) drawHud = false;
  else if (...) drawHud = false; ...` chain (c2 merges the six stores; each counts once in the packer weight): R temp, hero, pos,
  the tail int temps all land on retail's slots (differ/00439070.cpp). Sequential ifs (no else) 2643 bytes.
- Left (9 hunks, frame only): GetDimensions return temps ($T, first 0x10 / second 0x30 in retail, ours both 0x20) and, as a
  consequence, the colours (retail diffuse 0x20 with the GetTC `type` group, backlight 0x30 with the 2nd GetDimensions temp,
  ambient 0x40). Simulator: retail needs the 1st GetDim temp to conflict with an item of the type/width slot AND of the hero slot
  but NOT with pos, the 2nd to conflict with pos too. Measured c2 lifetime model: address-taken named locals (pos, sprite, GetTC
  `type`) conflict with every temp of their BLOCK (from block entry, also before the declaration; an inner `{` before width moves
  the 1st GetDim temp out of pos's range, T5); $T temps live within their statement; non-address-taken named (width) def->use.
- Not it (all 17 or worse before D1): ctor plain inline (no FORCEINLINE), k = 1..7 unused int locals, 1..9 dead asserts, colour
  temporaries in the call (39), no block (23), const-ref colours (17), pos as a temporary in the sprite ctor (45/41), both
  GetDimensions in pos's initializer (2579 bytes), GetDim 2 in pos's initializer (2591), one declaration height,width (9).
- Name lead: the `static long frameCount` is the anniversary PDB's file-static S_LDATA32
  `GNumberOfTimesDrawPreparePrimitivesForEngineHasBeenCalledBeforeUseProfileGammaShouldBeApplied` (one weak source only).
- 9 -> 1 real hunk: FableWin emits ?GetWidth@CTexture@@QBEKK@Z / ?GetHeight@CTexture@@QBEKK@Z COMDATs right after this row's
  TU functions (0x01A7EC70 / 0x01A7ECA0, beside DrawPreparePrimitivesForEngine 0x01A7D4F0): the row uses the inline accessors.
  `unsigned long height = DebugTGATexture.GetHeight(0);` + `C2DVector pos((GFVXToAX(W) - (float)DebugTGATexture.GetWidth(0)) * 0.5f,
  (GFVYToAY(H) - (float)height) * 0.5f);` (opt-ins FABLE_RETAIL_CTEXTURE_WATERDYNAMIC for GetHeight, _GETWIDTH) puts all three colours,
  the 2nd GetDimensions temp and everything else on retail's slots (G3 = differ/). Both accessors in pos (G2) 6 hunks; separate
  height/width statements with accessors (G1) 9.
- Last real hunk: 0x00439943 `lea ecx,[esp+14h]` (retail) v [esp+24h]: the GetHeight C2DExtentsI temp must share pos's slot (0x10).
  Simulator: it needs NO conflict with pos but a conflict with an item of the GetWidth-temp/type slot AND of the hero slot (as in G2,
  where the inline-expansion temps live to the end of pos's statement). Measured: inline-expansion temps live to the end of the
  calling statement; pos (named, address-taken) conflicts with every temp of its block, so no block layout gives both. Tried: const /
  const& / direct-init height (same), FITTED braces round pos (temp goes to the type slot as predicted), pos as a temp in the sprite
  ctor (E2/E3 35-41), sprite as a temp (H2/H3), K1 const pos (6). fg.py also flags a FALSE hunk at the four zero uv stores
  (`c7442460 00000000`: its '00000000 => REL' rule), identical bytes.

## 0x0048E78E (wfA2-3) -- still 14 hunks (differ/ unchanged); allocator diagnosis of the single-block shape
- c2prio (tools/c2prio_f.py --blocks/--trace) on s1 (retail's single block: `if (IsFlagSet(0x800)) layer = M2;` after box3):
  the DisplayFlags CSE temp (#100, prio 29) is coloured before the InCutscene byte temp (#113, prio 26) and takes eax, so
  InCutscene gets cl and the flags load cannot be hoisted above the block's eax uses (that is the whole 0x0048E9C6 /
  0x0048EA0C difference: post-RA scheduling follows the register). Retail = InCutscene first (al), flags ecx.
  Flags' prio: block 19 (the big block, K=9: #86 &ButtonPulseTimer, graphic_max_width, flags, button1_drawn, drawn, layer,
  const 0x45, this, const 0) gives +36; the rest -7. Needs K19 <= 8 or InCutscene >= 30 (one more candidate in its block 27).
  The two-block base has K19=6 (bools in their own block) -> prio 15 -> right registers, wrong block order.
- Not it: InCutscene through a hoisted `bool in_cutscene` (w2, 23), `layer = flag ? M2 : L2` before/after box3 (41/42),
  `bool behind` before box3 (24). IsInCutscene() needs the CSUBTITLERENDERER opt-in (SRC_FAIL as tried).
- C2DBoxF::Move has out-of-line retail copies (reloc_bindings 0x0041A810 / 0x00547F60) -> plain inline in retail; this /Os TU
  calls a plain inline Move, so retail's arm A is not Move unless forceinlined (not evidenced).

## 0x00435530 CDisplayEngine::DoRender (wfA2-3) -- still the stale local-model draft (102 hunks), now on TODAY's headers
- The draft compiles on today's rebuild/include + hdr/ after one re-applied opt-in (hdr/engine/CRenderManager2D.h
  FABLE_RETAIL_CRENDERMANAGER2D_DRAW3DSCALEDTEXT, hdr/README.md): DIFFER(2571v2617) 102 hunks, same as on hdr_STALE (probe/d1/base.cpp).
  It still carries ~25 local class models (unlandable): the shared-header port is the next job.
- Frame (packer probe + fixed tools/efm.py; its `L[st-2000:st]` slice was empty when the function starts before line 2000):
  in our-frame coordinates retail keeps every small slot where ours does (things, filter, window, the late CWideStrings
  at 0x14/0x18/0x1C) but has ~40 extra bytes of low-density slots between `centre` (retail -72, ours -148) / televisionArea
  (retail -88) and the camera (retail -16, i.e. 40 bytes higher; CCamera is 56 bytes in Ego_r, so not the camera). Find which
  temporaries retail keeps apart there (C2DBoxF console window temp, the Draw2DText C2DVector temps, the C3DVector text position)
  before re-porting; the earlier "retail shares no slots" note is wrong (CWideString slots ARE shared).
- Name: 0x00435530 = ?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z (protected); 0x00435F70 (landed as a DoRender
  forwarder) is the public Render wrapper (NAME_FIXES.tsv).

## 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine -- RELOCATION_MATCH, test PASS, addmatch + read-only vland PASS (successor 4; src/, test/)
- What closed the last hunk (0x00439943 GetHeight C2DExtentsI temp, 0x24 v retail 0x14): pos is passed by reference to the sprite ctor
  0x0043FA20, an out-of-line COMDAT of THIS TU (FableWin 0x01A7E8F0 beside the row). With the ctor's body visible c2 sees pos is not
  captured, so pos no longer conflicts with every temp of its block and the GetHeight temp joins pos's slot. Diagnostic first:
  `__declspec(noinline)` on a visible body (probe/s4/hdrN) gave RELOCATION_MATCH with both accessors in pos's initializer (c2d), 9 hunks
  with the separate `height` statement (c0d). Then: the body under a new opt-in FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_POS_CTOR_BODY
  (out-of-class inline after the class; text grades MATCH against 0x0043FA20 alone: probe/s4/ctor.cpp) + `// Flags: /d2inlT63`
  (Inline-budget: at the default 74 and at 64 c2 expands the ctor (2692 bytes), at 63 it calls it, at <= 62 other callees stop expanding
  (2422) -- the only N that matches). /d2inlS5..12 no effect. Also needed: FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR, C2DBOXF, C2DBOXF_CORNERS.
- Kept: the FITTED drawHud else-if chain (with the visible ctor the || spellings still grade 16 / 20 hunks at T63). Not it (this session):
  k = 1..7 unused int locals at function top or in the TGA block (no change, 14 grades); const C2DVector& pos bound to a temporary (same as
  named; G2 form 6 hunks); pos declared in an outer block and assigned (copy costs 14 bytes, 13-56 hunks).
- Local class models: the NPlayerGui illustrators without a header are declared in the row (as before). engine/CDrawMiniMap.h exists but is a
  generated, flattened GLOBAL `struct CDrawMiniMap` (no namespace, no methods), which cannot express NPlayerGui::CDrawMiniMap's members; the
  landed siblings 0x0064E83F / 0x0064401C / 0x00644660 also declare NPlayerGui::CDrawMiniMap locally. Coordinator: confirm.
- Test links the ROW OBJECT: fake vtables (CIEngine slots 23 / 67, CGameScriptInterface slot 395), raw object storage, 5 calls covering gamma
  on call 2, profile alpha, lighting colours, timer set choice, illustrator clearing, subtitle / fade / letterbox, the drawHud chain order, the
  first-person minigame cursor, ClearEffects conditions, the centred debug sprite. Mutants (unconditional health ClearEffects; always the
  Current timer set) FAIL.
- vland: WARN RELOC_CONFLICT ?SetCrimeAsDisplaying -> 0x00437770 here, 0x004376B0 in landed 0x00775B40. 0x004376B0 reads PMoney +0x2CC
  (SetMoneyAsDisplaying), 0x00437770 reads PCrime +0x2EC; Ego_r spacing 0x0044A350 / 0x0044A410 = retail spacing 0xC0. The landed caller
  0x00775B40 should call SetMoneyAsDisplaying (NAME_FIXES.tsv line for 0x004376B0 covers it).

## 0x0048E78E (successor 4) -- still 14 fg hunks = 2 real regions (fg's immediate-zero false hunks on `cmp byte [x],0` inflate the count)
- New level tried: callee body visibility. FableWin places BOTH sprite ctors as COMDATs in this TU (0x01A662C6 box / 0x01A663FC pos,
  beside DrawButtonText 0x01A665C5 / DrawButtonGraphics 0x01A67310). Proved texts: CRIME_CTOR body = RELOCATION_MATCH v 0x00493AC2 (pos);
  a box-ctor body (same style, ClipBox(box), ClipBoxIsPos(false), Texture(0)) MATCHes 0x0041BEB0 with FABLE_RETAIL_C2DBOXF_COPY (probe/s5/
  hdrB, ctors3.cpp). In this /Os TU neither body is expanded (length unchanged), but the grade is unchanged too: base 14, single-block
  s1/s2/s3 23/24/23 (probe/s5/v*.cpp) -- the boxes' capture is not what drives the flags/InCutscene colouring here.
- Moving the bools / layer into the inner (box3) block is SRC_FAIL (they are used after it). No measured lead left; parked.

## 0x00435530 CDisplayEngine::DoRender (successor 4) -- not re-authored (budget went to closing 0x00439070); port plan + measurements
- Draft (probe/d1/base.cpp, local models) still DIFFER(2571v2617), frame 0xa4 v retail 0xcc; frame-masked struct diff (C44 sd.py) 121 hunks,
  most are esp offsets (frame +0x28 also turns disp8 into disp32: a large part of the 46-byte length gap). efm.py (tmp/pk_dr0.asm): retail
  _info at +48 v ours +8 (the 0x28 frame gap); retail centre -72 (own low-density slot near televisionArea -88 / camera -16), ours -148
  (shared with a temp). Lesson from 0x00439070 applies in reverse: retail's extra slots = named locals that conflict with every temp of
  their block because their address escapes to an out-of-line callee (centre -> GetDebugCentre hidden return / filter ctor; things ->
  GetAllThingsInRoughArea 0x00436D60, a template COMDAT of THIS TU -- its body may be visible in retail).
- Shared-header coverage (grep of today's headers + hdr/): CDisplayEngine has InitialFadeReadynessCount / InitialFadeActive /
  ScreenFadeDuration / TimeSinceFadeStarted / PViewManager / DrawGame / Frame / World but NOT SetAboutToRun 0x00434CD0, EndScreenFadeOut
  0x00434C90, RenderHeroDrunkenness 0x00435070 (+ the 0x00435000 callee: the draft's "RenderHeroFuckShit" is unproven; reloc_bindings
  says ?GiveGold@CTCHeroMoney, a catalog label -- PROVE); CCamera ctor present; CRenderManager*.h: check CRenderManagerCore /
  CRenderManager2D for SetFogEnable, SetVWindow 0x00A0B560, ClearRenderTarget 0x009D8CF0, Draw3DScaledText (hdr opt-in exists), Draw2DText
  0x009DD8F0, SetTextureMipMapMode, DrawBufferedPolys 0x009D9C80, Render2DDrawList 0x009DA9F0; CDisplayManager has the television area /
  Begin / End / SwapScreens, not SetGamma 0x009BEAA0; CEngineManager lacks OnRender 0x00640500; NUISystem::CManager complete; CInputManager
  lacks the +0x38 Paused byte; CSystemManager has Debug + GFGetSystemManager; CPlayer has DrawFreeCamDebug / GetCurrentMode, not Active;
  CWorld lacks GetDebugCentre; CThingSearchTools has GetAllThingsInRoughArea; CDefStringTable has GetString, not the GDefStringTable
  global; CSurface lacks SaveAsTGA 0x009F4590; CConsole has NO header (a local declaration is allowed; ctor 0x009ECD80, Render 0x009EA550);
  CSubtitleRenderer has IsWaitingForClickPast.
- Globals: reloc_bindings names for the draft's placeholders are catalog-quality (g_LightsDrawnThisFrame 0x013BA85C, g_b13B860C, g_b13B8629,
  g_RenderFrame_013b7d70, GPInputManager 0x013B8388, g_pGlobalGui 0x013B8790, g_pConsole 0x013CAA40, FableFullBuildVersion 0x013B85E0);
  0x013B8A0C / 0x013B8A10 / 0x013B86EB / 0x013B86E7 / 0x01375455 / 0x013B862C / 0x01375720 unbound. Each needs a PDB/FSE/string proof or a
  descriptive name per the 2026-11-10 rule.
- Next: port onto those headers (about 15 additive opt-ins), then test the capture hypothesis above (define the 0x00436D60 template body
  above the row; GetDebugCentre declared returning C3DVector by value).

## 0x0048E78E CSubtitleRenderer::DrawButtonGraphics -- RELOCATION_MATCH, test PASS, addmatch + read-only vland PASS (wfA2-5; src/, test/)
- Start: the re-based acctB draft (14 fg hunks = 2 real regions). Two facts closed it, in ~45 grades:
  1. SAME-TU CALLEE BODIES. Retail compiles IsCutsceneSkippable 0x0048DE44, GetTextWidthFromText 0x0048E062 and IsTextReadyForClickPast
     0x0048E072 earlier in this TU; VC7.1 then knows they clobber only eax/edx (ecx kept), which changes the caller's candidate set /
     priorities. Defining their landed texts above the row under `#pragma auto_inline(off)` made retail's natural one-block shape
     (`if (IsFlagSet(0x800)) layer = M2;` AFTER box3; the old FITTED early placement is gone) colour flags=ecx, InCutscene=al, 0x45=edx:
     14 -> 10 hunks (only the IsCutsceneSkippable + IsTextReadyForClickPast pair matters; IsOpen / GetTextWidthFromText bodies alone change
     nothing). IsCutsceneSkippable on shared headers: `CMainGameComponent::Get()->PeekWorld().IsPerformingRegionLoadReally()` (needs
     FABLE_RETAIL_CWORLD + the existing ..._ISPERFORMINGREGIONLOADREALLY opt-in; FableWin COMDAT 0x01A652B5 right after IsCutsceneSkippable
     0x01A6519F; the landed local model called it g_pGameComponent->m_pDisplayEngine->m_busy, but +0x24 is PWorld and +0x104 RegionLoadStatus),
     timer tests raw `.Active` / `.Opening` as the landed text. All three grade RELOCATION_MATCH v their retail addresses from src/.
  2. BOX-SPRITE CTOR BODY VISIBLE (new opt-in FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY; text MATCH v 0x0041BEB0 alone,
     probe/w2/boxctor.cpp; FableWin places the COMDAT at 0x01A662C6 in this TU). With it, arm A in retail's natural order
     TLX,TLY,BRX,BRY gets both offset addresses CSE'd (lea eax 5FC / lea ecx 600) -> RELOCATION_MATCH. The position ctor body
     (..._CRIME_CTOR, v 0x00493AC2) is defined too (FableWin COMDAT 0x01A663FC, same TU); it alone does not close arm A.
- Defines added v the draft: C2DBOXF_COPY, CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY, ..._CRIME_CTOR, CENGINEPRIMITIVE_ID_CTOR, CWORLD,
  CWORLD_ISPERFORMINGREGIONLOADREALLY. No flags. FITTED (kept from the draft): the two bare blocks (box1..return, box3..arms).
- Test: links the row object; 30-odd checks over 7 scenarios (flag 4 / 0x800 / in-box Y / paused set, timer open/close, action mode,
  in-cutscene + IsCutsceneSkippable branches incl. region load and skip prompt, flag 2 ready / 0x48, last arm, override tail +
  fishing clamp). Mutant (no layer M2, no gap for button 3) fails 2 checks. Private member called via `/alternatename`.
- Names: 0x0048E062 is GetTextWidthFromText (landed as global operator_new), 0x0048E072 IsTextReadyForClickPast (reloc_bindings says
  IsTextFullyShown): NAME_FIXES.tsv.

## 0x00435530 CDisplayEngine::DoRender (wfA2-5) -- parked again; local-model draft improved, shared-header port still the job
- New best differ/00435530.cpp (= probe/dr/t4.cpp, still local models): DIFFER(2587v2617), fg 109 hunks but 64 STRUCTURAL hunks
  (tools/mdiff.py <q.txt>: mnemonics with all numbers masked) v 102 for the old draft (kept as probe/dr/differ_prev.cpp).
- Fact 1 (EVIDENCED, retail 0x00435962..0x00435A0F): the debug-map loop draws each nearby thing's DEF NAME, not a track-node name:
  `things[i]` +0x70 is CThing::PBaseDef (CDefPointer<const CThingBaseDef>, null-tested = HasBaseDef), the virtual is CDefClassBase slot 10
  `CDefString GetInstantiationName() const` (hidden return slot), then GDefStringTable.GetString(TablePos) (0x009D49B0, ecx=0x013CA828),
  CCharString::ToWideString 0x0099E480 straight into the by-value CWideString argument slot (`push ecx ; mov ebp,esp` reservation), then
  ~CCharString 0x0099EAE0 BEFORE the GFAYToVY / 0.7f / position arguments: that is an INLINED `CDefString::ToWideString() const { return
  GetString().ToWideString(); }` (engine/CDefString.h has it declaration-only, out of line 0x00415D90 elsewhere) -> needs an inline-body
  opt-in in the port. With it the whole argument order of Draw3DScaledText matches.
- Fact 2: CWorld vtable +0x34 is slot 13 PeekWorldMap() const (engine/CWorld.h list), and retail evaluates the map manager OBJECT before
  pushing DebugDraw's arguments: spelled through the inline accessor CWorldMap::GetThingMapManager() (engine/CWorldMap.h) the order matches
  (a raw member chain pushes the level first). Lesson: header accessor texts decide FE evaluation order -> port onto engine/*.h first.
- Not it: defining CFadeInFadeOutBase::Update / GetFadeInOutAlpha (COMDATs 0x00434870 / 0x004348D0 are in this TU's run) and
  EndScreenFadeOut 0x00434C90 (clobbers eax only) above the row under auto_inline(off): grade unchanged (f1/f2/t3). 0x00435000 calls
  external code (no register gain).
- Remaining structural hunks (mdiff on tmp/q_dr4.txt): the three Draw2DText blocks (retail builds the position / colour after the text
  temporaries, 0x00435A5C / 0x00435B95), the FramesBehind block (0x00435AB9: retail tests a pointer `mov ecx,[eax+N] ; test ecx,ecx ; je ;
  mov ebp,ecx`), the console block (0x00435C66..0x00435C9F: GFAYToVY / font height order), screenshot loops (0x00435E29..0x00435F3F: a
  `dec dword [N]` = `--g_ScreenshotCount` in memory), plus the whole-function zero-register / ebp-edi permutation and the 0x24-byte frame
  gap (retail keeps centre +0x6C and the C2DBoxI box at +0x1C: no slot sharing between them).

## 0x00435530 CDisplayEngine::DoRender (wfA2-6) -- shared-header PORT done: DIFFER(2617v2617), equal length, frame 0xcc, fg 31 hunks (scheduling only)
- differ/00435530.cpp (= probe/p1/k1.cpp) is on engine/*.h + overlay opt-ins (hdr/README.md "wfA2-6"); local declarations only for classes
  with NO header (NDisplayView::CViewBase, CProfileManager, CConsole). `// Flags: /d2inlT66` (Inline-budget: GFIntToWideString expanded at
  FramesBehind + wishworldshot, CALLED at the movie-shot branch 0x00435D9D; default T74 expands all three, T66..T63 match).
- What closed the structure (each measured): CWorld slot 5 is GetCurrentRenderCamera(const CInterpolationInfo&) const returning a CCamera
  (0x38 bytes at frame 0x6C; the old "centre" was its WorldPosition); MainWindow.Initialise(0,0,W,H) (C2DBOXF_INITIALISE; +edi CSE);
  no C2DBOXF_COPY; loop index `long i` (fimul) with `unsigned long count`; named `const CThing* thing = things[i]` (PBaseDef CSE);
  FramesBehind text `L"Frames behind " + GFIntToWideString(DebugNoFramesUnableToRender)` inside the Draw2DText call; build version with a
  named `CRGBColour colour` + `const CFontBank& font = PeekMainFont()`; `World->` (not PeekWorld()) in the map block; the filter as a
  TEMPORARY in the GetAllThingsInRoughArea call (a named filter lives through the loop and blocks retail's slot sharing: packer probe
  tools/pki.sh FUNC=DoRender); OldGammaRamp != GammaRamp; GetBackBuffer() (+0x2C) for the screenshot surface; (H * 5.0) / 12.0 (two fmuls);
  0.45f (retail constant 0x012316F8 = (double)0.45f).
- Remaining (fg): x87 fstp placement in Draw3DScaledText / Draw2DText position building, `mov ecx,[esi+8]` vs fld order, loop latch reload
  order, `xor eax,eax` vs `sar edi,2` order at 0x00435951. All register choices and slots match.
- (wfA2-6, later) Best now differ/00435530.cpp: mdiff 6 / fg 6 real hunks (fg shows 6 more false zero-immediate hunks). Added since k1:
  implicit double->float conversions in every C2DVector / C3DVector / C2DBoxF argument (NO `(float)` casts: the casts reorder fld vs the
  Component load and fstp vs `push lit` in every text block -- 23 -> 10 hunks); build version spelled
  `C2DVector(VIRTUAL_SCREEN_WIDTH * 0.1, VIRTUAL_SCREEN_HEIGHT - 2.0 * PeekMainFont().GetVFontMaxHeight())` with PeekMainFont() and a TEMP
  colour (CFontBank::GetVFontMaxHeight is the header's inline GFAYToVY(GetFontMaxHeight()); same bytes as named colour/font/height locals,
  so the named version was dropped); `long i = 0;` before `count` (FITTED, entry order); /d2inlT63 (range T61..T66 all equal).
- Residue tried and NOT it (all still 6): latch -- i++ / != / unsigned cast compare, `const unsigned long count`, `long i` before things or
  before the camera (worse), `for (unsigned long count = ...; i < count; ++i)`, count declared then assigned, `continue` form, Y spelled
  i * GFAYToVY(16) / (float)i / swapped addends, thing via *(begin()+i) / non-const, k = 1..7 unused named locals at the function top or
  before the loop (no byte change at all), the visible noinline GetAllThingsInRoughArea body (..._ROUGHAREABODY: no change).
  Build block -- 2.0f / *2 / *2.0 / h+h / -2.0*h+H, double or const height, named font pointer, const colour, colour after font,
  GetVFontMaxHeight via a named font, 0.1*W. c2prio (tools/c2prio_f.py, tmp/c2p.txt): i = candidate id 25 (prio 36, split, eax piece),
  things._M_start = local id 23 (prio 46, edx); retail reloads i first -> i must sort before the begin web (more refs on i or fewer on begin).
- Next ideas: (1) the wfB1 x87 slot-template rule (lessons/LESSONS_wfB*.md: a one-instruction difference in the integer stream when the
  stores are placed; find a statement fact of the build block that adds an instruction that later disappears); (2) for the latch, make the
  begin web lighter / i heavier with a natural spelling (e.g. the vector freed by an explicit clear, or a second use of i); (3) behaviour test
  is NOT written yet (test/00435530.cpp needed once it matches: ~45 stub callees, see tmp/relocs.txt for the exact symbol list).

## 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine -- re-audit (local class model): still RELOCATION_MATCH, test PASS
- Auditor finding: the source defined `namespace NPlayerGui { class CDrawMiniMap : public CDrawBase {...} }` although
  rebuild/include/engine/CDrawMiniMap.h exists (the base clause hid it from verify_and_land's `(struct|class) X {` regex).
- Fix: hdr/engine/CDrawMiniMap.h = today's generated header + new additive opt-in FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS
  (appended after the last assert; the real NPlayerGui::CDrawMiniMap : CDrawBase with Update 0x0064401C / Draw(.., bool) 0x00648820
  and the PDB members from +0x28, sizeof 0x160). Row and test: `#define` the opt-in, `#include "engine/CDrawMiniMap.h"`, local class
  deleted. Grade unchanged: RELOCATION_MATCH with // Flags: /d2inlT63 (1 grade); vt.py PASS (row object linked); addmatch redone under
  flock; read-only vland with LANDVERIFY_STRICT_HEADERS=1 on a one-row copy (tmp/00439070_payload.json): RELOCATION_MATCH PASS, no
  HEADER_POLICY / SHARED_HEADER_AVAILABLE.
- Hand re-check of the other local illustrators (27 local classes in src, 29 in test, regex incl. base clauses): none has
  rebuild/include/engine/<Class>.h, engine/NPlayerGui_<Class>.h or an overlay header; the only other generated one,
  CDrawQuickAccessIcons.h, is in engine/_quarantine/ (not on the include path, not checked by the lander); CPlayerGui.h only
  forward-declares them.
- SetCrime/SetMoney: NAME_FIXES 00437770 (appended) + 004376b0 (earlier) carry the two-source proof: Ego_r/FableWin donor order
  of the seven Set*AsDisplaying vs the retail 0x30-spaced run, and the member each reads (+0x2CC PMoney at 0x004376B0, +0x2EC PCrime
  at 0x00437770). Landed 0x00775B40 CTCShop::FrameUpdate must call SetMoneyAsDisplaying(); the landed
  CPlayerGui_SetCrimeAsDisplaying_004376b0.cpp is SetMoneyAsDisplaying; CPlayerGui.h CRIME opt-in address comment says 0x004376B0.
- LABELS appended: ClearEffects of CDrawHealth / CDrawStamina / CDrawAbilityButtons / CDrawMeleeActionButtonEffects
  (0x00644644 / 0x0064464C / 0x0064528A / 0x00645821; vland RELOC_NAME catalog labels).

## 0x00435530 CDisplayEngine::DoRender (wfA2-7) -- latch CLOSED; one residue left (build-version text x87/push placement)
- Best: probe/x/best_e1.cpp (copied to differ/ at the end of the session): real hunks = only 0x00435C9F / 0x00435CA5.
- Latch reload order (0x00435A14) closed by TWO facts, each measured with tools/c2prio_f.py --trace:
  (1) FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY (+ CTCMAPWHO, CTCMAPWHO_SEARCHID, CTHING_STATTYPEFLAGS, NThingMap_CThingMap.h, CTCMapwho.h):
      with the template body of GetAllThingsInRoughArea visible (retail instance 0x00436D60 is in this TU) c2 knows `things` is not
      captured, so things._M_start becomes a candidate LIVE ACROSS the Draw3DScaledText call, split like i (same bytes; before it was a
      fresh memory load at the block end, emitted ahead of i's piece reload);
  (2) a same-TU function compiled BEFORE DoRender: c2's candidate/piece ids carry state from the previous function in the TU (any
      function above DoRender flips the split-pass piece ids: i gets the lower piece id -> its reload is emitted first = retail). Used:
      the real same-TU callee CDisplayEngine::EndScreenFadeOut 0x00434C90 with its landed body under #pragma auto_inline(off) (grades
      RELOCATION_MATCH v 0x00434C90 from this file; retail calls it out of line at 0x00435850-ish).
- Also: 0x013B85E0 respelled GVersionString (landed name fix of agentNF26: was FableFullBuildVersion).
- differ/00435530.cpp now = probe/x/e1.cpp + header notes (DIFFER 2617v2617, fg 10 hunks of which 4 real lines, all in ONE region).
- Remaining residue, MEASURED (tools/b1/ = wfB1's c2 list-scheduler trace sc.sh + sim.py, copied and repointed; 14 s per trace):
  region = build-version block 0x00435C63..0x00435CC7 (33 nodes). Ours: cycle 11 fld+push0, 12 fmul+lea, 13 push eax, 14 push edi,
  15 fstp X (fmul->fstp latency 3) + push ecx, 16 push esp, 17 fadd ... Retail's order = the same schedule with the integer stream
  ONE cycle later from cycle 12 or 13. tools/b1/hy.py / edgeenum.py / pnode.py (offline sim) show exactly which DAG facts give retail:
  an extra NON-EMITTED integer-unit node ready at cycle 12-13 with prio >= 0x44 (any pred <= call12, or fmul with the fstp keeping
  latency 3), or an extra edge into lea(colour)/push(colour)/push(font) (e.g. lea->push latency 2, fstp X -> push font).
  c2's non-emitted integer nodes seen in this function: 0x269 (made by explicit parentheses / casts round a binary FP expression:
  `(W * 0.1)` or `(float)(W * 0.1)` -- but it also drops the fstp's latency to 0, so X goes one slot too EARLY: probes p1/d1), 0x2a0
  (inline-asm boundary, GFFPUSetRoundingNearest), 0x2c3 / None (block ends). A coalesced self-move (copy candidate with >= 3 refs or a
  split piece, both in edi) feeding `push edi` would also do it (sim E12->22), but no natural spelling made one.
- Tried for the build block and NOT it (all same 4 real lines unless stated): named vector (frame moves), named float height (font
  load moves ahead of the colour), named colour+font+height in retail order, `const CRGBColour&` bound to the temporary, font via
  Component->PeekMainFont() in either place, CWideString(copy) (+16 bytes), casts on X (early) / Y / h / 2.0*h, parens on X (early),
  Y, 2.0*h, both; 2.0f, W / 10.0, (double)h, inline wrappers returning float, `!= false`, dead asserts 1/2/4 at entry, /G6, /Op (+48).
  Dummy / real functions above DoRender change c2's ids but never this region (so it is not an id / piece-order fact).
- FITTED kept: `long i = 0;` before `unsigned long count` (the natural `for (long i = 0; ...)` puts `sar edi,2` before `xor eax,eax`,
  0x00435951: equal priority 0x18, tie broken by tuple order).
- Next ideas for a successor: (1) find the source of an invisible int node in that block (read c2's node creation for 0x269 /
  copies: which IL makes a paren node WITHOUT stealing the store's latency); (2) re-check FableWin's GetVFontMaxHeight (/Od 15i 33b v
  ours 14i 31b, fwsize DIFF +1i +2b: a different inline text of the helper is the one lead not tried -- needs a second source).
- (wfA2-7, end) differ/00435530.cpp also defines the existing opt-in FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE (FableWin /Od 18i =
  ours after it; release bytes identical). fwsize on the row (tmp/_fwsize): the only x87 helper whose FableWin size still differs is
  CFontBank::GetVFontMaxHeight (+1i +2b); three alternative texts (named float result, named float argument, `+ 0.0f`) gave no
  schedule change (scratch overlay tmp/hdrx, deleted from use; hdr/ untouched). Session total ~80 full grades + 14 c2 traces.
- Device check (owner's acceptance test): (a) FITTED spellings -- ~40 build-block spellings, none moves the region; (b) inline control
  -- not an inline residue (T61..T66 identical); (c) k unused locals -- earlier agent: no byte change at all; (d) dead asserts 1/2/4 --
  no change. The residue is a scheduler-DAG fact (one invisible integer node), so no listed device applies.
- (wfA2-7, last measurement) tools/b1/memil.py: ONE memory-order edge from the X store (fstp into the C2DVector temporary) to
  `push edi` (the font argument), lat 0, also gives retail's exact order (as does the IL order push0/colour -> X store -> font push with
  WAW edges). So retail's DAG had either an invisible int node in cycles 11-13 or c2 treated the font push as ordered after the X store
  (a memory operand at DAG time?). Neither has a known source spelling yet; this is the whole remaining gap of the row.

## 0x00435530 CDisplayEngine::DoRender (wfA2-8, successor) -- parked again; same ONE residue (0x00435C9F / 0x00435CA5), no measured progress
- Start: differ/00435530.cpp regraded = DIFFER(2617v2617), 4 real lines, unchanged (probe/s8/b0.cpp). ~75 full grades + 4 c2 traces this session;
  sweep scripts probe/s8/sw.sh (variant tsv -> grade + "fstpO" early/late signature) and probe/s8/tr.sh (scheduler region dump).
- New measurements: (1) the x87 side is BINARY: every spelling of X is either "late" (fstp X after push edi = ours, fmul->fstp latency 3)
  or "early" (fstp X before push eax: any paren / cast / comma / inline returning double round W*0.1 creates node 0x269 => latency 0).
  Retail is IN BETWEEN (fstp at fmul+2, or the integer chain one cycle later); the offline sim (tools/b1/hy.py) confirms both
  "fmul->fstp latency 2" and "lea->push(colour) latency 2" reproduce retail, as do the known invisible-int-node / X-store->push-edi edge.
  The DAG latency table is fixed per op pair (histogram over the whole function: fmul->fstp is always 3 for a data edge), so only the
  extra-node / extra-edge explanations are real. Priority games cannot do it: with the FP stream fixed (fld c11, fmul c12, fstp >= c15)
  retail's emission order REQUIRES one idle integer cycle in c12..c14 (push19->21->22 chain lat 1; mov25 waits on push23 by WAR on ecx).
  (2) pnode.py: the invisible node may depend on call9 / push10 / fstp11 / call12 / fld / fmul / push0 / lea / push(colour) with prio
  0x44..0x58 (or higher for later preds) -- so a no-code int node in IL BETWEEN the two calls (GetFontMaxHeight / GFAYToVY) with no
  edge to call12 also works; nothing natural put one there.
- Tried and NOT it (all 4 real lines, same "late" placement unless stated): X spellings (float)W*0.1, W*0.1f, W*(0.1), (0, W*0.1) [early],
  true?W*0.1:0.0, inline float/double helpers BuildX/Tenth [double = early], (float)(float)(...) / (float)(double)(...) [early], parens on
  both args [early]; Y spellings (float)H - ... (13 lines), H - h - h with two calls (2649), parens on 2.0*h, (double)(float)h, (double)H;
  named colour + named font + temp vector (n05: identical), named vector (frame same, but Y computed first: 10-27 lines), named colour +
  font + h with H-(h+h) / H-h*2 / H-2.0*h (identical 4) or 2.0f*h folded into h (8) or y named (10); inline by-value vector helper MakeVec
  (same 4 + the latch hunk returns: any inline function defined above the row perturbs c2 ids); font argument as *Component->PCutSceneMainFont,
  static_cast<const CFontBank&>(...), *&PeekMainFont(); C2DVector(float,float) ctor text `{X=x;Y=y;}` and `{Initialise(x,y);}` in a
  scratch overlay (tmp/s8hA, s8hB; no change, not adopted); TU flags /G5 /GB /Ox /Ob1 /Oi- (identical), /G7 (2597, worse);
  device c: k = 1..7 unused named locals in the block, plain and `= 0` (identical); device d: 1-3 dead FABLE_RETAIL_ASSERTs in / after
  the block (identical or worse).
- FableWin GetVFontMaxHeight +1i/+2b explained as codegen, not text: seven /Od texts measured (probe/s8/fw); the plain text is 14i/31b;
  the FableWin extra equals exactly `mov eax,[edx+8] ; call eax` (5 b) v `call [edx+8]` (3 b) (byte arithmetic; FableWin's bytes not
  available here), most likely a virtual-call codegen difference of that build, so that lead is treated as closed.
- Status: near_miss, parked. differ/00435530.cpp unchanged (still the best source). No header change in this session (hdr/ untouched).

## 0x00435530 CDisplayEngine::DoRender (wfA2-9, successor) -- session log (probe/s9/, sweep g.sh, variant generator mk.py)
- Baseline regraded: differ/00435530.cpp = probe/s9/b0.cpp DIFFER(2617v2617), 4 real lines (0x00435C9F / 0x00435CA5), unchanged.
- NEW EVIDENCE: retail has out-of-line CDisplayEngine::PeekMainFont at 0x00434DE0 (`mov eax,[ecx+8]; mov eax,[eax+0x1614C]; ret`)
  and PeekDebugFont at 0x00434DF0 (+0x16144), i.e. both are ordinary (non-inline) functions of DoRender's own TU, compiled before
  DoRender (Ego_r 0x00446BE0 / 0x00446BF0 have them out of line too). Defining them above DoRender (row source, after
  `#define FABLE_ENGINE_CDISPLAYENGINE_FONTS_DEFINED`) instead of the header's inline bodies: byte-identical row, same 4 lines
  (probe/s9/pf1.cpp in retail order after EndScreenFadeOut, pf2.cpp before it). Not adopted (no change); LABELS.tsv rows added.
- c2 trace facts (tools/b1/cy.py on tmp/sc_s9b0.json): cyc 11 fld|push0, 12 fmul|lea, 13 push eax, 14 push edi (the ONLY ready
  int node), 15 fstp X|push ecx, 16 push esp, 17 fadd|mov ecx. Retail = the same with cycle 14's int slot taken by a no-code node.
  Offline sim (tools/b1/hy2.py / hy3.py / hy4.py / hy5.py): ONE extra no-code int node whose successor is lea(colour), push(colour),
  push(font), fstp X (lat 0) or fadd gives retail for any predecessor; single extra edges that do it: push19->lea20/2,
  lea20->push21/2, push21->push22/2, fstp15->push22/0, fmul14->lea20/1, call12->lea20/3.
- PARTIAL: double parentheses round X, `C2DVector(((VIRTUAL_SCREEN_WIDTH * 0.1)), ...)` (also `(float)((W * 0.1))`, triple or
  more, or with any extra paren on Y) = 2 real lines: fadd / push esp now in retail order; only `fstp X` is still after `push edi`.
  c2 makes TWO 0x269 tuples: the inner one has fmul->paren latency 3, prio 0x6800 and takes the int slot at cycle 15 with the fstp;
  so push edi still issues at 14. Retail cannot have that tuple (its push edi must share cycle 15 with the fstp), so double
  parens are a NEAR form, not the answer; not adopted (unnatural, and not a match).
- Tried, all = same 4 lines: named font ref / pointer after a named colour (u01-u04), font via Component->PeekMainFont(),
  *Component->PCutSceneMainFont.Get(), height via Component->...GetVFontMaxHeight() (w03-w06), derived-to-base conversions on
  colour / font (d01, d02: conversions fold before scheduling), int<->pointer casts round the font / colour (d03-d05), parens
  round each argument (r01-r05), parens / unary plus / casts on h (x01-x05), (W) * 0.1 / 0.1 * W (x06, x07), h * 2.0, h / 0.5,
  W / 10.0, W / 10, 0.5f / 2.0f forms (z01-z06), CRGBColour 3-arg / literal / copy forms (c01-c04), CTOR4_INITIALISE removed (y01).
  GFAYToVY(PeekMainFont().GetFontMaxHeight()) written out (w01): 21 lines (no font CSE, Y first) -> the inline GetVFontMaxHeight
  layer is evidenced. __assume anywhere: 20 lines function-wide (dead end).
- MATCHED (wfA2-9): src/00435530.cpp = b0 + `#define FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN` -> RELOCATION_MATCH /d2inlT63.
  New overlay opt-in in hdr/engine/CFontBank.h: GetVFontMaxHeight `return (GFAYToVY(GetFontMaxHeight()));` (#else = default line).
  Found by a probe: a local inline height helper with a parenthesised return closed the row (probe/s9/h01.cpp), so the paren tuple
  of the inline body is retail's missing no-code integer node. Corroboration: landed 0x00494900 CFrontendGameComponent::Output
  matches with the opt-in AND the natural `y += MainFont->GetVFontMaxHeight();` (probe/s9/users/out2.cpp); with the plain body it
  needs its named line_height temporary (out0 matches, out1 = opt-in + temporary DIFFERs); 0x00487570 DrawFreeCamDebug matches with
  both (users/fc0, fc1). Coordinator: consider making the paren body the default and respelling Output naturally (one body for all rows).
  Still needed with the paren: `long i = 0;` before count (hp2: 2 lines), EndScreenFadeOut body above (hp3: 2 lines), /d2inlT63
  (no flag 2649 bytes; T61 / T66 match). FableWin /Od size of the helper is 14i / 31b with or without the paren (no /Od evidence).
- (wfA2-9, closing) Test written: test/00435530.cpp (links the row object; five frames: SetAboutToRun / initial-fade end, main window,
  normal scene + view, profiles debug font, fade clock + EndScreenFadeOut + subtitle wait, Recording / Frames behind <n> / Skipping
  texts, console box (0,0,640,200), build version at (64, 470) main font white, wish-world and movie shots with 6-digit padding,
  gamma only on change, swap only when render, Frame++). vt.py PASS; mutants (unconditional SwapScreens; `g_ScreenshotCount >= 0`)
  FAIL. The row needed a `// Retail-asm:` line (0x00435594 fnstcw / 0x00435598 and / 0x004355A0 fldcw through the shared
  GFFPUSetRoundingNearest.h helper) to grade retail_asm instead of NOT_GENUINE in vland. addmatch done; read-only vland
  (LANDVERIFY_STRICT_HEADERS=1, one-row payload copy tmp/one_payload.json) RELOCATION_MATCH PASS; the full payload vland after the
  CFontBank.h overlay change: the other four rows RELOCATION_MATCH PASS (tmp/vland.log). WARN RELOC_NAME only for catalog labels
  (0x009F8D60 DrawProfiles, 0x009EA550 CConsole::Draw: LABELS; 0x00435000 RenderHeroFuckShit: NAME_FIXES line 10).
  Old best source moved to tmp/differ_00435530_pre_s9.cpp (differ/ now holds no open rows). wfcommon style_* sources do not use this
  overlay (not regraded). Grades this session ~125 (fg/q) + 8 c2 traces.

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).
