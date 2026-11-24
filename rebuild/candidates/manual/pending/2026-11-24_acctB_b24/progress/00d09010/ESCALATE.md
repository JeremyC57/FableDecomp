
## B24A4 (2026-11-24): Ego_d questions for the Codex list (Ego_d.pdb is not on the lane)
1. egod_proc / egod_lines for ??1CTimer@@QAE@XZ, ??0CTimer@@QAE@XZ, ??4CTimer@@QAEAAV0@J@Z: file, line numbers, and the S_LOCAL /
   S_BLOCK records (is there a local -- e.g. a game-interface pointer or the index -- or an inline helper call in the original body?).
   Our ~CTimer costs c1 14 in c2; the family's retail decisions need c1 > 19 (00d09010/NOTES.md B24A4).
2. egod_proc / egod_lines for ??0 / ??1 CScriptGameResourceObjectMovieBase, ??1 CScriptGameResourceObjectScriptedThingBase,
   ??1 CScriptGameResourceObjectBase, ??1 NScript::CIsThingAlive: user-written or compiler-generated, and in which file/line.
3. egod_locals on NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main and CAssassinMarker::Main: the declared types and
   scopes of `speech_movie` / `movie` and of the timer local(s).

## B24A5 existence-proof search (2026-11-24): landed stock-flag rows with ~MovieBase EXPANDED and ~CTimer CALLED on one exit path
Scan: 00d09010/scan/mbscan.py over every landed oracle body (rebuild/oracles/auto-re-candidates.tsv, 57k rows): straight-line windows
(no ret / jmp) holding call 0x006E7AB0, the 0x0126008C store, call 0x0099A430 and call 0x00CD4470. Hits (scan/hits.tsv): 3.
- 0x00D0A830 CBanditKing::Main (same BanditCampBossBattle TU, STOCK flags): the ONLY real one. Form: terminate checks inside the movie
  scope are `do { if(IsActiveThreadTerminating()) goto Terminate; } while (0)` (movie + pause destroyed at the jump, expanded) and the
  function-end `Terminate: return _Fail_Val.Return();` wrapped in `#pragma inline_depth(0)` (timers called in the shared tail), plus a
  FORCEINLINE row-local CTimer::operator=. This is the goto device B24A2/B24A3 already tried on this row (v8 / v10 / vG..vQ) and on
  0x00D01630 (a1): timer counts right, frame slots off (gotos change IL reference weights), and this row's evidence favours returns.
- 0x00E799F0 CEM_Rhodri::Main: false positive (movie destroyed at its scope end in the normal flow; the ~CTimer call follows after a
  conditional branch; not one exit).
- 0x00D29880 CHookCoastTeleporter::Main: not stock (/d2inlT1000 + three inline_depth(0) regions).
So no landed stock-flag row reaches "movie expanded + ~CTimer called at one RETURN" with returns; the only stock-flag precedent is the goto
form above. Row stays blocked on the cost reversal (no header/source form found); next idea only if the owner accepts the goto device
as the family's original text despite the frame cost: re-fit slots under the goto form (B24A3 v10 is the best goto draft).

- Positive control (B24A5, scan/mbscan3.py: after each expanded movie dtor follow fall-through and unconditional jmps to the next ret,
  report paths calling ~CTimer 0x00CD4470): on the unlanded oracles it finds 0x00D09010 x2 (0x00D09DCB, 0x00D0A613) and 0x00D01630 x2
  (0x00D0288D, 0x00D028E1), all "via-jmp" (movie expanded in the exit block, then jmp to a shared tail that calls ~CTimer), and none in
  0x00D11930 (as expected). On all 57k landed oracles it finds ONLY 0x00D0A830 (scan/hits3.tsv). The first same-block scan (mbscan.py)
  missed jmp-tail exits; mbscan3 supersedes it. So: the only landed precedent is the goto-Terminate form, and retail's exits in both
  blocked rows have exactly that jmp-to-shared-tail shape (consistent with gotos or with cross-jumped returns; bytes cannot tell).
