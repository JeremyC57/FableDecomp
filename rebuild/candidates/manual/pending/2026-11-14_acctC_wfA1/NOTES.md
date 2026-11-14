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

## 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed -- NOT RE-WORKED (1 grade: SRC_FAIL)
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
