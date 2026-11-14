# wfD3 NOTES (acctC lane, quest-script coroutine rows)

## 0x00D3D150 NScript::CQ_GuildTrainingScript::CAppleGirl::Main -- parked DIFFER(4252v4252), 10 hunks (same residue as hand805/823/835)
- Name: entity vtable 0x012CD7E8 slot 1 (landed Alloc 0x00D3D060), Ego_r + FableWin ?Main@CAppleGirl@CQ_GuildTrainingScript@NScript@@EAEXXZ (draft header, rechecked).
- Extent: 0x00D3D150..0x00D3E1EB (4252, ends `ret`), confirmed by graded length equality.
- The stale draft ports unchanged onto today's headers + overlay (CQ_GUILDTRAININGSCRIPT_ENTITIES is in the repo header now).
- Residue: in the MANY_MORE_APPLES / NO_MORE_APPLES arms (0x00D3DCA2..0x00D3DD5A) retail: vtable temp eax, line-string lea ecx, this-load after the push;
  ours: edx / eax / this-load hoisted (the ONE_MORE_APPLE arm pattern). Retail's 8 AddNewConversation sites: edx at D3B8, DA9B, DC39; eax at the rest.
- Tried here: k = 1..6 dead initialised named locals at function top (no change: not a symbol-count tie).

## 0x00ED17D0 NScript::CV_SingingStonesScript::CManWithDoorName::Main -- RELOCATION_MATCH, test PASS, addmatch + read-only vland WINS
- Name: Ego_r + FableWin ?Main@CManWithDoorName@CV_SingingStonesScript@NScript@@EAEXXZ; entity vtable 0x012F2C98 slot 1 (Alloc 0x00ED16E0).
- Extent 0x00ED17D0..0x00ED2886 `ret` (4279), confirmed.
- What closed it (from the stale DIFFER(4149v4279) draft, in ~25 grades):
  1. Loop shape `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return; ... }` (EVIDENCED by the c2 inline trace: do/while
     and while(!T) give all six movie sites f 0.2-1.0; the for(;;NSF) model gives bribe 0.198, intro 0.076, the rest < 0.04, so only the
     bribe-site implicit movie ctor and one of its exit dtors expand, as retail).
  2. Movie ctor/dtor IMPLICIT (dropped the draft's OUTOFLINE_CTOR/DTOR opt-ins) + `// Flags: /d2inlT51` (window T51..T54; smallest chosen).
  3. Predicate copy called inside the expanded CIsThingAlive ctor: scoped `#pragma inline_depth(0)` around the row-local inline ctor +
     existing opt-in FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (CPredicateBase.h documents exactly this recipe). depth(1) calls both the
     CPredicateBase ctor and the copy, depth(2) expands both.
  4. FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (existing per-TU opt-in) for PresentedItem == "OBJECT_GEMSTONE_RUBY" (score 115 at f 0.078).
- Tool: tools/ct.py (call-target sequence diff by retail address) -- showed the 4 call-level differences at once.

## 0x00DC4D70 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Main -- parked DIFFER(4255v4319), 13 hunks
- Name proven: Ego_r + FableWin ?Main@CSecretPassageMan@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ (the catalog's "CSecretPa..."),
  entity vtable 0x012DA6A8 slot 1 (Alloc 0x00DC4C40). Extent 0x00DC4D70..0x00DC5E4E (4319), ends `ret`.
- Closed here: predicate copy/dtor (retail calls 0x004ABE90 / 0x004AA840 inside the EXPANDED CIsThingConscious ctor/dtor): `#pragma inline_depth(0)`
  around both row-local inline bodies + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (retail's expanded dtor stores only 0x012C2FE8, no base store). The
  call-target sequence then differs only in two exit pads.
- Residue: constant-0 register piece. Ours re-zeroes ebp right after the talk-block movie (0x00DC5114 `xor ebp,ebp`), keeps it through the hit
  block (`cmp ecx,ebp` in GetPos null tests at 0x00DC52CD / 0x00DC549F where retail has `test ecx,ecx`) and the loop bottom (xor at 0x00DC557C v
  retail 0x00DC5582); retail's second expanded seh_me exit pad (0x00DC56F1) zeroes esi locally, so the two pads stay separate; ours share ebp
  and tail-merge (-64 bytes). /d2inlT70..82 do not help (T>=75 expands extra STB dtors).
- Next: c2prio trace of the const-0 candidate pieces (lesson 0x006D0830); loop-model variants of the wander loop (frequency feeds RA priority).

## 0x00DCFA60 NScript::CQ_OrchardFarmRaidScript::DoCutsceneIfRequired -- near miss DIFFER(2412v2412), 4 hunks, call list identical
- Name: Ego_r + FableWin ?DoCutsceneIfRequired@CQ_OrchardFarmRaidScript@NScript@@QAEXXZ. Extent 0x00DCFA60..0x00DD03CB (2412), `ret`.
- The repo now has engine/CQ_OrchardFarmRaidScript.h (no overlay needed; the hdr_STALE copy is obsolete).
- Residue (call-level, tools/ct.py): one extra expanded CScriptThing release: the post-region-wait `if (T) return;` (0x00DCFC14) jumps to
  the shared called tail 0x00DD03B8 in retail; ours expands it (pad 0x00DCFE63) and tail-merges it with the in-wait pad; plus the
  DeactivateQuest `lea` schedule at 0x00DD033A. Inline trace (default T): ~CScriptThing sites f 1.25 (sc 56) / 0.625 (sc 65) / 0.156 (sc 106).
- Tried here: /d2inlT47..66 (all worse: T<=66 stops expanding the CCountedPointer assignment, score 67). Chunk 868 tried 20 loop forms.
- Next: identify which trace index is the post-wait return (marker calls), then a loop form giving it f < 0.5.
- UPDATE: closed (A): outer loop rewritten `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return; ... }` (trace: top
  return f 2.5, in-wait 1.25, post-wait 0.625 sc 65) and a scoped in-body `#pragma inline_depth(0)` / `#pragma inline_depth()` around the
  post-wait `if (IsActiveThreadTerminating()) return;` only (T alone cannot: the CCountedPointer assignment scores 67 and must expand).
  Now 2412v2412, 4 hunks: only the DeactivateQuest block schedule at 0x00DD0338 (see differ/00dcfa60.cpp line 2). Tried 0/false/0UL,
  explicit CCharString temp (2411), 1..9 dead locals: unchanged.

## 0x00D4A270 NScript::CQ_GuildTrainingScript::CCombatApprentice::Main -- not authored (skeleton only)
- Name: Ego_r + FableWin ?Main@CCombatApprentice@CQ_GuildTrainingScript@NScript@@EAEXXZ (landed siblings Alloc 0x00D41A30 / Init 0x00D41AE0).
  Layout (struct_layouts_egor): ParentClass +0x14, PMasterData +0x18, WaitingForFight +0x1c (bool), BadHit +0x20 (long); 0x24 bytes.
  The class is only forward-declared in engine/CQ_GuildTrainingScript.h: a new opt-in (e.g. FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_COMBATAPPRENTICE)
  is needed, modelled on the ..._MELEEAPPRENTICE block.
- Extent: 0x00D4A270..0x00D4C71C (9388) = code to 0x00D4C6FF + 7-entry jump table 0x00D4C700 (switch at 0x00D4C03B `jmp [ebp*4+0x00D4C700]`).
- Machine skeleton (tools/sk.py: Game / Me slot names from the header comments, strings, branches; tools/fold.py folds the
  movie+pause+Speak blocks): ref/00d4a270/skeleton_raw.txt (777 lines), skeleton_folded.txt (511 lines). Content: STB acquire loop,
  wander + Q_GuildTrainingDeparture info, melee-apprentice comments (APPRENTICE_COMMENT / NOT_APLUS / APLUS), ~20 speech blocks
  (movie ctor/dtor CALLED everywhere: Construct 0x006E7B60 / DestroyOutOfLine 0x006E7B80 -> the OUTOFLINE_CTOR/DTOR opt-ins), a
  yes/no departure question, the whisper melee fight (teleports, health bars, hit / block / battle insults with CTimer), a 7-way switch.
  Caveat: a few slot names in the skeleton look shifted (e.g. "MsgIsRegionUnloaded" taking "SCRIPT_NAME_HERO") -- check slots by number.
- Not drafted in this run: a ~350-line hand authoring job; the batch's near misses got the remaining budget.
- (row 0x00DCFA60, later) the same `push -1 ; lea ecx ; push str` interleave appears naturally in the landed same-class row
  CQ_OrchardFarmRaidScript::Init 0x00DCC613 (second of two back-to-back CCharString ctors); duplicating the post-arm tail into both
  arms (so cross-jumping rebuilds the merge) gave 2510 bytes: not it.
- (row 0x00D3D150, later) guarded do-while loop models: first loop -> 4008 bytes; second loop -> same residue + one block reorder.
- (row 0x00DC4D70, later) 1..12 dead locals and the do/while second loop: unchanged (13 hunks); while(!T) second loop 4271.
- Row 0x00ED17D0 also RELOCATION_MATCHes with FABLE_HDR_OVERLAY pointing at an empty directory (repo headers only).

## 0x00D4A270 CCombatApprentice::Main -- RELOCATION_MATCH, test PASS, addmatch + read-only vland WINS (successor agent)
- Hand-authored from the disassembly in ~3 windows (tools/an.py annotated listing + tools/v.sh ranges) in the idioms of the landed
  same-class CMeleeApprentice::Main 0x00D40CF0 (macros, START loop, moan block) and the CQ_GuildTrainingMeleeScript::CTheRealGuildmaster
  ::Main 0x00D58490 fight / grade / cast-map text (std::map + RunCutsceneMacro, CTimer inline texts, pGlobalScriptDef GUI_* fields).
  First grade 9372v9388; after the `else { T; if (HeroTakingGuildTest) ...}` fix 9388v9388 / 21 hunks, all in exit pads.
- Residue = inline decision only: c2 trace (tools/inl.sh) shows ~CTimer (ChatTimer) expanded at 8 return sites; retail at 4
  (scores 18/26/33/25), calling 0x00CD4470 at the other 4 (scores 70/69/65/65: 0x00D4A4F1, 0x00D4A5C2, 0x00D4A7A2, 0x00D4A8D0).
  /d2inlT impossible (START wait seh_me dtor score 68 must expand); /d2inlS5..55 no. Closed by scoped `#pragma inline_depth(0)` /
  `#pragma inline_depth()` around those four `CA_T;` statements (EXTRA_RULES 2026-11-08 form 3; marked `// Inline-budget:`).
- Header: NEW overlay opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_COMBATAPPRENTICE (engine/CQ_GuildTrainingScript.h, appended after the
  SPEEDTEST block; no assert moved). Extent confirmed: code to 0x00D4C6FF + jump table 0x00D4C700..0x00D4C71B (oracle length 9388 OK).
- Names: row Ego_r + FableWin; locals from the Anniversary PDB S_REGREL32 list (ChatTimer, HasInformation, CombatApprenticeTargetMarker,
  Whisper, InsultTimer, Power, speech_movie, PauseMyEntitiesPlease, seh_Hero, seh_Whisper, mymap); descriptive: answer, healthBar,
  heroStartHealth, whisperStartHealth, fightAborted, guildWarning, heroEndHealth, whisperEndHealth, score, grade, i; macro names CA_*.

## Successor agent: new evidence on the three open rows (all still open; best sources unchanged in differ/)
- 0x00DCFA60 (near miss, 4 hunks): c2 list-scheduler trace (tools/sched.sh = hand892 sched/t13 re-pointed; region of 41 tuples
  starting at the shared ~CCharString 0x00DD032F): IL order of the DeactivateQuest block is ours (`push -1, push str, lea ecx, call`;
  `mov, mov, push ebp, lea eax, push eax, call`). Both residue spots are exact PRIORITY TIES (cycle 3: push str idx4 v lea idx5,
  prio 39936 each; cycle 8: push ebp idx9 v lea idx10, prio 37888 each) broken by the lower IL index. Retail picked the lea both
  times, so in retail's IL the two leas come BEFORE the pushes (or sit one level deeper): the temp's address is materialised before
  the ctor's string push and before the 0 push -- look for a spelling whose IL computes &temp first (conversion result reused as the
  argument). Tried here without change: explicit `CCharString("...")` (2411: pushes 0 before the ctor), `CCharString("...", -1)`
  (2411), a scoped named `CCharString quest` (same 4 hunks), a probe-only default argument `unsigned long = 0` on DeactivateQuest
  (same), grouping parentheses on float / int arguments before and after (same). Grades this session: 8.
- 0x00DC4D70 (parked, 13 hunks): c2prio trace (tools/c2prio.py from wfD1): const 0 (#1, 91 refs) splits; piece #29 (prio -181,
  only ebp allowed) is coloured ebp at step 610 because its spill weight is > 0 -- that is the piece covering the hit block's two
  GetPos null tests (0x00DC52CD / 0x00DC549F) and the two T exits 0x00DC5261 / 0x00DC5431 whose pads retail keeps separate with a
  local `xor esi,esi` (0x00DC56F5 / 0x00DC5722). Retail's piece must have spill <= 0 (immediate): find which reference in that
  region (trace lines 700-1400, `#1 const 0 referenced, cost 2: +N`) ours has extra. Grades this session: 1.
- 0x00D3D150 (parked, 10 hunks): c2prio shows the arm temps are colour-identical in all three arms (Game temp ecx, Conversation
  ebp, vtable temp ebx); the residue (AddNewConversation vtable eax v edx, AddLine string lea ecx v eax with the Game reload after
  the push) is in the local allocator / scheduler after colouring, not a global candidate. Retail's arms 2/3 load ApplesGiven into
  eax for the `test eax,eax` arm test (0x00D3DC8E) that arm 1 lies between in layout. Tried (no change, the arm register choice
  is the same in every shape): switch(ApplesGiven) {case 1 / default / case 0} (4256, sub/dec compare), nested `else { if (!=0) ...
  else ... }` (10 hunks), `==0` arm order (11), one outer `long Conversation` for arms 2/3 or for all arms (10). Grades this
  session: 6. The difference must be inside the arm bodies of retail's arms 2/3 v arm 1 (statement or temporary spelling).

## Decisions for the coordinator
- 0x00ED17D0: accept `// Flags: /d2inlT51` (inline-budget window T51..T54, smallest chosen) + a scoped `#pragma inline_depth(0)` around the
  row-local CIsThingAlive ctor + the existing repo opt-ins FABLE_RETAIL_CPREDICATEBASE_NOVTABLE and FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
  (all marked in the source header with `// Inline-budget:` lines and retail addresses).
- 0x00DCFA60 (near miss, not handed as matched): its best source uses an in-body scoped `#pragma inline_depth(0)` around one
  `if (IsActiveThreadTerminating()) return;` -- an inline control for a pure inline-decision residue (EXTRA_RULES 2026-11-08 form 3).
- No new overlay opt-ins, no name fixes. Row 0x00D4A270 needs a new CCombatApprentice opt-in in engine/CQ_GuildTrainingScript.h when drafted.
- 0x00D4A270: accept four scoped `#pragma inline_depth(0)` regions (one `if (IsActiveThreadTerminating()) return;` each; evidence in
  the source's `// Inline-budget:` lines) and the new overlay opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_COMBATAPPRENTICE.
