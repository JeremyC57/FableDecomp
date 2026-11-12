## 0x007F0F40


## 0x007F0F40 CTCKrakenTentacle::ReturnToDesiredPos -- RELOCATION_MATCH (author wf13, 9 grades)
- Made it match: port onto shared headers (CThing/CIntelligentPointer/ThingAlive/C3DVector/CRightHandedSet/CTCPhysicsBase/
  NGraphicAppearance_CTCGraphicAppearance/CKrakenTentacleDef/GFAngles); `C3DVector dir; dir = desired - current;` (assignment, not copy-init)
  fixed the length; splitting the kraken facing into its own local `krakenAngle` and `float angle = GFGetSignedAngleDifference(facing, krakenAngle);`
  fixed the dist/angle frame-slot swap (decl position of one `angle` variable did nothing); IsNonZero in `squared +=` Z,Y,X form fixed the last Y-load hoist.
- Headers/flags: overlay hdr/engine/C3DVector.h opt-ins FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE (declaration) and
  FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX_SQUARED; default flags; strict-headers regrade RELOCATION_MATCH. Local CTCKrakenTentacle model
  (no shared header); PTentacleDef modeled as a raw def pointer word (CDefPointer layout-equivalent). Name fixes: none found (draft's
  GetPosition/Normalise/AngleDifference spellings were already corrected in the dossier; landed names used).

## 0x006B08F0

## 0x006B08F0 CTCPhysicsBase::OnSerialise -- DIFFER(565v565), 6 hunks (author attempt 1, ~25 grades)
Best: differ/006b08f0.cpp (= base3.cpp). Overlay: hdr/engine/CWorld.h (FABLE_RETAIL_CWORLD_SAVEGAMEMARKERPOS: inline const C3DVector&
GetSaveGameMarkerPos() const, FableWin.pdb), hdr/engine/C3DVector.h (ISNONZERO_REFS_XYZ). Ported to shared headers, PDB names
(PeekWorldMap/PeekWorld, DELTA_NOTIONAL_ZERO via IsNonZero, SetCurrentMapNumber(long), 2-arg Transfer + GetDefaultVal<float>).
Fixed: IsNonZero FP order (REFS_XYZ or REFS_YXZ both match; one-line/paren/accum spellings all wrong), final
`GetParentThing().SetCurrentMapNumber(...)` (ParentThing loaded into esi before the vcall, as retail).
Residue: region compare right side `ParentThing->GetCurrentMapNumber()` -- ours `mov edx,[esi+4]` hoisted above `mov ebx,eax`,
retail loads eax after it; and the `Position = marker` copy register (ecx vs eax pointer). The two move together: right side via
GetParentThing() fixes the copy but CSEs ParentThing into ebx/ebp (28-31 hunks).
Tried (no gain): named region locals (one/two/const), bool sameRegion, markerMap local, thingMap local, swapped ==, !(!=),
early returns, && combined flags/IsNonZero, const marker, marker at function top, thing ptr/ref locals, GetParentThing in
flags/version/right combos, GetPosition() arg, C3DVector copy-ctor body form / explicit operator= (worse), /G5 /G7, QFE toolset,
direct CurrentMapNumber probe. FP load order shifts with unrelated region edits (fragile).
Next idea: a different whole-function IsNonZero+region combination (e.g. default IsNonZero with a named region local), or
a packed-C3DVector CWorld member instead of the cast accessor (changes the alias view of the +0x141 reads).
Landed name to fix: 00/4c CThing_SetCurrentMapNumber (old fastcall stub) -> ?SetCurrentMapNumber@CThing@@QAEXJ@Z (per DOSSIER).

## 0x006B08F0 CTCPhysicsBase::OnSerialise -- RELOCATION_MATCH (author attempt 2, ~75 grades)
What made it match: the region test's right side reads the parent through a CONST reference local taken after the marker's region:
`long markerRegion = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(marker)); const CThing& thing = GetParentThing();
if (markerRegion == PeekWorldMap().GetRegionNumberMapIsIn(thing.GetCurrentMapNumber()))` (the const GetParentThing() overload,
FableWin.pdb ?GetParentThing@CTCBase@@QBEABVCThing@@XZ). That fixed BOTH residue hunks at once (region ParentThing load in eax after
`mov ebx,eax`, and the `Position = marker` copy through eax). Without the region local (h1) it is 29 hunks; non-const ref / pointer /
GetParentThing() inline in the expression all fail. IsNonZero: repo C3DVector.h FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ (one-line
X*X+Y*Y+Z*Z) -- the attempt-1 REFS_XYZ overlay opt-in is not needed (dropped). Final GetParentThing().SetCurrentMapNumber stays.
Headers/flags: overlay hdr/engine/CWorld.h (FABLE_RETAIL_CWORLD_SAVEGAMEMARKERPOS accessor); defines as in final.cpp; Flags: none.
Tried this attempt without gain (all same 6-hunk residue): CWorld SaveGameMarkerPos as a packed C3DVector member (+FABLE_RETAIL_C3DVECTOR_PACKED,
direct member read), Position reference locals, early-return structures x six IsNonZero bodies, long-lived/unsigned/bool region
locals, thingMap long/short locals, single shared origin var (worse), version/save-arm arithmetic spellings, final statement forms
(mapNumber local, CThing&/CThing* locals, assignment inside GetMapNumberAt arg, explicit operator= call), flags /Ob1 /Oa /Ow /GX /Ox
/G6 /GB /Gr /Zi /Gs (/Op worse). Probes: residue was local to the tail (persisted without version block, save arm, copy, final, IsNonZero).
Landed name to fix (from attempt 1): 00/4c CThing_SetCurrentMapNumber old fastcall stub -> ?SetCurrentMapNumber@CThing@@QAEXJ@Z.
  Sources: Ego_r.pdb (egor_signatures.tsv 0x004C9140) and FableWin.pdb (FableWin.exe.signatures.tsv 0x01CF8A20) both name
  ?SetCurrentMapNumber@CThing@@QAEXJ@Z; repo CThing.h:463 maps it to 0x004C7580. Handed back in NAME_FIXES.tsv.

## Repair (verifier findings)
- GetDefaultVal<float> specialisation now carries its retail proof (default 0 stored as `mov dword ptr [esp+0x34],0` before each
  `call 0x410620` at 0x006B097F/0x006B099D/0x006B09BB) and names the landed rows it copies (00b13530, 00761c00). Still RELOCATION_MATCH (also strict headers).
- NAME_FIXES.tsv written for 0x004C7580 with the Ego_r.pdb + FableWin.pdb sources.

## 0x00731EA0

## 0x00731EA0 CTCSpotLight::OnAppearanceDraw -- RELOCATION_MATCH (final.cpp, default flags, strict headers OK, test PASS)
- What made it match: (1) port onto shared headers with a real two-base CTCSpotLight (overlay opt-in) and the real callee identities
  (AddPersistentPrimitive(CPrimitive(3)), ~CPrimitive, CEnginePrimitiveShadowedSpotLight ctor 0x7324A0, SetPrimitiveGraphics(const&));
  (2) the spot-light primitive as a NAMED local (a temporary in the SetPrimitiveGraphics call pushes its `0` first);
  (3) the inline accessors spelled `if (Overridden) return a; else return b;` (the ternary spelling swaps esi/edi -- component vs full this);
  (4) owner position held as `const C3DVector& pos = GetParentThing().GetPos();` and copied in the argument `C3DVector(pos)` (a named
  copy or GetPosition() by value gives a different frame / schedule); (5) `const CThing& thing = GetParentThing();` AFTER the GetPos call
  and `thing.GetUniqueID()` in the args (inline GetParentThing().GetUniqueID() in the args schedules the id loads after the pos stores).
- Headers: overlay engine/CTCSpotLight.h (FABLE_RETAIL_CTCSPOTLIGHT_BASES) + engine/CSpotLightDef.h (FABLE_RETAIL_CSPOTLIGHTDEF); flags none.
- Local declarations (no shared header exists): CGraphicAppearanceDrawInfo {Interpolate}, CEnginePrimitiveShadowedSpotLight (PDB layout).
- Name fixes: 0x007324A0 is ??0CEnginePrimitiveShadowedSpotLight@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MMMW4EEngineInterpolationSet@@K@Z
  (landed as CEnginePrimitiveLine::CEnginePrimitiveLine; Ego_r 0x0074A620 + FableWin 0x02581D70, size 0x38 and member stores match);
  0x004C4160 is ??1CPrimitive@NGraphicAppearance@@QAE@XZ (landed as made-up SPrimitiveHandleHolder_004c4160 dtor; FableWin 0x017C5E2F).

## 0x009341C0


## 0x009341C0 CAIStateGroup_MinionKidGroupTag::TagTarget -- RELOCATION_MATCH (wf13 author, ~25 grades + 12 probes)
- What made it match: (1) port onto shared headers (class on CAIStateGroupBase via new overlay opt-in, PDB callee types/enums);
  (2) single-exit nested positive ifs -- probes showed ANY extra early `return`/branch pushes VC7.1 past its caller inline budget and
  the 2nd by-value CIntelligentPointer<const CThing>(T*) ctor goes out of line; (3) reach test in plain floats
  (`C2DVector targetPos(..PeekPos()); C2DVector here(..PeekPos()); float dx, dy; float reach; if (dy*dy + dx*dx <= reach*reach)`)
  -- an 8-byte C2DVector delta (named or temporary) costs 4 frame bytes (0xc8 v 0xc4); (4) fps read inside the comparison
  (`> 2.0f * CMainGameComponent::GetConstantFPS()`) so TaggedFrame is loaded first; (5) FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
  (FABLE_FORCEINLINE on the (T*) ctor, same idea as the landed FABLE_RETAIL_C2DVECTOR_FORCEINLINE_CTOR) -- still needed even
  single-exit; without it the 2nd site is a call (v45: 564v580).
- Headers: overlay hdr/engine/CAIStateGroup_MinionKidGroupTag.h (FABLE_RETAIL_CAISTATEGROUP_MINIONKIDGROUPTAG) and
  hdr/engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR). No flags. CTCCreatureGroupTag has no shared
  header: local model on retail CTCCreatureGroup (FABLE_RETAIL_CTCBOASTINGCROWDCONTROL) with SetTagger(const CThingAICreature*) and
  inline GetTaggedFrame() (FableWin ?GetTaggedFrame@CTCCreatureGroupTag@@QBEKXZ).
- Coordinator: the FORCEINLINE opt-in is a macro in a shared header (FABLE_FORCEINLINE), not a bare __forceinline in the row; review
  against ASM_RULES/owner rules before landing. NAME_FIXES candidate (dossier): landed SetTagger 0x008137B0 leaf takes `const CThing*`
  but Ego_r + FableWin spell ?SetTagger@CTCCreatureGroupTag@@QAE_NPBVCThingAICreature@@@Z (this row's mangled call is masked as reloc).

## 0x00754050

## 0x00754050 CTCLook::UpdateTurningAnims -- RELOCATION_MATCH (strict headers), 13 grades
- Port onto shared headers (CTCLook.h with the OnInitialActivate opt-in set + GETTC_EXPANDED, CTCCreatureModeManager, CAppearanceDef::Animation,
  CAnimFilterNoGroup, STLport vector with CLEAR_OUTOFLINE + OVERFLOW_DECL_ONLY) got to 3 hunks; `size=fabs(rotation)` before the
  `0.5f-DELTA_NOTIONAL_ZERO` limit fixed the fld order. Last residue (push_back fast path reloading pair.second from the temp instead of
  edi, + slow-path lea register swap) was fixed only by FABLE_RETAIL_STLP_CONSTRUCT_BYVAL (_Construct(T*, U v) by value).
  Not it: make_pair, named pair, *it direct, implicit pair copy ctor, no converting ctor, ::new(void*), fastcall _Construct.
- Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CMODEBASE_DEFAULTANIMFILTER, FABLE_RETAIL_CANIMATIONENTRY_GETROTATION,
  FABLE_RETAIL_STLP_CONSTRUCT_BYVAL. No flags. Coordinator: the by-value _Construct is a byte-proven form, not a verified STLport 4.x
  spelling -- check whether other inline push_back rows of pair elements agree before generalising.
- NAME_FIX (dossier): 0x00755350 landed leaf uses placeholder element CUnprovenElement_00755350; this row proves it is
  std::pair<float,const CAnimationEntry*> (vector<pair<float,const CAnimationEntry*> >::_M_insert_overflow, __false_type).

## 0x00951D60


## 0x00951D60 CAIStateGroup_JackDragon::IntoHover -- RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1; repaired after verifier review)
- What made it match: the OnActivate (0x009500E0) shape (same-TU noinline GetSubDef<CJackDragonDef> real body, CCountedPointer
  VISIBLE_CALLED_CTOR/_DTOR, SetFacingAngleYZ(0.0f), GetFacingAngleXY, RotateXY, GetPos) plus two source facts:
  (1) marker select `GoLeft ? PHoverMarker1.Get() : PHoverMarker2.Get()`; (2) the route point summed per component in a block
  (retail stores X, Y, Z in order; C3DVector::operator+ evaluates its ctor args Z first).
- Headers / flags: no flags. Needs the overlay `FABLE_HDR_OVERLAY=$W/hdr` (opt-ins FABLE_RETAIL_CJACKDRAGONDEF and
  FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON, hdr/README.md). Everything else is shared headers: CAIStateGroupBase (+_MEMBERS,
  _GETCREATURE), CAIBrain (_GETCREATURE; GetCreature() returns CThingAICreature& as both PDBs), CTCPhysicsBase (PDB virtuals),
  CThing / CThingBaseDef / CThingCreatureBase / CThingAICreature, CDefinitionManager, CCreatureActionBase / CCombatActionBase,
  stlport/fable_containers.h std::vector<C3DVector> with FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE, ThingAlive.h
  IS_THING_ALIVE<T> with FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT.
- Callee spellings emitted (checked in the .obj): push_back = landed 0x0066E630 leaf
  ?push_back@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXABVC3DVector@@@Z; StartFlying / IsStillRunnable = the PDB
  spellings (private AAE). The landed 0x00950C30 / 0x0094FDF0 leaves spell them QAE (and StartFlying with made-up parameter types):
  NAME_FIXES.tsv carries both (StartFlying: Ego_r egor_signatures 00975720 + FableWin _fablewin_api 017a1d4a / 02be4110;
  IsStillRunnable: Ego_r 009747b0 + FableWin 017c1e60 / 02bd9900).
- Only local class: CCreatureAction_DragonHoverInto (no shared header): : CCombatActionBase (RTTI), ctor
  ??0CCreatureAction_DragonHoverInto@@QAE@AAVCThingCreatureBase@@@Z (Ego_r), size 0x114 = CCombatActionBase (`push 114h`).
- Made-up names (DESCRIPTIVE, found in neither PDB, FSE, EgoCore or strings): the EDragonState enumerators in the overlay
  (DRAGON_STATE_INTO_GROUND 0 / _GROUND 1 / _INTO_HOVER 2 / _HOVER 3 / _STRAFE 4 / _INTO_GROUND_SWOOP 5, named from the Update
  0x00952E80 switch; the landed OnActivate row's local `DRAGON_STATE_1` is the same value 1 under a placeholder spelling);
  locals hoverPos, pos, offset, back, marker, route, action. The made-up `CVector3DList` stand-in is gone (real std::vector<C3DVector>).
- Verifier findings 1-11: all applied (local models of CJackDragonDef / CDefinitionManager / CTCPhysicsBase / CThingBaseDef /
  CThingCreatureBase / CCreatureActionBase / CAIBrain / CAIStateGroupBase / CAIStateGroup_JackDragon replaced by shared headers or
  overlay opt-ins; PEnemy is CIntelligentPointer<CThingCreatureBase>; GetCreature is the PDB protected const CThingAICreature&).
  Verifier findings not applied: none. The pre-repair source is kept as final_prerepair.cpp.

## 0x009CCEF0


## 0x009CCEF0 CBankFile::FindIndicesByFilename -- RELOCATION_MATCH (wf13, 2 grades)
- Matched on the first port: shared headers (CBankFile / CArray / CPackedUIntArray / fable_string / fable_map) with typed members,
  const multimap equal_range over the out-of-line tree bounds (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE), inline GetEntrySymbol /
  GetEntryDataType (landed bodies, GetSize() test), `!symbol.StartsWith("UNASSIGNED")`, `return indices;` (CArray copy). No flags.
- Overlay opt-ins: FABLE_RETAIL_CBANKFILE_FILENAMEINDICES (engine/CBankFile.h), FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE (fable_string.h),
  FABLE_RETAIL_STLP_MULTIMAP_EQUAL_RANGE (stlport/fable_map.h); see hdr/README.md.
- Name fix: 0x0099ED10 is ?GetAsLowerCase@CCharString@@QBE?AV1@XZ (Ego_r 00afa690, FableWin 02f61580), landed/draft spelling "ToLower" is wrong.
  Param names (type, excludeUnassigned) are descriptive (not in any PDB dump).

## 0x00492973 (not landed)

## 0x00492973 CSubtitleRenderer::SetText (full 4-caption wide overload) -- RELOCATION_MATCH (15 grades)
- What made it match: the tail (found list in the dead text slot +0xC, filter +0x14, unrotated removal loop with the sentinel in edi)
  is callee-body visibility: the TU's COMDATs _List_base<const CMessageEvent*> ctor 0x00493BC2 / dtor 0x00493BDD, RemoveMessage
  0x00493B5B and FindAllMessages<IsOfType> 0x00493EAB get their real (STLport / landed) bodies in the row under auto_inline(off).
  RemoveMessage + FindAllMessages bodies alone did nothing; adding the list base ctor/dtor bodies matched at once.
- Then ported onto shared headers (no local class models); strict-header regrade RELOCATION_MATCH. Pragmas y-off + s-on, no flags.
- Overlay opt-ins (hdr/README.md): CSUBTITLERENDERER_SETTEXTINFO, CMESSAGEEVENTMANAGER_FINDALL, CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR,
  CTCHERORECEIVEITEMS_DISPLAYINGITEMS, STLP_DEQUE_PUSH_BACK_OUTOFLINE, STLP_QUEUE.
- Names: message type 0x11 is spelled with the shared header's enumerator MESSAGE_EVENT_CONVERSATION_CLICK_PAST
  (engine/EMessageEventType.h, generated from the Ego_r enum, value 17). No rename of landed code is handed back (see below). Hero component byte +0x26 is CTCHeroReceiveItems::DisplayingItems
  (Ego_r +0x32). Callee names from the dossier: GetPControlledCreature, InitPlayerMode, CFontBank::Preload.
- The 0x00493BC2 / 0x00493BDD COMDATs are modelled here by the STLport _List_base<const CMessageEvent*> ctor/dtor bodies
  (shape only: one allocator& arg, `and [out],0`, allocate + self-link the sentinel). Their retail symbol names are NOT proven.

## Verifier findings (repair pass)
- Withdrawn: the claim that landed global_ListNodeCreateCircularSentinel_00493bc2 / std_list_T_00493bdd_dtor_00493bdd are
  ??0/??1?$_List_base@PBVCMessageEvent@@...  Neither symbol is in egor_pdb_names.tsv or fablewin_pdb_names.tsv (both PDBs are
  Dinkumware builds listing only ??0/??1?$list@PBVCMessageEvent@@...); the only support is STLport code shape, one source.
  No NAME_FIXES row is handed back for them; the bodies in this row are the STLport header's own, so the byte match stands.
- Withdrawn: NAME_FIXES row 0x11 -> MESSAGE_EVENT_CONVERSATION_CLICK_PAST (replacing Clear 0x00492BAB's
  MESSAGE_EVENT_TEXT_FINISHED_2). The name has one source only (rebuild/include/engine/EMessageEventType.h, from the Ego_r enum);
  a repo-wide grep finds it nowhere else (no FableWin extract, fable_types.h, FSE). This row still uses the shared header's
  enumerator (that is the header's name, not a new one); the coordinator may reconcile Clear when a second source appears.
  NAME_FIXES.tsv now holds the header line only.

## 0x00918640 (not landed)


## 0x00918640 CAIStateGroup_Gaze::FindFreeParticularGazePos -- author pass 1 (wf13): DIFFER(575v570), 5 hunks, ~116 grades
Best source: differ/00918640.cpp (= v/w1.cpp). Ported onto shared headers (the 0x00931200 MinionReactToCriminal include set +
CTCThingOwner/CThingMarker/GFRandom.h), no overlay, no flags. Leaf ?FindFreeParticularGazePos@CAIStateGroup_Gaze@@IAEPAVCThingGameObject@@J_N@Z.
What fixed the body (from 20 hunks to 5):
- plain std::vector (no push_back opt-in): VC7.1 keeps the header push_back out of line by itself (call 0x00436C80);
  FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE (decl only) folds the free test badly; NOINLINE = same as plain.
- CThingFilter_IsNotBeingExclusivelyUsed derives std::unary_function<CThing,bool> (as every other CThingFilter_* header) and
  is used as a TEMPORARY `CThingFilter_IsNotBeingExclusivelyUsed()(*it->Get())`: puts it at [esp+13h] / the bool param slot
  like retail; without the base the temp is zero-filled (POD value-init), a named local takes a dword slot.
- nested `if (GetTC(...)) { ... }` (single exit) instead of early returns: moves `this` into ebx and the zero into esi.
- tail `if (found.size() > 0) return found[GFRandom(found.size(), GetPWorldSeed())]; else return 0;` keeps retail's
  `test edi,edi ; je` inside the random (any count local / `!= 0` / bare test lets VC fold it AND promotes _M_start
  globally into ebx, +31 hunks).
Residue: retail keeps _M_start in ebp from the join (0x0091880F) across GetPWorldSeed + GFROR13 (index read + both free
tests); ours reads it as a memory operand and reloads after the calls.
Tried without effect on the residue: count local (before/after guard, at function top, signed long), size()==0/<=0/empty()/
!=0/>=1 guards, ternary, result/index/seed/iterator locals (`it = begin(); it += n`, Q31 lesson), reference alias, inline
helper (probe), random as inline member (probe), WorldRandomRange spellings (ternary, result local, ==0 / !=0), vector<CThing*>,
unguarded ~_Vector_base (overlay probe, removed), declaration order of locals, vector after GetOwner, IS_THING_ALIVE,
pragma optimize t/g/s, /G5 /G7 /Ob1 /Oa /Ow.
Probes: making GetPWorldSeed's body visible alone: no change; making GFROR13's real (landed, GF_ROR13 asm macro) body visible
out of line (gr2.cpp) DOES keep _M_start in a register across the calls (edi) but VC then also knows GFROR13's registers
(seed ptr kept in edx, count esi) -> 13 hunks; as `inline` (gr3) it is treated as opaque again.
Best next idea: find what tells retail's c2 that GFROR13 / the `*seed` stores cannot touch the local vector while still
treating GFROR13 as a register-clobbering call -- e.g. GFROR13 declared in the retail header in a form c2 summarises
(a different declaration, __declspec, or the GFRandom.h / CWorld seed header the TU really included), or the vector reached
through a different owner. Also worth one try: Ego_r's real GFRandom body (?GFRandom@@YIKKPAK@Z @ Ego_r 0x004183C0) spelled
exactly, with its own seed local.
Names: none of the landed spellings proven wrong beyond the dossier's (FindFreeGazePos slot 15 = InsideAtNight).

## 0x00918640 -- author pass 2 (wf13, attempt 2): still DIFFER(575v570), 5 hunks, ~75 grades/probes (variants in v2/)
Best source unchanged: differ/00918640.cpp (no overlay; the probe overlay hdr/stlport was removed again).
Residue (same as pass 1): retail holds the vector's _M_start in ebp from the tail join 0x0091880F across GetPWorldSeed + GFROR13
(index read, both dtor tests/free args); ours rematerialises it from [esp+1Ch] (sub mem operand, then `mov eax,[esp+1Ch]` twice).
What pass 2 established (probes, read before trying more tail spellings):
- It is NOT aliasing/escape: even on the zero path (no call between) ours reloads; a source local `first = found.begin()` (w1) is
  rematerialised from memory too. It is a c2 register-allocation decision about _M_start as a candidate.
- Whenever _M_start DOES get a register in our compiles (p1/p2/p3 no range test; i4/v3 count local -> inner test folded; gr2
  GFROR13 body known; t3/t7/t9 IsPointInside clause removed; t1 village branch removed), it is a WHOLE-FUNCTION variable whose
  register also carries the zero used by the ctor stores (`xor ebx/ebp/edi,reg`) -- retail instead zero-fills from esi and has
  _M_start only at the tail (ebp), i.e. a split live range. Decisive probes: t9 (GetPos kept, building not used in the loop) ->
  ebp promoted; t8 (building->IsPointInside kept, GetPos removed) -> not. So the competitor is `building` in ebp over the
  init->loop region; ours cannot split _M_start around it, retail did.
Tried in pass 2 with no effect (all 575v570 / 5 hunks unless noted): GFROR13 declared __declspec(nothrow) / throw();
flags /EHsc /GX- /Gf /Ox /Oi /Gr /GR /J /Zi /Op (/GX 21 hunks, /O1 23); /d2inlS sweep 39..200 (<=80 and 175+: 31/48 hunks);
QFE-4035 compiler via FABLE_VC71; explicit instantiation of vector<CThingGameObject*> / push_back before the row; earlier
noinline functions instantiating LowerBound/push_back (z1-z3); push_back replaced by a known non-capturing noinline fn (y1,
probe); explicit LCG expansions in the tail (s1-s3, sl1-sl3 own seed/result locals, Ego_r-style); GFRandom as FABLE_FORCEINLINE
with ternary / if / *= += (f1-f3); count local on one side only (v1/v2 same, v3 folds -> 31); begin local / begin()+index /
end()-begin() range (w1-w3); tail inside while(true)/for(;;)/do-while(false) (wl1-3); vector in an inner block (bk1/bk2);
result=0 local; gazeType compare signedness; vector ctor forms (copy-init, explicit allocator, typedef); building in if-decl /
inner block (sc1/sc2); building const (bd1); inside-clause spellings (c1 continue, c3 nested, ip1-ip3; c2 if/else = 12 hunks);
nested filter ifs (l1-l3); village list via CTCVillage* (m1, 8 hunks) / non-const ref / const_iterator; IS_THING_ALIVE(*it) +
(*it)-> (a1-a3, FABLE_RETAIL_IS_THING_ALIVE + COMPARE_NULL_INT); GetTC HASTC_FORCEINLINE; CThing::GetPos real body visible
noinline (gp1/gp2, Q32 lesson -- GetPos is an inline accessor in Ego_r, out of line in retail at 0x004C73D0); STLport overlay
probes: ~vector without _Destroy, size() via end()-begin(), op[] via _M_start[n], 4.6 deallocate(n*sizeof) to a static free,
unguarded ~_Vector_base; branch order inverted (9 hunks).
Best next idea: make `building`'s register range not cover the ctor stores, or give _M_start's tail web its own identity:
(a) find a spelling where the vector's zero-init is not the same IL variable as the tail value (e.g. the real STLport 4.6
_Vector_base ctor through _STLP_alloc_proxy with a by-value allocator temporary, or a vector header path that constructs via
_M_start(0) on a base sub-object differently); (b) re-derive whether GetOwner()'s result is really one local -- e.g.
`CThingBuilding* building` declared in the building branch only, with the null test on a separate `GetOwner()` value is NOT
possible (one call) -- so try a pointer of a different class (CThingGameObject*/CThing* from GetOwner, cast at IsPointInside);
(c) compare with other retail rows where a local STLport vector filled by out-of-line push_back is read across calls after a
diamond with ebp (search callers of 0x00436C80 for `mov ebp,[esp+..]` at their tail) and copy their source shape.
No landed name proven wrong in pass 2.
Callers of push_back 0x00436C80 (xref): 0x0052534D/8D, 0x005C55A3/CA, 0x0061B842.., 0x0061E83C.., 0x0066700F, 0x007A0F20,
0x008A7B87, 0x008F79FD/7B36/7DBE, 0x0090C0EC (CAddFreeBenchToList), 0x0091872D/0x009187FB (this row), 0x00931BD9. The unlanded
function holding 0x00931BD9 (after MinionReactToCriminal::IsStillValid) keeps its local vector's _M_start in esi across
GetPWorldSeed + out-of-line GFRandom 0x004CDAE0 after an `empty()`-style `cmp esi,eax ; je` guard -- a sibling worth porting
next to learn the source shape of a tail-only _M_start register in this module.

## Attempt 3 -- RELOCATION_MATCH (src/00918640.cpp, test tests/00918640_test.cpp; no overlay, no flags)
What made it match: the failure paths are EARLY RETURNS, not a single exit. Owner branch `if (building->GetTC(TCI_THING_OWNER,&owner))
{loop} else return 0;`; village branch `if (!GetCreature().GetTC(TCI_VILLAGE_MEMBER,&member)) return 0; if (!member->GetPVillage())
return 0;`; tail `if (found.size() > 0) return found[GFRandom(found.size(), GetPWorldSeed())]; return 0;`. With those, c2 makes the
vector's _M_start a tail-only CSE (ebp from the join 0x0091880F); with a single exit the GetTC-fail edges reach the tail's free test
and _M_start becomes one whole-function web that loses ebp to `building`. Exact form matters: all three as `if(!x) return 0;` gives
this in esi / temp in ebx (14 hunks); only the owner one as `if {..} else return 0;` matches. Subsets A / B / C / AC give the old 5 hunks.
Probes this pass (for the record): GFROR13 asm body after the row = same as before it (13 hunks, c2 knows its registers);
push_back / _M_insert_overflow explicit specialisations or noinline before the row = no effect; tail without the GFROR13+seed pair
(k2/k3/k4) already CSEs _M_start, so the old residue was register pressure plus the whole-function web, not aliasing.
No landed name proven wrong.

## Repair pass (verifier findings) -- still RELOCATION_MATCH (also with LANDVERIFY_STRICT_HEADERS=1)
- Added `// Model: local (engine/CAIStateGroup_Gaze.h flattens the base; member names from it).` -- same justification as the
  landed siblings 0x00918880 / 0x00918190 (the generated header has no CAIStateGroupBase base, only pads at +0x04..+0x13).
- `protected:` section in the local struct: FindFreeParticularGazePos is now ?..@@IAE.. (matches the Leaf line / Ego_r / FableWin),
  GetOwner is protected virtual (?GetOwner@CAIStateGroup_Gaze@@MAEPAVCThingBuilding@@XZ). Bytes unchanged.
- Test: added deterministic fixtures where a dead / exclusively-used / outside marker is the only one of its gaze type (owner
  and village branches) -> 0 with the seed untouched, plus outside accepted when inside==false and village ignoring the flag.
  Mutants (alive removed, !exclusive removed, inside clause -> 1) now FAIL (verify_evidence/repair/mut_*.cpp).

## 0x00881A40 (not landed)

## 0x00881A40 NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags -- RELOCATION_MATCH (13 grades)
- Matched after porting onto shared headers (FULL stack branch, CThing, CTCGraphicAppearance MOVEMENT, CIntelligentPointer ASSIGN, fable_map set/tree opt-ins) plus: set find result in a block-scoped local; loop-1 restore = inline CThingGraphicByPassFlagState::Reset (PDB); loop 2 tests the thing via one `const CIntelligentPointer<CThing>&`; last 2 hunks (node in eax vs edi) closed by giving the map's tree find its body visible but noinline (FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE) -- a Q32-style callee-visibility fact.
- Needs overlay hdr/ (3 opt-ins, see hdr/README.md). No flags. NAME_FIX hint: landed 0x00883860 `_Rb_tree<CUnprovenKey_00883860,...>::find` is map<CThing*, CScreenFilterEntry::CThingGraphicByPassFlagState>::find (key/value proven by this row).
- Repair (verifier findings): the block-scoped set-find iterator is load-bearing and now cited in the source (retail 0x00881A49 lea esi,[ecx+0A0h]; 0x00881A54 lea ecx,[esp+10h]/push/mov ecx,esi; 0x00881A64 mov edx,[esp+0Ch]; cmp edx,[esi]); spelled inline = 4 hunks, function-scope local = 22 hunks (ATTEMPTS.log r1/r2). NAME_FIXES 0x00883860 now cites FableWin.pdb _Tree::find (_fablewin_api 0x01817856/0x02A5B6F0) + Ego_r map::operator[] (egor 0x008C43B0 = retail 0x00884C70). Regraded RELOCATION_MATCH (also strict); vt PASS.

