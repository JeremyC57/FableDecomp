# wfA1 NOTES (acctC lane)

## 0x006D0830 CTCWife::GlobalUpdate -- PARKED DIFFER(2581v2584), differ/006d0830.cpp (~30 grades)
- Name: Ego_r 0x00705510 / FableWin 0x02483170 ?GlobalUpdate@CTCWife@@UAEX_N@Z, vtable 0x0125ED5C slot 26 (draft). Extent ok (`ret 4`).
- Port of C42's unported draft to today's headers: switched fable_game.h -> engine/CMainGameComponent.h (as landed CTCWife::Marry);
  GetHomeBuilding inline body in the row (as landed 0x00946400 / 0x00958840) -- without it 2571 (out-of-line call);
  dowry wait `CMessageEventManager& messages = GetWorld().GetMessageEventManager(); if (messages.Find...)` (filter store after the
  manager load, EVIDENCED by bytes 0x006D1100..0x006D110A); IsScreenFullyFadedOut = direct member tests (FITTED: IsOpen() on the
  subobject kept `add eax,0BCh` + [eax+0Dh]).
- New overlay opt-ins (hdr/README.md): CDISPLAYENGINE_ENDSCREENFADEOUT, CDISPLAYENGINE_ISSCREENFULLYFADEDOUT, CSUBTITLERENDERER_CUTSCENEFLAGS,
  CTCSOUNDPLAYER_ATTACHEDDIALOGUE, CTHINGCREATUREBASE_AUTOPLACENEAR, CTHINGCREATUREBASE_GETSEX, CTCVILLAGE_ONHEROSLEPT,
  CGAMECAMERAMANAGER_RESETCAMERA, NHEROINFORMATIONSCREENS_CMANAGER_ISOPEN; CWifeDef.h (C42 new header) gained StatGetTypeName + virtual decls.
- RESIDUE: (1) wedding: retail movie call pushes 8 immediate zeros; ebx (= loop index i, copy-propagated zero for the FastForwardTimeTo
  0.0f push and the vector's three zero stores) is first set at 0x006D0928. Ours sets ebx=0 before the movie pushes and on the no-movie
  arm (`jmp ; xor ebx,ebx`), i.e. i's zero is hoisted through the join. Diagnostic: without later zero uses the movie pushes are
  immediates, so the hoist comes from i. (2) dowry: wife_def in ebp (retail ebx) -- probably coupled to (1).
  Tried (no change): dowry decl orders (3), i declared before the for, villages in own scope (worse), ESex local, named movie string,
  movie call in own block, no time-manager ref (worse), loop index long/int, count long.

- SUCCESSOR c2 trace (c2prio.py --trace --blocks on differ/006d0830.cpp; tmp/pt_base.trace): both residues are const-0 splitting.
  The wedding loop index `i` (#23) is coloured ebx (only ebx left), which triggers split passes of `const 0` (#1, prio -105,
  allowed ebx only); piece #29 (prio 18 -> ebx) covers c2 block 8 (the movie call, lines of the movie + quest string; its 8 zero
  pushes are cost-0 references of const 0) AND block 9 (the join: 0.0f push + the vector's 3 zero stores), hence the hoisted xor
  on both arms. Retail's piece starts at block 9. The dowry: wife_def (#38, allowed ebx/ebp) gets ebp because ebx carries a
  201 penalty = a const-0 piece left with only ebx is a live neighbour (100 x weight 2 + 1); in retail that piece is absent in
  the dowry region -> coupled to the piece layout. Lead: whatever makes block 8's zeros NOT const-0 references (or splits
  block 8 from block 9) closes both.

- SUCCESSOR measurements (8 grades + 3 c2 traces/what-ifs): DIAG movie zeros -> 5 (all 8 args) removes the wedding hoist completely
  (retail allocation there) => the wedding residue is exactly "block 8's zero pushes are const-0 references". Defaults on the
  declaration (`GFPlayFullScreenXBoxMovie(name, 0x1b)`, FableWin signature + the landed 0x0041E6D3 default list) give the SAME
  bytes (re-checked). The dowry residue is NOT coupled: it stays with the movie diag. Dowry priorities: dowry #39 = 20,
  wife_def #38 = 19 (keys 659/662). What-if wife_def forced to 21 (tools/wf/wf.py '38:19=21') -> wife_def edi, dowry ebp
  (not retail either): retail = dowry coloured first (edi), then wife_def ebx WITHOUT the 201 ebx penalty, i.e. the const-0
  piece live in the dowry region must still allow another register in retail. Dowry spellings tried: ternary init (2583),
  nested ifs, named `const CWifeDef* def = wife_def.Get()` (no change).

## 0x00DFCA90 NScript::CQ_TraderConflictGoodScript::CBanditExtra::Main -- RELOCATION_MATCH 373v373, test PASS (3 grades), addmatch + vland PASS
- Name: Ego_r ?Main@CBanditExtra@CQ_TraderConflictGoodScript@NScript@@EAEXXZ (0x00E3D170) + landed Alloc 0x00DFBED0 (vtable 0x012DEE14 slot 1). Extent ok.
- Written by hand from the disassembly in the CTraderToRescue::Main 0x00DFE0F0 idioms (shared NScript headers). Callees: Pause (376),
  IsDistanceBetweenThingsOver 0x00CBE3EA (not ...Under), CreateCreature (91, implicit CCharString temps), GetDistanceBetweenThings,
  GFFloatToLongCeil (shared retail-asm helper; row cites the expansion in a `// Retail-asm:` line -> purity retail_asm), rand, GiveThingBestEnemyTarget (451), UpdateLiveEnemies.
- New opt-ins: FABLE_RETAIL_CQ_TRADERCONFLICTGOODSCRIPT_BANDITEXTRA (class decl after CTraderToRescue's; new assert at file end, none moved),
  FABLE_RETAIL_SCRIPT_GETDISTANCEBETWEENTHINGS (fable_script_distance.h). No flags.
- EVIDENCED: every statement (bytes). FITTED: local names (bandit, chance); `* (1.0f / 15.0f)` (same constant as 0.06666667f).

## 0x00C271F0 CPhysicalPlayer::Play (descriptive, reloc-bound name) -- RELOCATION_MATCH 846v846 (successor), src/00c271f0.cpp, test PASS, addmatch + read-only vland PASS (no RELOC_IDENTITY; RELOC_NAME warns only = manifest labels of libpk callees)
- FIX-UP (wfA1 fix-up team, 2026-11-14): the local `class C3DVector` was rejected by LANDVERIFY_STRICT_HEADERS=1 vland (HEADER_POLICY).
  Now `#include "engine/C3DVector.h"` with the new additive opt-in FABLE_RETAIL_C3DVECTOR_SWAPYZ (decl only; hdr/README.md). The shared
  header includes <math.h>, so the old local `__declspec(dllimport) double __cdecl ceil(double)` became C2375 (C++ linkage) and an
  `extern "C"` dllimport placed before the include is overridden by math.h (direct `call _ceil`, DIFFER 845v846). Closer: flag /MD
  (EVIDENCED: retail links MSVCR71 and calls ceil via IAT `call [0x014400F4]` at 0x00C2738B; /MD is the CRT model of the vendor
  libs in docs/pipeline/LIBRARY_LINKING.md) -> `_CRTIMP` = dllimport -> RELOCATION_MATCH 846v846. Flags now `/EHsc /MD` (payload too).
  vt.py PASS, addmatch redone, LANDVERIFY_STRICT_HEADERS=1 read-only vland: RELOCATION_MATCH PASS (RELOC_NAME warns only), 00dfca90 PASS too.
  Coordinator: the landed libpk siblings 0x00C1C280 / 0x00C27550 still carry the same local C3DVector (could respell on the opt-in).
- SUCCESSOR closer (6 grades): the scratch registers of the whole tail (from the 3D block on) were rotated by one (retail
  `lea edx,[esp+20h] ; lea ecx,[eax+4] ; push edx` before SwapYZ; ours pushed the return pointer first in ecx). A diagnostic with
  CODA_SourcePlay(src, 0) showed the rotation is independent of the ebp save. Reading the position through a named
  `const C3DVector& eventPosition = desc->m_position;` then `position = eventPosition.SwapYZ();` (FITTED) fixed the rotation AND
  the ebp save (the late flag then takes ebx, ebp nests mid-body). An inline const-ref accessor or a tail-scope decl did NOT.
- (earlier agent's notes follow; the old residue below is closed)
- Name: the landed caller CSoundEvent::PlayPhysical 0x00C20A20's reloc binding ?Play@CPhysicalPlayer@@QAEXPAUCSoundEventManager@@PAUSPlayDesc@@PAUCSoundEvent@@PAUSEventLevel@@MM@Z
  (no PDB covers libpk). Extent ok (EH frame, `ret 18h`). The player is the class landed 0x00C27550 / 0x00C276B0 call CCodaPlayback
  (vtable 0x012B4034, same offsets) -> coordinator: one class, two descriptive names.
- Written by hand, local libpk models in the landed rows' style (Flags /EHsc, PKCProtectionInstance shared header, extern "C" __stdcall CODA API).
  What closed most: `buffer->m_format.X` spelled per access with a named `CStreamBuffer* buffer` (retail reads the format fields via
  ebp+0x24/26/28 and calls the format methods on a CSE'd lea edi,[ebp+18h]) -> 846v846; HasLoop returns long (`test eax,eax`);
  CODA_SourcePlay's flag param long + an inline `bool IsLooping() const` (retail `setne al ; movzx ecx,al`).
- RESIDUE: retail saves ebp mid-body (push 0x00C27283 after the source is created, pop 0x00C2742D right after m_players.Add) and
  computes the late looping flag in ebx; ours saves ebp at entry and uses ebp for that temp (all later esp offsets +4, 3 disp32 bytes).
  Tried (no change): same-TU bodies of 0x00C271E0 / 0x00C271B0 above the row, early return instead of the `offset < m_length` block,
  locals declared at top, bool/ternary/local spellings of the flag, format pointer local. GOTCHAS l.358 (nested mid-body saves need
  the earlier-pushed registers to have no hoistable restore) is the lead.
- Descriptive callee names (FITTED where no landed name fits): CODA_SetSourceEnvelope 0x00C35B30, CODA_CreateSample 0x00C36080,
  CODA_SetSourceRangeCallback 0x00C33F20, CODA_SetSourcePitch 0x00C35B10, FullRange 0x00C271B0, UpdateManagerTime 0x00C271E0,
  CSoundEventManager::GetTime 0x00C11700, CPlayerList::Add 0x00C21E90, CSoundTicker::Update 0x00C1C460, CStreamBuffer::GetData 0x00C26240;
  the landed rows' symbols kept where they fit (AudioSystem_SetModeFlags / _SetOutputFlags, CODA_SetSourceVolume, CODA_SetSourcePosition,
  AudioObject_SetSource, CODA_SourceSetPlayCursor, CODA_SourcePlay, CAudioFormat::GetBlockAlign / GetLoopEnd / GetBytesPerSample).

## 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- first draft DIFFER(750v910), differ/006b2ca0.cpp (3 grades)
- Name: Ego_r 0x006EE710 / FableWin 0x0244D960 ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ + reloc binding of the landed caller
  Update 0x006B3FF0 (call 0x006B42A4); header already declares it (private). Extent ok (`ret`, 910). Anniversary PDB locals:
  sun_vector, moon_vector, diffuse_vector (C3DVector). FableWin debug body 0x3F0 bytes (helpers out of line there).
- Opt-in set copied from the landed RenderUpdate 0x006B42F0 (CENVIRONMENT + TICKSTATES + RENDERUPDATE + SCREENEFFECTS ...; TICKSTATES alone
  fails the layout asserts). Reads: GameTickState.Time (+0xC0C), BlendedTheme[0].Scalers[5] (+0x19C), [8..13] (+0x1A8..+0x1BC),
  DiffuseClampAngle (+0x40); writes SunVector +0xC30, MoonVector +0xC3C, DiffuseVector +0xC48; AccurateNormalise 0x00A14440 x4. Constants:
  2*pi as double 0x0124F2B8, 0.0f 0x0122DEDC (ten `fmul` by zero kept: rotation matrices with constant 0/1 entries), 1.0f 0x0122DED8.
- DECODED EXACTLY (0x006B2EDF..0x006B3023): sun/moon stores; `diffuse = (-Sun) * (1 - S5) - Moon * S5` (Moon*S5 evaluated first);
  AccurateNormalise; `s = sin(DiffuseClampAngle*2pi)`; `if (-diffuse.Z < s) { diffuse.Z = -s; k = sqrt((1 - s*s) / (Y*Y + X*X)); X *= k; Y *= k; AccurateNormalise(); }`; store.
- OPEN: the sun (0x006B2CB4..0x006B2DBF) and moon (0x006B2DC4..0x006B2EDA) blocks: v = (cos t, sin t, 0) with t = Time*2pi, turned by
  -S8 (resp. -S11) and S9 (S12) through 3x3 rotations whose 0/1 entries are compile-time constants, then + S10 (S13) on Z.
  tools/x87sym.py <dis> <first> <last> prints the symbolic stores; its trace shows terms like `(-sin a + sin t) * 0` (an add, `fadd st(2)`,
  then `fmul 0.0f`), i.e. the helper is NOT a plain matrix*vector -- find the FableWin helper (CMatrix33 / CRightHandedSet rotation +
  multiply) whose text gives that, fwsize.py it, then rebuild the two blocks.
- (0x00C271F0, later) also tried `default: { __assume(0); break; }` in the format switch (GOTCHAS l.358 dead-end idea): drops retail's
  reachable `dec eax ; jne` default (844) and still saves ebp at entry. Not it.
- (0x006D0830, later) villages vector declared before the time-manager line / before the InitWife calls: no change.

- SUCCESSOR (row 0x006B2CA0, ~10 grades): the rotation text is CMatrix3x4::SetYRotate / SetZRotate + TransformPoint (FableWin COMDAT
  run 0x02453F00 SetYRotate 88b, 0x02453F70 SetIdentity, 0x02454000 SetZRotate 88b, 0x024543E0 TransformPoint(C3DVector&) 168b = our
  default text SAME by fwsize). Decoded from the x87 trace: v = (cos t, sin t, 0); Y-turn by -S8 (E11=E33=c, E13=s, E31=-s), Z-turn
  by S9 (E11=E22=c, E21=s, E12=-s), Z += S10, AccurateNormalise; same for the moon (S11..S13). New overlay opt-in
  FABLE_RETAIL_CMATRIX3X4_SETYZROTATE. 750 -> 914 (ctor init) -> 908v910 with componentwise init (`v.X = cos; v.Y = sin; v.Z = 0`).
  The TransformPoint opt-in texts PARTICLE_POINT / LTR / SUMS / ZYX_SUMS all give 892 (identical: un-parenthesised rows), the default
  parenthesised one 908. Residue: retail spills cos t through [esp+20h] -> [esp+8] and keeps sin t on the stack (ours spills sin);
  retail products are `xx*E11` operand order (fld cT ; fmul cA), and the transformed X'/Y' go through frame temps copied by integer
  moves (in-place TransformPoint locals). Next: SetYRotate entry order / c,s local order (c2 symbol numbering), a `CMatrix3x4` per
  body (two locals) v one reused, the 2pi multiply inside the helper v at the call.

- SUCCESSOR further tries (all 908v910 or worse): helper entry orders (sin-first 920, no locals 914, E31 before E13 908), chained
  `E11 = E33 = c` (908; no-local chain 914, double sin 918), one matrix per body / separate Y and Z matrices / matrix declared after
  the vector (908, scalarised away), Z store first (908), Y before X (876), `float angle = (float)(Time * 2pi)` local (908, 61 hunks v
  63: tools/../probe/env/C1.cpp). Retail clue not yet reproduced: cos A is stored to TWO frame slots (`fst [esp+50h] ; fstp [esp+70h]`)
  = two matrix entries kept in memory, and the vector X store survives (cos t -> [esp+20h] = sun_vector.X, then xx copy [esp+8]) while
  the Y store is dead-store eliminated (sin t goes straight to yy [esp+0Ch]); ours keeps one c and eliminates the X store instead.
  tools/hsw.sh <tag> <src> '<SetYRotate body>' '<SetZRotate body>' sweeps helper texts on an overlay copy.

## Header-overlay regrade (successor, after adding FABLE_RETAIL_CMATRIX3X4_SETYZROTATE): includers of hdr/engine/CMatrix3x4.h among
   matched sources = style_small 00a5cb90, 00bbddd0, 00bd2d90 -> all RELOCATION_MATCH (unchanged); own src/ rows do not include it.

## 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed -- DIFFER(3439v3442), 40 masked hunks (successor 2, ~110 grades + c2/inline traces), differ/00682810.cpp
- SUCCESSOR-2 closers (each EVIDENCED by a measured mechanism; v9 = 90 hunks -> 40):
  1. list<long>::push_back EXPANDED (retail malloc 0x0068337F) is a FREQUENCY fact, not a budget flag: c2 inline score sc=(c/f+6c)/7 < T=74
     (T read from c2 0x108819A4 by tools/inl.sh); push_back c=37 needs f>0.125, ours had f=0.0315. The three early returns (hero==0,
     !GetTC, log==0) each halve the static frequency; retail has ONE: `CTCHeroOpinionDeedLog* log = 0; if (hero) hero->GetTC(TCI_HERO_
     OPINION_DEED_LOG, &log); if (log == 0) return;` (GetTC's own branch is inside the forceinline body and does not count; c2 threads the
     hero==0 / HasTC-false edges straight to the return, bytes as retail). push_back then f=0.126 (expanded), insert stays called (sc 81).
     /d2inlT and /d2inlS sweeps cannot do it (push_back<long> sc 199 > push_back<COpinionTransientOffset> 184/192 which retail calls).
  2. effects loop end cached: `for (it = def.Effects.begin(), end = def.Effects.end(); it != end; ++it)` (retail ebx = [def+4Ch] once,
     0x006833C9; ours re-read [ebp+4Ch] each pass). Length then 3442v3442.
  3. wives loop: `const CThingAICreature* wife_creature = *it;` + `IS_THING_ALIVE(const_cast<CThingAICreature*>(wife_creature))` + the same
     const_cast in PostDeed: retail keeps the wife in edi and passes IS_THING_ALIVE a TEMP copy ([esp+28h] = edi, 0x00682FA7); naming the
     non-const pointer and passing it made it address-taken (memory). This also gave the top block its zero register (ebx=0) and
     village_member its memory home (83 -> 47 hunks). (w4 = `IS_THING_ALIVE(const_cast<const CThingAICreature*>(p))` same bytes.)
  4. bribed: `GetWorld().GetFrame() < village->GetFrameBribeEnds()` (Ego_r ?GetFrameBribeEnds@CTCVillage@@QBEJXZ; new opt-in
     FABLE_RETAIL_CTCVILLAGE_GETFRAMEBRIBEENDS): retail loads +0xA8 between GetWorld and GetFrame (0x00682BE5).
  5. against_me: `crime->PTarget.Get() == &GetParentThing()` -> retail reloads ParentThing after GetPItem for GetTC(TCI_GUARD)
     (0x00682C11); with plain `ParentThing` c2 CSEs it across the call (FITTED-ish: GetParentThing() is the TC's normal accessor).
  6. marker: `PUnknownThreatOpinionSource = CTCDInternalMarker::Create(...)` by IMPLICIT conversion (retail `lea edx,[temp]` for
     operator=, 0x006834EF), not an explicit CIntelligentPointer<CThing>(...) temp.
- Measured dead ends this round: top-of-function decl of village_member (stores 0 at the top; vb/vg/vh), non-const/GetTC/1-arg PeekTC
  spellings of village_member (no change), crimes_count as one bool expr (same bytes), CDefPointer Release opt-ins (POINTEE /
  COUNTREF / SELFDELETING all worse), `if (deed) { rest }` instead of the deed==0 early return (dup vanishes, 3407), spouse local vs
  IS_THING_ALIVE(me->GetSpouseCreature()) (same bytes), FrameBribeEnds operand order (setg), for v while on the pending loop (code-identical).
- REMAINING (u1 = differ): (a) guard !crimes block: retail log in memory + const 0 in ebx (`xor ebx,ebx` 0x00682A1C) and keeps a
  redundant `cmp esi,ebx ; je` after `mov esi,eax` in the effects assignment (0x00682A3E); ours log in ebx, 0 in ebp, test folded.
  (b) flirt: retail hero_thing edi (+[esp+1Ch]), me ebp (+[esp+30h]), targeting map ebx; ours hero_thing memory, me ebx, map ebp;
  also retail evaluates the spouse GetPItem BEFORE `push 1` for IsAwareOfThing (0x00682EDD). (c) wives loop: retail `it` ebx, const 0
  piece ebp (GetWives push, CHeroMarriageInfo stores, PostDeed pushes), stats spilled [esp+2Ch] -> frame 0x58; ours it ebp, stats ebx,
  ctor zeros via eax -> frame 0x54. c2 trace: `it` (prio 2) picks ebp because ebx costs 14300 (byte-only candidates); retail must have
  coloured a const-0 piece in ebp before it. (d) violence/boasting: retail this in ebx (0x006830AC). (e) duplicated epilogue: ours
  at the criminal return (0x00682CE9), retail at deed==0 (0x0068318F); l2.cpp (before closer 3) had it right -- it flips with
  unrelated register changes, probably settles when (a)-(d) do.
- Mechanism of the remaining register residue (c2 traces tmp/s/u1.trace, u1.blk): in the wives loop retail needs a const-0 piece
  covering blocks 159..172 (GetWives push, ctor stores cost 6 = +72, PostDeed pushes; live-through -48 => prio +24) coloured
  ebp BEFORE `it` (prio 2); ours never forms that piece before `it` (const-0 pieces after the split passes at #77 local->ebx and
  crime_handle->ebp have prio <= 0), so `it` takes ebp (ebx costs 14300 from byte-only neighbours), stats takes ebx, the hero map
  temp (#227, lines 229/250) takes ebp and `me` is split late into ebx -- retail: map ebx, me ebp, it ebx, 0 ebp, stats memory.
  The flirt/violence differences follow from the same ordering. Local spellings in that region (stats GetTC, uid local, state local,
  const_iterator, it outside the for, nested ifs, separate IsAware statement, single/double hero_thing, GetParentThing() forms,
  no `me` local (82 hunks), IsDead text, /GS (cookie, +44), /Gr) do not move it. y1.cpp (one hero_thing in the violence block) is
  39 hunks but keeps hero in edi across both blocks where retail reloads [ebp+4] -- rejected.
- Tools: tools/gf.sh <src> <tag> [flags] (grade + masked structural hunk count), tools/sdx.py, tools/c2/c2prio.py (--trace / --blocks),
  tools/inl.sh (inline decisions with f and sc; FUNC=?InformOfDeed@... HDR=$W/hdr). Variants in tmp/s/ (b0 = old differ v9).

### (previous fix-up team notes follow)
#### 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed -- PARKED DIFFER(3409v3442) on SHARED headers (fix-up team, ~28 grades), differ/00682810.cpp
- Re-authored by hand on shared headers (no local class model left; only file-scope decls: IS_THING_ALIVE<T> template decl as the landed rows,
  NCreatureAI::IsAwareOfThing / IsAwareOfThingIndirectly, NGlobalConsole::EnableUnknownThreatOpinionSources, and the TU's anonymous-namespace
  UNKNOWN_THREAT_OPINION_SOURCE definition copied from the landed staticinit row 0x0121DFE0). New opt-ins + 2 new headers: hdr/README.md (wfA1 fix-up block).
- Names: draft's CanCreatureSeeCreature 0x008645E0 is NCreatureAI::IsAwareOfThing (Ego_r name set + reloc binding); draft's Partner = SpouseCreature
  (+0x188, FableWin GetSpouseCreature inline); Flags1c9 bit 3 = CanBeCourted (FableWin IsCourtable inline); CrimesSuspended (+0xB3) = LaggyLimbo via
  the existing GetVillageLimbo() (CTCVILLAGE_LIMBO); LineOfSight +0x2F = IgnoreFOVLineOfSight (IsThingIgnoreFOVLineOfSight, CTCPERCEIVETHING_AWARE_CALLED);
  COpinionDeed Victim/LinkedHandle = PTarget/ExtraData; g_UnknownThreatMarkers 0x01375733 = NGlobalConsole::EnableUnknownThreatOpinionSources
  (ONE source only: Anniversary 360 Test PDB S_GDATA32; neighbours 0x01375753 are NGlobalConsole globals) -- WEAK, coordinator to confirm.
- BEHAVIOUR FIX vs the stale draft (EVIDENCED, 0x00683342 `cmp edx,ebp ; setge bl` then `jne` insert): the new deed is inserted before the first
  pending deed whose ReactionPriority >= ours (draft had <).
- What moved it (hunks 185 -> 90 masked-structural): frame 0x58 needs crowd_group declared uninitialised inside the boasting-crowd block (the
  top `mov [esp+30h],0` is some other slot); `effects` declared before `if (deed)` (retail `xor esi,esi` before the null test); the courting test
  as one bool over a TEMPORARY PeekPDeedEffectsDef(type)->IsFlirting with ParentThing re-read (retail `mov bl,1/xor bl,bl`, `mov eax,[eax]`);
  `since = gap >= GetFrame() ? 0 : GetFrame() - gap` (0 arm first); out-of-line CIntelligentPointer ctor/operator= (CTOR_OUTOFLINE /
  ASSIGN_OUTOFLINE: retail calls 0x004A96F0 / 0x004A9760); ~list out of line (STLP_LIST_DTOR_OUTOFLINE: retail calls ??1?$list 0x00685BC0);
  GetDefGlobalIndexFromName("INTERNAL_MARKER_LIGHT") by IMPLICIT conversion (retail builds the temp, calls GetWorld, then `lea ecx,[temp]`).
- RESIDUE (v9 = differ): (1) register allocation of the first ~0x150 bytes: retail keeps 0 in ebx (cmp eax,ebx; village_member homed in
  memory [esp+20h], sharing the slot with the 0x23 lookup key; `handle` cached in ebx through the shop block), ours keeps village_member in
  ebx and has no zero register -> knock-on register/reload differences through the guard block; (2) retail keeps the TCI_WIFE key 0xD7 as
  an immediate, ours hoists it into edi in the flirt block; (3) retail uses ebp=0 for the GetWivesInMarriageState / CHeroMarriageInfo /
  PostDeed zeros; (4) list<long>::push_back is EXPANDED in retail (malloc(0xC) 0x0068337F) but called out of line in ours (inline budget; insert
  0x0042AB33 is out of line in both) -- candidate for a /d2inl flag once the rest closes; (5) the duplicated epilogue: retail duplicates the
  region_def release + epilogue at the `GetDeedFromHandle(handle)==0` return (0x0068318F..0x006831B0) and the bribed / criminal returns jump to
  0x00683566; ours gets the dup at the deed==0 return only with `GetWorld().DefinitionManager->...(CCharString(...))` (tmp/v7.cpp, 90 hunks),
  at the criminal return with the implicit-conversion spelling (v9); `goto finished` for the bribed/criminal returns moves the dup to the
  bribed return or removes it (v10/v11). The dup choice is a c2 layout heuristic sensitive to unrelated code size; not closed.
- Previous note (stale draft, kept for history):
- SUCCESSOR: still not re-worked (context spent on the three closer rows). Inventory for the port: the stale draft's ~25 local classes
  all have shared headers today (CThingAICreature, CRegion(Def), CWorldMap, COpinionDeed(EffectsDef), CTCVillage(+Member), CTCShopKeeper,
  CTCGuard, CTCWife, CTCPerceiveThing, CHeroMarriageInfo, CTCHeroStats, CTCCreatureGroup, CTCBoastingCrowdControl, CTCDInternalMarker,
  CDefinitionManager, COpinionReactionManager, CWorld) except the COpinionPersonality* / COpinionOfHeroDef models; ~20 landed same-TU
  siblings in rebuild/src/compiled/00/68/ (CTCCreatureOpinionOfHero_*) carry the opt-in set to copy. Descriptive member names in the
  draft (Partner, Flags1c9, FrameBribeEnds, CrimesSuspended, LineOfSight, Victim, g_UnknownThreatMarkers) need their header spellings.
- differ/00682810.cpp is the earlier lane's draft copied as is (DIFFER(3385v3442) on its own stale overlay). It models CThingAICreature,
  CRegion, CRegionDef, CWorldMap, COpinionDeedEffectsDef, COpinionDeed, ... as LOCAL padded structs (forbidden: shared headers exist);
  on today's headers it fails on COpinionTransientOffset(offset, traits) and PUnknownThreatOpinionSource (today's opt-in is
  FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREATSOURCE, the stale one was _THREAT). A port onto the shared headers is the next step.

## Header-overlay regrade (WF_RULES): style_small 006a1a00 0074af50 007b57c0 007c0690 007ef200 007fc9d0 00944940 00945060 include
   overlay headers changed here; all eight still RELOCATION_MATCH under hdr/ (q.py, their own Flags lines).
