# Hand chunk 836 (CNecropolisGhost .. CImprisonedBanditLeader): 2 of 6 byte-exact

Both matches are RELOCATION_MATCH. relpair shows each relocation bound to one retail target, and the string literals are in retail order.
The read-only vland with the overlay is clean: 2 RELOCATION_MATCH PASS. It gives two RELOC_NAME warnings, one of which is a placeholder
label (see Label corrections). Every test prints `<addr>_TEST PASS` under g++ and VC7.1, including the four differ tests.

## What matched (2)
- **0x00D328C0 NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main**: the hero's sister in the final boss battle.
  - Behaviour: she waits for the intro and is taken under script control, untouchable and unconscious. When the final decision starts:
    she becomes targetable again (+10000 health), CS_JACK_DEATH plays, and the hero gets the Sword of Aeons. Then a sword hit from the
    hero plays CS_JACK_BOSS_ENDING_EVIL (JackBossBattleResult 1). A cut-scene that needs her makes her release and retake control.
  - Header: new engine/CQ_EndGameBossBattleScript.h (see hdr/README.md).
  - Movie: MOVIEBASE_FORCEINLINE_CTOR + _OUTOFLINE_DTOR.
  - seh_me / seh_Hero dtors and the map value assignments are calls (STB OUTOFLINE_DTOR / _ASSIGN). The map ctor / dtor are
    declared-only (0x00CDBF70 / 0x00CDBFB0).
  - Shape lesson: retail's final idle loop is `jmp test` with the seh_Hero return pad placed before it. That is the chunk 817
    else-wait form `if (T) return; for (;;) { NSF; if (T) return; }`. Neither `while (!T) NSF;` nor `for (;; NSF) { if (T) return; }`
    gives it: both rotate the loop and put the pad after it (b2 / b3, 26 lines).
- **0x00ECD9E0 NScript::CV_SickChildScript::CSickChildsMother::Main**: the sick child's mother.
  - Behaviour, intro: within 7 m before the intro she plays CS_SICK_INTRO and gives the quest card.
  - Behaviour, talked to: she asks the cure question when the hero holds the EndPotion. Yes plays CS_SICK_OUTRO, takes the potion,
    sets FinishedQuest and ends the loop; no gives ASK_CURE. Otherwise she says CURE_REMIND_10.
  - Behaviour, hit: MOTHER_HIT, then the two become allies.
  - Source: the Codex pass-13 body on the current headers (Game->, the CSickChildsSister park's define set, MOVIEBASE_OUTOFLINE_DTOR).
  - Movie ctors: the implicit ctor reproduces retail's mixed sites by itself (calls 0x006E7B60 at the intro and hit sites, expands the
    talk site).
  - The last residue was both `seh_Mother` releases: `add edi,48h` against retail `lea edi,[eax+48h]`. The chunk 808 lever fixed it:
    `CV_SickChildScript& parent = ParentClass; if (parent.seh_Mother.IsValid()) parent.seh_Mother.Release();`.
  - Callee names: CDefString::operator CCharString 0x00415D70 (pGlobalScriptDef->EndPotion, +0x744) and
    CV_SickChildScript::PlayCutscene 0x00ECE460. PlayCutscene is labelled correctly but not yet in reloc_bindings.tsv: this row is
    its first binding.

## Parked for Fable 5.1 (4)
- 0x00CFBDF0 CQ_AwakeningTheOracleScript::CNecropolisGhost::Main DIFFER(2278v2317): the predicate's ~CIsThingAlive is called whole
  (retail: outer expanded with the filter vtable in ebx, nested ~CScriptThing 0x004AA840 called). Knock-on: register / frame shifts
  over the whole body (303 lines). | tried:
  - __forceinline outer (a2; both levels expand)
  - definition order of the inline ~CScriptThing and ~CIsThingAlive (a3 / a4; identical)
  - no inline ~CScriptThing (a5; predicate right, the other ~CScriptThing sites become calls)
  - diagnostic /d2inlT60: md54 (meter only)
  | next: lane gap: nested inline-dtor threshold (chunk 822 family).
- 0x00E799F0 CV_ExposeMayorScript::CEM_Rhodri::Main DIFFER(2580v2580): the only residue is the predicate: ~CIsThingConscious is called
  whole (ebx not loaded with 0x012C2FE8, the ctor stores the immediate, one align pad). | tried:
  - Sheriff's define set (c1)
  - the `parent` reference release (c3; fixed the lea / add)
  - no inline ~CScriptThing (c2 / c4; predicate right, retail's other expanded ~CScriptThing temporaries become calls, 60 bytes short)
  | next: lane gap: nested inline-dtor threshold (chunk 822 family).
- 0x00CF0D20 CQ_ArenaHoldingScriptScript::CEntranceGuard2::Main DIFFER(2655v2755): every movie dtor is called (0x006E7B80), where retail
  expands all 8 sites with tail-merged / cross-jumped pads. Knock-on: ebp holds zero instead of the hoisted movie vtable, the talked-to
  bool is not spilled, frame 0xec against 0xf0. | tried:
  - IMPLICIT_COPY (e2; fixed the by-value Me copy for SetIsPushableByHero(CScriptThing, bool))
  - TRetVoid returns (e3)
  - MOVIEBASE_FORCEINLINE (e4; adds a vtable reset per site)
  - no declared ~CCountedPointer<MovieBase> (e5)
  - STB OUTOFLINE_DTOR (e6; movie dtors start expanding, ~seh_me wrong)
  - /d2inlS20 meter: md60 (only exit-pad cross-jumping left)
  | next: lane gap: function-wide inline budget, the same gap as the parked fan Mains in this TU (0x00CF18D0 / 0x00CF2B70 / 0x00CF3D20,
  chunk 804).
- 0x00D21040 CQ_BreakSiegeScript::CImprisonedBanditLeader::Main DIFFER(2742v2758): the by-value DestMarker copy is called, where retail
  expands it (one CScriptThing vtable store, PImp copy and inc). Knock-on: mask / Guard slots 0xc lower, and the reassignment's +4 is
  folded. | tried:
  - copy declared (h1; the predicate's nested copy stays a call as retail)
  - IMPLICIT_COPY / _ASSIGNMENT + declared-only CCountedPointer<CScriptThing>::operator= 0x00704580 (h2; DestMarker and the reassignment
    are exact, but the predicate's nested copy expands, md63)
  - named filter block (h3; identical)
  - row-local plain-inline copy ctor (h4; identical)
  | next: lane gap: the filter copy split (GOTCHAS inline revisit 3).

## Skipped
None.

## Label corrections
- 0x00CDBFB0 `CRetailTree_erase00cd344a::~CRetailTree_erase00cd344a` should be
  `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map` (FableWin + Ego_r PDB). reloc_bindings.tsv also binds three
  other spellings to this address (NAME_FIXES.tsv).
- 0x00CDBF70 `std::_Rb_tree<T_node0x24_00cdbf70>::_Rb_tree` should be `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map`.
  reloc_bindings.tsv also binds the CCutsceneCastMap ctor / Construct spellings to it (NAME_FIXES.tsv).
- The six rows' own labels are already correct.

## Names
- Proven: every callee and global these rows bind (relpair against reloc_bindings.tsv: no symbol bound at a different address).
- Descriptive: none. Member names are Ego_r.pdb: MyGraveNumber, ExplainedPlight, IntroFinished .. FinalCutscenePlayed, seh_Jack, Jack,
  InHitCutsceneAlready.
- Local names are descriptive: speech_movie, PauseMyEntitiesPlease, mymap, HaveChatted, RestlessMarkers, DestMarker, Guard. They are
  the FSE / Codex spellings and do not reach the object.

## Header debt
None. The four header edits are opt-in blocks (hdr/README.md).
