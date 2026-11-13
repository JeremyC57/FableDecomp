# Chunk 2053 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x0087F3A0) x87 operand order inside an expanded `C3DVector::Dot` / `GetMagnitude` pair (projection of a vector on a
  normalised direction, then its length) was decided by the TEXT of the two shared inlines, not by the row: `Dot` as
  `float dot=(X*other.X+Y*other.Y);dot+=Z*other.Z;return dot;` and `GetMagnitude` as `float squared=(X*X+Y*Y)+Z*Z;return
  (float)sqrt(squared);` (opt-ins FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z / _MAGNITUDE_XY_THEN_Z). Texts with identical /Od bytes
  grade differently in the optimised row: the parentheses around the X/Y pair are the difference between 714 and 715 (match)
  equal instructions for Dot and between 710 and 715 for GetMagnitude.
- (0x0087F3A0) The FableWin debug-size oracle picked the GetMagnitude text: of the candidate texts only the named squared sum is
  26i/66b like FableWin's ?GetMagnitude@C3DVector@@QBEMXZ (the repo default `(float)sqrt(GetSquaredMagnitude())` is 13i/31b, the
  fitted running-sum text 27i/70b), and of
  the two 26i/66b spellings (`X*X+Y*Y+Z*Z` v `(X*X+Y*Y)+Z*Z`) only the parenthesised one keeps the match. When fwsize leaves
  several texts of the right size, the row grade separates them: try the grouping parentheses.
- (0x0087F3A0) A helper whose /Od size is one byte over FableWin's (Dot 23i/60b v 23i/59b; the landed GetSquaredMagnitude2D
  18i/44b v 18i/43b) can still be the right text family: the +1 byte is the `x += ...; return x;` tail. Treat it as FITTED.
- (0x0087F3A0) A by-value vector sum passed to a `const C3DVector&` parameter (`Create(effect, pos + C3DVector(0, 0, 0.3f),
  false)`) came out 9 bytes short; retail's own frame words and `lea edx` (0x0087FC50..0x0087FC5C) are a NAMED local
  (`C3DVector effect_pos = ...; Create(effect, effect_pos, false);`).
- (0x0087F3A0) A CCharString call result that retail re-addresses with `lea` and destroys before the next statement is a named
  local in its own block: `{ CCharString shot_class_name = def->ShotDefinitionClassName.GetString(); shot = AddTC(shot_class_name,
  true, 0); }` (0x0087F7F4..0x0087F820). Same shape as the c2040 lesson, here in a speed TU.
- (0x0087F3A0) One `&&` holding a by-value def handle, an implicit CCharString and a filter temporary gives retail's
  conditional-destruction flag word (`mov [esp+1Ch],2` then `test al,8 / 4 / 2`, 0x0087F5A5..0x0087F643); write the string
  arguments as bare literals (implicit conversion), not `CCharString("...")`.
- (0x0087F3A0) The visible shared GetDef body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY) next to the visible
  GetSubDef body took the first full draft from 434 to 637 equal instructions: use both opt-ins from the first draft of any row
  that looks up a def and a sub-def.
- (0x0087F3A0) A flat (ground-plane) distance test is `(a - b).ToC2DVector().GetSquaredMagnitude()`, not
  GetSquaredMagnitude2D, when retail subtracts Y before X and never forms the Z difference (0x0087F3CB..0x0087F3E6).
- (lane) md.py does not mask the DIR32 of a scaled table load (`mov eax,[eax*4+table]`): its "first difference" at 0x0087F969
  on a row q.py grades RELOCATION_MATCH is that artefact; a full match shows as 715 equal of 716.
- (lane) An agent directory that inherits another chunk's overlay should be pruned to the headers its own TU includes
  (`g++ -M` with the overlay first in the include path lists them) before the hand-back.

Review at landing:
- 66 full-row grades over three sessions (46 author incl. 4 SRC_FAIL, 9 scout, 11 finisher incl. 5 diagnostics); probe compiles
  on top: about 30 fwsize / /Od size probes (scout/fw, probe/), 1 vland run.
- Two shared inline BODY opt-ins in engine/C3DVector.h (DOT_XY_THEN_Z fitted, MAGNITUDE_XY_THEN_Z proven by FableWin size).
  Default text unchanged; if either is ever made the default, regrade all C3DVector users.
- NAME_FIXES.tsv: 0x006624F0 (class CThingCreatureBase, landed caller 0x00911C20), 0x007BF790 (namespace, CDefPointer and
  reference parameters, landed caller 0x008F3630), 0x0087E760 (CEventKicked::GetType, landed placeholder gapscan_sub0087e760),
  0x0087FEA0 (CEventKicked scalar deleting destructor, landed as a global CEventBase destructor).
- New headers engine/CTCRotate.h and engine/NEntityEvents_CEventKicked.h; the latter cannot be combined with
  engine/NEntityEvents_CEventHitBase.h (two CEventBase declarations): merge them when a row needs both.
- `OPINION_POST_DEED_TYPE_ALL = 7` is a landed spelling (0x006D00A0, 0x00ECB0E0), not PDB-proven; the row defines the enum
  locally as 0x006D00A0 does. A shared EOpinionPostDeedType header would remove the per-row copies.
- vland prints RELOC_NAME for 0x005DCC00 (`AnimDef_FilterAnimationList`): stale label, the bound spelling is
  CAnimationSet::GetRandomAnim.
- Header debt (fwsize DIFF, repo default texts, not changed here): C3DVector operator+ / operator- / operator*(float) are 6
  instructions short of FableWin (named-result pattern), ToC2DVector and both GetSquaredMagnitude texts 2 short, CThing::IsDead
  4 over.
