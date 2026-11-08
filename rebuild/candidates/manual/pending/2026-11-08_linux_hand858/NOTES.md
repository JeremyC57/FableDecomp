# Chunk 858 notes

Result: 7 matched (all RELOCATION_MATCH), 3 parked in differ/. Every row has a test in test/ (PASS under tall.sh g++ and tall.py VC7.1).

## Small rows (matched on the first grade)
- 0x00CB6E20 std::vector<std::pair<CCharString,CScriptInfo> >::~vector: STLport 4.6 _Destroy loop (stride 0x18, Name at +8 of CScriptInfo)
  then ~_Vector_base. Probable layer: the FableWin outer CArray<pair<...>> dtor layers call it.
- 0x00D3B3D0 NScript::CQ_GuildTrainingScript::Init: vtable 0x012CD458 slot 3; tail jump to GossipSetup 0x00D3B410 (opt-in
  FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_INIT declares it).
- 0x00D7AE30 NScript::CQ_HeroSoulsThunderScript::OnPersist, 0x00D8DCC0 NScript::CQ_HobbeCaveScript::OnPersist (opt-in
  FABLE_RETAIL_CQ_HOBBECAVESCRIPT_ONPERSIST), 0x00EB6980 NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::OnPersist: persist two
  bools by name, default false (strings are the PDB member names).
- 0x0121AD80 dynamic initializer of BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION (0x013B8BFC). The `$E` index in the label is descriptive.

## 0x00494900 CFrontendGameComponent::Output: RELOCATION_MATCH (attempt o9)
- The last residue (10 sites: retail loads CursorHighlight `mov eax,[esi+1E44h]` before the `fadd` that steps y) went away when the font
  height became a named float temporary (`line_height = MainFont->GetVFontMaxHeight(); y += line_height;`). Every spelling of
  `y += MainFont->GetVFontMaxHeight()` / `y = y + ...`, colour-before-y orders, helpers and the /G5 /G6 /G7 /GB /Ox diagnostics kept the
  fadd first.
- Names: the version string 0x013B85E0 is bound as FableFullBuildVersion (landed GFParseCommandLine spelling, kept); Ego_r GVersionString
  is probable, not proven. g_WorldToLoad 0x013B8668 is the landed spelling.
- engine/CSystemManager.h (retail branch) puts PFontManager at +0x7C and the unnamed 8-byte composite at +0x84; retail has them the other
  way round (Output and the landed ~CText 0x0054F6E0 read the font manager at +0x84; the ctor 0x009A7440 zeroes four counted pointers at
  +0x6C/+0x74/+0x7C/+0x84, Graphic +0x6C and Mesh +0x74 by landed readers). The row uses the shared header with the nested opt-in
  FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER (hdr/). Header debt: fold that order into the FABLE_RETAIL_CSYSTEMMANAGER branch once its
  includers are rechecked (~CText 0x0054F6E0 still carries a local CSystemManager model).
- 0x0121AD80 now uses engine/CRightHandedSet.h (FABLE_RETAIL_CRIGHTHANDEDSET + _CTOR_INLINE, with FABLE_RETAIL_C3DVECTOR, which its GetRight
  body needs); the landed siblings 0x0121AC30 / 0x0121ACD0 / 0x0121AD50 still carry local CRightHandedSet models (vland warning
  SHARED_HEADER_AVAILABLE there too).

## 0x009D9C80 CRenderManager2D::DrawBufferedPolys: parked, DIFFER(3451v3435), 17 lines, 710 graded attempts
- Residue: the `memset(&current_window,0,16)` before the alpha list: retail loads ecx first and zeroes eax, ours zeroes edx and eax, then two
  loop-alignment pads move. Everything else matches.

## 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed: parked, DIFFER(3385v3442), 464 lines, 51 attempts
- Best: e20 (single `PeekTC(TCI_TARGETING)` with the goto, a temporary `IS_THING_ALIVE(const_cast<...>(*it))` so the wife stays in edi,
  `GetFrameBribeEnds()` getter). e4 ties on raw md.
- Structural finding: retail tail-duplicates the region_def-dtor epilogue at the `GetDeedFromHandle(handle) == 0` return and jumps to
  the shared one from the bribed and criminal returns. `goto finished` on both (e21) reproduces that (normalised diff 401) but repacks
  the frame (crowd_group at [esp+24h] vs retail's [esp+30h]; retail shares that slot with the key temporaries and the `me` spill).
- Other residues: retail keeps the redundant `cmp esi,ebx` of the guard-effects CDefPointer assignment; register allocation in the flirt
  section (hero in edi then spilled, ebp = 0 in the wives loop).
- Descriptive names (marked in the source): CThingAICreature Partner +0x188, Flags1c9 +0x1c9, COpinionDeed Victim +0x18, CTCVillage
  FrameBribeEnds +0xa8 / CrimesSuspended +0xb3 (the FableWin getter ?GetFrameBribeEnds@CTCVillage@@QBEJXZ is proven, the member name is
  not), CTCPerceiveThing LineOfSight +0x2f, g_UnknownThreatMarkers 0x01375733.

## 0x00648820 NPlayerGui::CDrawMiniMap::Draw: parked, DIFFER(3419v3464), 648 lines, 50 attempts
- Best valid: d19. The six region-change tiles match after `tile_height = ...; tile.BRY = tile.TLY + tile_height;` (a named temporary,
  the same lever as Output). d22/d23 scored lower only because their invented helper stayed out of line, so they are not candidates.
- Borders: FableWin.pdb / Ego_r.pdb `??H@YI?AVC2DBoxF@@ABVC2DVector@@ABVC2DExtentsF@@@Z` (C2DBoxF operator+(const C2DVector&, const
  C2DExtentsF&), __fastcall, inline in retail). With it, and float locals for pos.X / pos.Y (d26), the x87 sequence matches. Retail then
  copies the GFToScreenScaled result into the box (movsd x4, the box at the argument temporary's slot). Every copy spelling (d28-d32,
  d37-d40, d49-d52) moves `this` into ebx for the whole function and drops the `lea ebp,[esp-70h]` bias.
- Frame 0x324 vs retail 0x334 (one C2DBoxF): the missing GFToScreenScaled return temporaries.
- The source still uses local C2DVector / C2DBoxF / CThing / CFadeInFadeOutBase models (the shared C2DVector has a user copy ctor and
  pack(1)); porting to the shared headers is part of the next idea.
- Names: the second parameter `forceDraw` is descriptive. CGraphicDataBank slots 23/24 GetGraphicWidth / GetGraphicHeight as the landed
  CStatBar row.

## Label corrections (also in NAME_FIXES.tsv)
- 0x00D3B410: rebuild/oracles/auto-re-candidates.tsv still labels it CGameflowScript::DeclareGossipCategories (vland RELOC_NAME warning on
  0x00D3B3D0); the landed source was respelled to NScript::CQ_GuildTrainingScript::GossipSetup on 2026-11-08 (chunk 853), which this row
  uses.
- 0x0082C060: catalog / manifest label `TailJmp_DrawMap_0082c060` should be CMiniMapDisplay::DrawPreparePrimitivesForEngine
  (?DrawPreparePrimitivesForEngine@CMiniMapDisplay@@QAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@E_N@Z: both PDBs; Draw passes
  (info, box, alpha, RegionChange) matching that signature; its body is a tail `jmp` to DrawMap).
- 0x00829D20: landed CMiniMapDisplay::IsPositionInsideClipEdges should be CMiniMapDisplay::IsThingAsPosBeingDrawn.
- 0x0048DA30 IsOpen is const (QBE); 0x004348D0, 0x00643CA1 carry stray spellings in reloc_bindings (NAME_FIXES.tsv).
