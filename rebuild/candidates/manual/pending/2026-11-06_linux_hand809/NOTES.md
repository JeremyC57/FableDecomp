# Chunk 809 notes (six gap-scan entity Mains)

Result: 3 RELOCATION_MATCH + PASS (vland, tmp/vland.log), 3 parked. Every name was proven from two sources: the address
appears once in .rdata, at slot 1 of the entity vtable that the landed Alloc / ctor stores, and `?Main@<Class>@<Parent>@NScript@@EAEXXZ`
is in both the Ego_r.pdb and FableWin.pdb name sets. Each source names its callees and the evidence in its `// Callees:` / `// Data:` lines.

## 0x00DA0EF0 NScript::CQ_MinionCampScript::CBriarRose::Main -- RELOCATION_MATCH (3 attempts)
- Shape: a generated entity script (a T-check at the start of every nested block and after every WAIT loop). Retail has no
  out-of-line copy of DoChant or DoOnHeroHitSpeech, though both are in both PDB name sets, so Main expands them.
  - DoChant must be `__forceinline` (it has two sites; plain inline leaves both as calls).
  - DoOnHeroHitSpeech is plain inline (one site).
- Key spellings:
  - ONHIT_STAGEONE: the conversation line is a named block-scope `CCharString Line("...")` built before AddNewConversation. The
    FOLLOW line stays a temporary.
  - The chant bar is `AddQuestInfoBar(72.0f, 0.0f, CRGBColour(0,255,0,255), CRGBColour(255,0,0,255), "", "", 1.0f)`. Arguments are
    evaluated right to left, so the red colour is built first.
  - The CCharString + CCharString in the source needs FABLE_RETAIL_CCHARSTRING_CONCAT_STRING (0x0099F570). Without it the call is
    ambiguous (C2666).
  - The intro picks CS_..._LOWER when the distance to the upper rose marker is greater than the distance to the lower one.
  - The stage-4 effects loop uses a row-local std::vector<CScriptThing> stand-in (out-of-line ~vector 0x008AC970).
- New header engine/CQ_MinionCampScript.h (see hdr/README.md).
- Descriptive local names (no PDB local names): Line, SaidFollow, MoveState, HeroFurther, Shrine, UnusedString, HitCount, Minions.

## 0x00F063B0 NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Main -- RELOCATION_MATCH (2 attempts)
- Shape: the CAT_ConvoyMember 0x00F0CAA0 model. A T-check comes before every statement; the main loop is
  `for (;;) { ...; NewScriptFrame(); T-return; }`; the expression name is a function-scope CCharString.
- a1 had 46 difflines: the expression `==` chain reloaded the data pointer after each Compare. The fix was a same-TU
  `inline __declspec(noinline)` CBasicString<char>::Compare body (chunk 807 lesson), which keeps the pointer in esi as retail does.
- Five new bindings, all in PDB spelling: SwitchBanditAllegience 0x00F079D0, AddBanditHealthBar 0x00F07A80,
  WatchForBanditSpeechMarkers 0x00F07B80, AskBanditHireQuestion 0x00F07ED0, BanditHireSetup 0x00F081E0. Strings 30/30.

## 0x00F0D630 NScript::CQ_AmbushTradersScript::CAT_ConvoySpy::Main -- RELOCATION_MATCH (2 attempts)
- Shape: function-scope locals (the expression name, the teleport record, an unused CScriptThing, seh_me) and the loop
  `for (; !IsActiveThreadTerminating(); NewScriptFrame())`.
- The unused default CScriptThing needs a row-local `inline CScriptThing::CScriptThing() {}`. With it VC7.1 expands the first site
  and calls 0x006E7B40 at the later region_exit site, as retail does.
- Four new bindings in PDB spelling: WatchForSpyDeath 0x00F0EDA0, WatchForHeroProximity 0x00F0EE90,
  WatchForSpySpeechMarkers 0x00F0EF60, ProcessInitialSpyExtraction 0x00F0F680. RefreshSpyAllegiences 0x00F05A10 is reused in its
  correct NScript spelling (see NAME_FIXES.tsv). Strings 45/45.
- vland warns RELOC_NAME on 0x00CDBFB0, the cast-map dtor. The catalog label CRetailTree_erase00cd344a is a placeholder; the map
  spelling used here is the proven one (labels_chunk584, naming wave 1).

## 0x00DAC760 NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Main -- PARKED, DIFFER(5906v5867), 93 difflines (18 attempts)
- Shape: `for (;; NewScriptFrame())` with the T-check at the top of the body. This loop form fixes the latch where retail zeroes
  ebx. The GetHealth > 0 speech blocks follow the CNOVI_Victim 0x00DBCD60 model.
- Facing calls: at the first (movie-scene) site the call is `EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false)`. The
  second site stays (Me, GetHero(), false). Flipping both (g_r3) was wrong.
- Residue: one inline-threshold split of the implicit seh_me (STB) dtor. VC7.1 expands it at one T pad more than retail does,
  so the operator-delete tails are shared the other way round.
- This exact source grades RELOCATION_MATCH under `/d2inlT72` (also T70). That flag is diagnostic only, not a landing flag, so the
  row is a park; no /d2 flag is left in the source. /d2inlS has no effect on this row.
- Descriptive names (no PDB name): 0x013AC840 g_NOVIGuardArrestDistance (3.0f) and 0x013AC844 g_NOVIGuardNoticeDistances
  ({0,5,10,15}, indexed by min(bad deeds,3)).
- Header: the CNOVI_Guard opt-in in engine/CQ_NewOakValeIntroScript.h.

## 0x00E8EF80 NScript::CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main -- PARKED, DIFFER(6038v5996), 85 difflines (13 attempts)
- Shape: the landed sibling CGhostGrannyDaughterInLaw::Main 0x00E8BEF0 plus the cast-map cut scene of CGhostGrannyNecklace::Main
  0x00E8DFA0. The repo header already declares CGhostGrannySon, so no header change was needed.
- Residue: the same STB-dtor inline split as the Guard row. Retail calls 0x007E74D0 at the T pad of the first talk block; VC7.1
  expands it there.
- This source grades RELOCATION_MATCH under the diagnostic `/d2inlT72`, so the row is a park.
- s_v10 tried a row-local user-declared `inline ~STB() {}` (needs the OUTOFLINE_DTOR declaration). It took the row to 27 lines,
  but every expanded copy then stores the STB vptr, so retail's dtor is the implicit one. differ/ holds s_v6 (implicit dtor).
- Ruled out on the frequency side (identical code): a named bool, `!(!A&&!B)`, and a do-while(0) loop head.

## 0x00E9E420 NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main -- PARKED, DIFFER(6035v5915), 440 difflines (11 attempts)
- Shape: three unused function-scope locals (a timer, a CScriptThing, a timer) and the main loop `for (;; NewScriptFrame())` with
  the T-check at the top. That loop form keeps the OpinionDeedTimer index in ebx, as retail does.
- Residue:
  - Per-exit dtor splits: retail expands ~CTimer, ~CScriptThing and the movie dtor at the natural exit and the outer T pads, but
    calls 0x00CD4470 / 0x004AA840 / 0x006E7B80 at the shared nested pad. Ours expands one more site of each.
  - The in-loop LookAtMe.IsAlive is a call to 0x004AB130 in retail; ours expands it.
  - Ours keeps 0 in ebp where retail pushes immediates.
- /d2inlS6-8 changes nothing; S12 gives 537 lines and S20 gives 720.
- This is the same family as the chunk 807 park at 0x00EE57B0.
- New header engine/CV_IntroductionToTrophiesScript.h.
- OnPredicateFail 0x00E9DA80 (landed) is in the FableWin.pdb name set but not in Ego_r's. Its vtable slot 5 is the second source.

## Labels to correct (LABELS.tsv)
All of these are landed rows whose current labels are placeholders or wrong:

| Address | Current label | Correct name |
|---|---|---|
| 0x00D9E9C0 | sub_ | CBriarRose::GetParentScript |
| 0x00D9E9D0 | _unattributed::SetFlag_00d9e9d0 | CBriarRose::OnPredicateFail |
| 0x00D9FEB0 | CVtable_012d6d0c::... | CBriarRose `scalar deleting destructor' |
| 0x00DA2CA0 | FUN_ / Game_InitializeRoseCharacter | CBriarRose::PlayCutscene |
| 0x00F03840 | sub_ | CAT_BanditAlly::GetParentScript |
| 0x00F0A8E0 | _unattributed::PersistTransfer2 | CAT_BanditAlly::OnPersist |
| 0x00F04400 | sub_ | CAT_ConvoySpy::GetParentScript |
| 0x00F04470 | sub_ | CAT_ConvoySpy::OnPersist |
| 0x00DAC620 | sub_ | CNOVI_Guard::GetParentScript |
| 0x00E8CB10 | sub_ | CGhostGrannySon::Init |
| 0x00E8CB00 | sub_ | CGhostGrannySon::GetParentScript |
| 0x00E9DAC0 | NScript::CBriarRose::Init (parent class missing) | CV_IntroductionToTrophiesScript::CBriarRose::Init |
| 0x00E9DA70 | sub_ | CV_IntroductionToTrophiesScript::CBriarRose::GetParentScript |
| 0x00E9DA80 | sub_ | CV_IntroductionToTrophiesScript::CBriarRose::OnPredicateFail |

## Symbol conflicts (one symbol, one address) -- NAME_FIXES.tsv
- 0x00DA2610 DoSpeech: bound by landed row 00da27b0 without its class nesting.
- 0x00F05A10 RefreshSpyAllegiences: bound by row 00f058d0 without NScript.
- 0x00DAC580 CNOVI_Guard::Alloc: bound by row 00dabac0 as CGuard.
- 0x01375748: bound under three names. This chunk uses the shared fable_script_animation_defaults.h spelling
  NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations.
- 0x00CDBFB0 (cast-map dtor): four names, already known.

## Header debt
- CQ_MinionCampScript::LeftSpawnPoints / RightSpawnPoints (+0x90 / +0x94) are opaque `void*` words. Retail has a 4-byte STLport
  std::list<CScriptThing>; rebuild/include has no std::list model. The exact change is:
  `std::list<NScript::CScriptThing> LeftSpawnPoints, RightSpawnPoints;` once a 4-byte STLport list header exists.
- The STB dtor inline cost (lane-wide). Retail behaves as if the implicit ~CScriptGameResourceObjectScriptedThingBase (or its
  ~CCountedPointer member) costs a few IL units more than the shared model does. Guard and Son both match under /d2inlT72.
  - A row-local user-declared dtor adds the right cost but also adds a vptr store, so the fix has to change cost only. That would
    be a respelling of the shared STB / CCountedPointer dtor model (for example, the member's release path spelled as one more
    statement), checked over every landed STB includer.
  - This chunk does not change it: it would touch the default path of a header with hundreds of includers.
