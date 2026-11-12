## 0x00808E40

## 0x00808E40 CThingFilter_IsValidTargetForPlayerCreature::operator() -- RELOCATION_MATCH (14 grades)
- Made it match: port onto engine headers (CThing PeekTC expanded, CTCStockItem/NTargeting/CTCStealth/CTCActionUseWindow); class-name
  test as a nested `if` (an `&&` adds a temporary-destruction flag); identity test two-return `if (P==&thing) return true; return false;`
  (`return P==&thing;` emits xor/sete cl and moves regs); melee branch `if (flags&8) { if (!Vuln) return false; } else {...} return true;`.
- Defines: FABLE_RETAIL_CCHARSTRING(+_FORCEINLINE_EQ), CTCBASE, CTHING, CENGINEPERSISTENTPRIMITIVEBASE, CTCSTOCKITEM_TARGETING,
  NTARGETING_MELEE, CTCSTEALTH_ISINDANGER (overlay hdr/engine/CTCStealth.h), CTHING_GETTC_EXPANDED, CTHING_HASTC_FORCEINLINE. No flags.
- Made-up name: g_bPlayerMeleeTargetingCheck (bool @ 0x013B85F4, no name found anywhere; only this function reads it).
- IsInDanger inline body inferred: timer reset to -1 in OnCreate/ClearDangerStatus => in danger = InDangerCoolOffTimer >= 0.
- Leaf line uses the prefix `??RCThingFilter_IsValidTargetForPlayerCreature` (anon-namespace hash is TU-dependent).
- Repair (verifier finding 1): PlayerCreature retyped `const CThingPlayerCreature*` per PDB/struct_layouts; needs added defines
  FABLE_RETAIL_CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CTHINGPLAYERCREATURE + #include "engine/CThingPlayerCreature.h".
  Still RELOCATION_MATCH (also strict headers); test PASS.

## 0x0070CE10

## 0x0070CE10 CAnimSet::UpdateDistanceMoved -- RELOCATION_MATCH (strict headers), 19 grades
- What made it match: the STLport vector<float> fill ctor must be VISIBLE but called out of line (new opt-in
  FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE = real STLport body under __declspec(noinline)); with the body visible VC7.1 knows `speeds`
  does not escape, so _M_start stays in ebp through the PeekAnimation calls and the dtor, and the empty allocator temp lands on the
  `distance` arg slot (frame 0x10). With the declaration-only FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE it is 0x14 frame + reloads.
  Other facts: UpdatePercentageCompleteTo is only `Last = Current; Current = p;` (FableWin 14 instr), the wrap/clamp is written in
  UpdateDistanceMoved (UpdateTimePassed idiom incl. `float& current`); GetMovementDistance = MovementVector.GetMagnitude() in the
  retail C3DAnimationInfo branch (a by-value copy loads Z,X,Y instead of Z,Y,X); plain speeds[i] (no float* local).
- Headers/opt-ins (overlay hdr/, see hdr/README.md): CAnimSet.h FABLE_RETAIL_CANIMSET_UPDATEDISTANCE, C3DAnimationInfo.h
  FABLE_RETAIL_C3DANIMATIONINFO_MOVEMENT, CAnimateThing.h FABLE_RETAIL_CANIMATETHING_GETTHINGSCALE, fable_containers.h
  FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE. No flags/pragmas.
- Names: draft's GetLength / DistancePerCycle / g_PMainGameComponent / PWorld were wrong (GetMagnitude via GetMovementDistance,
  GetThingScale()->Scale, CMainGameComponent::Get()->GetWorld().GetMeshBank()). No landed name proved wrong.

## 0x00949380


## 0x00949380 CAIStateGroup_GoToPub::ThankBarmaid -- RELOCATION_MATCH (strict headers)
- Matched by: porting onto shared headers (CThing GETTC_EXPANDED + CTCWEAPONTRAIL_UPDATE stack-byte HasTC, CIntelligentPointer,
  ThingAlive, CCreatureAction_PlayAnimation IMPLICIT_DTOR, CTCTalk, CAIStateGroupBase PERFORMACTION/AIACTIONS, CAIBrain GETCREATURE);
  tail `return IsStopping() ? false : true;` (byte neg/sbb/inc; `!IsStopping()` widens to eax); and the phrase read through the
  inline accessor CTCAIScratchpad::GetThankingPhrase() const (FableWin.pdb QBEKXZ) -- reading the member directly left three
  register-choice hunks (phrase edx, tankard presence ecx, brain eax) that no loop/decl/creature spelling moved.
- Needs overlay opt-in FABLE_RETAIL_CTCAISCRATCHPAD_GETTHANKINGPHRASE in engine/CTCAIScratchpad.h; no flags.
- Local classes (no shared header): CAIStateGroup_GoToPub, CTCTavernTankard. CTCTavernTankard::SetBarmaid 0x0094B400 is the PDB name
  (?SetBarmaid@CTCTavernTankard@@QAEXPAVCThingAICreature@@@Z in Ego_r.pdb egor_pdb_names.tsv @0096f7d0 and FableWin.pdb
  _fablewin_api.tsv @017c46e2). Landed row 0x0094B400 still carries placeholder CClass_0094b400::AssignTarget_0094b400 and the
  label CTCTavernTankard::SetPBarmaid is wrong; reported in NAME_FIXES.tsv.
- Repair (verifier findings): respelled callee SetPBarmaid -> SetBarmaid(CThingAICreature*) in source + comment; still
  RELOCATION_MATCH (also strict headers), test PASS; NAME_FIXES.tsv added. All three findings applied.

## 0x006E3580

## 0x006E3580 CTCTalk::PlaySpeech -- DIFFER(504v502), wf10 author pass 1 (~35 grades)
Ported onto shared headers (overlay hdr/, see hdr/README.md); best source differ/006e3580.cpp (grade with FABLE_HDR_OVERLAY=$W/hdr).
What fixed most: inline IsTalking + SetAsUsingNarrator bodies in the header (draft's plain in-row inline did not inline SetAsUsingNarrator); `CThing& thing = *ParentThing;` local before OnDialogueStart (ebx load order); a same-TU `__declspec(noinline)` copy of the landed std::__find body (0x4B8FF0) under auto_inline(off) moved the empty tag into the arg7 slot as retail.
Exact residue: `first` (PermittedAINarrators.begin()) is evaluated early into eax; retail loads `mov ecx,[eax+100h]` after the tag push. Proven: direct `_M_start` (diag_hdr_M_start/, protected member made public: NOT landable) => RELOCATION_MATCH.
Tried without effect on that residue: vector ref / CGlobalDef ref / pointer locals; `last` named or end() thrice; std::find wrapper vs direct __find; named tag local; first local; fastcall find wrapper; find via iterator_category(first); ctor-less tags (all, incl. forward_iterator_tag); non-const begin(); begin() spelled without this-> / returning const _Tp*; /Ob2; optimize("s").
Note: every other retail caller of 0x4B8FF0 (IsQuestRegistered 0x4B0122, 0x92F5F2/0x92F67F, SetAsUsingNarrator 0x6E2D1E) also loads first late, and the landed IsQuestRegistered / SetAsUsingNarrator rows used direct-field local models -- so it is a general STLport-vector-begin()-in-find modelling gap, not row-specific.
Best next idea: find the VC7.1 spelling where vector::begin() in a __fastcall register arg is not pre-evaluated (probe: toy vector with begin() vs field in __find(begin(), last, v, tag()) ; try begin() as a non-template/out-of-class definition, _STLP vector with `_M_start` in vector itself rather than _Vector_base, or iterator typedef as a wrapper class collapsed by the inliner), then apply it as an STLport opt-in.

## 0x006E3580 CTCTalk::PlaySpeech -- RELOCATION_MATCH (wf10 author pass 2, ~20 grades + 1 probe)
What made it match: the real body of CBasicString<T>::Compare (0x00411570, the landed body, `?Compare@?$CBasicString@D@@KIJPBD0@Z`) out of line under `#pragma auto_inline(off)` in the row TU, next to the existing noinline __find body (0x004B8FF0). With Compare visible, VC7.1 knows the __find call writes no memory, so the inline `PermittedAINarrators.begin()` load sinks after the tag push straight into ecx, as retail. Both bodies are needed (without the __find body: 504v502). Plain /O2 (no flags).
Probe: toy vector begin() into a fastcall find: early with an opaque/impure find, late with a pure visible find body (or under /Oa; /Oa on the row matched the load but swapped edi/ebp, so /Oa is not the TU flag).
Headers: hdr/ overlay unchanged from pass 1 (CTCTalk PLAYSPEECH + PLAYSPEECH_INLINES, CMainGameComponent SERVERFRAME_OUTOFLINE, CTCSoundPlayer BANKSAMPLE, NSpeechGainManager ONDIALOGUESTART).
General lesson: "begin() of a vector loaded early before a find call" = the find's comparison callee body (here CBasicString::Compare) is not visible; the landed IsQuestRegistered / SetAsUsingNarrator direct-field local models could be re-ported this way.

## 0x006E3580 repair pass (verifier findings applied, still RELOCATION_MATCH incl. LANDVERIFY_STRICT_HEADERS=1; test PASS)
1. Test model: no sound player now returns 1 (SPEECH_PLAYBACK_FAILED_FINAL = 1 in CTCTalk.h; retail 0x006E3773 `mov eax,1`), and the test asserts 1.
2. hdr/engine/CTCSoundPlayer.h BANKSAMPLE citation corrected to `?PlayAttachedSample@CTCSoundPlayer@@QAEKAAVCASoundBank@@ABVCSoundPair@@ABVCSoundInit@@_N33@Z` (FableWin.pdb 0x01E7FCA0, Ego_r.pdb 0x00574680); comment only, no code change.

## 0x00966AF0

## 0x00966AF0 CAIStateGroup_TavernServer::GetBar -- RELOCATION_MATCH (8 grades)
- Ported onto shared headers exactly like sibling GetServingPoint 0x00966DA0 (same defines/includes) + IS_THING_ALIVE twice + TCI_TAVERN_BAR.
- What made it match: the pick sits in `while (true) { if (!bars.empty()) { ...pick...; return true; } return false; }`. VC7.1 inlines
  ~list only at returns OUTSIDE a syntactic loop (first `if (bars.empty()) return false;`) and calls 0x00792240 out of line at returns
  inside the loop; `while (!bars.empty()) {...} return false;` puts the final return outside the loop -> inlined + tail-merged (4 hunks).
- Headers: overlay engine/CAIStateGroup_TavernServer.h opt-in FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_GETBAR (declares GetBar). Flags: none.
- Repair (verifier findings): overlay now only ADDS the 3 opt-in lines (original ` bool IsCarryingJug();` restored byte-for-byte); source comment
  cites retail 0x00966B60-0x00966B97 (two GetPItem-nonnull + [+0x91]&1 pairs) for the repeated IS_THING_ALIVE; the unverified ~list 0x00792240
  NAME_FIXES claim was dropped from NOTES/DOSSIER (labels_chunk240/259 corrections already name it std::list::~list; no NAME_FIXES.tsv handed back).
  Regraded RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1); vt.py PASS.

## 0x006992A0

## 0x006992A0 CGameCameraManager::PreparePrimitivesForRendering -- RELOCATION_MATCH (grade 7 of ~12)
- Made it match: port onto shared headers (CThing GETTC_EXPANDED, CThingSearchTools, CIEngine, CEnginePrimitiveLine ONHIT inline ctor + ID_CTOR, CYLINDER CBase), a local pair copy per debug point, then the Q32 lesson twice: FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY (template body visible -> list homed in the dead info slot, frame 0x64) and FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE (retail GetAllThings 0x004ACD70 calls operator() 0x006A80A0 -> filter store scheduled before call setup).
- Headers/flags: overlay hdr/ (two opt-ins, see hdr/README.md); Flags: none.
- NAME_FIXES candidate: 0x008764E0 is NCameraModes::DrawPyramid (Ego_r 0x0085D1F0 / FableWin 0x029C4CB0, ?DrawPyramid@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@M@Z); landed 0x007617A0 spells it EditorDrawCross.

## 0x008F0EB0


## 0x008F0EB0 CTCTavernGameCardBase::CardsTurning -- RELOCATION_MATCH (10 grades)
- Matched by: porting onto the shared headers with the ShuffleCards 0x008F10B0 preamble (same opt-ins minus RANDOM_SHUFFLE) + real
  GetCardDef 0x008F0B20 and CCard::Move 0x008F0660 bodies in the TU; ONE loop summing both X and Y (two fld 0.0); named CCharString
  in its own `{}` block (destroyed before the inlined Move); explicit `return;` after SetState(0x1a) in the state-0x19 branch
  (gives retail's separate ebp release). Flags: none. Headers: no overlay needed (repo headers as they are).
- Gotcha: with FABLE_RETAIL_CCHARSTRING the ctor has no default length -> write `, -1`.

## 0x00929340


## 0x00929340 CAIStateGroup_BanditPlayDice::Update -- RELOCATION_MATCH (wf10, ~20 grades)
- What made it match: port onto shared headers (CAIStateGroupBase MEMBERS/GETCREATURE/MEALACTIONS/PERFORMACTION, CAIBrain
  GETCREATURE+CREATURE_REFERENCE, CThing chain, CTCPhysicsBase GetRHSet, CTCCarrying, CCombatActionBase MELEE); LookAt = named
  `const CRightHandedSet& rh = ...GetRHSet();` then `GetPos() + rh.GetForward()`; ++players via Get/SetNumberOfPlayers, --players
  direct; Unsheathe failure as early `Terminate(); return;` then `WasCarryingWeapon = false;`; entry test through a local
  `CTCExclusiveUse* position = DiceGamePosition;` (gives retail's `mov eax ; lea ecx,[eax+0Ch]`).
- Headers: overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS (slots 12..16 before Act) and
  FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS (polymorphic class, sizeof 0x118); see hdr/README.md. No flags.
- Local models (no engine header exists): CTCExclusiveUse, CTCDiceGame, CCreatureAction_SheatheWeapons.
- Name note: EAIActionRepeat 1 is ACTION_REPEAT_UNTIL_FINISHED (fable_ai_action_enums.h); the old draft/siblings' "DiceMethods"
  stand-in and AI_ACTION_DONE are not PDB names. Landed PutWeaponAway row 0x008FE7D5 uses leaf
  ?PutWeaponAway@CAIStateGroup_BanditPlayDice_Methods@@QAE_NXZ; PDB is ?PutWeaponAway@CAIStateGroup_BanditPlayDice@@MAE_NXZ.
  Landed GetDiceGame 0x00929220 leaf says QAE; PDB is MAE (protected virtual, slot 14).

## 0x009F4390

## 0x009F4390 CSurface::CopyFromMemory -- RELOCATION_MATCH (wf10, ~14 grades)
- What made it match: the extents are four scalars written as CONDITIONAL EXPRESSIONS per field (`long destWidth = destRect ? BRX-TLX : GetWidth();`
  ... `long sourceHeight = sourceRect ? BRY-TLY : destHeight;`). VC7.1 merges the paired tests, homes the widths in the dead destRect/sourceRect
  arg slots and gives ebp to sourceFormat. Every if/else spelling (scalars or C2DExtentsI) gave a width a register (frame 0x38/0x50, 493..511 bytes).
- Dossier correction: the null-sourceRect arm DOES copy the height (srcH = dstH): 0x009F44A3 jumps back to 0x009F4409 and the shared
  `mov [esp+14h],eax` stores eax (= dstH on that path). Tail merge, not a half-assigned extents.
- Headers: overlay CPixelFormat.h (_SIGNED), CSurface.h (_DIMENSIONS_INLINE, _COPYFROMMEMORY_BODY) + FABLE_RETAIL_C2DBOXI; no flags/pragmas.
- Names: callee overload 0x009F3B00 is ?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0J@Z (draft's CopyFromMemoryDirect was wrong);
  0x009E37F0 is IsEquivalent (draft's operator== was wrong). Landed CopyFromUnsignedDataToSignedSurface 0x009F36E0 spells
  (const unsigned char*, unsigned long) vs PDB (const void*, const CPixelFormat&, long) -- see dossier, NAME_FIXES candidate.

## 0x00641C30 (not landed)


## 0x00641C30 CTCLight::OnAppearanceDraw -- DIFFER(502v490), 3 hunks (author pass 1, ~60 grades)
Best: differ/00641c30.cpp (shared headers CThing/CTCLight/CTCPhysicsBase/CTCGraphicAppearance; CGraphicAppearanceDrawInfo modelled
locally from struct_layouts_egor because no shared header exists). No overlay headers needed. Flags: none.
Residue: the fallback DrawLight(0, ...) argument. Ours stores the merged ecx/edx/eax twice (the promoted local `pos` at [esp+0x14] and
the argument temp C3DVector(pos.X,pos.Y,pos.Z) at [esp+0x38]); retail stores once at [esp+0x14] and passes that.
What got it from 25 hunks to 3:
- one helper-position local for BOTH loops, declared in the if(Active) block before the first loop (gives retail's slot [esp+0x2c]);
- class-2 arm `C3DVector p=Phys->GetPosition(t)+Phys->GetFacingVector(t)*0.35f; pos=C3DVector(p.X,p.Y,p.Z+1.0f);`
  (equivalently `pos=P+F*0.35f+C3DVector(0,0,1)`; VC folds the +0s), else `pos=GetParentThing().GetPos();`;
- passing a COPY built from pos (C3DVector(pos.X,pos.Y,pos.Z) or C3DVector(pos)) -> VC promotes pos, loads in the arms, stores after
  the merge (retail's register merge). Passing pos itself makes VC store inside the arms (frame 0x4c).
Ideas tried (all worse): ternary forms (init / assign / arg / const-ref / both arms explicit copies) -> address merge (lea eax temp);
inverted condition; two DrawLight calls (per-arm locals, cross-jump hope); float x,y,z locals (FP loads instead of int moves);
field-wise fills; C3DVector copy-ctor variants (CTCTARGETINGAI_ATTACK), ADD/SUBTRACT_XY_GROUP; pos scope moves; `pos.Z+=1`;
offset as named local; zero-init pos.
Best next idea: find the spelling that gives a promoted phi with NO home stores -- e.g. the argument object itself being the only
C3DVector (something VC promotes without materialising: a by-value temporary returned from an inline accessor, or a C3DVector
declared without the user default ctor path); or check whether a later QFE cl removes the dead `pos` stores (if so it is a
toolchain residue).
Names proven (DOSSIER, checked): GetNoHelperPoints (not GetNoDummyObjects), CThing::GetPos (not GetPosition), GetFacingVector(float)
slot 69, MESH_LIGHT_POINT_NAME / MESH_FLARE_POINT_NAME. FableWin.pdb shows no helper function between OnAppearanceDraw (0x0222A240)
and DrawLight (0x0222A550), so no CTCLight inline helper (GetLightPosition-style) existed.

## 0x00641C30 -- author pass 2 (~25 grades): ROOT CAUSE FOUND = callee-body visibility; blocked on DrawLight 0x006414A0
- PROBE RESULT (v2/pr1.cpp): the natural source (`C3DVector pos; if(type==2){p=...; pos=C3DVector(p.X,p.Y,p.Z+1.0f);} else pos=GetPos();
  DrawLight(0,pos,...)`) grades RELOCATION_MATCH when CTCLight::DrawLight has a VISIBLE, non-escaping body earlier in the TU
  (`#pragma auto_inline(off)`). With a visible body that passes &pos on (v2/pr3.cpp) it is back to 24 hunks / frame 0x4c. So VC7.1
  checks whether the callee lets the reference escape; retail DrawLight only copies pos ([esp+0x78] read once at 0x00641A3B into the
  CEnginePrimitiveLight it builds). Retail CTCLight.cpp order: DrawLight, DrawFlare, OnAppearanceDraw.
- The probe body is a STAND-IN, so nothing is landed. To land: reconstruct DrawLight 0x006414A0 (1596 bytes; GetFadeDistance 0x4bc180,
  Lights vector resize 0x6426a0, CPrimitive 0x99a2d0/dtor 0x4c4160, AddPersistentPrimitive 0x4c0050, 0x686a80 x5, vtbl+0x118/+0x10,
  0xbfea70 x4, SetPrimitiveGraphics 0x4bd620, global counter 0x013BA85C++), put its real body before OnAppearanceDraw out of line, and
  the row matches as written in differ/00641c30.cpp (pass pos directly). Disassembly saved: $W/drawlight.dis.
- Also learned: with an implicit (compiler-generated) C3DVector copy ctor, `DrawLight(0,C3DVector(pos))` drops the dead pos home stores
  (490 bytes) but slots/esi-edi swap differ (30 hunks) -- a dead end; FableWin.pdb shows a user copy ctor ??0C3DVector@@QAE@ABV0@@Z.
  Tried and worse: inline by-value helper (not inlined), two DrawLight calls with per-arm locals (frame 0x4c), ternaries in implicit
  world, named arg locals, pass via ref/ptr/cast/inline identity, /Oa, /Ob2, QFE cl (no change).
- Previous 3-hunk copy-arg spelling kept as differ/best_3hunk_copyarg.cpp. No header overlay needed. Flags: none.
- Best next idea: reconstruct DrawLight (a separate row) first; then this row lands with it in the same TU.

## 0x00641C30 -- author pass 3: RELOCATION_MATCH (strict headers too), test PASS, addmatch done
- What made it match: CTCLight::DrawLight 0x006414A0 reconstructed from its disassembly ($W/drawlight.dis) with its real structure and put
  out of line (#pragma auto_inline(off)) before OnAppearanceDraw in the TU, as retail CTCLight.cpp orders them. OnAppearanceDraw is the
  natural pass-2 source unchanged (one pos local, if/else, DrawLight(0,pos,...)). VC7.1 sees DrawLight never lets &pos escape.
- DrawLight body is a faithful reconstruction but NOT byte-verified (no oracle registered for 0x006414A0; fg.py: "no oracle"). It should
  become its own row next; it already sits in this TU. Its pieces: GetFadeDistance (unused result), Lights.resize + AddPersistentPrimitive
  (CPrimitive type 3, as DrawFlare), CRGBFloatColour(GetColour()), flicker = (1-t)*OldAccumilativeFlickerFactor + AccumilativeFlickerFactor*t,
  DaylightAttenuator: 4 fade hours *(1/24) vs CGameTimeManager::GetTime with a 3-stage wraparound search then switch(stage) {fraction, 1, 1-fraction, 0};
  SunlightAttenuator: XY facing (GetFacingVector() slot 70) . XY -sun vector (PeekSunVector slot 4), both fast-Normalised, mapped by
  (dot-FadeStart)/(FadeEnd-FadeStart), clamped [0,1], colour = env.PeekDiffuseColour(); IsInverted -> 1-factor; *flicker; colour*factor;
  radius = GetOuterRadius()*factor, spot = GetInnerRadius()*factor; mask switch(EffectChannel){0: 1<<PeekLightingChannel().ChannelIndex0,
  1: ~1, 2: 1, 3: ~0}; SetPrimitiveGraphics(Lights[i], CEnginePrimitiveLight(0,pos,colour,...,ENGINE_INTERPOLATION_SET_0,mask),0); ++counter.
- Headers: overlay hdr/ (4 opt-ins, see hdr/README.md). Local models in the row: CGraphicAppearanceDrawInfo (no shared header) and
  CTCLight::CLightInfo (nested, Ego_r 24 bytes: Colour, InnerRadius, OuterRadius, Flicker, LightingChannelsEffected, Inverted).
  Inline CTCLight::GetColour/GetInnerRadius/GetOuterRadius/IsInverted bodies (Overridden ? PLightInfo->X : PLightDef->X) defined in the row.
  Flags: none.
- Made-up name (not found in PDB name sets / signatures): GNoLightsDrawn = global 0x013BA85C (zeroed by CDisplayEngine::DoRender
  0x004355BB, incremented once per DrawLight). PeekDiffuseColour's body (GameTickState+4 = BlendedTheme[0].Colours[0]) is inferred from
  the retail 16-byte copy at env+0x58; the colour index name is unproven.

## 0x00641C30 -- repair pass (verifier findings 1-6): MATCH WITHDRAWN, row is DIFFER(502v490), blocked on CTCLight::DrawLight 0x006414A0
- All six findings are real and share one cause: the pass-3 RELOCATION_MATCH needed a full body of CTCLight::DrawLight 0x006414A0 in the TU,
  and that body is not retail (1612 bytes vs 1596, mnemonic similarity 0.894, never byte-verified), the address is reserved by
  2026-11-11_acctB (rebuild/candidates/manual/pending/2026-11-11_acctB/RESERVED.tsv line 50), and it would collide with DrawLight's own
  row at landing. The brief forbids a stand-in body, so the match cannot be kept: without a visible non-escaping DrawLight body VC7.1
  treats &pos as escaping and the frame/store pattern differs (pass 2 probe, ~85 grades over passes 1-2 on other spellings).
- Done: payload.json / oracle.tsv emptied (no matched row); the 4-header overlay (CEnginePrimitiveLight, CEnvironment, CGameTimeManager,
  NGraphicAppearance_CTCGraphicAppearance opt-ins -- they existed only for the DrawLight body; PeekDiffuseColour index unproven) is
  withdrawn. The pass-3 TU, test, overlay and old payload are kept for reference ONLY in blocked_not_for_landing/ (do not land).
- differ/00641c30.cpp = the copy-arg spelling, no overlay, no DrawLight body: DIFFER(502v490), 3 hunks (regraded this pass). Residue:
  the fallback DrawLight(0, C3DVector(pos.X,pos.Y,pos.Z), ...) stores pos at [esp+0x14] AND into an arg temp at [esp+0x38]; retail stores once.
- Way to land: (1) DrawLight 0x006414A0 is byte-matched and landed as its own row (acctB's reservation); (2) this row then goes into the
  same TU after it (retail CTCLight.cpp order DrawLight, DrawFlare, OnAppearanceDraw), with the natural source (`DrawLight(0,pos,...)`,
  see blocked_not_for_landing/tu_with_UNVERIFIED_DrawLight_body.cpp minus the DrawLight body), sharing DrawLight's out-of-line real body
  under #pragma auto_inline(off). The pass-3 DrawLight reconstruction can seed that row but must be byte-verified first.

