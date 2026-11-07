# Chunk 816 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- VC7.1 rotates a `while (!flag) { ...; NSF; if (T) return; }` loop; writing the bottom as `NSF; if (!T) continue; return;` keeps the retail top-test layout. A `for(;;){ if (flag) break; ... }` is still rotated (0x00E2C100, DoTricksterIntro loop).
- Blocker 1 isolates cleanly on the Beggar: with FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY the only residue is the copy inside the expanded CIsThingAlive ctor (retail calls 0x004ABE90). A row-local `inline CScriptThing::~CScriptThing() {}` does not change it (0x00E58D40).
- The "everything inline" diagnostic (IMPLICIT_COPY + inline ~CScriptThing + inline movie ctor/dtor) makes VC7.1 expand EVERY site of a 9 KB Main. Retail's state-0-only expansion in CApprenticeSpeedTest is not a function-wide budget cut-off that the inline spellings can reach (0x00D3E2E0).
- A declared-only `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (0x006E7AB0) with the movie ctor/dtor inline-visible gets Madame from 887 to 715 diff lines: the movie dtor's counted release stays a call (0x00E3BB70).
- A forceinline `CScriptThing::operator==` reproduces TollBandit's expanded thing compares (0x00E4B5B0).
- Retail overlays later-declared function-scope CTimers on earlier nested temporaries and pause spill slots (TollBandit: the state-1 conversation temp on call_timer, the state-2 PauseMyEntitiesPlease Game spill on warn_timer). A block from call_timer to the end reproduces the first half only; a second inner block over-packs the frame to 0x110 (0x00E4B5B0).
- Scalar spill-slot colouring (the 0x44/0x48/0x4c ints of the Guildmaster) does not follow declaration order: 10 permutations left it unchanged (0x00D5AE70).
- The skill-lesson Guildmaster needs the `if (T) return;` after the SkillRepeating loop that its melee twin has; mapping the cseq first insertion back to retail (0x00D5D816) found it (0x00D5AE70).
- TRetVoid `_Fail_Val` returns made no difference in any of these six Mains.

Review at landing:
- Every row needed more than 4 grades (Trickster 10, TollBandit 10, Guildmaster 14; the blocker rows were parked after 2-3, with the forms tried listed in NOTES.md).
- ALREADY-LANDED name errors: 0x00E4E0E0 `CTollBanditScript::HandleBanditToll` is CTollBandit::SetUpBanditToll; 0x00E4DF20 is landed as the global `CTollBandit::Should_I_Flee` instead of the nested NScript::CV_BanditTollScript::CTollBandit (NAME_FIXES.tsv).
- reloc_bindings.tsv already binds three spellings of RunCutsceneMacro to 0x00CBFB7D (local CCutsceneCastMap / Row00e80d60 structs vs the std::map one).
- Two new headers (engine/CV_BeggarAndChildScript.h = chunk 813's pending draft + CLookoutPointBeggar; engine/CQ_GuildTrainingSkillScript.h): coordinate with chunk 813's hand-back of the first.
