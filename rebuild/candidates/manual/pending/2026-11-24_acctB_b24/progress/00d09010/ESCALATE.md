
## B24A4 (2026-11-24): Ego_d questions for the Codex list (Ego_d.pdb is not on the lane)
1. egod_proc / egod_lines for ??1CTimer@@QAE@XZ, ??0CTimer@@QAE@XZ, ??4CTimer@@QAEAAV0@J@Z: file, line numbers, and the S_LOCAL /
   S_BLOCK records (is there a local -- e.g. a game-interface pointer or the index -- or an inline helper call in the original body?).
   Our ~CTimer costs c1 14 in c2; the family's retail decisions need c1 > 19 (00d09010/NOTES.md B24A4).
2. egod_proc / egod_lines for ??0 / ??1 CScriptGameResourceObjectMovieBase, ??1 CScriptGameResourceObjectScriptedThingBase,
   ??1 CScriptGameResourceObjectBase, ??1 NScript::CIsThingAlive: user-written or compiler-generated, and in which file/line.
3. egod_locals on NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main and CAssassinMarker::Main: the declared types and
   scopes of `speech_movie` / `movie` and of the timer local(s).
