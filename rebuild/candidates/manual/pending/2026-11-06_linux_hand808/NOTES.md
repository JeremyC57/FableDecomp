# chunk 808 notes

All six names are proven (no descriptive names): each Main is in the Ego_r.pdb and FableWin.pdb name sets, and its address has a single
.rdata reference, the Main slot of the vtable that the landed Alloc stores (LABELS.tsv). Every call target was checked against retail and
against rebuild/oracles/reloc_bindings.tsv: one symbol, one address. The symbols bound under other spellings are in NAME_FIXES.tsv.

## 0x00E78630 CV_ExposeMayorScript::CEM_Sheriff::Main: RELOCATION_MATCH (src/00e78630.cpp, a1)
- Shape: the CEM_SuspiciousVillager model. The @SAY seh_Sheriff blocks are GetHealth > 0, Speak, a wait loop and T. The stage chain is
  if/else; the tail is `while(!T)`.
- Spellings: declared-only `~CCountedPointer<MovieBase>`; FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR.
- Header: opt-in nested CEM_Sheriff (hdr/README.md).

## 0x00DE8370 CQ_RansomVictimScript::CRansommer::Main: parked at a6, DIFFER(4894v4894), SDIFF 4
- Shape:
  - first-meeting cave cut-scene, ransom question (7500, or 10000 after the helpers were attacked);
  - a talk loop that asks again;
  - a hero hit turns him bandit and he stays that way.
- Spellings: TRetVoid returns; `CScriptGameResourceObjectScriptedThingBase& handle = ParentClass.seh_Ransommer` at the start; OUTOFLINE
  movie ctor and dtor.
- Residue: register choice in the two rotated IsPerformingScriptTask tests (eax + lea in retail, ecx + add in ours).

## 0x00DE6C20 CQ_RansomVictimScript::CVictim::Main: parked at a2, DIFFER(5479v5511), SDIFF 26
- Shape:
  - Help-line loop until he is rescued.
  - Thanks cut-scene when RansommerDied, then follow the hero with a health bar
    (`AddQuestInfoBarHealth(Me, CRGBColour(255,0,0,255), "HUD_QUEST_ICON_GRANDSON", 1.0f)`).
  - Main loop, in order:
    1. GotOutOfCave once "Witchwood2" loads;
    2. talk: cave thanks / waiting bandits or creatures / nagging `"TEXT_QST_B06_VICTIM_NAGGING_" + GFIntToCharString(rand() % 4)`;
    3. hit by others: `seh_Victim.MoveToThing(GetHero(), 3.0f, RUN, false, true, false, true)`;
    4. hero hit while the timer is idle: ON_HERO_HIT, timer = 5, allied both ways;
    5. near the hero: ON_HIT_<n> with n = 10, 20, .., back to 10 when TextEntryExists fails;
    6. an enemy (GetNearestWithScriptName(Me, ParentClass.ActiveEnemyName)) within 15: wait (scared, under scripted control);
    7. ThanksCutsceneStarting: stop for good;
    8. waiting and the nearest enemy is dead (virtual IsDead on the temporary) or more than 15 away: follow again.
- Spellings:
  - The spawn: `ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_RansomVictimScript, CScriptThing>(CCharString("ParentClass.") +
    CCharString("CheckVictimHit"), 0, &ParentClass, &CQ_RansomVictimScript::CheckVictimHit, Me), CCharString(""))`.
  - The parent-reference seh release.
  - `SetEntityAsRegionFollowing(Game->GetHero(), Me, b)`.
  - `__forceinline` CTimer::operator= and GetTime.
  - Declared-only ~CCountedPointer<MovieBase>.
- Callees (bound, one address each):
  - CSpawnedFuncBase(name, id); AddSpawnedFunction 0x00CB7E50; GFIntToCharString 0x0099F830; operator+(const char*, const CCharString&)
    0x0099F690; IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA; rand 0x00BFEB16; CTimer 0x00CD4450 / ~CTimer 0x00CD4470.
  - CSpawnedFunc1 vtable 0x012DD860; CheckVictimHit 0x00DE81B0.
- Residue: per-site inline budget (STATUS.tsv a3 and a7-a10).

## 0x00E90BE0 CV_GuildMasterScript::CGuildMasterGameFlow::Main: parked at a7, DIFFER(4977v4914), SDIFF 49 (a9 SDIFF 17, 81 bytes short)
- Shape:
  - Q_EndGameFocalSites active: focal-site intro cut-scene (map HERO/GUILDDUDE), quest card and objective, NEWTELEPORTER2 effect, minimap
    marker.
  - Per frame while the quest is active: teleport a hero standing on the residue to FocalSitesHSP, answer talks
    (ParentClass.GetGuildMasterSpeech()) and hits (TEXT_QST_081_ATTACKED).
  - Outside the quest: hits and talks; at EGP_WAITING_FOR_WIZARD_BATTLE the take-card question (yes: Q_WizardBattle card, the maze
    journal taken).
- Residue: the movie ctor/dtor expansion per site (inner loop sites expanded, outer sites call 0x6E7B60 / 0x6E7B80).

## 0x00E93EB0 CV_HauntedHouseScript::Main: parked at a4, DIFFER(5171v5321), SDIFF 514
- Shape: the barrier loop until Q_TraderEscort is done, then:
  - remove the forcefields;
  - four CEntityScriptBinding<CV_HauntedHouseScript> (declared-only ctor 0x00E95390, ICF) and PostAddScriptedEntities x2;
  - spawn CheckRegionLeaving (declared-only CSpawnedFunc ctor 0x00E95F10);
  - the in-house / candle loops.
- Residue: inline budget (IsAlive wrappers, the Return stub 0x00E95380, the natural-end dtors).

## 0x00E7C8E0 CV_FishermanScript::CFisherman::Main: parked at a9, DIFFER(5358v5358), frame 0xdc matched, SDIFF 38
- Shape: the full quest flow (see the source line 1). The predicate is `CIsThingAlive(Me) && CIsVariableFalse(ParentClass->FishermanMustRunAway)`
  (row-local CIsVariableFalse, vtable 0x012EA43C).
- Spellings:
  - `C3DVector hidingPos` + run loop in its own block (frame);
  - both TEXT_EQ opt-ins for `RelationSuffixes[RelationCounters[2]] == "HATE"`;
  - `__forceinline` CTimer.
- Residue: the helpTimer register caching, the T-exit block placement in the thanks scope, and one register.

## Labels to correct
See NAME_FIXES.tsv:
- the CPredicate_And ctor / vtable stand-in names bound by row 0x00E8B020;
- the CSpawnedFunc<CV_HauntedHouseScript> vtable bound as <CV_ExposeMayorScript> by row 0x00E95F10;
- the DoMission stand-in for CheckRegionLeaving.

## Header debt
None. Every edit is opt-in or a new header (hdr/README.md).
