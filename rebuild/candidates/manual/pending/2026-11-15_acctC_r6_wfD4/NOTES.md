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

## 0x0049A0B0 CProgressDisplay::InitialiseTextures -- MATCHED (RELOCATION_MATCH, test PASS, vland PASS) (successor 2, 2026-11-15)
- src/0049a0b0.cpp = the near-miss source + two FITTED blocks: `{ RandFunc random(&seed); std::random_shuffle(...); }` in the background
  arm and `{ RandFunc random(&seed); shuffle TextTags; shuffle TextTagsPC; }` in the tip arm (seed stays OUTSIDE the block in both).
- How it was found: c2 frame-packer probe (tools/pks.sh / pkq.sh = wfD6's pk tools). Retail joins: background RandFunc in the slot of
  {tag, name, name, contents, i}; tip-arm temp T8345 in the slot of {display, tip RandFunc}; tip seed in the slot of {debug GetMiscDir temp,
  bg seed, seal operator+ temp}. A RandFunc at the scope of its seed conflicts with every later temporary of that scope (address-taken
  locals live for their whole scope) -> own slots, frame 0xb4. Tip seed inside the block too (v4) joins the newest slot (wrong one):
  retail's seed conflicts with the TextTags.txt path temporaries, so it is declared at the tip-arm scope.
- Also matches with `std::random_shuffle(b, e, RandFunc(&seed))` for the background arm (v7) but that binds a temporary to a non-const
  reference (MSVC only): the block form is the portable one. vland warnings only RELOC_ALIAS wcscmp thunk / RELOC_NAME 0x00403C60 label.

## 0x0066CF00 CRegionMinimap::PopulateRegionMinimap -- near-miss DIFFER(3248v3248) (successor 2, 2026-11-15; was 3236v3248)
- differ/0066cf00.cpp (p11/r3). New levers, each measured with tools/mg.sh (q.py + esp-masked diff) :
  (1) NEW overlay opt-in FABLE_RETAIL_CREGIONMINIMAP_TGACOORDS_LANDED_TEXT: the inline GetTGAPositionFromWorldCoordinates text = the landed
      0x0066C880 body (GetRegionMapExtents(origin, extents) + GetRegionMinimapSize()); frame 0x1ec -> 0x1e4 (= retail), 3236 -> 3240.
  (2) NEW overlay opt-in FABLE_RETAIL_CREGIONMINIMAP_WORLDCOORDS_INITIALISE: GetWorldCoordinatesFromTGAPosition ends in one
      `world.Initialise(x, y, 0.0f)`; this text compiles OUT OF LINE to retail 0x0066C810 byte for byte with the PDB signature
      (p11/c.cpp MATCH; the landed row models it as a C3DVector-returning shell). No byte change inside this row, kept as the real text.
  (3) FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY + _TYPEMASK (+ CTHINGBUILDING_TYPEFLAGS): the visible noinline GetAllThings<CThingBuilding,..>
      body (retail instance 0x0066E5A0 has the 0x47 mask test). c2 then knows `buildings` is not captured: retail keeps buildings._M_start in
      ebx across the loop (reload 0x0066DB44, free 0x0066DB67), `building` in memory [esp+48h], zero in ebp. Fixed the whole building-loop
      register structure.
  (4) second nest on scalars: `for (long y = tgaTopLeft.Y; ...) for (long x = tgaTopLeft.X; ...) { C2DCoordI position(x, y); ...` (retail homes
      x at [esp+48h] and y at [esp+24h], not adjacent, and tgaBottomRight sits in the first nest's position slot [esp+28h]); 3240 -> 3248.
- Residue (all inside equal-length code): (A) first nest: x in ebp / themeDef.Object in ebx, retail the reverse. Diagnostic p11/d1 (one more
  read of position.X in the loop) flips it to retail's, so x is one reference short of themeDef in c2's priority. No natural source found:
  declaration orders (a4, a7), world outside the loop (a1), if/else forms (a2, a3), nested call (a5), post-increment (a6), named def pointer
  (a8), `long x, y` + position(x, y) in the first nest (r1: frame 0x1dc), an empty inline ValidateRegionMinimapPosition call in the setters
  (y1..y3; retail has the empty out-of-line copy 0x0066CA70 `ret 8`), k = 1..7 unused locals: no change. (B) both
  GetWorldCoordinatesFromTGAPosition expansions: ours converts the size first (`mov edx,[esi] ; fild`), retail loads topLeft first and keeps
  RegionMapSquare.TLX in edx (`sub ecx,edx`): follows from scratch-register assignment (retail reuses eax for topLeft.X then size).
  (C) unrolled corner min/max loop: pointer base `lea ecx,[ebx+0Ch]` in retail, +1Ch in ours (same body otherwise). (D) small scheduling
  hunks at 0x0066D19B..0x0066D1BA (PeekWorld loads v regionBox fild/fstp) and 0x0066D95B.
- Local classes still in the source: CEngineThemeDef (no engine/CEngineThemeDef.h exists) and the in-row inline GetDef<T> (text = the shared
  GETDEF_GLOBAL_BODY, inline instead of noinline because retail expands it here).
- Next: (A) is a one-reference priority gap: look for a retail-natural extra use of position.X or one fewer of themeDef (CDefPointer Release /
  SetDynamicCast texts: default Release has the fewest Object references); then (C) with spellings of the corner loop.

### 0x0066CF00 addendum (same session, later): 259 -> 95 esp-masked diff lines, still DIFFER(3248v3248)
- (C) CLOSED: the corner min/max loop written with `corners[i].X` / `corners[i].Y` at every use (no `const C3DVector& corner = corners[i]`)
  gives retail's unrolled pointer base `lea ecx,[ebx+0Ch]` (p11/c1; by-value copy c2, iterator c5, pointer c6, long index c3 are wrong).
- (B) mostly CLOSED and with it every scratch-register permutation after the second nest (intersection temporaries, free, final release):
  NEW overlay opt-in FABLE_RETAIL_CREGIONMINIMAP_WORLDCOORDS_REGIONEXTENTS = the GetTGA sibling's preamble in GetWorldCoordinatesFromTGAPosition
  (`C2DCoordI topLeft; C2DExtentsI extents; GetRegionMapExtents(topLeft, extents); long size = GetRegionMinimapSize();`). With
  WORLDCOORDS_INITIALISE this text also compiles out of line to 0x0066C810 byte for byte (p11/d.cpp MATCH). Scratch-register picks are
  sequential state: one wrong pick in an expansion permutes every later pick of the region, so fix the earliest hunk first.
  `squareSize` before `size` (p11/f.cpp) breaks the out-of-line match and is worse inline: not the text.
- Left: (A) x / themeDef swap (diagnostic p11/d3 on this base: one more read of position.X removes the swap, 63 lines left, so (A) is
  independent of the rest); (B') inside both expansions retail emits `mov ecx,[esi+8] ; mov edx,[esi+14h]` before `mov [tl.x],eax ; mov eax,[esi]`
  and the fdiv after the Y-extent loads, ours converts the size first (same registers); (D1) 0x0066D19B..0x0066D1BA PeekWorld loads v the
  regionBox fild / fstp; (D2) 0x0066D95B `mov edx,[esi+20h]` before `mov ecx,[esi+18h]`.
- Not tried yet: the same out-of-line text proof for the setters' callers' order (SetRegionMinimapThemeValue 0x0066CBC0 / Lighting 0x0066CC00
  all texts tried give the same out-of-line bytes), CDefPointer Release / SetDynamicCast text variants for (A), an inline
  IlluminateRegionMinimapTheme-visible TU (same-TU callee 0x0066C900 defined above the row: lessons wfB5) for (A) / (D1).

## Session summary (successor 2, 2026-11-15)
- Matched: 0x0049A0B0 (src/, test/, payload.json, oracle.tsv; read-only vland PASS). Near-miss: 0x0066CF00 (3248v3248, 95 masked lines).
- 0x00974AA0 re-measured only (tools/mg.sh on differ/00974aa0.cpp): DIFFER(3072v3080), frame 0x808 v 0x80c, 347 esp-masked diff lines in
  ~70 hunk groups from 0x00974AF3 to 0x0097566C: not a last-resort-device candidate, structure still open (see the section above).
- Not opened this session (state unchanged from the predecessor): 0x00715A70, 0x006D1E30 (does not compile), 0x0090D030 (local models).
- New tools in tools/: pks.sh / pkq.sh / pkc.py + pk/ (c2 frame-packer probe, copied from wfD6; ~9 s), ef.sh (efm map, starred only),
  mg.sh (q.py + esp-masked diff with retail addresses; logs to ATTEMPTS.log), mq.sh (one line per variant).
- Coordinator decisions: (1) accept the two FITTED `{ RandFunc random(&seed); ... }` blocks of 0x0049A0B0; (2) the three new
  CRegionMinimap.h opt-ins (hdr/README.md) and the NAME_FIXES entry for the landed 0x0066C810 signature; (3) LABELS for 0x0066CA70 /
  0x0066CBC0 / 0x0066CBE0 / 0x0066CC00 / 0x0066CC20 (PDB order + body shape; FableWin + Ego_r name sets).

## 0x0066CF00 -- successor 3 (2026-11-15): 95 -> 53 esp-masked diff lines, still DIFFER(3248v3248); differ/0066cf00.cpp (old 95-line source kept as differ/0066cf00_prev95.cpp)
- (A) x / themeDef register swap CLOSED by declaring `std::vector<CThingBuilding*> buildings;` BEFORE `C2DBoxF regionBox(...)` (p12/d1; FITTED
  order, natural C++). c2prio_f.py (wfcommon/tools/C41, copied to tools/) showed the mechanism: themeDef is split before x is coloured and its
  inner-loop piece #100 (prio -64) outranked x (#19, prio -76) for ebx; the declaration order changes candidate ids / split order.
  Measured NO effect on (A): dead FABLE_RETAIL_ASSERT arms (1..8, p12/a*), CDefPointer Release variants (POINTEE / COUNTREF / SELFDELETING /
  copy-first), no SETDYNAMICCAST_REREAD, CellAt texts (vector operator[], named index, X + Y*size: same priorities -- inline reference
  temporaries are not candidates), useLighting spellings (bool local, ==true, !=false, named row reference), scalar `long x` first nest.
- (D1) PeekWorld / PeekThingSearchTools loads scheduled above the regionBox stores CLOSED by defining the same-TU callee C2DBoxF::ClipTo
  (0x0066C560, landed text on the shared TLX/TLY/BRX/BRY members) above the row (lessons wfB5: c2 then knows regionBox is not captured).
  Still called out of line (equal length / call count).
- Left: (B') inside both GetWorldCoordinatesFromTGAPosition expansions retail emits `mov ecx,[esi+8] ; mov edx,[esi+14h]` before
  `mov [tl.x],eax ; mov eax,[esi]` and the fdiv after the Y-extent loads; ours stores topLeft.X and converts size first. Tried on the e1 base
  (no gain): size before GetRegionMapExtents (w1), squareSize first (w2), GetExtents().X inline (w3), squareSize.Y (w4: frame 0x1dc), named x
  (w5), C2DCOORDI_COPY, C2DEXTENTSI_FORCEINLINE_CTOR, C2DEXTENTSI_INITIALISE. (D2) 0x0066D95B `mov edx,[esi+20h]` one slot early.
- (B') measured with the c2 list-scheduler trace (tools/sc.sh + tools/sched2 from wfB3/wfB1, tools/scshow.py <tag> <region> [n];
  `TARGET=PopulateRegionMinimap tools/sc.sh differ/0066cf00.cpp base '["fdiv","fimul","fiadd"]'` -> regions 2 and 105 are the two
  expansions). IL order is already TLX, TLY, store, store, size...; the scheduler picks by priority: ours store-TLX (node 3) 0x234 >
  TLY load (node 2) 0x204 > SqTLX load (node 8) 0x1f8 because node 3 precedes the size load (eax anti-dependence 3->5) whose chain
  (store size -> fild -> fdiv, latency 39 -> fimul -> fiadd -> fstp -> second fdiv ...) is the longest path. Retail picks 1, 2, 8, 3, 5, 4,
  12, 6, 15: its node 3 must rank below 0x1f8, i.e. the size/fdiv chain is shorter or nodes 2/8 have more successors there. Not resolved;
  next: find the text that changes the DAG (successor counts of the topLeft.Y / SqTLX loads, or the fdiv chain length), e.g. the
  topLeft.Y / squareSize values used by more than one consumer in retail's inline text.
- k unused locals (long, float; entry or after `strength`; k = 1..7) and GetExtents / GetTGAPosition text variants (x1..x3, t1..t3): no gain.
- Total grades this successor on 0x0066CF00: ~60 (ATTEMPTS.log).

## 0x0066CF00 -- successor 4 (2026-11-15): 53 -> 51 esp-masked diff lines, still DIFFER(3248v3248); differ/0066cf00.cpp (53-line source kept as differ/0066cf00_prev53.cpp; probes in 0066cf00_p/)
- (D2) 0x0066D95B CLOSED: the building block (GetBoundingInfo .. second GetTGAPositionFromWorldCoordinates) is ONE c2 scheduling region of
  exactly 81 tuples (t13 trace region 96, 83 nodes incl. start/end): the cut lands inside the second TGA expansion. Two extra no-code PAREN
  tuples anywhere before it (pa2 / pa3 / n3 / n4 = any 2 or 3 parenthesised float args of buildingBox; 1 or 4 do not) give retail's
  `mov edx,[esi+20h]` before `mov ecx,[esi+18h]`. Kept: FITTED parentheses round the two `+ boundsRadius` arguments (marked in the source).
  Without the cut the tie is prio 88 v 87: c2 prio = depth + successors + 32 for loads; the TLY load has one more successor (the call).
- (B') still open, measured: region 2 DAG (tools/sc.sh base) prio = depth + succ count (+32 load). Retail's order 1,2,8,3,5,4,7,6,15 needs
  prio(store TLX) < prio(SqTLX load)=126 and prio(store size) < prio(store TLY)=98; ours has both stores on the fdiv chain through the eax
  anti-dependence (store TLX -> size load, latency 0; depth 136 / 133). The edges are register anti-deps (no store->load memory edges to the
  other [esi] loads), so retail's DAG at scheduling time must differ in edges or latencies. No effect: /G5 /G6 /GB (/G7 breaks the row),
  15 paren placements in the WORLDCOORDS Initialise args (0066cf00_p/mkw.py w01..w33), k = 1..7 unused longs at entry, named C3DVector args
  for the TGA calls (n1/n2), TGA Y text variants (h1..h3). Park.

## 0x00974AA0 -- successor 4 (2026-11-15): DIFFER(3072v3080) 347 lines -> DIFFER(3080v3080) 339 lines, frame 0x80c = retail; differ/00974aa0.cpp (old source differ/00974aa0_prev347.cpp; probes 00974aa0_p/)
- Lever: `float height = battleDef->BattleFloatHeight;` before `const C3DVector& arena = PArenaCentre->GetPos();` in the main loop
  (retail 0x009750EA loads +0x50 and spills it to [esp+6Ch] BEFORE the GetPItem/GetPos calls): length and frame now equal (p1).
  `CTCPhysicsBase* physics = GetCreature().GetPhysicsTC();` named first as well (p2, retail keeps it in edi across GetPos 0x009750E7):
  329 lines but 3084 bytes; q1..q3 (Z + height order, named pos, parens) no better. Retail copies arena.Y through x87 (`fld [eax+4]` ..
  fstp) and X as an integer: not reproduced yet.
- HasTC(TCI 0x30) at 0x00974B3B: retail `mov ecx,[eax+24h] ; shr ecx,10h ; and cl,1` -- a byte-load narrowing (`mov cl,[eax+26h]`) of a
  stored HasTC bit NEVER occurs in retail (exe scan: 0 narrowed, 14 shr-16 / 15 shr-8 dword forms; the landed SellToBuyer 0x0094C050 /
  EatAtTable 0x0096CD70 use GETTC_EXPANDED + the PLAIN HasTC). So retail's materialisation comes from the inline budget (CThing.h comment
  at HASTC_FORCEINLINE: "plain inline leaves the test() result materialised in a stack byte once the function's inline budget is spent"),
  not from HASTC_MATERIALISED (whose `const bool&` text lets c2 narrow bit 16). Measured: no MATERIALISED + `/d2inlS25` (any S >= 25; S <= 22
  = default) removes EVERY hunk from 0x00974AA0 to 0x00974C2C (347 -> 201 on the old base; p1n + S25: 197 lines, frame equal) but makes
  the row 64..80 bytes long: with S25 ours keeps 5 exit tails (Terminate + battleDef release + ret) where retail cross-jumps them into 3
  (in-loop IsStillRunnable fails `je 0x975645` / `je 0x975665 -> jmp 0x974C1A`). /d2inlT sweeps (20..150) are no use (T<=50 calls
  bitset::test out of line). Next: with S25, find why the in-loop `Terminate(); return;` tails do not cross-jump (call counts: ours
  +1 GetPItem, +1 Terminate, +2 indirect release calls v retail; tools 00974aa0_p/cnt.py / calls.py), then whether a smaller inline
  device (scoped pragma round the GetTC sites) gives the same materialisation without changing the rest.
- Also checked: the Jack action classes and Stage2 are local classes, but engine/CAIStateGroup_JackOfBladesStage2.h,
  CCreatureAction_JackFloatUp.h, _JackShotFloat.h, _JackFlash.h, _PlayCombatAnimation.h exist (generated flat structs): before landing,
  each needs an additive FABLE_RETAIL_* opt-in branch deriving the shared base (the lander's strict-header check rejects local models);
  engine/fable_ai_action_enums.h now exists (replace the row-local EAIActionReturn / EAIActionRepeat enums).
- (later, same session) differ/00974aa0.cpp is now t1 = 322 lines, DIFFER(3060v3080), frame equal (p1 = 339 lines / 3080 kept as
  differ/00974aa0_prev339.cpp). Call sequence by target (00974aa0_p/cseq.py, retail addresses mapped by hand) equals retail except:
  the LookAt `CIntelligentPointer<const CThing>(PTargetThing.Get())` (ours calls the template ctor out of line, retail expands it into
  CBaseIntelligentPointer ctor 0x00A01B10 + SetPItem 0x00A01B90 after the GetPos pair at 0x00975151) and the exit tails (+Term +2 IND).
  New levers: scale-up loop = `float scale = GetScale(); step = (BattleScale - scale) / (fps * ScaleUpTime); if (scale < BattleScale)
  do { if (!target.. || IsStopping() || ..) break; scale = step + GetScale(); SetScale(scale); DoNothing(); } while (scale < BattleScale);`
  (retail 0x00974EBC.. reads GetScale once, bottom `jnp 0x974EF0` to the top tests: not an inverted while); wait loop `elapsed =
  GetWorld().GetFrame() - startFrame;` then `while ((float)elapsed < (float)GetConstantFPS())` (retail fild of the global after the call).
  Byte drift (address delta) first appears at the facing blocks 0x00974CC0..0x00974DD0 (-11 .. -24 bytes: retail spills diff Y/X to
  [esp+10h]/[esp+28h], open item (1) of the first session) and the extra `jmp` over the loop-alignment pad at 0x00974F9A follows from it.
- (later still) differ/00974aa0.cpp = u1 = t1 + the EXISTING repo opt-in FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR: 210 lines,
  DIFFER(3128v3080), frame equal; the entry block (both HasTC materialisations, boss / combat GetTC, PeekBaseDef in edi) and the facing
  blocks now match; address delta stays within +-7 until the exit tails. Calls = retail except +1 Terminate +1 indirect release at
  0x00975610 and +1 at the end. Remaining structure: (a) retail 0x00975665 merges the case-5 / post-idle IsStillRunnable-fail tail into
  the entry exit's release (`jmp 0x974C1A`, suffix after the entry exit's null test); ours duplicates it (0x00975611 in ours);
  (b) retail's wait-loop exits jump to the float-up-fail tail 0x00974E9A (battleDef still in edi through the scale and wait loops);
  ours makes a separate tail (battleDef in ebx). Then registers (main loop, LookAt block). t1 (322 lines, 3060) kept as
  differ/00974aa0_prev322.cpp. Next: battleDef's register in the wait loop (it is the last use before the main loop's reload
  `mov ebx,[esp+1Ch]`): try the scale/wait loops' statement scopes, battleDef->BattleScale read through a named reference, etc.

## 0x0090D030 CAIStateGroup_MinionGossip::Update -- wfD4 successor 5 (2026-11-15): re-authored on shared headers, DIFFER(3269v3269) near-miss
- differ/0090d030.cpp (Flags /d2inlS30 /d2inlT100): equal call sequence (141 calls, same order, cseq.py), equal length, frame 0x580 with
  every frame slot equal; residue = register picks only (see its Grade line). Prior local-model draft kept as tmp/90d/differ_prev_localmodels.cpp.
- Shared headers: class via new overlay opt-in FABLE_RETAIL_CAISTATEGROUP_MINIONGOSSIP (engine/CAIStateGroup_MinionGossip.h, : CAIStateGroupBase,
  vtable slots 12..16), new headers engine/CTCGossip.h and engine/CThingFilter_IsNavLineOfSightToThing.h (8 bytes, Ego_r), opt-ins
  FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH and FABLE_RETAIL_CMESSAGEEVENTFILTERS_IN2DRANGE_CTOR (hdr/README.md). Existing: GETTC_EXPANDED,
  CMESSAGEEVENT_ORIGINATOR_ORIGINAL (xor 1Fh inline / not in the 0x0090DF30 COMDAT -- same text), CTHINGAICREATURE_STATTYPEFLAGS (0x1F),
  CINTELLIGENTPOINTER_FORCEINLINE_CTOR / CONVERT_INLINE / COPY_OUTOFLINE (0x0050F410 for InteractWith's by-value copy), IS_THING_ALIVE.
- What fixed what (all measured): (1) the three filters + Bind4 built as temporaries inside ONE FindMostRecentMessage expression (retail's
  right-to-left argument order: LOS filter, IsValidGossipee ctor, In2DRange, IsOfType) with only the LOS filter a named local, in its OWN
  block around the search (frame 0x590 -> 0x578 -> 0x580; its slot is shared with a later CSoundPair); (2) the wait loop as
  `while (IS_THING_DEAD(PTCGossip->Gossipee))` (retail keeps &Gossipee in edi: by-reference helper); (3) the talk loop's no-repeat pick
  `do anim = GossipTalkAnims[r]; while (anim == last_anim); ... last_anim = anim;` (retail ebp = last anim, `cmp edi,ebp`; the old draft's
  `anim == 0` was wrong) -- puts i / count in memory as retail; (4) `bool arrived;` at function top (1-byte packer tie: shares nothing with
  the HasTC bytes as retail); (5) CSoundPair() inline default ctor; no CVECTORMAP_INLINE_LOWERBOUND (retail calls 0x0040F020 5x).
- Inline decisions: retail inlines GetPOriginator<CThingAICreature> at sites 1-2 and calls 0x0090DF30 at site 3, and expands the
  CIntelligentPointer<const CThing> conversion at all three LookAtThing sites. Plain build: conversion expanded once, GetPOriginator once.
  /d2inlS30 fixes the conversions, /d2inlT100 (70..90 also) the GetPOriginator split; no single flag does both. Tried: T100..6400, S15..100.
- Register residue tried (no change): k = 1,2,3,5 unused named locals; originator block braces; IS_THING_DEAD on the originator (T100 inlines
  site 3; at T90 identical to best). Observation: with site 3 inlined, message takes ebp as retail (it then interferes with the site-3 `xor ebx,ebx`
  temp) -- in retail the site-2 originator temp / vtable-constant web out-prioritises message for ebx.

## 0x006D1E30 CTCWife::FrameUpdate -- wfD4 successor 5: ported onto today's headers, graded DIFFER(3432v3484)
- differ/006d1e30.cpp now COMPILES (was SRC_FAIL): + wfA1 006d0830's define set, FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE / _DEEDS / _GREET
  (the only branches with StatGetInterfaceType), STLP_LIST, STLP_TREE_ERASE(_BODY) (map erase inline, _Rebalance_for_erase 0x0042A34E),
  CTCPERCEIVETHING_AWARENESS / _AWARE_CALLED, CTHINGAICREATURE_SETNEWBRAIN, CTCBUYABLEHOUSE_FRAMEUPDATE, and NEW overlay opt-ins (hdr/README.md):
  CTHINGAICREATURE_WIFE_QUERIES (IsBrainUpdateZoneActiveThisFrame decl = 0x00830130; IsCourtable inline = CanBeCourted bit 3, `test [ebx+1C9h],8`),
  CTCBUYABLEHOUSE_CURRENTDRESSLEVEL, CTCPERCEIVETHING_CANSEEORSMELL, CTCHEROSTATS_PEEKMARRIAGEINFO, COPINIONREACTIONMANAGER_REACTIONMATCHSTRENGTH,
  CTCSOUNDPLAYER_SETSPECIFICSAMPLEVOLUME, CTCCREATUREOPINIONOFHERO_SETPERSONALITYOVERRIDE_INDEX, CHEROMARRIAGEINFO_STATE; GetHomeBuilding's
  inline body in the row (as landed 0x00958840 / 0x00946400).
- Call sequence equal to retail (102 calls, tools/cseq2.py). 52 bytes short; first drifts: brain-name literal arms (retail `jg ; push A ; jmp ;
  push B`), ebx=0 from the prologue, then a block moved at 0x006D22B7 (-92) and the gift-opinion x87 block. Not worked further (pace).
- Name evidence: 0x00830130 is IsBrainUpdateZoneActiveThisFrame (Ego_r 0x00536F40 QBE_NXZ; one retail call, at the brain-zone test), NOT
  IsCourtable (inline). NAME_FIXES row added.
- (successor 5, later) Register-residue probes on differ/0090d030.cpp (all 3269v3269, frame 0x580): the first-half ebx<->ebp swap covers
  every callee-saved web from the opening GetTC map pointer (0x0090D05E) to the interaction pointer (0x0090D6A6): message, site-2
  originator temp, vtable constant, ConstantFPS / manager (post arm), LookAt arg-block pointer (0x0090D406), home pointer (0x0090D609).
  The second half (LISTENING / talk loops) already matches except the zero register (ours hoists `xor ebp,ebp` to 0x0090D923 and uses it
  for the CROUCH INTO site and `cmp [esp+24h],ebp`; retail per-site `xor ecx,ecx` and `xor ebp,ebp` only before the CROUCH loop 0x0090D9D8).
  Lever found: the `initiator` bool. `bool initiator = false;` with no answer-arm assignment (vD1 / vE2) gives retail's ebp for message /
  originator / vtable / post-arm webs (hunks 50 -> 38) but adds an entry store and moves initiator to [esp+28h]; the arg-block / home /
  interaction webs stay flipped. Retail's initiator code is v12's (al at the join, `test al,al ; mov [esp+2Ch],al`). Dead inits (vH*),
  ESex-typed sex locals, `!other_sex && my_sex`, implicit Gossipee conversions, merged originator tests: byte-identical to v12.
  Flags: only /d2inlS30 with /d2inlT70..T100 gives retail's inline split (S35/S40 -> 3237, S50 -> 3301, T50 -> 3205).
- Correction (flags window, measured on differ/0090d030.cpp itself): /d2inlS30 needs /d2inlT in ~91..109 (T60..T90 -> 3237: GetPOriginator
  site 2 called; T110 -> 3285: site 3 inlined, but only 39 hunks -- worth a look by a successor); S28 fails. The earlier "T70..90" was the
  IS_THING_DEAD-originator variant (vC1). Inline-budget line for a hand-back: `// Inline-budget: /d2inlS30 /d2inlT100 -- the three
  CIntelligentPointer<const CThing> conversions at the LookAtThing sites (S) and GetPOriginator<CThingAICreature> sites 1-2 inline, site 3
  called 0x0090DF30 (T)`; only acceptable once the register residue is closed.

## 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera -- successor 5: inspected only (no grade spent)
- Plan for a successor: re-host the draft on the model of the landed sibling rebuild/src/compiled/00/71/NTCScriptedControlCActionTalkBase_Update_00716de0.cpp
  (43 opt-ins; its local CSubtitledMessage / CActionTalkBase models are allowed: no engine header exists for either, and landed siblings
  model them locally). The sibling's member names replace the draft's descriptive ones: FarEnoughAwayForFacingCamera +0x6D (draft
  FarApart), ThingLookingFromPos / ThingLookingAtPos +0x74 / +0x80 (float[3] in the sibling; draft CameraFromPos / CameraAtPos),
  ThingLookingPosSet +0x8C (draft CameraSetUp), ParentIsDemonDoor +0x8D. Draft-local models to replace with shared headers: CThing,
  CThingCreatureBase (GetHeadPos 0x00661B40), CTCPhysicsBase (GetFacingVector slot 70), CThingSearchTools / CThingFilter_None / C2DBoxI,
  CMainGameComponent, CWorld / CAIGameCameraBase / CGameCameraManager (SCRIPTEDMODE opt-in), CTCScriptedControl (ACTIONBASE opt-in).
  NCameraModes::CScriptModule* have no headers (local or new headers). Then the 496-byte structural gap (focus-case cross-jump) remains.

## 0x0066CF00 -- successor 5 (2026-11-15): still DIFFER(3248v3248), 51 esp-masked lines; differ/0066cf00.cpp unchanged (probes 0066cf00_p/s5/)
- Residue (B') re-measured: only the two GetWorldCoordinatesFromTGAPosition expansions (0x0066CF75, 0x0066D9E3); the other masked lines are
  the switch table 0x0066DB90 and the `mov cl,[edi*8+X]` table relocation (not real).
- New measurement: region 2 (site 1) is NOT cut (38 real nodes; t14 tuple dump shows no no-code tuples at scheduling time), and the cycle
  model issues one integer op per cycle, so retail's order is a PRIORITY fact. Inferred from retail's two schedules with ours' DAG model:
  site 1 needs prio(store size, node 6) < prio(store TLY) = 98 and prio(store TLX) < prio(SqTLX load) = 126; site 2 needs 107 < prio(6) <
  ~136. I.e. retail's size->fdiv chain is much SHORTER at site 1 than at site 2 -- consistent with retail's scheduling region ending soon
  after IL node 8 at site 1 (a region {1..8} reproduces 1,2,8,3,5,4,7,6 exactly: loads +32 first) and later at site 2. Ours never cuts
  there. Not reproduced: what makes retail cut (not the 81 count inside region 2; successor 4's 15 paren placements inside the inline did
  nothing).
- Inline-text probes (probe overlay 0066cf00_p/s5/hdr, PROBE_S1..S8): scalar `long tlx/tly` (fiadd [esi+4], frame 0x1dc), plain GetTLPos copy
  (c2 hoists the size load out of the loop: 155 lines), GetTLPos().X in the expression, the (C2DCoordI&, C2DCoordI&) GetRegionMapExtents
  overload 0x0066C6D0 text (S4: byte-identical to best, 51), GetBRPos-based square extents, size unnamed, float size, squareSize unnamed:
  none better. No REGIONEXTENTS: 155; no INITIALISE: identical 51. ~22 grades this successor. Parked.

## 0x00974AA0 -- successor 5 (2026-11-15): DIFFER(3128v3080) 210 -> 200 esp-masked lines; differ/00974aa0.cpp (210-line source kept as differ/00974aa0_prev210.cpp; probes 00974aa0_p/s5/)
- CLOSED the arena reset block 0x00974C33..0x00974C68 (retail `fld [eax+8] ; fld [eax+4] ; mov ecx,[eax]` = the 3-float C3DVector ctor on
  the GetPos components, not the copy ctor): `{ CTCPhysicsBase* physics = GetCreature().GetPhysicsTC(); const C3DVector& arenaPos =
  PArenaCentre->GetPos(); C3DVector resetPos(arenaPos.X, arenaPos.Y, arenaPos.Z); physics->ResetPosition(resetPos); }` (w10). Without the
  named resetPos (w2/w8/w9: 204) the physics vtable load moves above the stores; arena reference before the physics load (w6): 280.
- Facing blocks 0x00974CC4 / 0x00974DA8 still open: retail sums X*X + Y*Y then + Z*Z (X kept with `fst [esp+28h]`), ours Y*Y + Z*Z + X*X.
  Byte-identical for every text order (inline sum XYZ / ZYX, SQUAREDMAGNITUDE_XYZ / _ZYX opt-ins, Dot, IsNonZero with NONZERO_SQUARED_XYZ /
  ISNONZERO_ZYX(_PAREN): 200), worse with ISNONZERO_ZYX_SQUARED (216), named distanceSquared (214), reversed compare (220), `diff(a - b)`
  same, diff declared then assigned / hoisted (228, frame 0x818), SUBTRACT_XY_GROUP same, named creature GetPos (234). k = 1..7 chained
  const C3DVector& rulers: no change (pass-through references count nothing); float component locals x/y/z: 200 / 207 / 275, sum order
  unchanged. So the order is keyed on symbol numbering (lessons C41: period 8); not found.
- Exit tails re-read (unchanged): ours keeps 4 epilogues (retail 3) and does not cross-jump the case-5 / idle IsStillRunnable-fail tail
  (ours 0x00975611: Terminate + release with edx + epilogue) into the entry exit's release 0x00974C1A, although the suffix is identical in
  ours too; the phase-change tail's release uses eax for the vtable in both builds. The loop-continue edge in ours needs `mov ebx,[esp+18h]`
  (battleDef back into ebx for the loop head); retail's loop head needs no reload. ~31 grades this successor.

## 0x0090D030 -- wfD4 successor 6 (2026-11-15): c2prio measurement of the ebx/ebp swap (20 grades, scratch in tmp/s6/)
- Devices measured: k = 4,6,7 unused named locals and k = 1,4,12 dead initialised locals at function top: byte-identical (48 hunks). Device c closes nothing here.
- c2prio --trace (tools/c2p/c2prio.py, 40 s, tmp/s6/c2p_tr.txt): message #21 (prio 58) is coloured with costs {ebp:100}, ebx 0 -> ebx. Retail's ebp needs
  ebx dearer than ebp for message = an uncoloured neighbour left with ebx only (a BYTE value live across calls: ebx is the only callee-saved byte reg)
  live across the message arm. Proof: `bool initiator = message == 0;` (or `message ? false : true`) before the if, no arm stores (tmp/s6/90d_iA.cpp,
  90d_iK.cpp) -> 33 hunks: message / map95 / originator temp / vtable const / post-arm ConstantFPS+manager all take retail's registers (message costs
  {ebx:200}), but adds `sete [esp+2Ch]` + join reload (retail: `xor al,al` / `mov al,1` at the arm ends, store at the join). Constant defaults
  (`bool initiator = true/false` at top or before the if, with one-arm stores, iC/iD/iH/iI/iJ/iN) are constant-folded: no candidate, no change.
  `if (message) initiator=false; else initiator=true;` before the main if (iL) 49; `!initiator` test (iM) 39; arm-start stores (iE/iF) 45-49.
- Second, independent web: home pointer #143 (line 163, prio 24, ebx) v interaction #40 (prio -19, live through the walk loop, ebp). Retail home ebp /
  interaction ebx: needs interaction's ebp removed (or ebx-only) before #143 is coloured. Third: const-0 piece #2 of #1 (prio 48) -> ebp = the hoisted
  LISTENING zero register (retail per-site immediates).
- Open lead for a successor: find the retail-plausible source of a byte live across both arms (iA proves the mechanism); parked again.

## 0x006D1E30 CTCWife::FrameUpdate -- wfD4 successor 6 (2026-11-15): 3432v3484 -> DIFFER(3484v3484), 151 -> 125 hunks (scratch tmp/s6d/)
- differ/006d1e30.cpp = tmp/s6d/y1.cpp. Equal length; what is left is register / frame-slot choice (see its Grade line). Fixes, each measured:
  (1) brain block = two complete arms `if (IsMarriedToThePlayer()) { CDefPointer<const CBrainDef> brain; GetDef("BRAIN_WIFE",&brain); SetNewBrain(brain);
      NeedsToChangeBrain = false; } else { ...DIVORCED... }`: VC7.1 cross-jumps the identical arms, leaving retail's `push -1; brain=0; lea ecx; jg; push A;
      jmp; push B` (sibling evidence: the 47 retail push-in-arms CCharString sites map to landed if/else rows, e.g. 0x00DCF000, 0x0065A8EE; a ternary
      argument or a ternary-into-named-CCharString always compiles to the `mov eax,A; jcc; mov eax,B` select: 0x008559E0, 0x00D26E60).
  (2) `CHeroMarriageInfo info;` needs the inline default ctor (new opt-in FABLE_RETAIL_CHEROMARRIAGEINFO_DEFAULT_CTOR; FableWin ??0CHeroMarriageInfo@@QAE@XZ;
      retail zeroes 5 dwords + 1 byte at 0x006D268E) (+21 bytes).
  (3) SetMarriageState: one call after an `EMarriageState state = 0` set in the arms and `if (state != 0)`: c2 duplicates the call tail into the arms,
      giving retail's `mov eax,N ; push eax` in each (0x006D26FD / 0x006D2730 / 0x006D2746).
  (4) gift opinion: `opinion->RelativeOpinion[3]` through the new opt-in FABLE_RETAIL_COPINIONSET_SUBSCRIPT_CONST (FableWin ??ACOpinionSet@@QBEMJ@Z, float by
      value: the stack temporary GFLimitReturn binds, 0x006D2488) and `float distance = a * a; float b = ...; distance = sqrt(distance + b * b);`
      (retail squares the first term before computing the second) (+16 bytes).
  (5) culling loop: named `end` before the loop (retail keeps the header in ebx across the erase), `next` declared before the loop with `++next` at
      the top, and `long age = GetFrame() - it->second; if (age > Seconds * FPS)` (retail calls GetFrame before reading ConstantFPS).
- Open: register permutation (c2prio tmp/s6d/c2p_y1.txt): hero #15 prio 12 is coloured after the line-246 tracker temp #166 (prio 16) took ebp, so hero
  gets ebx and the const-0 piece #38 (top block to the hero test 0x006D2006) is left ebp. Retail colours hero before #166 (tracker temp spilled
  [esp+18h] at 0x006D2167). Lever to find: hero's priority (refs / live-through) or #166 not callee-saved.
- (s6, later) differ/006d1e30.cpp = tmp/s6d/z1.cpp: + `tracker->GetPThingInside() == wife->GetHomeBuilding()` in the house-dressing test (retail calls
  GetHomeBuilding first, 0x006D235A: VC7.1 evaluates the right operand first) -- that block now differs only by the ebx/ebp permutation. Tried, no
  change: tracker declared before perceive / with deed_log (packer), Peek/direct POpinionReactionManager for the attitude call (retail loads
  ParentThing into edi BEFORE the GetWorld call at 0x006D1ED0 -- unexplained), GetParentThing() arg (worse). The header comment's residue list
  still describes the old draft; the Grade line is current.

## 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera -- wfD4 successor 6 (2026-11-15): 2860 -> DIFFER(2920v3356), 138 -> 107 hunks (tmp/s6a/)
- differ/00715a70.cpp = tmp/s6a/a1s.cpp (STILL the old local class models: the shared-header re-host plan of successor 5 above is not done).
- Measured facts about the 496-byte gap (focus switch, retail 0x00715DD9..0x00715FB8): retail's four focus cases are NOT cross-jumped although
  their x87 code is identical: each case ends with three integer copies of the at_pos temporary (`mov r,[esp+14h] ; mov [esp+4Ch],r` x3) and
  the copy registers alternate eax/edx/eax v edx/eax/edx between cases (ecx is busy: retail holds `tag` in ecx from 0x00715D92 through the
  switch to `mov eax,[ecx+4]` (CameraPosition) at 0x007160B4), so the tails differ by register and c2 cannot merge them. Ours keeps tag in
  esi; the copies use eax/ecx/edx (3 or 6 per case = full rotation) -> identical tails -> merged.
- Fixed (measured): `float height = -(zoom * zoom); C3DVector offset(0.0f, 0.0f, height);` and per case `offset = C3DVector(0.0f, 0.0f, height * k);
  at_pos = from_pos + delta * k;` give retail's x87 0.0 kept on the stack across the dispatch (`fld 0.0 ; fst X ; fst Y`, per case `fst ; fstp`)
  (+60 bytes); `(me->GetPos() - other->GetPos())` (retail calls other->GetPos first). Direct member stores in the cases (a3) are worse (2872).
  No `delta` local (a2: 3276 bytes) also stops nothing (still merged; longer because each case recomputes the difference).
- Retail registers: this = ebp, me = edi, other = ebx, frame 0x7C (ours this edi / me ebp, frame 0x84).
- (s6) differ/006d1e30.cpp now tmp/s6d/n1.cpp: same object as z1 with the natural `float distance_squared = a * a; ... distance_squared += b * b; float distance = sqrt(distance_squared);`.

## 0x0066CF00 CRegionMinimap::PopulateRegionMinimap -- MATCHED (RELOCATION_MATCH, test PASS, read-only vland PASS) (successor 7, 2026-11-15)
- src/0066cf00.cpp = differ/0066cf00.cpp + `#define FABLE_RETAIL_CREGIONMINIMAP_WORLDCOORDS_PAREN` (new additive opt-in in hdr/engine/CRegionMinimap.h,
  hdr/README.md): the inline GetWorldCoordinatesFromTGAPosition text is the INITIALISE text with the X quotient parenthesised,
  `world.Initialise(topLeft.X + ((float)tga.X / (float)size) * squareSize.X, topLeft.Y + (float)tga.Y / (float)size * RegionMapSquare.GetExtents().Y, 0.0f)`.
  Out of line the same text is still 0x0066C810 MATCH (0066cf00_p/s7/oo_paren.cpp, the p11/d.cpp twin). Parenthesising only the Y quotient does nothing;
  both or X only match; other shapes (`squareSize.X * (q)`, `q * sq + tl`) were also tried (0066cf00_p/s7/w*.cpp).
- How it was found (method worth reusing): dumped region 2 / 105 DAGs with tools/sc.sh, replayed them with tools/sched2/sim.py (reproduces ours exactly),
  then searched DAG edits that reproduce RETAIL's pick order at both sites (tmp/s7_search.py, s7_pair.py): the only single edit common to both sites is an
  added edge sub(square extent X) -> fdiv (lat 1). That pointed at the X quotient's expression tree; a parenthesised float sub-expression keeps a no-code
  PAREN tuple in c2's DAG (MATCHING_PLAYBOOK scheduler note) -- first graded variant with it matched.
- FITTED items in the source header: the parentheses (this opt-in), `buildings` declared before regionBox, parentheses round the two `+ boundsRadius` args.
- Coordinator: CEngineThemeDef (padded local model, no engine header exists), RegionMinimapLightingMap / g_RegionMinimapLightingMap (descriptive name) and
  EMinimapThemeType are local declarations inherited from the earlier draft; generating engine/CEngineThemeDef.h would be the clean fix.
- Test test/0066cf00.cpp: model of grid theme mapping / lighting / clip + grown range + mesh marking; mutants (no clip, one case changed, no range growth,
  no IsPosInMap) FAIL.

## 0x0090D030 -- successor 7 (2026-11-15): still DIFFER(3269v3269), 48 hunks / 75 masked lines best (differ/0090d030.cpp unchanged); ~45 grades (tmp/s7/)
- Devices re-measured with the owner's acceptance test in mind: dead FABLE_RETAIL_ASSERT(PTCGossip) x 1,2,3,4,6,8,12 after the first GetTC (tmp/s7/A*.cpp):
  byte-identical (the residue is a register-COST decision, not a key tie, so asserts / k locals cannot move it). Declaration positions of arrived /
  initiator / message / mode_manager (d1..d6): no change. Flags: plain build 3173 bytes (inline decisions wrong), /d2inlS30 alone 3237, /d2inlT100 alone 3205.
- Mechanism (c2prio costs, tmp/s6): every ebx/ebp web follows message's choice: a whole-function constant candidate (e.g. const 0xC1, the GetTC key) left
  with one register adds 100 to that register for every later neighbour (map #62 costs flip ebp:100 <-> ebx:100 with message's register). So only message
  matters; it needs an uncoloured ebx-only (byte, live across calls) neighbour inside its range.
- LEAD (differ/0090d030_p05_lead.cpp, 38 hunks / 65 lines): `initiator = false;` placed right before RemoveMessage(message) (sweep of every statement
  boundary of the message arm, tmp/s7/p00..p08: 45,45,45,42,42,38,49,49,53 hunks) makes initiator overlap message -> message ebp, map web closed. But
  initiator then wins ebx itself (out-prioritises the 0x123E7E0 vtable const), giving `xor bl,bl`; retail keeps initiator in MEMORY: its arm-end stores were
  tail-merged into `xor al,al` / `mov al,1` + one store at 0x0090D3BD, which also means retail's message-arm def sits after the arm's last call (no
  overlap with message there). Else-arm `initiator = true` positions on top of p05 (q00..q05): 49,43,42,61,40,41. Contradiction unresolved: retail needs
  an ebx-only neighbour of message that is spilled, and initiator's retail defs are both at arm ends. Open: some other byte value across calls in
  0x0090D125..0x0090D241 (none visible in retail's bytes: so a spilled candidate), or a different message-arm statement order.
- (s7, later) `initiator = false;` at the message-arm START (p00) also flips message to ebp AND gives the vtable const / originator temp ebx as retail,
  but initiator then becomes a plain memory candidate in a NEW slot [esp+28h] (stores at its defs + join reload) where base and retail share [esp+2Ch]
  with the arm-end stores tail-merged (xor al,al / mov al,1). Base's initiator handling is therefore retail's; the ebx-only neighbour of message must be
  something else. Else-arm positions on p00 / p02 (r*, t*): 46..61 hunks. Chained `MyTurnToSpeak = initiator = false` (c1 53, c2 47), `= !initiator`
  (c3 49): no gain. Parked again.

## 0x006D1E30 CTCWife::FrameUpdate -- wfD4 successor 8 (2026-11-15): 125 -> 13 hunks, DIFFER(3484v3484) (scratch tmp/s8d/, ~75 grades)
- differ/006d1e30.cpp = tmp/s8d/v10.cpp. Byte-identical to retail from 0x006D1E30 to 0x006D291D (incl. frame 0x2C, const-0 ebx, hero ebp,
  deed_log ebx, all frame slots). What closed the 125-hunk register permutation (each measured, EVIDENCED):
  (1) the parent thing read through the inline accessor `GetParentThing()` (CTCBase.h; FableWin.pdb ?GetParentThing@CTCBase@@QAEAAVCThing@@XZ;
      landed same-TU siblings Marry 0x006D18A0 / InitWifeAtEndOfMarriageFrescoStartFadeIn 0x006CF3B0 use it): `GetThingAttitudeToPlayer(GetParentThing())`
      (retail loads ParentThing into edi BEFORE the GetWorld call, 0x006D1ED0), `static_cast<CThingAICreature*>(&GetParentThing())` in the PostDeed /
      SetMarriageState args (retail stores DivorceDeedPending=0 before reloading [esi+4], 0x006D20C1 / 0x006D2107: the inlined accessor's this copy
      blocks the hoist) -- 133 -> 24 hunks; all other ParentThing-> uses respelled GetParentThing(). (byte-identical either way there)
  (2) brain arms: `CThingAICreature& creature = static_cast<CThingAICreature&>(GetParentThing()); creature.SetNewBrain(brain);` (Marry's idiom; retail
      loads ecx=[esi+4] before the CDefPointer arg copy, 0x006D1E88).
  (3) IS_THING_ALIVE<CThingPlayerCreature*> body visible, kept out of line: engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE + _NOINLINE (repo opt-in, as
      chunk 1021): retail keeps the hero in ebp and passes a [esp+20h] copy (0x006D28B9) -- 24 -> 13 hunks.
- Open (13 hunks, all expression temporaries): from the scoreboard key 0x006D291D on, every scratch pick is retail's shifted by one step of c2's
  eax/ecx/edx rotating pointer (docs/pipeline/C2_ALLOCATOR_PROBE.md section 4; tools/c2p/c2prio.py --rotation, tmp/s8d/c2r_v10.txt: ours has no
  rotation/hint event between the stats key (v10 line 368, edx) and the scoreboard key (line 371, eax); retail must have exactly one (taking eax:
  e.g. the HasTC flags load 0x006D2915 or the lovers copy 0x006D290F as an expression temporary instead of a global candidate) -- or eax busy at
  0x006D291D). Byte-identical (no effect): k = 1..7 unused locals (top / courting block), 1..16 dead FABLE_RETAIL_ASSERTs (fable_assert.h, now in hdr/),
  declaration order of hero / stats / scoreboard / sound_player (scoreboard even at function top), nested ifs, `== true`, two-arg GetTC(TCI_*, &p)
  (+FABLE_RETAIL_CTHING_ADDANDCOPYTC), GETTC_EXPANDED, HASTC_FORCEINLINE, lovers as int / const long / `+= 1` / post-increment / implicit float;
  worse: `float lovers`, `long& lovers`, unsigned, `lovers = x + 1; x = lovers`, HASTC_MATERIALISED (3580).
- Anniversary PDB locals (weak source) for this function: pdef, pdef, player, ptc_buyable_house, ptc_deed_log, ptc_volume_tracker, ptc_containment,
  info, ptc_hero_stats, tc_sound_player, player, ptc_hero_stats, next_it (stack-homed only).

## 0x006D1E30 CTCWife::FrameUpdate -- MATCHED (RELOCATION_MATCH, test PASS, addmatch + read-only vland PASS) (successor 8, 2026-11-15)
- src/006d1e30.cpp (= tmp/s8d/fin2.cpp), test/006d1e30.cpp (model test; 5 native mutants fail). The last 13 hunks (rotation offset, see the
  section above) closed with the original helper calls: `stats->LogLoverAcquired();` and `(float)stats->GetNumberOfLoversAcquired()` as the
  scoreboard argument (new overlay opt-in FABLE_RETAIL_CTCHEROSTATS_LOVERS; FableWin.pdb names both, LogLoverAcquired's COMDAT sits in the
  CTCWife TU run and its /Od size is SAME). c2 forwards the stored count and keeps it in [esp+1Ch] across the lookup, which removes one global
  candidate and turns the copy into the expression temporary that advances the rotating pointer.
- Hand-back hygiene done in the same pass: GFLimitReturn<T> = the landed 0x0057EF86 text (conditional form, no `inline`), GetDef<T> by name via the
  repo opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE (was a row-local body), IsMarriedToThePlayer = the landed 0x006CEC80 text
  (`? true : false`): each still RELOCATION_MATCH. Opt-ins used: see the #define block; new in hdr/: CTCHEROSTATS_LOVERS (README). FITTED items
  are marked in the header comment (brain-arm `creature` reference, SetMarriageState state/if, gift-term order, culling-loop locals).
- vland warnings: RELOC_NAME 0x00830130 (NAME_FIXES row already there: IsBrainUpdateZoneActiveThisFrame); RELOC_CONFLICT 0x007448F0 v 0x00744950
  for ?GetExtendedSightRadius -- ours (0x00744950, x ExtendedSightRadius +0x44) is right; the landed 0x00864620 binds it to 0x007448F0 =
  GetSightRadius (+0x40): NAME_FIXES row added.

## 0x00974AA0 -- successor 8 (2026-11-15): length lever found, still DIFFER (8 grades, tmp/s8j/)
- differ/00974aa0.cpp unchanged (3128v3080, 45 esp-masked hunks). New alternative differ/00974aa0_g4.cpp = DIFFER(3076v3080), 53 esp-masked hunks:
  (1) drop FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (declaration-only GetSubDef<T>): 3128 -> 3084. Evidence: retail reloads battleDef inside the main
      loop body (`mov ebx,[esp+1Ch]` at 0x009750CF, after the alive tests), so c2 saw the CDefPointer's address escape at the GetSubDef call; with the
      body visible ours keeps it in ebx across the loop and needs the extra loop-continue reload + duplicated exit tails (the +48 bytes).
      Visible body via engine/CDefClassBase_GetSubDef.h (with or without _NOINLINE): 3128 too.
  (2) `const CThingBaseDef& baseDef = GetCreature().PeekBaseDef();` named before `CDefPointer<..> battleDef;` (FITTED): retail computes PBaseDef
      (`mov edi,[edi+70h]`, creature kept in edi from the boss GetTC) before the ctor's zero store (0x00974B28); without it ours stores first and
      reloads the creature (escaped-object alias): 3084 -> 3076, entry block instruction-equal except frame slots.
- Open in g4: the entry exit (IsStillRunnable fail -> Terminate + battleDef release, retail in place at 0x00974C07 with the later tails jumping to
  0x00974C1A) is laid out at the end in ours (`je 0x975643`); frame slots (boss / key temp / HasTC byte: ours [esp+30h]/[esp+20h]/[esp+1Fh] v retail
  [esp+24h]/[esp+18h]/[esp+23h]); the x87 facing sums; main-loop registers (retail battleDef ebx + physics edi). The local action / Stage2 class models
  still need shared-header opt-ins before this row could land (successor 4 note).
- (s8, later) g4 detail: with the declaration-only GetSubDef the battleDef releases at the exits compile to a long form (`mov esi,[eax+4] ; mov ecx,eax ;
  add eax,4 ; dec esi ; mov [eax],esi ; mov edx,esi ; test edx,edx ; jne`, ours 0x0097561A / 0x0097564A) where retail keeps `dec [ecx+4] ; jne` with
  the null test (0x00974C0E) and no `Object = 0` store -- check which CDefPointer Release text the row's includes select (none is defined in the
  row; a header may pick COUNTREF) before working the tail placement; g5 (named baseDef + visible body) = 3128, same as the main differ.
- 0x00715A70: not opened this session (successor 8 spent the budget on 0x006D1E30, matched, and the 0x00974AA0 length lever).

## 0x0090D030 -- successor 9 (2026-11-15): mechanism of the ebx/ebp cascade found (scratch tmp/s9/)
- The first-half swap is NOT decided at `message`: it is decided at #109 = the arg-block pointer of the greeting LookAtThing
  (`mov ebp,esp` 0x0090D406, prio 80, allowed {ebx,ebp}). Base: costs {ecx:-2} -> ebx; once #109 holds ebx, a whole-function
  constant is left with ebp only and every later candidate gets ebp:100 (message, map, originator temp, vtable const ...).
- Retail #109 = ebp needs an ebx-only (byte, live across calls) uncoloured neighbour over 0x0090D3F9..0x0090D42C. Measured:
  `initiator` is NOT a c2 candidate in base (it is passed straight to AIAction_InteractWith -> memory, MATCHING_PLAYBOOK "named
  bool b; f(b) lives in memory"; c2prio of truncated variants: a byte-store use makes it a candidate 125..178 `.....b.`).
- differ/0090d030_s9_ia1.cpp (`AIAction_InteractWith(..., initiator != false)`): initiator becomes a spilled candidate -> #109
  ebx:800 -> ebp, message ebx:100 -> ebp: the whole message / map / originator / vtable / post-arm web now MATCHES retail. New
  residue: initiator spilled to [esp+17h] with byte stores in both arms + join reload, `mov cl ; test ; setne` at the push
  (retail: `xor al,al` / `mov al,1` arm ends, `mov [esp+2Ch],al` at the join, dword reload + push), so frame slots shift;
  63 hunks / 61 esp-masked lines (base 47 / 75). Home/interaction and LISTENING const-0 still flipped (initiator is split at
  0x635 of the trace, before home #143 is coloured).
- Same class (candidate initiator): `!!initiator`, `initiator ? true : false`, `== true`, `!= 0`, `+initiator`, `? 1 : 0`.
  Simplified back to memory (= base bytes): `(bool)`, `bool()`, `static_cast<bool>`, `| false`, `^ false`, `(int)`, `(long)`, `(char)`.
  `initiator & true` 3285 bytes. Join-computed `initiator = message == 0` etc. after the if: 90 hunks.
- Operands swapped in the home comparison: worse (49). HasTC MATERIALISED / FORCEINLINE opt-ins: byte-identical.
- (s9) LEVEL CHANGE that closed the expression-temp rotation residue: drop `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` (default
  GetTC<T>(T**) text: HasTC + Components.Find + GFDebugPDynamicCast). differ/0090d030_s9_gt0.cpp: DIFFER(3269v3269) 41 hunks /
  57 lines (was 47 / 75): the 0x0090D099..D0BB, D249, D28D, D29B, D2C8, D330 rotation hunks are gone; left = the ebx/ebp swap
  webs (first half), home/interaction, mode-manager creature edi (D6D1), LISTENING const-0. With iA (`bool initiator = message
  == 0;`) on top: 25 hunks (sete residue); with ia1: 59.
- c2 cost source found (c2.dll 0x10784B48: penalty = 100 * neighbour.spill(+0x40) for a neighbour left with one register;
  tools/c2pnb/c2prio.py prints `pen nb #id -> reg`): in base the message's ebp:100 comes from a const-0 PIECE (#29, created when
  the byte-only whole-function const 0 is emptied by the first setne-bl local), left with ebp after #109 took ebx.
- (s9) differ/0090d030.cpp is now the s9 best: default GetTC + `bool initiator = message == 0;` (FITTED) + Anniversary-PDB local names
  (WellingtonGame-Xbox360-Test.4k.pdb S_REGREL32 of CAIStateGroup_MinionGossip::Update: this_sex, their_sex (ESex),
  nav_route_calculated (bool, = the MoveToPosProximity out flag), ptc_mode_manager; names only, bytes unchanged; ESex typing of the
  ternaries does not compile against GetSex's return type, kept long). 25 hunks / 38 lines. Previous best kept as
  differ/0090d030_s8_prev.cpp; unfitted s9 alternative differ/0090d030_s9_gt0.cpp (41 hunks).
- (s9) Opt-in sweeps on the 25-hunk source: removing any single define (41 tried) and adding any of the 182 unused opt-ins of the
  included headers (tmp/s9/sweep_add_iA.txt) never improves (GETTC_EXPANDED back: 32; CONVERT_MUTABLE 31; others worse/fail).
- (s9) Remaining webs, measured with tools/c2pnb (neighbour penalties):
  * home (#146, prio 24, key 550, line 160 temp) costs {eax:-7} -> ebx, then interaction (prio -19, live through the walk loop
    w2 K13) is left with ebp. Retail needs an ebx penalty on home: the first-half const-0 piece (#29, spill 1) that supplies it
    is split when the AddMessage manager pointer (#99, retail ebx 0x0090D2AE) takes ebx; its pieces have spill <= 0. Natural
    rewrites of the sex / interaction / home / roll / walk-loop statements (h1..h5, wl0/wl1, swapped ==): byte-identical or worse.
  * `if (!initiator)` after `bool initiator = message == 0` (no threading: sete + store at the test): 31.
  * a visible (diagnostic, noinline) CThingFilter_IsValidGossipee ctor / operator() above the row: byte-identical.
  * named creature for the mode-manager GetTC (D6D1): byte-identical (forwarded).

## 0x00974AA0 -- successor 9 (2026-11-15): DIFFER(3088v3080), 61 -> 20 struct hunks (tools/006d1e30_sd.py count); best source differ/00974aa0_s9.cpp (scratch tmp/00974aa0_s9/, grader tools/00974aa0_s9v.sh)
- Base = the main differ (visible GetSubDef body KEPT: only with it do the exit releases drop the null test and use `dec [ecx+4]` as retail; the g4
  declaration-only route gives the long COUNTREF-like release form, a dead end). Each lever measured on the main differ:
  (1) LookAt block `CThingCreatureBase& creature = GetCreature(); CThing* target = PTargetThing.Get(); const C3DVector& from = creature.GetPos();
      const C3DVector& to = target->GetPos();` (retail 0x0097512F loads the creature into edi BEFORE GetPItem): 61 -> 48 hunks, 3128 -> 3096; as a side
      effect the main loop now reloads battleDef inside the body like retail (0x009750CF) and the duplicated exit tails shrink.
  (2) named physics first in the main loop (`CTCPhysicsBase* physics = GetCreature().GetPhysicsTC(); float height = ...; arena = ...`; retail edi =
      physics before the GetPos call 0x009750E1).
  (3) drop FABLE_RETAIL_CTHING_HASTC_MATERIALISED: frame slots (battleDef [esp+1Ch], HasTC byte [esp+23h]) and the talk-site `test ch,ch ; jns`
      (0x00975443) now equal (46 -> 27).
  (4) implicit LookAt conversion `AIAction_LookAtThing(AI_ACTION_REPEAT_1, PTargetThing, 0x11, 0.05f)` + existing repo opt-in
      FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE (retail 0x00975168: base ctor 0x00A01B10, vptr store, GetPItem 0x00A01B50 on PTargetThing, SetPItem
      0x00A01B90 = the converting ctor, not ctor(T*)); it changes the inline budget so the entry HasTC bits are no longer materialised unless
      `// Flags: /d2inlS28` (smallest; S<=27 = default; S28..100 identical) -- device (b) candidate, residue it fixes is purely the HasTC inline form.
      CONVERT_MUTABLE text: same bytes. FORCEINLINE_CTOR no longer used by the row (kept defined, no effect).
  (5) `C3DVector position(arena.X, arena.Y, height + arena.Z); physics->SetPosition(position);` (FITTED name, retail stores X before the push).
- Left (20 hunks, +8 bytes): (a) float-up exit tail (Terminate + `dec [edi+4]`) laid out at the END in ours (shared with the 5 wait-loop exits);
  retail keeps it in place at 0x00974E9A and the wait loop jumps back. In the old main differ (wait tail in ebx, not merged) it was in place, so
  the copy that survives the merge is the later one in ours, the earlier in retail. Tried: goto into the float-up block (same), if/else around the
  rest of the function (p3 3108 / p4 float-up merges with tail A), `!(..==2)`, named bool / result (same or worse). (b) case-5 / idle
  IsStillRunnable-fail tail (ours 0x00975642, own release + epilogue) is not cross-jumped into the entry exit's release (retail 0x00975670 jmp 0x974C1A).
  (c) x87 tie in both facing sums: ours squares Y first and reloads X (`fstp [esp+28h]` .. `fld/fmul [esp+28h]`), retail `fst [esp+28h] ; fmul` X first.
  `(X*X + Y*Y) + Z*Z` in the row fixes the grouping (only the inner commute left); a running sum `squared = X*X; squared += Y*Y;` fixes the order but
  keeps `squared` on the stack (`fadd st(1)` + extra pop) or pops Z early; named x / y / z components, k=1..7 unused locals, 1..3 dead asserts
  (boss / battleDef / combat): no change. (d) scale loop: retail `fld step` before the creature loads, `fadd [ecx+34h]`; ours fld scale, fadd step --
  every text order / named step / const / scale+=: same. (e) eax v edx at 0x0097502A (Pillars.size()) and 0x009750C9.
- Header work still needed before landing (unchanged): Stage2 + JackFloatUp / JackShotFloat / JackFlash class models are row-local; the repo has the
  pattern (engine/CCreatureAction_JackShotFar.h / CAIStateGroup_JackOfBladesStage1.h FABLE_RETAIL_JACK_STAGE1_UPDATE branches,
  CCreatureAction_PlayCombatAnimation.h has the 4-arg ctor under FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION + JACK_STAGE1_UPDATE).

## 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update -- MATCHED (RELOCATION_MATCH, test PASS, addmatch + read-only vland PASS) (successor 9, 2026-11-15)
- src/00974aa0.cpp (Flags /d2inlS28, `// Inline-budget:` line) = the 20-hunk differ/00974aa0_s9.cpp above plus, each measured:
  (6) wait loop exit as `bool stopped = false; do { if (!alive..) { stopped = true; break; } ... } while (..); if (stopped) { Terminate(); return; }`
      (FITTED): fixes BOTH exit-tail items (float-up tail in place 0x00974E9A with backward wait-loop jumps; case-5/idle tail cross-jumped
      into the entry release `jmp 0x974C1A`) and the eax/edx picks at 0x0097502A / 0x009750C9 (20 -> 14 hunks, 3088 -> 3076).
  (7) `const C3DVector& diff = X->GetPos() - GetCreature().GetPos();` in both facing blocks (FITTED; default GetSquaredMagnitude text): retail's
      X*X-first `fst [esp+28h] ; fmul` (14 -> 6 hunks, 3080 = retail length).
  (8) scale-up loop `scale = GetCreature().GetTCGraphicAppearance()->GetScale(); scale += step;` (FITTED; retail `fld step ; fadd [ecx+34h]`
      0x00974F37) -> RELOCATION_MATCH. Plain `while (scale < BattleScale)` replaces the if + do-while (byte-identical, natural).
  (9) shared headers: the row-local Stage2 / JackFloatUp / JackShotFloat / JackFlash / PlayCombatAnimation models and enums replaced by the new
      overlay opt-in FABLE_RETAIL_JACK_STAGE2_UPDATE (5 headers, hdr/README.md), FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION,
      FABLE_RETAIL_NCREATUREMODE_QUERIES (engine/NCreatureMode.h IsKnockedDown) and engine/fable_ai_action_enums.h (AI_ACTION_FINISHED,
      ACTION_REPEAT_UNTIL_FINISHED, Ego_r enum records). Still RELOCATION_MATCH. No other matched row of this batch includes those headers.
- The only in-row definition is the inline CJackOfBladesBattleDef::StatGetTypeName (text = the landed 0x008C33E0 row's); GetSubDef<T> body is the
  repo FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (= landed 0x008C33E0 text). vland warnings: DELTA_NOTIONAL_ZERO labelled g_NormaliseEpsilon (LABELS.tsv),
  __CIfmod import-thunk alias (fine).
- Test: test/00974aa0.cpp, a model over a fake world (entry exit, facing angles 0.25 / 0.75 / none, float-up exit, scale steps, wait, pillar choice,
  attack schedule incl. the fall-through fireball count and the second-round flash summon, taunts 0..4, LookAt failure, phase-change end);
  mutants (no fireball count, taunt case 5, fixed pillar choice) fail.

## 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera -- successor 9 (2026-11-15): re-measured only (1 grade, DIFFER(2920v3356), 108 struct hunks; scratch tmp/00715a70_s9/, grader tools/00715a70_s9v.sh)
- New measured fact behind the `tag`-in-esi problem (successor 6: retail keeps tag in ecx through the focus switch, so the four case tails differ by
  copy register and do not cross-jump): retail's esi is the ZERO register. `xor esi,esi` at 0x00715AEB, the eight zero stores of from_thing /
  from_pos / at_thing / at_pos are `mov [esp+..],esi` scheduled AFTER the GetCameraSetupPositions pushes, and the zero stays live across that call:
  the from_thing / at_thing test after it is `cmp ecx,esi ; je` (0x00715B54) then `mov esi,[esp+20h] ; test esi,esi` (esi reused only there).
  Ours zeroes through eax before the pushes, tests with `test ecx,ecx`, and then has esi free for `tag` (`mov esi,[esp+0A0h]` 0x00715ADD) for the
  whole function. So the next step is a source form whose IL compares the first thing pointer against the zero constant after the call (keeping
  the constant's web across it), which frees ecx/esi exactly as retail; then the focus-case tails should stop merging. Also: retail this = ebp,
  frame 0x7C (ours this = edi, 0x8C). Local class models still to be re-hosted (plan of successor 5).
- Not worked further this session (budget went to closing 0x00974AA0).
