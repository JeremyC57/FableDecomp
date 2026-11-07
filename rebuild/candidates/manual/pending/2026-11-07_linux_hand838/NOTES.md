# Hand chunk 838 (CSpawnNewHobbe .. CWiseVillager): 2 of 6 byte-exact

Both matches are RELOCATION_MATCH. Every row name is the PDB name: Ego_r.pdb `?Main@<Entity>@<Script>@NScript@@EAEXXZ`, slot 1 of the
entity vtable. The anniversary label files already carry the right names for all six Mains and their Alloc / Init / GetParentScript
siblings, so no label correction is needed. Older placeholder rows (`sub_00dee240`, `CVtable_012f0610::scalar deleting destructor`, ...)
are superseded by labels_anniversary_2026-11-04_waves23.tsv and labels_naming_wave33_2026-11-03.tsv. No descriptive names were used.
Every test prints `<addr>_TEST PASS` under g++ and VC7.1, including the four differ tests. NAME_FIXES.tsv is empty (header only): every
callee spelling used matches reloc_bindings.tsv, one symbol per address.

## Matched (2)
- **0x00D9DAF0 NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe::Main**: RELOCATION_MATCH, attempt 10.
  - Behaviour: waits out the drinking hobbes. Then, until the outro, each frame it counts the Q_HobbeToothHobbe things. When neither
    the hero nor the whisper is within 10 and fewer than 3 hobbes live, it spawns a grunt (rand % 4 > 0), else a spellcaster (second
    roll), else a lieutenant. It sets the new hobbe on the nearer of whisper and hero, then pauses 8 s.
  - Shape: two while loops. The vector<CScriptThing> is declared per iteration, with a declared-only ~vector 0x008AC970 (called at the
    exit pad, expanded at the loop end).
  - Key lever: a row-local `__forceinline CScriptThing::operator=` for the three if-chain CreateCreature assignments (chunk 805 lever).
  - Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF and GetSquaredDistanceBetweenThings 0x00CBE583 (both __fastcall, in both PDBs),
    rand 0x00BFEB16, interface slots 75 GetAllThingsWithScriptName, 91 CreateCreature, 376 Pause, 451 GiveThingBestEnemyTarget.
  - relpair: all 18 relocations bind the retail targets; the string order was checked by hand.
  - Header: opt-in CSPAWNNEWHOBBE in engine/CQ_HobbeToothContestScript.h. Members used: CS_OutroBegin +0x59, DrinkingHobbesNotHit +0x5a.
- **0x00EB7B00 NScript::CV_MayorsInvitationScript::CWiseVillager::Main**: RELOCATION_MATCH, attempt 5.
  - There is no Codex or Lua source; the model was written from the retail disassembly.
  - Behaviour, main loop: until Thunder is beaten or the hero loses to him, the villager waits to be talked to. Hits re-take control
    (NumTimesHit < 3). The 4th hit makes him a plain AI narrator attached to the quest, and he then idles forever. On a talk he gives
    hint TEXT_QST_044_HELPFUL_VILLAGER_HINT_<WhichQuest> (GFIntToCharString 0x0099F830 + operator+ 0x0099F690). If the hero struck the
    deal with Lady Grey and WhichQuest == 5, he gives the _5_ALT line instead.
  - Behaviour, ending: on a talk he proposes (yes / no). Yes makes him marryable and a follower, and sets MissionFailed.
  - Levers:
    - MOVIEBASE_OUTOFLINE_CTOR (all three movie ctors are called).
    - The idle-forever loop must be `while (true) { NSF; if (T) return; }`. Spelled `for (;;)`, VC7.1 calls ~STB at its exit instead
      of expanding it into the shared pad.
  - Header: opt-in WISEVILLAGER in engine/CV_MayorsInvitationScript.h, with NumTimesHit +0x1c (Ego_r.pdb name, sizeof 0x20 = the
    Alloc's new size).

## Parked (4)
- **0x00D33730 CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main**: DIFFER(3292v3292), ONE byte. md.py shows 0 lines;
  bin/bcheck.py shows the byte.
  - Residue: the talk branch's first `if (T) return` (jne at 0x00D33D4A) jumps to the loop-exit copy of the expanded ~CTimer
    (0x00D343B9). Retail jumps to the shared pad that calls ~CTimer 0x00CD4470 (0x00D343E6). This is VC7.1's pad cross-jumping choice.
  - 26 attempts (see STATUS.tsv). Spellings that did not move it: plain / TRetVoid returns, do{}while(0) round the check, the talk body
    in an inner block, a named bool, plain-inline GetTime.
  - Worse: forceinline ~CTimer, plain-inline operator=, `== true`, and do{}while(0) round the talk body.
  - This is the "per-exit ~CTimer split" residue the brief lists.
  - No Codex source; the model was written from the disassembly. The levers that got it from md712 to 1 byte are in LESSONS.md.
  - Header: opt-in CTHEREALGUILDMASTER (GuildmasterChat +0x4c).
- **0x00D21B40 CQ_BreakSiegeScript::CBanditSiegeLeader::Main**: DIFFER(3013v2965), 66 lines.
  - Residue: the if-block's cut-scene objects take swapped frame slots. Retail has movie 0x40, cast map 0x50 and the hit-flag spill
    0x5c; ours has map 0x40, spill 0x4c and movie 0x50. Retail expands ~MovieBase at both of its sites; ours calls 0x006E7B80 at the
    teleporter-loop pad.
  - This is the documented every-site movie-dtor case (chunk 814: gated by frame-slot sharing).
  - 10 attempts. The default-path CQ_BreakSiegeScript.h class is used unchanged.
- **0x00DEE2E0 CQ_SecretPassageScript::CMotherTorture::Main**: DIFFER(3134v3102), 313 lines (129 structural).
  - Residue: the frame is one dword short. Retail keeps a temporary slot at 0x14 and the CTimer at 0x18; ours puts the CTimer at 0x14.
  - Knock-ons: retail spills the GetHealth bool to [esp+0x13] and the hit-branch PauseMyEntitiesPlease receiver to [esp+0x14], where
    ours keeps them in bl / ebp. The ebx / ebp roles swap from there on. The talk-branch ~MovieBase is also expanded where retail
    calls it.
  - 10 attempts. The Codex pass-13 body was the start.
  - Header: opt-in CMOTHERTORTURE in engine/NScript_CQ_SecretPassageScript.h (MotherFound +0x4b).
- **0x00F0A960 CQ_AmbushTradersScript::CAT_BanditContact::Main**: DIFFER(2142v1985), 181 lines.
  - Residue: the documented CIsThingAlive / by-value CScriptThing nested-copy blocker (GOTCHAS 786 / 810 / 835). Retail calls the copy
    0x004ABE90 inside the inlined CIsThingAlive ctor and at the 5th by-value Me copy, and expands the other four.
  - Parked after 5 attempts as a documented blocker.
  - Non-GOTCHAS lever tried: f0_a3, the CIsThingAlive ctor defined after Main (no change). The loop forms were also tried.
  - Header: opt-in BANDITCONTACT (ContactAttackedByHero +0x56, BanditContactIntroComplete +0x67, ContactSeed +0x88).

## Label corrections
None. All six Mains and their entity siblings already carry the PDB names in the latest label files (see the first paragraph).

## Header debt
- The five nested entity classes live in opt-in blocks only. A later chunk landing any of these rows should promote its block as is.
- CAT_BanditContact's block needs FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN as well, like the other CAT_* blocks.

## vland (read-only, final overlay)
2 RELOCATION_MATCH PASS. Two warnings, both benign:
- RELOC_ALIAS `_rand` -> the MSVCR71 import thunk 0x00BFEB16.
- RELOC_NAME `??BCCharString@@QBEPBDXZ` -> 0x0099E4C0, whose manifest label is spelled `CCharString::operator_char_const*`. The
  symbol is already bound in reloc_bindings.tsv, by 0x00EB10C0.
