# Chunk 819 notes

Result: 0 matched, 6 parked (differ/<addr>.cpp + .test.cpp, all tests PASS). Every row has 10 or more logged attempts (STATUS.tsv notes,
ATTEMPTS.log). No `/d2` flag, pragma, asm or volatile in any source; the `/d2inlT` scans were diagnostic only.

Names: every emitted external symbol of the six objects is either in the Ego_r / FableWin PDB name sets or a repo-wide spelling already
bound by landed rows (pDSTGame, pGlobalScriptDef, NullCoord, the STLport map insert_unique, CSpawnedFunc / CPredicate_And ??_G, CRT).
Every callee binds one retail address (reloc_bindings check; the "TARGET shared" aliases and the NullCoord addend are pre-existing).
No descriptive names were introduced. NAME_FIXES.tsv: none.

## 0x00E84B80 NScript::CV_FisticuffsClubScript::CFisticuffsMember::Main (retry, chunk 813) -- DIFFER(8968v8836), 132 lines
- Shape: the chunk 813 a6 body (FC_Status switch, cases 0,3,2,4,5,1 by temporary flag bits); the CTyler 0x00E87030 define/include set,
  the declared-only CCountedPointer<MovieBase> dtor spec and `CDefPointer<const CScriptDef> pGlobalScriptDef` change nothing.
- Residue: the natural loop exit expands the implicit ~seh_me where retail calls 0x007E74D0 and jumps to the shared tail 0x00E86DC6.
  The `for(;;NSF){T_RET; ...}` form (a7) is byte-exact to 0x00E86976 and then expands ~seh_me in the loop-top pad.
  /d2inlT60..100 never matches: not a pure budget offset.
- Labels: the ctor 0x00E83920 label was already corrected at landing 2026-11-06n (no new correction).

## 0x00D0D910 NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main (retry, chunk 812) -- DIFFER(4928v4752), 525 lines
- Shape: the chunk 812 a10 body, regraded as-is (best); GATE2GUARD1 opt-in carried in hdr/.
- Callees: OpenGate 0x00D0EBC0 through CSpawnedFunc2<CQ_BanditCampBossBattleScript,float,CCharString> (vtable 0x012C9918).
- Residue: frame 0x110 vs 0x10c (timer / hit-bool slots); MOVIEBASE_FORCEINLINE adds the MovieBase vptr store retail lacks; the
  implicit movie pair + declared-only spec calls the COMDAT where retail expands (914); plain inline ~CTimer gets the 2 expanded Dereg
  sites but colours eax/edx and adds 4 to the frame (643). Chunk 812 a1 already had the spec (with FORCEINLINE_CTOR).

## 0x00D09010 NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main (retry, chunk 812) -- DIFFER(5880v5864), 170 lines
- Shape: chunk 812 a10 + `TalkedTo = false;` before ClearThingHasInformation (a3: 203 -> 170): stores al (the thread-check result) into
  the right byte as retail, though scheduled before the vtable load where retail stores after the push.
- Residue: the movie dtor vptr store (MOVIEBASE_FORCEINLINE), one unexpanded attacked-wait exit (retail: Dereg x2, ~Hostage, ~seh_me),
  one alignment pad, eax/edx colouring.

## 0x00D5E0C0 NScript::CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main (new) -- DIFFER(15372v15084), 133 lines
- Shape: twin of the melee lesson 0x00D58490 (chunk 815) and skill lesson 0x00D5AE70 (chunk 816); body built from the skill twin.
  Key levers: the GM_PLAY_WHISPER_QUESTION arms written `if (play != 1) {YES arm} else {NO arm}` (877 -> 190; the no-return arm second);
  the final loop as `for(;;NSF){if(T) return; ...}` (190 -> 133).
- Data: new header engine/CQ_GuildTrainingWillScript.h (hdr/README.md); CQ_SunnyvaleMasterData WillTrainingStarted +0x9f, WillScore
  +0xac, MeleeApprenticeNeededForCutscene +0xb0, GuildWarningOccuring +0xc0, WillTestOccuring +0xc2, HeroTakingGuildTest +0xc6;
  pGlobalScriptDef GUI_WillGrades +0xECC, GUI_WillTimer +0xF08.
- Residue: VC7.1's exit inline budget (GOTCHAS chunk 809 class): the final loop's thread-exit pad expands ~seh_me where retail calls
  0x007E74D0, so the movie-scope pad at 0x00D5E970 gets its own exit copy; the last movie ctor expands where retail calls 0x006E7B60.
  /d2inlT70-72 (diagnostic) leave 70 lines.
- Identity: proven (PDB name sets + slot 1 of entity vtable 0x012D1070 stored by Alloc 0x00D5DDE0). Label already correct.

## 0x00E40E80 NScript::CV_BordelloScript::CMagicman::Main (new) -- DIFFER(14772v15104), 282 lines
- Shape: CMadame / CBordelloClient sibling shapes, MM_* macros, CSpawnedFunc spawn; LOVELY_SKIN spoken to Me (retail pushes ebx = &Me);
  case 5 `HeroPartying = true` after the if/else so VC tail-duplicates it as retail.
- Data: engine/CV_BordelloScript.h (HeroPartying +0x48 .. seh_Boss +0x74; CMagicman DoneToutIntro +0x1c, MentionedPimpHat +0x1d).
- Residue (332 bytes short): retail expands two more return epilogues (0x00E445B8-0x00E4472E, 0x00E4474C-0x00E447F8: Dereg x2 + ~item)
  where ours shares one; the FEMALE hit thread exit calls ~MovieBase where retail jumps to the shared pad 0x00E4489C; eax/edx at the
  PauseAll pads.
- Identity: proven (PDB name sets + slot 1 of the CMagicman entity vtable 0x012E3B2C). Label already correct.

## 0x00F0FB70 NScript::CQ_ArenaScript::Main (new) -- DIFFER(18320v18144), 1458 lines
- Shape: UndeadRising quest-Main model: objective, 14 entity bindings, holding quest, termination and crowd watchers, ArenaState machine,
  9-round switch, 20-entry hero-title switch. Lever: implicit movie ctor/dtor + declared-only PImp dtor spec + implicit STB ctor
  (2221 -> 1458): the special-member counts now match retail (first 2 movie ctors / dtors, first 3 STB ctors expanded, the rest called).
- Data: engine/CQ_ArenaScript.h (ArenaState +0xa4 .. StopTimeIndex +0x118); CQ_SunnyvaleMasterData WhisperKilledByHero +0x74.
- Residue: frame 0x2D8 vs retail 0x2BC (28 bytes: temporaries' slots differ from the first objective call on); retail keeps 0 in edi
  through the binding block where ours keeps 2 (a temporary-flag bit); one extra ~STB / ~Movie exit copy.
- Identity: proven (PDB name sets + slot 2 of the CQ_ArenaScript level vtable 0x012C7798). Label already correct.

## Header debt
- None beyond hdr/: the new engine/CQ_GuildTrainingWillScript.h (FABLE_RETAIL_NSCRIPT only) and the carried chunk 812 BanditCamp
  opt-ins. The 16 landed CQ_GuildTrainingWillScript rows keep their local models; moving them onto the new header is a later retype.
