# e12 notes (egod lane, w23)

## 0x00949580 CAIStateGroup_GoToPub::WalktoSeatAndSit -- RELOCATION_MATCH (808v808), test PASS, vland RELOCATION_MATCH PASS (17 grades)
- Restart pass: test gained look-loop guard cases (table dies / tavern time ends before and during the look loop, perform IN_PROGRESS).
- Port onto shared headers (IsRunnable 0x00949AB0 + EatAtTable::Update 0x0096CD70 include sets: CModeSitTavernStool, CCreatureAction_SitDown,
  CTCTavernTable SEATS (SetSeatState 0x007B8530 -- the e11 note's "SetSeatTaken" is wrong; Ego_r spells SetSeatState), CTCVillage TAVERN)
  with Ego_d's locals verbatim. First compile: 808v808 with 24 frame-offset hunks (psit_mode not sharing pos_to_look_at.Z's slot, +4 frame).
- What closed it: dropping FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY -- the by-value CCountedPointer argument's copy ctor visible (VC7.1
  still calls its COMDAT 0x008AA6A0) keeps psit_mode's address tracked so it packs. No inline controls needed (the VISIBLE_CALLED_CTOR /
  _DTOR noinline devices of the twin are unnecessary here: the plain header inlines are called, measured).
- Tried without effect (with COPY_DECL_ONLY): ctor/dtor visibility variants, declaration order (vectors / ret / village up front),
  C3DVector operator+ texts (INITIALISE_ADD, ADD_NAMED_RESULT, ADD_XY_GROUP), ctor/copy texts.
- Header: new opt-in FABLE_RETAIL_CAISTATEGROUP_GOTOPUB_WALKTOSEATANDSIT (hdr/README.md). Names: all Ego_r / FableWin spellings.

## 0x008F3310 CCreatureAction_DragonFireBlastBase::PerformAction -- RELOCATION_MATCH (796v796), test PASS, vland PASS (8 grades)
- Port onto shared headers with the landed same-TU sibling CCreatureAction_DragonRearUpFull::PerformAction 0x008F3630's include / opt-in
  set + NEW retail opt-in FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBLASTBASE (hdr/README.md) + CTCGRAPHICAPPEARANCE_DUMMYOBJECT, CRIGHTHANDEDSET
  (GetUp), CGAMEDEFINITIONMANAGER, CENGINEPERSISTENTPRIMITIVEBASE. First compile: 802v796, only the direction block differed.
- What closed it: Ego_d's names gave the statement structure: `fire_direction = rhs.GetUp(); fire_direction *= -1.0f; fire_direction.Z = 0;
  norm = fire_direction; norm.Normalise(); line = target - mouth; norm *= line.GetMagnitude(); target_pos = mouth_pos + norm;` (the by-value
  operator* forms evaluate Z first). The old draft's "scheduling slot" residue was an artefact of its local model.
- Not needed (measured, dropped): CDEFCLASSBASE_GETSUBDEF_NOINLINE (GetSubDef COMDAT is called anyway). GFFASTONEOVERSQRT_NEWTON_PAREN kept
  only for one-text-per-TU consistency with 0x008F3630 (the default text also matches).

## 0x00B3FC40 CEngineMap::PeekInterpolatedMapNormal -- RELOCATION_MATCH (528v528), test PASS, vland PASS (8 grades; purity retail_asm)
- What closed the 29 frame hunks (frame 0x54 vs 0x60): defining the previous function of the object file, the landed body of
  CEngineMap::PeekMapNormal 0x00B3F5E0, in the row ahead of this function (Ego_d TU order). One tail hunk then remained (x87 / epilogue
  interleave 0x00B3FE3A..0x00B3FE44); FITTED FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN closed it (GROUPED / PAREN leave 5 hunks).
- // Flags: /d2inlT444 (restart pass, 2026-11-18: was T576) -- smallest N, measured: no flag = this row RELOCATION_MATCH but the defined
  PeekMapNormal body DIFFER(298v1624) at 0x00B3F5E0; T443 = PeekMapNormal expanded into this row (DIFFER 1403v528); T444 = both
  RELOCATION_MATCH (landed 0x00B3F5E0 also carries T444). Test strengthened with floor boundaries (-2.0, -1.5, 2.5, 3.5, -0.5).
- Ego_d names used (xp, yp, int_xp, int_yp, frac_x, frac_y, normal00..normal11, normal). The row keeps the draft's `// Retail-asm:` line
  (the shared GFFloatToLongFloor helper), so purity grades it retail_asm.

## 0x00985440 CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate -- RELOCATION_MATCH (625v625), test PASS, vland PASS (12 grades)
- Port onto the shared retail class (WIFEUPDATE + THINGREACTINGTO as SayToThingReactingTo 0x0095EE70) + new nested opt-in HEROIMITATION, a new
  engine/CThingFilter_IsCreatureType.h (Ego_d record), CTCPERCEIVETHING (+ _GETSIGHTRADIUS, + _AWARENESS / _AWARE_CALLED for its inline
  StatGetInterfaceType), CTCCREATUREMODEMANAGER, CTHINGSEARCHTOOLS(_FIRSTTHING), FILTER_BIND3_FUNCTION, CONVERT_MUTABLE, CMainGameComponent
  GetConstantFPS, Ego_d's locals. First compile 620v625; the final return as a direct `||` expression (int-materialised) gave 625v625 / 14
  frame hunks.
- What closed it: the same-TU template COMDAT GetPFirstThing<CFilter_Bind3<IsNot, IsCreatureType, In2DRange>, const CThingAICreature>
  (retail 0x00986E30, called) made visible with the existing FIRSTTHING_BODY opt-in (noinline; Inline-budget line) + CTHINGAICREATURE_STATTYPEFLAGS,
  then Ego_d's ret_bool placed on TurnToFace's result (as the speech test it leaves 2 hunks; FITTED between the two placements). With
  IsCreatureType::operator() a header inline and CTHING_SCANINCLUSION_ISALIVE that body ALSO grades RELOCATION_MATCH at 0x00986E30
  (148 bytes) -- that COMDAT could be landed from this TU (leaf: the GetPFirstThing instance; not added to payload).

## 0x00497FD0 CProgressDisplay::DisplayCenteredText -- RELOCATION_MATCH (1076v1076), test PASS (24 grades)
- Port of the old draft onto today's headers with Ego_d's names (vbox, pdisplay_manager, prender_manager, parser, counter, boxlength,
  num_lines at function scope, max_num_lines, text_line; loop block: left, length, start_draw_pos, draw_pos) + the if/do loop shape:
  1076v1076 with 5 hunks -- the white (4th) draw's `draw_pos = start_draw_pos` copy scheduled as a block (load, load, store, store)
  where retail interleaves (0x00498328..0x0049834B).
- What closed it: the residue is a scheduling-region cut (MATCHING_PLAYBOOK "Scheduling regions"). Ego_d's unused-looking `left`
  local is the half spare width: `float left = (boxlength - length) * 0.5f; C2DVector start_draw_pos(left + vbox.TLX, ...)` plus ONE
  PAREN tuple: FITTED the existing FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN (FableWin text, already proven by 0x00435530 / 0x00494900);
  `vbox.BRY - (font.GetVFontMaxHeight() * num_lines)` with the default text matches equally. Without left the two parens together also
  match (p12), each alone leaves 2 hunks; left = (...) + TLX forms give 27-29 hunks.
- Tried without effect: memberwise copy (20 hunks, start_draw_pos scalarised), C2DVector(start) temp / ctor copy (61 hunks), copy-init
  draw_pos (25 hunks), +C2DVector(0,0).
- Header: NEW nested opt-in FABLE_RETAIL_CWSTRINGPARSER_INLINE_CTOR (hdr/README.md). GFRoundV?ToNearestPixel are declared in the row as
  in the landed same-TU DrawRetailDisplay 0x00498490 (no header declares them). Names: Ego_r / FableWin; Ego_d for locals.
- vland (read-only, restart pass, all five matched rows incl. this one): RELOCATION_MATCH PASS, no RELOC_IDENTITY. Same-TU landed rows of
  progress_debug_display.obj use GetVFontMaxHeight nowhere (checked every landed 0x0049xxxx sibling), so the PAREN opt-in leaves the TU
  one text.

## 0x00657580 NPlayerGui::CDrawQuickAccessIconsPC::Draw -- OPEN, DIFFER(1057v1057), differ/00657580.cpp (pass 1, ~15 grades)
- Port of the hand846 local-model draft (rebuild/candidates/manual/pending/2026-11-07_linux_hand846/differ/00657580.cpp) onto shared
  headers: NEW engine/NPlayerGui_CDrawQuickAccessIcons.h + NPlayerGui_CDrawQuickAccessIconsPC.h (Ego_d class records 0x3DF32 / 0x6113C:
  HotBarPrimitiveHandles is the real member name), NEW opt-ins NPLAYERGUI_GFTOSCREEN_CALLED (retail calls 0x00643BE2),
  NPLAYERGUI_CDRAWBASE_GETPLAYER_CALLED (calls 0x00643D7C), CGAMEPLAYERINTERFACE_QUICKACCESSBUTTONS (0x00445950); the existing
  CPLAYERGUIDEF_CLASS, CPLAYERGUI_PEEKGUIDEF / _GRAPHICBANK / _CTOR, CGRAPHICDATABANK_GRAPHICSIZE (slot 23), CTCINVENTORY_QUICKACCESS,
  NPLAYERGUI_ANCHORS, CDRAWBASE_HANDLES / _VSLOTS / _SETASDRAWING, IS_THING_ALIVE (inline), CTHING_GETTC_EXPANDED; global HUDRingAlpha
  (descriptive, as landed 0x00652FA4). Ego_d structure: phero at function scope; the rest inside `if (IS_THING_ALIVE(phero)) { ... }`
  with ptc_inventory, num_allocated_quick_access_buttons (last used slot + 1), num_quick_access_slots, `{ i }`, `{ gui_def { hot_bar_sprite,
  hot_bar_pos } else { hot_bar_sprite, end_graphic, hot_bar_pos { graphic_bank, hot_bar_end_sprite } } }`.
- First compile 1054v1057 (as hand846). Writing BOTH branches as `GFAnchorToBottomLeft(gui_def.DPadRingsPos) + ...[k].Offset +
  gui_def.HotBarOffset` (v4) gives 1057v1057 with only two real sites (the `mov dword 0` hunks are byte-identical alignment artefacts):
  0x00657663 branch 1 Y sum (retail anchor.Y first, ours offset.Y first) and 0x00657772 branch 2 X sum (retail offset.X first, ours
  anchor.X first). The alpha byte load (`xor edx,edx ; mov dl,[HUDRingAlpha]`) follows the order.
- Measured: our compiles give X this-first / Y argument-first whatever the operand order (v1, v4, v5); retail is uniform within a branch
  and opposite between the branches. Offset-first spellings fold &Offset into the displacement (`[ebp+48h]`) where retail materialises it
  (`add ebp,48h` / `lea ebx,[edx+ecx-8]` before the call) -- only anchor-first (argument-reference) spellings reproduce that, in both
  branches. Tried, all worse or inert: NAMED_RESULT_ADD (Ego_d's 68-byte operator+ text; 1070/1073, adds a temp store), declare-then-assign
  hot_bar_pos (1073..1090), branch permutations (v1 1054, v5 1054 / 10 hunks, v6 1051), the sprite position-ctor body visible (inert).
- Second change of level (c2 record ids, as landed 0x00652FA4's FITTED (1)): sweep of k no-code `C2DVector unusedN;` locals (k = 1..8,
  12, 16, 24, 32) at function top / top of branch 1 / top of branch 2 (probe/657580/sweep.log): site 0x00657772 flips to retail for k mod 4
  in {2, 3} at ANY of the three points; site 0x00657663 never moves. Natural supplier found: the base row's two-argument
  `phero->PeekTC(TCI_INVENTORY, &ptc_inventory)` (n1) closes 0x00657772 with no device (one-argument PeekTC<T>(&p) is the shifted form).
  `Drawing = true` (n2) is inert; the Clear -> Reset -> Release FORCEINLINE texts (n3) also close 772 but are inline controls, not needed.
- Remaining residue: ONLY 0x00657663 (branch 1 Y sum: retail anchor.Y first, ours offset.Y first; our X / Y orders in branch 1 are always
  opposite, retail's equal). Tried on n1, inert or worse: `.begin()->Offset`, `[num_allocated_quick_access_buttons - 1].Offset` in branch 1,
  `+=` for HotBarOffset (1055), regrouped / reordered sums (1054 / 1070), every k of the sweep. differ/00657580.cpp = n1.
- Ideas left for pass 2: the C2DVector(float,float) ctor / operator+ texts that make both components follow the same operand order
  (needs a measured header text with /Od evidence: Ego_d's operator+ is 68 bytes = NAMED_RESULT_ADD, which adds a temp store here); the
  GFAnchorToBottomLeft declaration form (a body visible under auto_inline(off) as the base row does) changing the result's ids.
- Test: not written (row open). Names: all PDB spellings (Ego_r / FableWin + Ego_d locals); HUDRingAlpha descriptive (bound as both ?HUDRingAlpha@@3JA and @@3EA in reloc_bindings: coordinator should pick one type).
