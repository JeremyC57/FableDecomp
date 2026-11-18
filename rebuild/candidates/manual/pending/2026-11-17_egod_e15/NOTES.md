# e15 notes (w23 egod IDENTIFY batch)

## Identities (all six proven from two sources; see NAME_FIXES.tsv)
- 0x00BBA1E0 = CEngineInternalPrimitiveParticleGroup::`scalar deleting destructor' (RTTI vtable 0x012A59D4 slot 0; FableWin ??_G symbol; Ego_r ??1 order)
- 0x00BF2980 = CEngineTreeFilerBase::GetTreeMemSize (Ego_d order + locals; Ego_r/FableWin symbol before DeleteTree)
- 0x00498410 = CProgressDisplay::GetCharWidthW (private static fastcall)
- 0x006D63F0 = CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete
- 0x009BCE70 = CXMVPlayer::GetCharWidthW (public static; font CXMVPlayer::PlayerFont 0x013CA7F0)
- 0x009BCEF0 = CMovieSubtitles::GetCharWidthW (public static; font CMovieSubtitles::PFont 0x013CA7F8)

## 0x00498410 CProgressDisplay::GetCharWidthW -- RELOCATION_MATCH, test PASS (2 grades)
- Ported onto the shared headers (fable_string.h, engine/CFontBank.h under FABLE_RETAIL_CFONTBANK, engine/CProgressDisplay.h): the
  header's inline GetVStringExtents writes X back into the extents slot (the old draft's "own frame word" residue disappears), PFont->
  is the CCountedPointer inline operator-> (Data via eax).
- The decisive spelling: an IMPLICIT float -> unsigned int return conversion (`return PFont->GetVStringExtents(char_text).X;`):
  VC7.1 runs ~CWideString before `fld ; __ftol2` (retail order). The explicit `(unsigned int)` cast variant was not measured (timed out
  in fg, so it is unmeasured). Ego_d locals: c, char_text only (no extent / width local).
- No header overlay, no flags. Test compiles the body verbatim against recording doubles.

## 0x009BCE70 CXMVPlayer::GetCharWidthW / 0x009BCEF0 CMovieSubtitles::GetCharWidthW -- RELOCATION_MATCH, tests PASS
- Same text as 0x00498410 plus Ego_d's `unsigned int res` (both list c, char_text, res): `unsigned int res = <font>->GetVStringExtents(char_text).X; return res;`
  matched at the first grade (fg x1); retail bytes of all three rows are identical apart from the font address.
- New overlay opt-ins FABLE_RETAIL_CXMVPLAYER_GETCHARWIDTHW (CXMVPlayer.h: GetCharWidthW + private static PlayerFont 0x013CA7F0) and
  FABLE_RETAIL_CMOVIESUBTITLES_GETCHARWIDTHW (CMovieSubtitles.h: GetCharWidthW + protected static PFont 0x013CA7F8); hdr/README.md.
- COORDINATOR: 0x013CA7F8 is bound in reloc_bindings to `?g_pSubtitleFont@@3PAVCFontBank@@A` (landed CMovieSubtitles::Play 0x009BD910,
  a descriptive extern CFontBank*). The real symbol is CMovieSubtitles::PFont (protected static CCountedPointer<CFontBank>:
  `?PFont@CMovieSubtitles@@1V?$CCountedPointer@VCFontBank@@@@A`). Two symbols on one address: respell Play at landing (NAME_FIXES.tsv).
  0x013CA7F0 is bound to the placeholder `?g_pointer_013ca7f0@@3UCCountedPointerStatic@@A`; real: `?PlayerFont@CXMVPlayer@@0V?$CCountedPointer@VCFontBank@@@@A`.
- vland (read-only): 3 WINS, RELOCATION_MATCH + PASS; only WARN STANDIN_LEAF (catalog still carries the made-up _global::GetCharWidthW label: the name fix in NAME_FIXES.tsv resolves it). No RELOC_IDENTITY.

## 0x00BF2980 CEngineTreeFilerBase::GetTreeMemSize -- RELOCATION_MATCH, test PASS (2 grades)
- Shared header engine/CEngineTreeFiler.h (_INLINES, _NODEHEADER_STRUCT) + new overlay opt-in FABLE_RETAIL_CENGINETREEFILER_MEMSIZE.
- What closed the old "strength-reduced child walk" residue: Ego_d's locals (table = (CNodeHeader**)(node + 1), memcount, block-scoped
  `sb` loop counter) and the same-TU GetNodeMemSize defined as a plain member before it (expanded for the leaf case). First grade.
- GetNodeMemSize's out-of-line copy compiles byte-identical (by objdump) to retail 0x00BF2950 (38 bytes, NOT in the catalog: no
  oracle). COORDINATOR: catalog 0x00BF2950 as ?GetNodeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z (Ego_r 0x00AAA0B0,
  FableWin 0x02EF0510); 0x00BF2940 (14 bytes, operator delete of the arg, `ret 4`) is presumably FreeNode (Ego_d order FreeNode,
  GetNodeMemSize) -- unverified, not graded.
- Test: builds trees in a mirror of the 28-byte header (the class's members/nested type are default-private), reaches the members
  through a deduced member-pointer template; a planted child just past each table kills the `<=` mutant (verified FAIL).

## 0x006D63F0 CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete -- RELOCATION_MATCH, test PASS (8 grades incl. 4 SRC_FAIL include fixes)
- Shared headers (CTCHeroExperience.h retail branch, CHeroExperienceDef.h) + overlay FABLE_RETAIL_CTCHEROEXPERIENCE_FRACTIONCOMPLETE (decl).
- Ego_d's three float locals (param_per_level = PDef->..., multiplier = param / param_per_level + 1.0f, diff = multiplier -
  (long)multiplier) in the landed GetRunningNumHits 0x006D63A0 text fixed the reciprocal order (`fld [def+64h] ; fdivr 1.0f`) at once.
- Last residue (64v68): `return diff / param_per_level;` lets VC7.1 tail-duplicate the division into the clamped path (`fmul 0.0f`);
  retail merges. Closed by the in-place `diff /= param_per_level; return diff;` (GFLimitLower(diff, 0.0f) and a `?:` clamp: no change).

## 0x00BBA1E0 CEngineInternalPrimitiveParticleGroup::`scalar deleting destructor' -- OPEN, DIFFER(30v82), differ/00bba1e0.cpp
- Ported onto shared headers (CEngineInternalPrimitiveParticleGroup.h retail branch, STLport list with _LIST_CLEAR_OUTOFLINE, CArray)
  with three new overlay opt-ins: FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE_INLINE_DTOR, FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_INLINE_DTOR
  (empty inline base destructors: no Ego_r ??1, FableWin debug-only ??1) and FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_CTOR
  (FableWin protected ctor decl; its out-of-line definition in the row only emits the vtable / ??_G).
- Residue: our ??_G is `push esi; mov esi,ecx; call ??1; test flags; delete` (30 bytes); retail expands the destructor (82 bytes).
  Our implicit ??1 itself already has retail's no-own-vtable-store shape and only the root 0x012354D0 store (dead stores of the
  inlined empty base dtors eliminated), but it expands ~_List_base<long> (clear call + free) where retail 0x00BBA140 / 0x00BBA1E0
  CALL ~_List_base<long> 0x0042AB73 and expand list<CEnginePrimitiveHandle>'s.
- Tried (all still `call ??1`): /d2inlT200, /d2inlT400, /Ob2, /d2inlS100, /d2inlS1000; the implicit dtor ODR-used before the ctor
  (explicit `g->~X()` in an earlier function); a derived class with an implicit dtor; a user-declared in-class `virtual ~X() { }`
  (temp overlay; also called); pass 2: an out-of-class user `X::~X() {}` (temp overlay), FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE (smaller
  ??1), /GX -- all still `call ??1`, so it is not an inline-size decision. Earlier (draft): `__forceinline ~X() {}` expands it but adds the group vtable store at entry; a
  local-model novtable drops ??_G.
- Observation for pass 2: the landed CEnginePrimitiveAttachmentManager ??_G 0x00B3ADC0 gets its destructor expanded via an
  OUT-OF-CLASS user definition in the TU (`X::~X() {}`), which VC7.1 compiles before the ??_G. Ego_d says this class's dtor is
  compiler-generated, so a user ~X would store the group vtable unless it is dead... untested here. Also the list<long> ~_List_base
  call vs list<Handle> expansion split is unexplained (both are `clear(); deallocate` with clear declared only).

## Final check (after all overlay changes)
- All five matched rows regraded on the final hdr/ overlay: RELOCATION_MATCH; all five tests PASS under vt.py.
- Read-only vland: WINS 5, no warnings, no RELOC_IDENTITY (payload names are the plain method names; each TU has one symbol of
  that name -- the earlier mangled-leaf payload gave WARN STANDIN_LEAF only).
- Mutants verified to FAIL: 0x00498410 X->Y, 0x00BF2980 `<` -> `<=`, 0x006D63F0 clamp threshold 0 -> -1.

## Review fixes (after advisor check)
- Wide char spelled `unsigned short` in the two new opt-ins, the three row sources and tests (same type / mangling under VC7.1).
  COORDINATOR: engine/CProgressDisplay.h (repo) still declares `GetCharWidthW(wchar_t c)`; not changed (not my opt-in).
- Moved plain asserts pinned: CMovieSubtitles.h `FABLE_STATIC_ASSERT_AT(32, ...)`; CEnginePersistentPrimitiveBase.h `_AT(38/60/61/62, ...)`.
- FITTED marks: 0x006D63F0 (`diff /= param_per_level; return diff;`), 0x00498410 (implicit return conversion).
- 0x006D63F0 test: added param -1 / per-level 2 (0.25; 0 without the +1); the dropped-`+ 1.0f` mutant FAILs.
- Tests use `#define private public` (and `protected public` for CMovieSubtitles) around the shared-header includes to call the
  private / protected statics; 0x00BF2980 reaches default-private members through a deduced member-pointer template. No rule found
  against it; COORDINATOR may prefer another access route.
- After the fixes: all five regraded RELOCATION_MATCH, tests PASS, addmatch re-run, vland WINS 5 with no warnings.
- LABELS.tsv also carries 0x00BF2950 GetNodeMemSize (uncatalogued; proven, see the 0x00BF2980 section).

## 0x00BBA1E0 next steps (pass 2 / next author)
- Solve the implicit ??1 first: ours expands ~_List_base<long> where retail 0x00BBA140 calls 0x0042AB73; grade the same TU at 0x00BBA140.
- Check the TU: Ego_d's Decals.obj list has no ParticleGroup member; retail order ??1 0x00BBA140, ParticleDecalGroup ctor 0x00BBA180,
  ??_G 0x00BBA1E0, DecalGroup ??_G 0x00BBA240 suggests Engine_Primitive_Manager_Particle_Decal_Group.obj (or Particle_Group.obj) is
  the real TU. Model retail's trigger there (inline base ctor + derived inline ctor + `new` in CreateInternalPrimitive) instead of the
  out-of-line base ctor used here.
- TU check done (procmod.tsv): Ego_d emits CEngineInternalPrimitiveParticleGroup's ctor, `scalar deleting destructor' and
  ~CEngineInternalPrimitiveParticleGroup in Engine_Primitive_Manager_Particle_Group.obj (ctor also in ..._2D_Particle_Group.obj);
  Particle_Decal_Group.obj has the DecalGroup ctor / ??_G / ~ and CreateInternalPrimitive. ref/00bba1e0/egod_tu.txt (Decals.obj)
  is the wrong object for this row; the next attempt should start from Particle_Group.obj's list.
