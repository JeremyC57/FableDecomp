# Hand chunk 816 (CAS_Trickster .. CTheRealGuildmaster): 0 of 6 byte-exact

All six rows are large NScript entity `Main`s with no catalog name. Every name is proven from two sources: slot 1 of the entity
vtable that the landed Alloc stores (slot 0 = the landed scalar deleting dtor, slot 2 = Init), and the full
`?Main@<Class>@<Script>@NScript@@EAEXXZ` spelling in both the Ego_r.pdb and FableWin.pdb name sets (LABELS.tsv). No name is
descriptive. Every parked source is graded against the retail bytes; every test passes under g++ and VC7.1.

## What matched (0)

None. Guildmaster (equal size and call sequence) and Trickster (one 2-instruction swap) are the closest.

## Parked for Fable 5.1 (6)
- 0x00E2C100 NScript::CV_AmbushScamScript::CAS_Trickster::Main DIFFER(8945v8945): return pad 0x00E2E38E of the HELP_GOOD / HELP_EVIL scene: retail `mov edx,[ebp]; mov ecx,ebp; push 0`, ours `mov edx,[ebp]; push 0; mov ecx,ebp`. The other 54 diff lines are relocation-only. | tried: `for(;;)` with a flag break (a2, loop still rotated); `NSF; if (!T) continue; return;` at the bottom of the intro loop (a3, best: stops the rotation); TRetVoid returns; a nested movie block; `else if (!T)`; a named morality bool; the inverted MoralitySwitch test (only the jcc sense flips); the shared `if (T) return;` hoisted out of both arms; a trailing `continue` (10 attempts). | next: a spelling for the last cut-scene branch whose cross-jumped return pad gets the this-first schedule.
- 0x00D3E2E0 NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::Main DIFFER(9028v9440): blocker 1 + the function-wide inline split. Retail expands the by-value SetIsPushableByHero Me copy but calls the CIsThingConscious copy 0x004ABE90. It expands the movie ctor/dtor and the GetHealth temp ~CScriptThing only at the state-0 sites and one exit path. | tried: everything out of line (a1); everything inline (a2: IMPLICIT_COPY + inline ~CScriptThing + inline movie; VC7.1 expands every site, 2098 lines, so the split is not a plain budget cut-off; not in GOTCHAS); TRetVoid returns (a3, no change). Parked early: documented blockers. | next: blocker 1 first, then a way to confine the expansion to the state-0 block.
- 0x00E58D40 NScript::CV_BeggarAndChildScript::CLookoutPointBeggar::Main DIFFER(9620v9616): blocker 1 only. With IMPLICIT_COPY the CIsThingAlive ctor expands its nested CScriptThing copy, where retail calls 0x004ABE90. Without it (a1, 167 lines) the by-value SetIsPushableByHero Me copy is called, where retail expands it. Everything else matches: both movie splits, the mixed ~CTimer pads, the expression compares, both jump tables. | tried: IMPLICIT_COPY (a2); a row-local inline ~CScriptThing (a3, chunk 815's budget lever; identical; not in GOTCHAS). Parked early: documented blocker. | next: lane gap: blocker 1.
- 0x00E3BB70 NScript::CV_BordelloScript::CMadame::Main DIFFER(10069v10149): blocker 1 + the per-site movie split. Retail expands the movie ctor at the beer / work-offer / deeds / attacked scenes but calls 0x006E7B60 at the intro, EARN_SOME, chit-chat and both loop-2 scenes. It expands the movie dtor only at the beer / deeds / attacked scenes. The ~CTimer exit pads are mixed: called at the natural end, an expanded DeregisterTimer at two early exits. | tried: movie out of line (a1, 887 lines); movie inline-visible + a declared-only `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` 0x006E7AB0 (a2, 715 lines; expands every first-loop ctor; not in GOTCHAS). Parked early: documented blockers. | next: blocker 1, then per-scene blocks / frame-slot sharing for the movie split (chunk 814 lesson).
- 0x00E4B5B0 NScript::CV_BanditTollScript::CTollBandit::Main DIFFER(10674v10594): the frame is one dword too large (`sub esp,0x11c` v 0x118). Ours gives the state-2 PauseMyEntitiesPlease Game spill its own slot [esp+0x18]; retail overlays it on warn_timer [esp+0x14], and the state-1 conversation temp on call_timer [esp+0x10]. Every later frame offset then shifts (disp8 / disp32). One return pad is also homed differently: retail homes the state-2 SAY return pad in the BC_PAY question block 0x00E4C1A7. The call sequence is otherwise equal. | tried: forceinline CScriptThing::operator== (a3, best: the compare now expands like retail); block-scope probes around kill_distance / state 1, warn_distance, state 4, call_timer..end (v1 1222 lines: the state-1 temp now shares with call_timer, frame still 0x11c), plus an inner warn_distance block (0x110, over-packed); warn_timer declared before warn_distance (10 attempts). | next: one inner block at the state-3 loop with warn_timer hoisted.
- 0x00D5AE70 NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main DIFFER(11636v11636): size and call sequence equal. Left: the stack-slot colouring of the scalars at [esp+0x44/0x48/0x4c] (retail commentCount 0x44, ticks 0x4c/0x44/0x48), two comment-loop temporaries (the "" movie and the line CCharString), and the RemoveQuestInfoElement load order that follows. | tried: 815's melee twin defines (a1); TRetVoid (a2); the missing post-SkillRepeating-loop `if (T) return;` (a3, best: size and calls equal); 5 tick declaration orders, 3 commentCount placements, a fresh second tick set / ticksRemoved placement (a4-a14; none move the colouring). | next: chunk 815's exact talk-block shape (else-if hit whisper), to see whether the comment-loop temps move, then the ticks.

## Per-row details
- **Trickster** (CV_AmbushScamScript, no overlay): 813/814 Trickster defines; macros TS_START / TS_SAY / TS_MOVIE / TS_FACE_HERO / TS_SPAWN.
  Callees are bound as landed. The spawned WatchForHeroAttack is `ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_AmbushScamScript,CScriptThing>(...))`.
- **SpeedTest** (opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDTEST): nested class from the Ego_r layout (RaceMode +0x1c, sizeof 0x20 = the landed Alloc size).
  The race uses three CTimers (race / argue / total), and two 5-entry jump tables (the pre-race argue lines and the post-race lines).
- **Beggar** (new engine/CV_BeggarAndChildScript.h = chunk 813's pending draft + nested CLookoutPointBeggar, Ego_r 28 bytes): generated from
  chunk 813's differ/00e5b3b0.cpp (CBeggarBully twin). It uses `rand()` 0x00BFEB16 (MSVCR71 import), the `item == "TEXT"` inline compare opt-ins,
  and a declared-only ~CCountedPointer<MovieBase>.
- **Madame** (CV_BordelloScript, no overlay): `ParentClass.seh_Madam = seh_me` is the out-of-line CScriptGameResourceObjectScriptedThingBase::operator=
  0x008ABD10 (opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN). The chit-chat line is
  `"TEXT_QST_B13_MADAME_CHIT_CHAT_0" + GFIntToCharString(chat)` (0x0099F830, already bound), with chat wrapping 5 -> 1.
- **TollBandit** (opt-in FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT): the callees SetUpBanditToll 0x00E4E0E0 and Should_I_Flee 0x00E4DF20
  (both spellings are in both PDB name sets; see Label corrections), and the parent's CheckForBanditTollPaid (PDB
  `?CheckForBanditTollPaid@CV_BanditTollScript@NScript@@QAEXXZ`, spawned through the CSpawnedFunc vtable 0x012E4B90).
  None of the three is bound in reloc_bindings.tsv, so there is no one-symbol/two-address conflict.
- **Guildmaster** (new engine/CQ_GuildTrainingSkillScript.h): a twin of the melee lesson 0x00D58490 (chunk 815). RunCutsceneMacro 0x00CBFB7D
  is declared with the std::map spelling that is already bound to that address. reloc_bindings.tsv already holds 3 spellings of 0x00CBFB7D
  from earlier landings (CCutsceneCastMap / Row00e80d60 local structs): one address, several names, pre-existing, not introduced here.
  AddLogbookTutorialEntry(PC) 0x00CBE9EE / 0x00CBEA81 are bound as landed.

## Skipped
None.

## Label corrections
- 0x00E4E0E0: landed as `CTollBanditScript::HandleBanditToll` (labels_landing_2026-11-03d, manifest_sync; autoname-t5 had
  CCreatureAction_TrollWhackGroundBase::HandleBanditToll). Correct: `NScript::CV_BanditTollScript::CTollBandit::SetUpBanditToll`,
  `?SetUpBanditToll@CTollBandit@CV_BanditTollScript@NScript@@AAEXXZ`. Both PDB name sets have it, and it is the first call of Main 0x00E4B5B0.
- 0x00E4DF20: landed with the class-less leaf `?Should_I_Flee@CTollBandit@` (global struct CTollBandit). Correct:
  `?Should_I_Flee@CTollBandit@CV_BanditTollScript@NScript@@AAEXAAVCScriptGameResourceObjectScriptedThingBase@@@Z` (both PDB sets;
  bsim_port_audit_final.tsv agrees).
Both are in NAME_FIXES.tsv and LABELS.tsv.

## Header debt
- None beyond the hdr/ hand-back. When the landed TollBandit-family rows are respelled, the landed 0x00E4DF20 / 0x00E4E0E0 sources can move to
  the nested CTollBandit in engine/CV_BanditTollScript.h (the FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT declarations).
- The verifier run (vland) is moot: there is no src/ row, so payload.json is empty.
