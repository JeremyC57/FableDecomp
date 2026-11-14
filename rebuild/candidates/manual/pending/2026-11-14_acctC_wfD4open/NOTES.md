# wfD4 NOTES (acctC lane, 2026-11-14)

## 0x0066CF00 CRegionMinimap::PopulateRegionMinimap -- parked DIFFER(3236v3248)
- differ/0066cf00.cpp = earlier draft with the stale hdr opt-in FABLE_RETAIL_CDEFPOINTER_ASSIGN_REREAD replaced by today's repo opt-in
  FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD and `out->SetDynamicCast(def)` in the in-row inline GetDef<T> body (the shared
  GETDEF_GLOBAL_BODY is noinline; retail expands GetDef here, call GetPDefFromGlobalIndex 0x009AD9E0 at 0x0066D01A). No hdr change needed.
- Residue: frame 0x1ec v 0x1e4; efm frame map (tools/fa.sh + efm.py) shows every frame symbol on a different slot (packer order);
  x (retail ebx, stored [esp+28h]) and themeDef (retail ebp, reloaded from [esp+7Ch] each outer iteration) swap registers.
- Tried (no change at all, identical output): declaration order of strength / themeDef / position (3 variants); helper opt-ins
  C2DEXTENTSI_INITIALISE, C2DCOORDI_COPY, C2DEXTENTSI_FORCEINLINE_CTOR.
- Next: derive retail's slot membership (which temporaries share) from the efm map, then node-removing spellings (lessons r3016).

## 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update -- DIFFER(3072v3080), in progress
- differ/00974aa0.cpp: earlier draft ported onto shared headers (local CJackOfBladesBattleDef / CTCBoss / CTCWillResponse /
  CAIStateGroup_JackOfBladesBase removed; Stage2 + the Jack actions stay local classes deriving the shared bases because their
  generated headers are flat pack(1) structs, as in the matched 0x00945060). New overlay opt-ins (hdr/README.md):
  FABLE_RETAIL_JACKOFBLADESBASE_STAGE2_UPDATE (condition extension, IsStillRunnable & co), FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARSOLD,
  FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE (condition extension), wfA2's FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE (same text).
- Levers that moved it: visible GetSubDef<T> body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY + inline StatGetTypeName body in the row)
  = escape analysis: retail keeps battleDef.Object in edi across calls and drops the null test in later releases (2988 -> 3056);
  implicit `"..."` CCharString conversions (retail re-reads the temp's slot, `lea ecx,[esp+14h] ; push ecx`); HASTC_MATERIALISED;
  FABLE_RETAIL_C3DVECTOR + DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO (retail `fld [129BA3Ch] ; fmul [129BA3Ch] ; fcompp`); GET_ROUNDED_ANGLE
  inside the SetFacingAngleXY call (retail loads the physics TC before the wrap).
- Open: (1) facing blocks: retail computes diff Z,Y,X and spills Y [esp+10h], X (fst) [esp+28h]; ours keeps X,Y in x87 regs
  (tried: IsNonZero variants, explicit xyz sum, reference components -- no change). (2) 0x974B1A: retail keeps PeekBaseDef in edi.
  (3) second HasTC (TCI 0x30): retail `mov ecx,[eax+24h] ; shr ecx,10h`, ours narrows to a byte load. (4) scale-up loop 0x974EBC..,
  fireball / summon blocks 0x975100.., 0x9754EE (a 29-instruction block placed differently), frame 0x808 v 0x80c.
  More tried on (1): `C3DVector diff(a - b)` (no change), named `const C3DVector&` positions (3056, worse).
- Next: x87 spill of diff X/Y (c2 x87 allocator: find what makes X and Y memory-homed -- e.g. a by-reference use of diff in the
  original such as an inline helper taking `const C3DVector&` for the angle), then the scale-up loop block 0x974EBC.

## 0x0049A0B0 CProgressDisplay::InitialiseTextures -- near-miss DIFFER(2922v2920) (successor session)
- differ/0049a0b0.cpp: re-authored on shared headers (engine/CProgressDisplay.h, CTexture SIZED / NEXTVALIDDIMENSIONS, CSurface BITMAP +
  COPYFROMMEMORY (inline dtor), CDisplayManager FRONTEND / TEXTUREFORMATS, CDiskFileWin32 ISOPEN_INLINE, CStringParser, CPersistContext
  NAMED_DEFAULTS, fable_string EndsWith); RandFunc { unsigned long* Seed; } + ctor in the row (FableWin ??0RandFunc@@QAE@PAK@Z, ??RRandFunc@@QAEJJ@Z,
  the landed 0x0049B1E0 model); globals / GetMiscDir / LongToWideString declared in the row as the landed siblings do.
- Levers, in the order found (each measured): (1) CSURFACE_COPYFROMMEMORY = inline ~CSurface (vptr store + Uninitialise 0x009F2E20);
  (2) CWIDESTRING_TEXT_EQ + _STLP_INLINE + STLP_CHAR_TRAITS_LENGTH + FORCEINLINE_DATA_TEXT_EQ + FORCEINLINE_EQ: retail expands the
  GResetProgressTipText == L"" test (wcscmp 0x00BFE94C on null, compare 0x00403C60 n=0); (3) NEW overlay opt-in
  FABLE_RETAIL_CSTRINGPARSER_DTOR_OUTOFLINE (virtual ~CStringParser decl, retail calls 0x004047A0 at 0x0049A9F5);
  (4) STLP_VECTOR_PUSHBACK_NOINLINE (retail calls push_back 0x00485FF3 / 0x0049B6B0 but keeps backgrounds._M_start in ebx after the
  loop: body visible, nothing escapes) -- OUTOFLINE (declaration) keeps this out of ebx; (5) `tag.EndsWith("0")` implicit conversion;
  (6) `for (long i = 2; ...)` (retail stores the index home at each def, 0x0049A26E / 0x0049A284); (7) one else-scope `C2DExtentsI size`
  reused by `size = C2DExtentsI(x, y)` for the seals (64, 64) and all six bars (retail `mov eax,X ; mov ecx,Y` then stores into the
  bg-size slot) -- this closed the bars, the seals, the debug-branch cross-jump and the tip block (2923 -> 2922, instruction-equal).
- Residue: frame 0xb4 v 0xb0 only. efm (tools/fa.py + efm.py, tmp/efm_b2.txt): ours puts texture with the debug-branch temps (T C2DExtentsI 8,
  T CSurface 16) at -100 above file and size with the debug size at -172; retail puts size with the debug temps at -132 (below this) and
  texture with the debug size at -172, and has fewer 4-byte slots. Tried with no frame change: texture before count (f1), a separate bars
  size local (c1: size goes to -100, texture to -172 -- retail's texture place, but bars then get their own node), C2DExtentsI
  INITIALISE / FORCEINLINE_CTOR, `size = C2DExtentsI(0x400, 0x200)` for bg (worse 2938); seed after the vector (v1), RandFunc before the
  loop (v2), `name(...)` v `name = ...` (v5/v6): frame 0xb4 unchanged; debug `size` as an assigned local (v4) or one function-scope size
  shared by the debug and else branches (v7/v8): 0xb8 (worse). Frame probes with tools/fm.sh (compile only, prints sub esp + 8-byte+ slots).
  efm detail (retail, own frame top): context -44, parser -84, file+CSurface temps -112, this -116, {size, debug C2DExtentsI temp,
  debug CSurface temp} -132, {seed bg, seed tip, debug GetMiscDir temp, seal operator+ temp} -136, backgrounds -148, -156 (8),
  display -164, format -160, {texture, debug size} -172, -176 (4); ours has one 4-byte slot more (-180: contents / i / names / tag).
- Next: the frame packer (density sort weight*1000/size, lessons r3011 / r3016): find what makes retail texture denser than size or a
  node fewer (e.g. the debug branch's `C2DExtentsI size = GetNextValidDimensions(...)` reusing a variable, the debug GetDimensions
  temporaries, format / display homes); a packer replay tool would settle it (the /opt tools named in the lessons are not on this machine).

## 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera -- parked, baseline graded only: DIFFER(2860v3356)
- differ/00715a70.cpp = earlier draft unchanged; 496 bytes short (focus cases cross-jumped, see its line 2). Not worked this session.

## 0x006D1E30 CTCWife::FrameUpdate -- parked, draft does not compile against today's headers (SRC_FAIL)
- differ/006d1e30.cpp = earlier draft + FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER / CTCHEROSTATS_MARRIAGE / CTCHEROSTATS_SERIALISE /
  CTCWIFE_MARRY defines. Remaining cl errors (need header opt-ins or re-spelling): CTCCreatureOpinionOfHero RelativeOpinion /
  OpinionValues, CTCHeroStats::PeekMarriageInfo, CThingAICreature::IsCourtable, COpinionReactionManager::GetReactionMatchStrength,
  CTCCreatureOpinionOfHero::SetPersonalityOverride(long) overload, CTCSoundPlayer::SetSpecificSampleVolume, std::map::erase.
  Compare with wfA1's differ/006d0830.cpp define set (same class). Not graded this session.

## 0x0090D030 CAIStateGroup_MinionGossip::Update -- parked, baseline graded only: DIFFER(3243v3269)
- differ/0090d030.cpp = earlier draft unchanged; fully local models (C3DVector, CThing, CIntelligentPointer, CTCGossip, filters...).
  Must be re-authored on shared headers in the 0x00945060 style (class deriving the shared CAIStateGroupBase) before residue work.
