## 0x007E1FC0

## 0x007E1FC0 CTCAmbientBird::FrameUpdate -- RELOCATION_MATCH (wf22 author, first attempt, ~30 grades)
- What made it match: port onto shared headers (CThing GETTC_EXPANDED, CTCBase PEEKPLAYERMANAGER, CTCPhysicsBase, CRightHandedSet CTOR_INLINE/ROTATE,
  CWorldMap, CPlayerManager/PLAYER, CThingPlayerCreature chain); `CThing* parent = ParentThing;` read before hero->GetPos (retail ebp);
  `const C3DVector& away = parent->GetPos() - hero->GetPos();` (temporary bound to const ref -- this alone flips VC7.1's x87 operand order
  to `fld [Y] ; fmul [r]` at 0x007E2259; a named C3DVector local always gives `fld [r] ; fmul [Y]`, ~25 spellings tried);
  IsNonZero X,Y,Z (FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ) shares the distance sum; Normalise with parenthesised squares (new overlay
  opt-in NORMALISE_PAREN_SQUARES_XYZ) re-sums the shared squares; forward member-copied (forward.X = away.X ...) and member-assigned in the
  else so it stays on the x87 stack across the join; CCharString as a named local in its own block (string built before the pushes).
- Headers/flags: overlay hdr/engine/C3DVector.h (NORMALISE_PAREN_SQUARES_XYZ), hdr/engine/CTCAnimatingObject.h
  (PLAYANIMATION_NAME: public PlayAnimation(const CCharString&, long, bool) 0x007F90B0). No flags.
- Dossier correction: CWorldMap +0x28 (slot 10) is IsMapLoaded(long) const, not IsMapUsed (slot 11) -- header vtable order (Ego_r TPI):
  dtor 0, DeleteAllLevels 1 ... ForceUnload 9, IsMapLoaded 10, IsMapUsed 11, ..., GetMapNumberAt(C3DVector) 16 (grade: +0x2c with IsMapUsed).
- Repair (verifier finding 1): hdr/README.md now records that C3DVector.h line 380 `#if ..._ZYX` was rewritten to `#if ..._XYZ ... #elif ..._ZYX`; regraded RELOCATION_MATCH (also strict headers), test PASS, addmatch rerun.

## 0x004BD6E0

## 0x004BD6E0 NGraphicAppearance::CTCGraphicAppearance::SetPrimitive -- RELOCATION_MATCH (4 grades)
- Made it match: port onto shared headers (CThing PeekTC/GetTC GETTC_EXPANDED, MOVEMENT layout, C3DAnimationInterpolator, set/list from
  stlport) with real identities (TC3 appearance-morph texture morphs, TC0xE9 texture decals, TC0x5E hidden material names); GetAnims'
  real body inline in the TU; implicit C3DAnimationInterpolator/BlendState ctor+dtor; _M_erase noinline; CIVCountedPointer RELEASE_INNER;
  `bool sent = IsAppearanceSentEveryRenderFrame(); Resend = Resend || sent;`; HiddenMaterialsUsed=false before ReasonsForDraw.
- Headers (overlay, see hdr/README.md): C3DAnimationBlendState.h + C3DAnimationInterpolator.h (FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_IMPLICIT),
  NGraphicAppearance_CTCGraphicAppearance.h (FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETPRIMITIVE), CTCHeroAttachableAppearanceModifiers.h
  (FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_HIDDENMATERIALS), stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE). Flags: none.
- Local models (no shared header exists): CGraphicAppearanceDrawInfo (Ego_r 36-byte layout), CTCAppearanceMorphBase::CMeshMorphSet,
  CEngineTextureMorphSet, CTCTextureDecal. Global 0x013B8A18 kept as the landed spelling g_pEngineGlobals_013b8a18 (no PDB name found),
  typed here as const CEnginePrimitivePrepareInfo* (what this row stores into it).
- Name fix: 0x00706860 is CTCHeroAttachableAppearanceModifiers::GetHiddenMaterialNames (Ego_r.pdb + FableWin.pdb
  ?GetHiddenMaterialNames@CTCHeroAttachableAppearanceModifiers@@QBE_NAAV?$set@VCCharString@@...@std@@@Z; TC 0x5E, set<CCharString>& arg), landed as CTCActivationReceptorBase_AddTrigger.
### Repair (verifier findings, 2026-11-13) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1)
- Global 0x013B8A18 renamed to the descriptive `g_pCurrentPrimitivePrepareInfo` (no name found in Ego_r / FableWin / FSE / EgoCore), typed
  `const CEnginePrimitivePrepareInfo*`: this row stores &info into it (0x004BD6E8) and the landed readers' +0x34 load is Ego_r
  CEnginePrimitivePrepareInfo::Engine (+52). The four landed readers (0x004BD620, 0x004BD690, 0x004BC340, 0x004BCFE0) still spell it
  `extern CEngineGlobals* g_pEngineGlobals_013b8a18` (a pad model whose +0x34 member is really CIEngine* Engine): NAME_FIXES.tsv asks the
  coordinator to respell AND retype them in the same landing so the address has one name and one type.
- CMeshMorphSet::MeshParts is now `CArray<CMeshMorphSetParts>` (nested CMeshMorphSetParts forward-declared, Ego_r spelling) and
  CEngineTextureMorphSet::Morphs is `CArray<CEngineTextureMorph>`, both from engine/CArray.h as in Ego_r; PTextureMorphs
  (vector<CEngineTextureMorph>*) takes &Morphs by the implicit derived-to-base conversion, as in retail. Bytes unchanged.
- Test: scenario 3 no longer overwrites the reasons; a new scenario has a component whose retry fails again with reasons = 5 and asserts
  they are kept (the all_drawn=false path, 0x004BD942 / 0x004BD952).

## 0x00A24520

## 0x00A24520 C3DMeshLODInfo::BuildMesh -- RELOCATION_MATCH (grade 9 of this agent; strict-headers regrade clean)
- What made it match: port onto the shared headers (fixed the global-pool swap order) + fold each upload result with `ok &= prim->Upload...(...)` in every arm (a shared `bool result; ... ok &= result;` made VC spill ok instead of the primitive index i into the dead `flag` home slot, and flipped esi/edi for bank/index). uploadFlags is the `= 0; if (... && DP3BumpEnable) = 2;` form (a ternary drops the 4th register and the spill).
- Headers: overlay hdr/ (9 opt-ins, hdr/README.md); flags none (no /EH). Defines: sibling 0x00A24D70 set + CDATAINPUTSTREAM, CMEMORYDATAINPUTSTREAM, CMOVABLERESOURCE, C3DMESHSTATS_DUMMYOBJECTS, C3DMESH2_HELPERPOINTS and the new opt-ins.
- Names: g_MeshDataMemoryPool (0x013D2D6C) is DESCRIPTIVE (no symbol found); g_NextMeshID / g_ParticleMemoryPool are landed spellings. Landed callee spellings that differ from PDB (not respelled here, informational): CMemoryDataInputStream ctor (const unsigned char*, long) vs PDB (const void*, unsigned long); LockPrimitive/UnlockPrimitive (long) vs PDB (unsigned long); InitialiseMeshGenerators pointer vs PDB `const CMeshDataBank&`; old draft LoadBinary 3rd param was `unsigned long flags`, PDB is `const CGraphicDataBank*`.

## 0x0054FBC0

## 0x0054FBC0 NUISystem::CText::SetValue(unsigned long, const CWideString&, bool) -- RELOCATION_MATCH (strict headers too)
- Matched after porting onto shared headers (CText VSLOTS, CMainGameComponent TEXTBANK, CNewFrontendGameComponent, NGameText GROUPENTRY,
  CIVCountedPointer RELEASE_MEMBER + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES, CWideString NULLSTRING, CFONTBANK). Key source facts:
  `textId > 0` (cmp/jbe); plain-entry arm FIRST (`!GetEntryDataType`) with a named `CWideString text = ...->GetText(); Value = text;`
  (pointer temp released before the assign; identical arms -> FE plain arm cross-jumped); join as if/else not `?:`; game loop
  keeps `const CArray<long>& ids` and reads `ids.begin()[i]` (ids[i] swaps eax/edx); FE loop copies CArray<long> and uses ids[i].
- Overlay opt-ins: FABLE_RETAIL_CTEXT_FITINWINDOW (NUISystem_CText.h), FABLE_RETAIL_CSUBTITLERENDERER_FORMATTEXTWITHGAMEACTIONNAMES
  (CSubtitleRenderer.h). No flags. Third parameter name descriptive (spaceSeparated; landed caller 0x0054F410 calls it keepSize).
- vt.py must run with cwd = repo (from $W it fails importing verify_and_land).

## 0x004C0560

## 0x004C0560 NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint -- RELOCATION_MATCH (author wf22, ~30 grades)
- Made it match: the shared HelperPoint header body + same-TU inline GetAllMeshes / IsResourcesLoadedForMesh (copied from landed
  0x004BE3E0) + inline C3DMeshObject ctor/dtor/IsAnimated, and the mesh loop spelled `while (it != end) { ...; ++it; }` instead of
  the header's `for (...; ++it)`. With the `for` loop VC7.1 expands ~C3DMeshObject at all three exits (1001v851); with the while
  loop it expands it only at the loop continue and calls the COMDAT 0x004C3200 at both returns, and the frame matches.
- Not the cause (tried): ~CCountedPointer via ReleaseData (+dead zero stores, +empty Validate), CMatrix3x4 dtor, visible std::sort
  body, VECTOR_RESERVE, HASTC_FORCEINLINE, /Gr, continue/nested-if spellings, extra caller IL. A real store in ~C3DMeshObject also
  tips the split (probe only) -- the inliner's per-site threshold is close to this dtor's size.
- Headers: overlay C3DMeshObject.h (FABLE_RETAIL_C3DMESHOBJECT_INLINE) + C3DMeshStats.h (FABLE_RETAIL_C3DMESHSTATS_ISANIMATED);
  defines as in the source. No Flags. The shared NGraphicAppearance_CTCGraphicAppearance_HelperPoint.h body should switch to the
  while loop (it is bytes-irrelevant for its only includer GetFocalPos 0x004C1060, which does not emit it as its leaf).
- Names: manifest label right. Old draft names wrong (see DOSSIER table 3). NAME_FIXES.tsv: 0x00A36100 (landed as made-up
  C3DMeshStatsOwner_00a36100::HasDummyObject) -> ?HasHelperPoint@C3DMeshObject@@QBE_NABVCCharString@@@Z. Proof (repair): Ego_r.pdb has
  it at 0x00B8CEA0 right before GetNoHelperPoints 0x00B8CEC0 = retail order 0x00A36100 / 0x00A36140 (landed GetNoHelperPoints); its
  body calls 0x00A2EA20 = landed C3DMeshStats::HasHelperPoint; FableWin.pdb exports the same symbol (0x017DA082). HasDummyObject@
  C3DMeshObject is a different function (Ego_r 0x00B8D110). Coordinator: respell the landed row and its callers.

## 0x0063B3F0


## 0x0063B3F0 CWorldMap::CreateLineSegment -- DIFFER(836v856) after first authoring pass (2026-11-13)
Best source: differ/0063b3f0.cpp (ported onto engine/C3DVector.h + engine/CWorldMap.h with FABLE_RETAIL_CWORLDMAP +
FABLE_RETAIL_CWORLDMAP_LINESEGMENT; no overlay header needed; default flags).
What is now right: frame size 30h (scalars `float dX,dY,dZ` instead of a C3DVector d -- the struct cost 8 bytes);
`C3DVector end; end = *from;` (assignment, NOT copy-init: the header's user copy ctor enregisters crossesX in bl, 799-808);
X/Y block products spelled `(dY * inv) * t` (parenthesised: stops the inv*t re-association, Y block now matches retail).
Exact residue: VC7.1 CSEs `(long)from->Y` from the Y-cell compare into `yHit.Y = (float)(long)from->Y` (retail 0x0063B489
calls __ftol2 again). The CSE temp takes [esp+14h], which shifts every slot: retail fild temp +14h, dY -> dead `from` slot
+44h, floorX -> dead `to` slot +48h; ours fromX/floorX share +44h, dY +48h. Flags crossesY/valid swapped (+11h/+13h) and
vectors rotated (retail xHit +1Ch, yHit +28h, end +34h) -- probably follow-on of the same lifetime change.
Empirical CSE rule found: CSE happens iff the d computation sits BETWEEN the compare and yHit.Y (d before the compare,
v72, or after yHit, v64: no CSE but wrong order). A store through a pointer between them (probe p1) kills it.
Ideas tried (all still CSE or worse): int/long/short/(double)/unary-plus casts on either side; named fromY; toY not a
variable (from evaluated first, worse); compare swapped; BlockOf(float) inline helper; ClearIfSame(bool&) helper; bool
array crosses[2]; C3DVector hit[2] array; alias pointer/reference `start` for from (GOTCHA F1); yHit/compare via end.Y
(loads stack, not [esi+4]); end member-wise / copy ctor / member ctor; C3DVector::ToC2DCoordI temporaries (FableWin
PDB inline, 842-848, temps in memory); step vector d*(1/|d|) (878); declaration orders of bools / vectors / floorX
(no effect on slots); local renames (no effect); flags /Op /Ow /Oa /G5 /G7 /Ox and optimize("y"/"p"/"a").
Best next idea: find the construct that kills the from->Y ftol CSE without code -- e.g. an aliased (address-taken) local
written between compare and yHit (crossesY or xHit escaping into a real out-of-line call), or a toy probe (same
compare / d / yHit skeleton) to learn VC7.1's ftol-CSE rule; then re-check the slot layout, which should follow.
Name fix for coordinator: landed caller 0x0063D960 declares CreateLineSegment public (mangles QBE); PDB is protected IBE
(Ego_r egor_pdb_names.tsv 006e7530, FableWin 024a8420).

## 0x0063B3F0 CWorldMap::CreateLineSegment -- RELOCATION_MATCH (attempt 2, 2026-11-13)
What made it match (source: row_0063b3f0.cpp, test: test_0063b3f0.cpp, vt PASS, strict-headers regrade RELOCATION_MATCH):
1. The `(long)from->Y` __ftol2 CSE is broken by reading the Y-cell compare through the FableWin.pdb inline
   `C3DVector::ToC2DVector()` (`if ((long)from->ToC2DVector().Y == toY)`, repo opt-in FABLE_RETAIL_C3DVECTOR_TO2D): the 2D
   temporary gives the load a different value number, so yHit.Y truncates again (retail 0x0063B489). Every direct spelling
   (casts, aliases, helpers, volatile, d placement, unsigned/int64) CSEs. Toy probes: the CSE appears iff an FP value
   (dY) is computed between the compare and yHit.Y -- not a usable lever.
2. No `floorX` float local: `xHit.X = (float)fromX;` and `0.0f == from->X - (float)fromX` -> the fild temp lands at +14h,
   dY/the CSE'd float go to the dead from/to arg slots, and `xHit.X += 1.0f` reloads xHit.X (no copy-prop from floorX).
3. Tail: `valid = true;` at the end of the crossesX branch and of the crossesY branch (neither branch leaves it false), then
   `if (valid && !IsPosInMap(*segStart)) {...}`. Gives retail's frame order (valid +11, crossesX +12, crossesY +13) and the
   block layout (both->xHit falls into the IsPosInMap block; other branches after `ret`). A plain `goto done` or a
   `crossesX || crossesY` gate leaves valid at +13.
Headers / flags: engine/C3DVector.h + engine/CWorldMap.h with FABLE_RETAIL_CWORLDMAP, FABLE_RETAIL_CWORLDMAP_LINESEGMENT,
FABLE_RETAIL_C3DVECTOR_TO2D (all existing repo opt-ins; no overlay header). Default flags, no Flags line.
Made-up name: MaxAbs2D (twice-inlined max(|dx|,|dy|); no PDB / FSE / EgoCore name found).
Name fix (from the scout, not re-verified here): landed caller 0x0063D960 declares CreateLineSegment public (QBE); PDB is IBE.

## 0x00F3A890


## 0x00F3A890 CRangeCompressor::CalcBestCompressionForBlock -- RELOCATION_MATCH (grade ~13 variants)
- Made it match: (1) ported onto engine/CRangeCompressor.h + engine/CArray.h (FABLE_RETAIL_STLP_VECTOR_C) with inline out-of-class
  GetBlockSizeFromFlags / CalcHeaderSizeNeeded / CalcBitsNeeded; (2) the REAL landed bodies of the same-TU callees
  CalcMemoryNeededBitCompressed 0x00F3A270, TestRangeBias 0x00F3A5D0, TestRedundantBitStrip 0x00F3A710 placed before the row under
  #pragma auto_inline(off) -- VC7.1 then knows they don't capture the CScriptEntry& args, so entries[0].Flags stays in ebp across
  vector::operator= and entries[1]'s ctor store dies (STLP_VECTOR_C alone / explicit instantiation / direct vector::operator= did nothing);
  (3) CalcBitsNeeded in the `bits = bit + 1; break; ... return bits;` form (fixed the loop's i/bestIndex/best slot map too).
- Headers/flags: no overlay, no flags; row defines FABLE_RETAIL_STLP_VECTOR_C. Callee bodies ported onto header member names
  (GapStart->RangeShift, KeptMask->ShuffleMask, SetMask->OrMask). g_RangeCompressorByteMasks (0x013AC944) is the landed rows' descriptive name.

## 0x009C9280

## 0x009C9280 NGameText::CDataBank::GetTextEntryFromGroup -- RELOCATION_MATCH (author wf22, 2026-11-13)
What made it match (in order of effect): ported onto shared headers + overlay; GetTextInfo/IsLoaded bodies NOT visible (retail
writes the loop's text-info lookup and the first pick's loaded test out in place, calls them elsewhere); single-exit outer
`if (groupId > 0 && type == 1) {...} return 0;` (fixes prologue/shared epilogue); CBasicString::Compare body visible (narrator
table base kept across Compare); named `long narratorIndex` + both compares index Narrators; `const CArray<long>& entries =
group->GetTextEntries()` (ids start CSE between size test and body); `bool loaded = ...; if (loaded || allowPending)` (lets c2
tail-merge the two successful returns); statement order `found = true; count; id = ids[0]; pick = 0`; GFRandom result-local form.
Headers/flags: overlay opt-ins listed in hdr/README.md; no // Flags line needed (op= forced inline by opt-in; /d2inlT100 is the
flag alternative). Callee 0x009D68B0 is CBankFileAsync::AsyncUpdatePending (Ego_r+FableWin PDB names; FlushAsyncUpdates is in
neither PDB) -- see NAME_FIXES.tsv. CDataBankTextEntry::NarratorIndex (draft "SpeakerIndex") and Narrators/Seed are PDB names.
Residual risk: GFRandom's result-local spelling is an opt-in body variant of the shared inline (semantics identical).
Repair (2026-11-13, after verifiers): test now asserts the re-roll (two loaded HERO candidates, group 9: every pick != previous
lastId) and the `_1_5` alternative (group 8: only a HERO_1_5 entry, found by "HERO"); both verifier mutants (verify_evidence/mA.cpp,
mB.cpp) now FAIL. Provenance fixed: GetTextEntries / GetNarratorIndex are FableWin.pdb names only (not in egor_pdb_names.tsv);
CGroupEntry Entries +0x1C is in struct_layouts_egor.tsv. Headers comment now cites FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ (retail
calls Compare 0x00411570 directly at 0x009C93AD / 0x009C93F4). Still RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1.

## 0x00646F67 (not landed)

## 0x00646F67 NPlayerGui::CDrawExperience::Draw -- DIFFER(843v843), wf22 author pass 1 (~55 grades)
Best source: differ/00646f67.cpp (= v14). Ported onto shared headers (CDrawBase, CPlayerGui MEMBERS, CPlayerGuiDef, CStatBar,
CEnginePrimitive2DSprite ENUM_CTORS, CIEngine, CGraphicDataBank FRAMESIZE, CFontBank, CMainGameComponent MAINFONT); local
NPlayerGui::CMultiStatBar : CStatBar (no header exists) with GetCurrentType/GetCurrentChange decls + inline GetFadeAlpha.
Overlay opt-ins (hdr/README.md): CPLAYERGUIDEF_EXPERIENCE (typed members), CPLAYERGUI_FRIEND_EXPERIENCE, CRGBCOLOUR_COPY_FORCEINLINE,
NPLAYERGUI_GFTOSCREEN_BOX (only used by experiments).
What got it from 824 to 843v843 (identical instruction stream): the shared headers + FABLE_RETAIL_CRGBCOLOUR copy ctor forced inline
(VC7.1 caller inline budget keeps the plain-inline copy ctor out of line; retail expands it: `lea eax,[ebx+3F8h]` then 4 byte reads).
Residue: frame/slot placement only. Retail: X(unused 8B)-0x58, middleEnd -0x50, screenPos -0x48, box T16 -0x40, pos T8 -0x38 (shared
with box BR and end pos), T8 -0x30 (icon uv_end + height tv -0x2c), text -0x28, alpha -0x24, T8 -0x20, T8 -0x18, colour -0x10.
Ours (v14): middleEnd -0x50, screen -0x48, T16, T8 -0x38(+height), text -0x30, alpha -0x2c, T8 -0x28, pos -0x20 (named), T8 -0x18; frame 0x14c.
Tried (all keep 843 or worse): flags /O1 /Oy-, /Ob2, no "s"; C2DVector/C2DBoxF forceinline opt-ins; Initialise / 4-arg colour (850);
float right instead of middleEnd; named box (0x14c); pos reused for end (v14), named endPos (v15), temp end (v9); const& bound temp pos;
x/y floats (misaligned layout); C2DVector(pos.X,pos.Y)+offset (pos packable, packs with middleEnd); explicit per-axis ctor (845);
GFToScreen on icon/end/box (box: 867); pos = GFToScreen(...) assignment (854); whole body inside if/block scope (no change);
declaration order swaps of pos/alpha/graphic/screenPos; textPos named; C2DVector size (841).
Best next idea: find what makes pos a temp-pool slot plus an unused named 8-byte slot (a named C2DVector whose stores were all
forwarded to a temp copy); read /FAs maps with $W/qa.sh + $W/sym.py (slot map per variant). Possibly a named textPos/pos fed through
a by-value inline copy; or retail TU's header spelling of C2DVector copy/operator= differs.
NAME_FIXES candidates (from DOSSIER, both PDBs): 0x00646D90 is NPlayerGui::CMultiStatBar::GetCurrentType (QBE?AW4EStatUpdateType@@XZ), 0x00646DA7 is
NPlayerGui::CMultiStatBar::GetCurrentChange() const (QBEJXZ). Made-up names: none (EStatUpdateType enumerators left empty).

## 0x00646F67 -- wf22 author pass 2 (~95 grades): DIFFER(843v843), 7 hunks (was 42)
Best source: differ/00646f67.cpp (= w129; pass-1 best kept as differ/00646f67.attempt1.cpp).
What fixed the frame (0x14c -> 0x154) and every slot: the middle piece is a NAMED box built from the corner and the extents,
passed through the PDB copy NPlayerGui::GFToScreen(const C2DBoxF&):
  `C2DBoxF box(middlePos, C2DExtentsF(width, height)); CEnginePrimitive2DSprite middle(0, GFToScreen(box), ...)`
  (same idiom as landed CQuestInfoElementTimer::DrawAt 0x006507D5 / Counter::DrawAt 0x006509F8). Retail -0x58 is `box` (only BRX stored,
  at -0x50, because the end cap needs it after the call), -0x40 is the GFToScreen copy; pos then packs at -0x38 by itself.
  Opt-ins: C2DBOXF_EXTENTS + C2DBOXF_COPY (repo), NPLAYERGUI_GFTOSCREEN_BOX (overlay); icon uses GFToScreen(pos).
End cap X = (BRX - TLX) + TLX = box.GetTLX() + box.GetWidth(): PDB has inline ?GetTRPos@C2DBoxF@@QBE?AVC2DVector@@XZ (overlay opt-in
  C2DBOXF_TRPOS); `GFToScreen(box.GetTRPos())` is the current spelling.
Residue (7 hunks, 0x0064721B..0x00647250 only): retail computes the end X before the two zero uv temps and copies Y right after the first
  fldz (X stored last, just before the ScaleAlpha call); ours builds both uv zeros first and computes X after them.
  Every form that puts X early (named endPos / pos.X,pos.Y / Initialise / GetTRPos into a named local, + GFToScreen copy) leaves Y one
  slot late (11 hunks: `fldz; fstp T0x; mov eax,[-4]` vs retail `fldz; mov eax,[-4]; fstp T0x`); named endPos passed directly
  (block scope keeps the frame) stores X at once (9 hunks).
Tried this pass (all 843 unless noted): end temp + operator+/named size (frame 0x144); named size forwarded (no slot); box via extents
  without copy (824); C2DBoxF(box) copy (= GFToScreen(box)); end pos: pos.X/pos.Y reuse (frame), pos = C2DVector(...) (855),
  pos = GFToScreen(...) (867), pos = middlePos; pos.X += w (852), pos = box.GetTRPos() (855/867), named endPos (+8 frame unless
  block-scoped), endPos = GFToScreen(...), const& bound temps, float endX / endX+endY locals, operand orders, middlePos + C2DVector(w,0)
  (849), endPos(middlePos) + X +=, Initialise (Y first), GetTRPos in arg / named / via GFToScreen(box).GetTRPos(), screenBox named (838),
  no middlePos (box from pos+offset), box from 4 floats, middlePos from X/Y floats (844), end sprite as a temporary in
  SetPersistentPrimitive (855), GFToScreen(C2DVector) body spellings (return pos / C2DVector(pos.X,pos.Y) / named local), GetTRPos
  body spellings, uv vectors as default ctor arguments, /O1 /Oy- without pragmas, /G5 /G6 /G7.
Best next idea: the Y copy must be in the IR before the uv temps (retail looks like "P built first, its x87 store deferred"). Find a
  spelling where the end position is a by-value temp built before the argument list but not a named slot -- e.g. a real inline
  helper of the TU taking the position by value, or GFAnchorToTopLeft (Ego_r 0x00686B8B, 13-byte identity copy like GFToScreen) on
  the end position / on pos at the top; or check whether ScaleAlpha / the uv args are really in the order modelled.
Made-up names: none (endPos-style locals are not in the final source).

## 0x00646F67 -- wf22 author pass 3 (~70 grades): still DIFFER(843v843), best = differ/00646f67.cpp (unchanged, 7 hunks)
Finding: the end-cap region has stable "attractors"; every spelling lands in one of them:
  A (7 hunks)  X chain computed at the argument (GetTRPos/C2DVector(...) at the arg, or a single-use float endX -- VC forwards it):
               X chain after the uv stores; tail `mov ecx,edi; fstp T.X; call` = retail.
  B (11 hunks) X computed in an earlier statement into a NAMED C2DVector (endPos/offset_pos style, any construction: GetTRPos,
               (x,y) ctor, Y-first/X-first member stores, copy+X+=, middlePos.X += w, reference vars, through 1-2 identity copies
               GFToScreen/GFAnchorToTopLeft, function or block scope, /O1 /Oy-, /Ox, int-literal uvs, C2DVector forceinline ctor):
               X chain on top (= retail) but the Y int copy lands one slot late (`fldz; fstp uv.X; mov eax,[-4]`), which phase-shifts
               the int/fp interleave so the tail is `fstp T.X; mov ecx,edi` instead of retail's `mov ecx,edi; fstp T.X`.
  C (9-10)     endPos passed directly / named screenEnd / comma `(endPos.X = x, endPos)`: X and Y stored before the uvs.
  Named uv locals (probe only, not a plausible source) give new shapes (6-8 hunks): with `C2DVector uvB(0,0)` declared BEFORE endPos
  the Y copy interleaves exactly like retail (`fldz; mov eax,[-4]; fstp; mov [-34h],eax`) but the X chain moves below those stores.
  So the Y-load timing is set by which stores precede it in the IR (VC's conservative load/store ordering on address-taken temps),
  not by the end-position spelling. Retail = B's IR order for X + a Y load that may not cross the first uv store.
Tried this pass (all 843 unless noted): GFAnchorToTopLeft wraps (arg and named), named endPos Y-before-X / X-before-Y / copy+X+= /
  (x,y) / middlePos-based / box-based / const& / const, passed directly or via GFToScreen / C2DVector(endPos) / (endPos.X,endPos.Y) /
  double copy, function-scope endPos, endPos = GFToScreen(endPos) (855), comma-expression arg, float endX/endY (forwarded -> A),
  const float& endY, middlePos/box as const& bound temps (839-852), const& screenBox = GFToScreen(box) (838/843-16), C2DVECTOR
  FORCEINLINE ctor, int-literal uv/0 args (sibling 0x0064619F idiom), middlePos.X += w / middlePos = GetTRPos (867), float middleX/Y
  (844), flags /O1 /Oy- (no pragmas), /Op (876), /Ox, /O2 /GF /Gy, named uv locals in 6 orders (6-11 hunks, probe).
Best next idea: find what makes the Y load unable to cross the first uv temp store while the X chain stays above it -- e.g. the uv
  temporaries built by something other than the inline (float,float) ctor in retail's header (a C2DVector ctor taking ints, a
  `C2DVector::Zero`-style inline, or default args on 0x00493AC2's declaration combined with a named endPos), or an end position
  whose Y is read through an address-exposed object (a C2DBoxF/C2DVector whose address escapes before the end cap).
Made-up names: none in the best source (endPos/uvA/uvB only in probes). No header opt-ins added (GFAnchor probe opt-in removed).
  Also tried after the above: const middlePos / const box / const pos (no change: 7 / 11), uv default args + named endPos (11, pass 2 w158).

## 0x00F3BA70 (not landed)

## 0x00F3BA70 CTriangleStripifier2::BuildCacheOptimizedVertexList -- DIFFER(834v853), 8 hunks (first author pass)
Best source: differ/00f3ba70.cpp (== h2.cpp). Ported onto engine/CTriangleStripifier2.h (overlay opt-in
FABLE_RETAIL_CTRIANGLESTRIPIFIER2_BUILDCACHE declares the private member) + STLport FABLE_RETAIL_STLP_VECTOR(_FILL_NOINLINE).
What got it from 51 hunks to 8:
- arg roles per dossier (a4 = per-vertex strip index, a3 = vertex->vertex map, a1 unused); `long ahead=counter+1` per i (VC7.1 makes the
  [esp+28h] induction itself; an explicit `now` variable is worse); vertex `unsigned long` (frame 0x44; ushort grows the frame/code).
- distance = `ahead-counter-1` computed per j after ++ahead (not an outer variable with ++distance); `needed=false` before ++ahead.
- KEY: loop bounds as full expressions, no reference locals: `j<strips[s+1].size()`, `unsigned long w=strips[s+1][j];` (read before
  ++ahead), `k<vert_polys[w].size()`, `f=vert_polys[w][k]`. With `const CArray& next/polys` locals VC7.1 CSEs/hoists the size; retail
  re-reads [ecx+4]-[ecx] every pass.
Residue (exact): only the inline push_back growth (_M_insert_overflow __true_type): retail len=ebx, list reloaded into ebp from [esp+6Ch]
after malloc and ebx restored at the end (+ `jmp ; lea esp,[esp]` align pad before the fill label 0xF3BCF0); ours keeps list in ebx, len in ebp.
Tried without effect on that residue (all stay 8 hunks): FABLE_RETAIL_CARRAY, FABLE_RETAIL_CARRAY_PUSHBACK wrapper, STLP_VECTOR off/_C/
_INDEX_START, push_back arg spellings (cast, (const ushort&)vertex, ushort temp), vertex long/ushort/function-scope, counter unsigned/int,
signed loop indices, num_verts types/order, clear()/erase(), out alias ref, ++counter split, dead temps, /G6 /G7 /Oa /Ow, optimize("y",off).
FILL_OUTOFLINE (decl-only ctors), stop-flag instead of goto, `needed` in the j condition: all worse.
Best next idea: the growth colouring is likely a callee-visibility / TU fact -- retail's sibling StripUnusedVerts (0xF3B970, same TU, just
before) calls vector<unsigned short>::_M_insert_overflow 0x005135D0 OUT of line; try putting StripUnusedVerts' real landed body (and an
out-of-line _M_insert_overflow instantiation, e.g. via FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE-style noinline of the overflow) earlier in
the TU so c2 has the overflow compiled as a function before this row, or c2prio-probe the len/this webs in the growth region.
No landed names proved wrong. Param names are descriptive (no PDB locals): triangles, vertex_to_poly_map, vertex_to_vertex_map, vertex_strip, strips, out.

## 0x00F3BA70 -- attempt 2 (still DIFFER(834v853), 8 hunks; best source unchanged: differ/00f3ba70.cpp)
New finding (level: the inlined STLport growth body): retail's `jmp ; lea esp,[esp]` pad before the fill store at 0xF3BCF0 is a
LOOP-HEAD alignment: in retail's IL the fill_n loop of the inlined `_M_insert_overflow(__true_type)` survived until after loop
alignment and was folded (n == 1) only later; in ours VC7.1 folds it early (no pad). Variants where the loop survives also flip the
growth colouring to retail's (len = ebx, the list reloaded into a scratch reg from [esp+6Ch]) -- so the 8-hunk residue is one fact.
Probe overlay kept in probe_hdr/stlport/fable_containers.h (FABLE_PROBE_* / FABLE_RETAIL_STLP_OVERFLOW_* switches, NOT for landing;
moved out of hdr/ so fg.py does not use it). Results (canonical STLport body = collapsed loop, 834/8 hunks):
- loop kept (pad present): fill loop written in the overflow body on newFinish (`for(n=fillLength;n>0;--n,++newFinish)`, 852/17),
  `fill_n((_Tp*)__copy_trivial(..),fillLength,x)` (852/17; len=ebx + list reload match retail, but newStart/list swap edi<->ebp and the
  fill iterator is esi instead of eax), the __copy_trivial ?: written inline in the body with the length expression duplicated (847/10:
  pad + canonical eax iterator, but len stays ebp and n is recomputed).
- loop folded early (no change, 834/8): fill_n body spellings (local iterator, local counter, const N&, postfix, no `inline`),
  uninitialized_fill_n, __uninitialized_copy/__copy_ptrs for the prefix, newFinish=newStart then assign, if/return __copy_trivial,
  if/else newFinish with a local length, fill_n(newFinish+0 / fillLength+0 / (int)), push_back passing 1UL / a position local.
- push_back local `one` count, /Ob1, `fill_n(first,last)` form: overflow no longer inlined (747).
- TU / order level, all no change: explicit instantiation of the overflow, of vector<unsigned short>, of fill_n before the row; an
  earlier caller with inline_depth(0)/(1); the real StripUnusedVerts body (shared headers, opt-in
  FABLE_RETAIL_CTRIANGLESTRIPIFIER2_STRIPUNUSED in hdr/engine) before the row; inline_depth 4/255 (3 = worse); dead row computations
  (inliner budget); QFE-4035 compiler; /Ob2; insert(end(),x) (overflow out of line, 701).
Best next idea: find the spelling of the REAL STLport 4.5/4.6 true_type overflow that keeps the fill loop (a canonical-copy
iterator, eax) alive in VC7.1's early passes -- compare with another retail row that inlines a POD vector push_back growth and shows the
same `jmp ; lea esp,[esp]` pad before its fill store (search retail for `eb07 8da42400000000` followed by a 16-bit store inside an inlined
malloc/memmove/free growth), and copy that row's header opt-ins. No landed names proved wrong.
- Byte search of retail .text: the `jmp ; align pad` right after a memmove 0x00BFEAE6 call occurs only in this row (0xF3BCE7) -- there
  is no sibling with the same retained-fill-loop growth to copy opt-ins from (the other hit 0xBDF58B is a fill<C3DBoundingSphere> COMDAT).
  Next level to try: whether retail's push_back value was a 16-bit lvalue that can alias the new block (fill_n then cannot be folded
  early) -- e.g. a `const unsigned short&` into strips[s] kept in the frame, or a ushort vertex homed in a dword slot -- combined with the
  canonical STLport body; and a c2 /Bd or -d2 dump comparison of the canonical vs the `fill_n((_Tp*)__copy_trivial(..))` probe.

## 0x00F3BA70 -- attempt 3 (still DIFFER(834v853), 8 hunks; best source unchanged: differ/00f3ba70.cpp, hdr/ = engine/CTriangleStripifier2.h only)
Tried this pass (all no change = same 8 hunks unless noted):
- push_back of an element reference (strips[s][i], a const ushort& local, also for emitted[]): worse (872-898 bytes, 48-57 hunks).
- overflow true_type marked `inline` (STLport 4.5 defines it in-class) -- alone and with /Ob1; allocator through a nested static
  malloc-alloc class (extra inline level, like __sgi_alloc): no change. (probe overlay kept in probe_hdr/stlport_attempt3, opt-ins
  FABLE_RETAIL_STLP_OVERFLOW_INLINE / FABLE_PROBE_ALLOC1; not for landing.)
- overflow template defined (decl-only header) AFTER the real StripUnusedVerts body and before the row: no change; our StripUnusedVerts
  still inlines the overflow (retail calls 0x005135D0), so the out-of-line split of the retail TU is not an IL-order fact.
- flags /Ox, /GX, /Oi-, "/Ob2 /Oi": no change. /d2inlT sweep: T<=60 calls the overflow (747), T>=65 = 834/8 (overflow score ~61-65,
  just under the default 74 -- retail's StripUnusedVerts call is consistent with that). /d2inlS 0..40 no change, 80 worse.
- BUDGET inliner (GOTCHAS "landing fw": `#pragma optimize("g",on)` + /Og- /d2vol10..100, + /d2inlS50..1000): 825/54 -- the first
  sites (clear()/erase, __copy_trivial, fill_n, _M_clear) become calls; retail expands them all, so this TU is on the heuristic path.
- LTCG test (/GL + link /LTCG, harness in lt/ltcg.py + lt/cmp.py): frame 0x48 and custom conventions -- retail is NOT LTCG here
  (0x009B4A20 push_back is plain thiscall, called from BuildRings 0xF3DE3A).
Observation: in retail the list in the growth region is a SEPARATE web (reloaded into ebp from [esp+6Ch] after malloc; the outer `out`
web in ebx is evicted and restored at 0xF3BD0E, like the outer [esp+5Ch] web in edi); ours re-uses the outer ebx web. The pad
(loop head at 0xF3BCF0) and the colouring are separable (probe ovT gives the pad with ours colouring).
Best next idea: trace c2 under gdb (pending/2026-11-12_linux_handWF19F/NOTES.md has the breakpoints: 0x107723E7 expand,
0x1078D1AA heuristic) to see the ORDER in which push_back / _M_insert_overflow / fill_n / max are expanded into the row; retail's
late fill-loop fold suggests one of max / fill_n was expanded in a later inline round than ours. Then find the source fact
(e.g. a Lionhead CArray<unsigned short>::push_back member -- Ego_r has ?push_back@?$CArray@G@@QAEXABG@Z) that gives that order.

