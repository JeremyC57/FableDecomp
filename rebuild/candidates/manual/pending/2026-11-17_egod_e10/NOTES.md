# e10 NOTES

## 0x007993F0 CTCShopKeeper::WitnessedCrime -- RELOCATION_MATCH (698), test PASS, vland RELOCATION_MATCH (0 RELOC_IDENTITY)
- Source = e09's best (ref/007993f0/e09_best.cpp: Ego_d locals / scopes, include + opt-in set of landed 0x00682810) with
  FABLE_RETAIL_GFGETWORLD2DDISTANCE replaced by the NEW opt-in FABLE_RETAIL_GFGETWORLD2DDISTANCE_X_FIRST (coordinator decision):
  dx-first body, FITTED by FableWin /Od size (28i/73b = 0x025B03E0). Default Y-first body untouched (landed 0x009711D0 unaffected).
- Matched at the first grade. Test = e09's 22-case test (define updated, body verbatim, diffed equal to src): every early return,
  10.0 exact threshold + 10.5, stock 1/2, shop v stall, talk TC present/absent, reported / opinion / knows branches, severity 1..4.
- Opt-ins also used: e09's CTCSHOP_STALL_STOCK_ACCESSORS and CTCVILLAGE_WITNESSEDCRIME (copied verbatim).
- Names: row Ego_r 0x0079EFA0 / FableWin 0x0269EAE0 + Ego_d. Callee 0x00659C32 = ?StartSecuritySweep@CTCVillage@@QAEX_N@Z (NAME_FIXES,
  from e09: landed 0x007B2FD0 / reloc_bindings spell it RecalculateSecuritySweep(long)). Rename candidates (e09): DIALOGUE_LAYER_0/1/2/NONE
  -> Ego_d NSpeechGainManager::DIALOGUE_LAYER_FOREGROUND/MIDGROUND/BACKGROUND/LAST.

## 0x006C14D0 CTCBuyableHouse::OnInitialActivate -- RELOCATION_MATCH (734), test PASS, vland RELOCATION_MATCH (0 RELOC_IDENTITY)
- Start: e09's shared-header port (probe/r6port.cpp, 666v734 here). Three changes closed it in 3 grades:
  1. the same-TU GetDef<CThingBaseDef> / GetSubDef<CBuyableHouseDef> bodies visible (shared FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY +
     FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, both noinline as retail calls them at 0x006C150A / 0x006C1519) with a NEW engine/CBuyableHouseDef.h
     (Ego_d record 0x2F237) for StatGetTypeName -> the pdef release `dec [edi+4]` form;
  2. FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (existing shared opt-in: GetPItem on the temporary, SetPItem; 0x006C1646 x5);
  3. AddTC("CTCThingOwner", false, 0) with the IMPLICIT const char* conversion (FITTED: retail builds the temporary before the pushes and
     reloads ParentThing after it; the explicit CCharString("..") spelling pushes 0, 0 first).
  Also Ego_d's end_it / it declared together in one block (it not in the for header).
- Test: 13 cases (def index 0 / set, GetSubDef info found / missing with ref counts, owner present / AddTC'd, each sign-post index incl. null
  item and other, last-wins, live post killed(true)+reset, dead kept, Needs false / true with null, dead, and live-at-test post). Mutants
  checked: rent->Dressing, dropped `!IS_THING_ALIVE` -> both FAIL.
- Names: GFRemapToThingAfterSerialisation is ?GFRemapToThingAfterSerialisation@@YIXAAV?$CIntelligentPointer@VCThing@@@@@Z (reference; Ego_r
  0x008CA220 / FableWin 0x0287E7A0); landed 0x006C5680 and reloc_bindings spell it with a pointer (PAV) -> NAME_FIXES. Ego_d parameter `ptr`.

## 0x00761280 NShape::DrawPrimitiveLineOverLandscape -- OPEN DIFFER(733v733), 2 hunks (one store), differ/00761280.cpp
- Port: the landed same-TU 0x00761560 already carries this body (Ego_d names) as an auto_inline(off) stand-in; with shared headers
  (NShape_CClosedShapeBase.h, CIEngine, CEnginePrimitiveLine, CWorldMap), the intrinsic tanh, C3DVECTOR_ACCURATE_MAGNITUDE (inline) and the
  inline CEnginePrimitiveLine ctor via the EXISTING opt-ins FABLE_RETAIL_PASS22_CREATURE_GENERATOR_DEBUG + FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
  (retail builds the line in place: Type 6, theme bytes, Width 0.05f, call [edx+50h] = AddPrimitive) it is 733v733 at once.
- Residue: max_index's home-slot store `mov [esp+20h],edi` sits right after the first start_pos copy load (ours 0x00761374) where retail
  puts it after the GetGroundSizeZAt argument push (0x0076138B). The old draft had the same one-store residue after 12 probes.
- Tried (~33 grades): max_index `+1` / `++` / `max_index++`; start_pos declared first (init: copy hoisted to the top; uninit + assign: 735);
  start_pos copy placed before line / magnitude / max_index (t6: the store lands right but the copy moves); if/else end_pos (753);
  Ego_d record-order text with (to-from).GetMagnitude (767); weight/GRANULARITY naming swaps; no-code casts / parens (81-tuple cut idea):
  `(float)(weight*5.0+0.25)` moved the store ONE instruction later (2 hunks, the differ source); further casts / parens / GetMagnitude no move.
- /Od size (fwsize) of the best text 146i/475b vs FableWin 162i/525b (= Ego_d debug length): the original text is longer by ~16i/50b
  (an if/else end_pos adds 6i; the Ego_d-order text with a separate (to-from) gets 157i/509b but is wrong in release). Unknown extra
  statements: next idea is to find them (e.g. a named temporary for the midpoint, an assert, GRANULARITY used in the loop).
- Ego_d facts not yet used: file-scope `const float DRAW_LINES_GROUND_HEIGHT_OFFSET` (LDATA32 in tc_geometric_shapes.obj, type const
  float) is almost certainly the 0.15f (POINT_PROXIMITY_FOR_SELECTION / MIN_DISTANCE_FROM_TRACK_FOR_SELECTION are its neighbours).

## 0x008F0BD0 CTCTavernGameCardBase::DealCards -- RELOCATION_MATCH (722), test PASS, vland RELOCATION_MATCH (0 RELOC_IDENTITY)
- Rewritten from scratch over the shared headers (the old draft was a local model with gotos and made-up member names): include /
  opt-in set of landed 0x007DB2E0 (blackjack NewRound) + existing opt-ins BLACKJACK_DEAL_PASS23 (GetPackPosition, CCard::Move decl),
  C2DVECTOR / C2DVECTOR_FROM3D, CTAVERNGAMECARDBASEDEF_CARDORDERS / _TYPENAME, CDEFCLASSBASE_GETSUBDEF_BODY. NO new header opt-ins.
- Ego_d structure: pdef / card_order / `bool moving` at function scope; if (all four bounds 0) index = 0; else if (!card->Moving)
  { ++index; if (index == Cards.size()) { SetState(ETGS_CARDS_FINISHED_MOVING); return; } } else moving = true; if (!moving) { offset
  block } else if (fabs(NewLocalPos.Z - height) > DELTA_NOTIONAL_ZERO) { current_position / y_dist / base_point / x_dist { new_pos } }.
- What closed it (8 grades): same-TU bodies in Ego_d order -- CCard::Move before the row (VC7.1 inlines it at the placing site and calls
  it at the second, as retail), GetCardDef after the row (plain, no inline control; its visible body + the shared GetSubDef body keep pdef
  in edi and give the `dec [edi+4]` release); implicit const char* PlaySoundW argument; FITTED grouping parens
  `(mult_to_stay_off_table * CardSeparation) + TableSeparation` (no code; moved the new_pos Y store after the Z fstp).
- Helpers graded at their own addresses with this file: CCard::Move 0x008F0660 MATCH, GetCardDef 0x008F0B20 RELOCATION_MATCH,
  GetSubDef<CTavernGameCardBaseDef> 0x008F27A0 RELOCATION_MATCH (landed versions are local models: port candidates).
- Names: `mult_to_stay_off_table` = Ego_d file static (LDATA32 float, tc_tavern_game_card_base.obj); retail .data 0x0138927C = 2.0f,
  read only here. vland WARN RELOC_CONFLICT vs landed 0x007DAEF0 is expected: the blackjack TU has its OWN file static of that name
  (Ego_d LDATA32 in tc_tavern_game_card_blackjack.obj; 0x01383C98). vland WARN RELOC_NAME: 0x0129BA3C is bound ?DELTA_NOTIONAL_ZERO@@3MB
  (reloc_bindings, from 0x0040ED10) but labelled ?g_NormaliseEpsilon@@3MB somewhere (label fix candidate). ETGS_CARDS_FINISHED_MOVING = 21
  is PROVEN by Ego_d (LF_ENUM 0x3394E CTCTavernGame::ETavernGameStates; the header calls the enumerator names unproven: all 48 are in
  Ego_d). Ego_d CCard::Move parameters new_pos / immediate; GetCardDef pdef / new_pdef.
- Test: 14 runs: empty area, each bound alone non-zero, bounds kept, Move's moving branch, last card -> state 21, height on / exactly eps
  above and below / beyond, pack distance exactly on width+height (both signs) / beyond width / beyond height; ref counts. Mutants
  killed: dropped `&& MaxY == 0`, `x_dist >=`, `>= DELTA_NOTIONAL_ZERO`. (The four bound `<`/`>` v `<=`/`>=` mutants are equivalent:
  equality stores the same value.)

## 0x007C7C40 CTCTurncoat::FrameUpdate -- RELOCATION_MATCH (744), test PASS, vland RELOCATION_MATCH (0 RELOC_IDENTITY)
- Rewritten over the shared headers (old draft: local model): include set of landed 0x007C6E50 (CTCTurncoatSpell::FrameUpdate) +
  FABLE_RETAIL_CTCTURNCOAT, NEW engine/CTurncoatDef.h (Ego_r layout; RTTI base checked), existing FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE (retail calls
  push_back 0x004AD520; declaration only, so no inline-budget device). Ego_d locals verbatim (parent as CThingCreatureBase&, the two fx).
- Closed in 9 grades: first port 794v744 (push_back inline) -> push_back out of line 744v744 with one residue (PDef temporary esi v ebp)
  -> same-TU IsTurncoated body used in the condition (no change) -> cur_stage computed FIRST in the stage block -> RELOCATION_MATCH.
- Same-TU bodies after the row (Ego_d order): IsTurncoatable 0x007C7870 MATCH, IsTurncoated 0x007C7890 MATCH (graded with this file).
- FITTED: IS_THING_ALIVE(fx.Get()) in the kill loop (one GetPItem tested); cur_stage-first order; implicit const char* arguments.
- Test: 14 runs over every condition (override / turncoatable, exact resistance boundary, timer 2->1 / 1->0, deactivate effect,
  turncoated effect alive / dead / null, sound TC, dead parent, recovery on / off / clamp, grow with alive / dead / null effects, shrink
  with alive / dead, unchanged, interp clamp at 0 and 1). Mutants killed: IS_THING_ALIVE(TurncoatedEffect) forced, timer `>= 0`, clamp 1->2.
  (Parent null is not run: the grow path would call GetPos on the null parent, as retail would.)

## 0x0065CB6B CTCVillage::ReportCrime -- UNTOUCHED by e10 (analysis only, not graded: context budget); differ/0065cb6b.cpp = the old local-model draft (DIFFER(747v747))
- Analysis done (retail read end to end) and a plan for the shared-header rewrite; two overlay opt-ins PREPARED but parked in
  probe/r5hdr/engine/ (not in hdr/, so no matched row depends on them):
  * CTCHeroOpinionDeedLog.h FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CRIMES: SetVillageForCrimes(CThing&) 0x006C55E0, SetCrimeDeedReportedToVillage(long)
    0x006C5A50, FindCriminalDeed(EOpinionDeedType, const CThing*) 0x006C5800 (Ego_r decorated names).
  * CTCHeroStats.h FABLE_RETAIL_CTCHEROSTATS_CRIMECOUNTS (nested in _PERSONALITY): inline SetNumberOfCrimes / GetNumberOfCrimesOfMurder /
    SetNumberOfCrimesOfMurder / SetTotalFine (Ego_d tc_village.obj keeps them right after ReportCrime; FableWin decorated names confirmed).
- Ego_d structure to follow: player (CThingPlayerCreature*) at function scope; { ptc_deed_log { deed { cur_fine_amount, pdeed_effects_def
  { HeroStats { ptc_hero_scoreboard } } { ptc_hero_scoreboard, old_crime, ptc_hero_stats } (murder, DeedType 9) { old_crime } (types 7/8) } } }.
  Retail facts: TU is optimize("y",off)+("s",on) (frame pointer); every GetTC is the expanded out-param form with the out-of-line
  bitset::test 0x00410DE0 (FABLE_RETAIL_STLP_BITSET_TEST_DECL) and its result unused (on a miss retail reads the uninitialised local:
  0x0065CBD0 `mov esi,[ebp-0Ch]`, 0x0065CCB6 `mov edi,[ebp+8]`, 0x0065CD74, 0x0065CDB5); first scoreboard via
  GetPlayerManager().GetMainPlayer() and the CPlayer CIntelligentPointer at +0x34 read in place (`lea ecx,[eax+34h] ; call GetPItem`: needs an
  inline PlayerCharacter accessor -- only the out-of-line GetPPlayerCharacterThing 0x00487B70 is declared today); the murder-branch stats /
  scoreboard GetTCs are on `player`; NumberOfCrimes is bumped before TotalFine += cur_fine_amount; activities 0x23 (total fine), 0x22
  (crime count), 0 (murders); PlayersCrimes.find 0x00614A10 compared with the header word, recursive ReportCrime, RemoveCrime(deed_handle),
  deed_handle = -1 (`or [ebp+8],-1`); the final insert 0x00514230 (hidden pair result at [ebp-18h]) when deed_handle != -1.
- Next: port with the row-1 include set + FABLE_RETAIL_CTCVILLAGE / _CRIMES / _WITNESSEDCRIME + STLP_SET / SET_FIND + the two parked
  opt-ins + a CPlayer inline PlayerCharacter accessor opt-in, then work the frame-slot residue the draft parked on (shared key slot).
