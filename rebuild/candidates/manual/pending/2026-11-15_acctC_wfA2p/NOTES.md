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
