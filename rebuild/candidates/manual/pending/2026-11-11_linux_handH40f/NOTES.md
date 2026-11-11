# Chunk H40f (Fable 5.2, sixth pass) -- 23 of the longest parked rows: hand-back notes

Every grade was made with `FABLE_HDR_OVERLAY=$A/hdr`. Result: **no row matched**; src/ is empty, payload.json carries 0 rows.
36 grades in all (ATTEMPTS.log), 21 of 23 rows graded at least once; nothing written under the repo, no git command run.
Mid-pass the coordinator relayed two changes from the user: grade more often / touch every row, and the budget is 10 grades per row
PER PASS (only this pass's ATTEMPTS.log counts). STATUS.tsv counts this pass's grades only.

## Rows that moved
- **0x00D0F640 CheckForSecondAreaMassacre -- 229 -> 108 lines, DIFFER(2131v2101), 3 grades.** H40d's constraint system has a
  code-free solution: `for (; !T && true; NSF)` (0.5), `if (Gate3Open || false) break;` (0.5), `if (hero.MsgGetThingsKilled(kills)
  || false)` (0.833: the join after an if whose arm holds a 0.5 return is 0.25 + 0.75 x 0.5 instead of 0.5 + 0.5 x 0.5) and
  `killed >= quota && true` for the massacre block. The inline trace then shows retail's call / expand decision at all 17 sites
  (vector E C E E C E C Cx6 E, ~CScriptThing C E E; tools/iv.sh, work/d0_E.cpp). Left (differ lines 2-3): the const 0 piece is split
  in the two follower loops (killed takes ebp; retail adds through edx and reloads kills._M_start, 0x00D0F81C / 0x00D0F8EB) and the
  four expanded loop exits keep private kills-dtor + epilogue pads where retail jumps to one (0x00D0FE42). Variant B (no `||` on the
  kills test, 2429 bytes, 247 lines) has the same loop hunk, so it is not caused by the folded operand.
- **0x00DB97A0 CNOVI_Theresa::Main -- 888 -> 248 lines by md.py, DIFFER(6856v7013), 3 grades.** `for (; !T && true; NSF)`.
  The byte gap grew (49 -> 157 short), so some sites are now too cold; `|| false` gives 7067 bytes / 773 lines. The zero-v-vtable
  register swap of the old draft is still there (0x00DB97AF).
- **0x0061E6D0 ConstructQuestListPC -- header debt cleared, DIFFER(12357v12356), 5 grades (3 SRC_FAIL while re-basing).** The
  hand882 draft compiles on HEAD and reproduces hand882's last grade exactly. Residue as hand882.

## Rows probed without progress (drafts unchanged; measurements in each differ line 3)
- **0x00C4F600 CEAXManager::SetEAXProperty** (catalog name wrong, LABELS.tsv): only ROOM / ROOMHF / ROOMLF differ. Retail needs one
  more step of the expression-temporary rotation between the buffer apply and ROOM: a temporary that takes ecx and leaves no code.
  The buffer apply has exactly two rotation temporaries (size -> ecx, then the id argument -> eax; `this` -> ecx is not one), the
  ALLPARAMETERS memcmp zero takes ebp and leaves the pointer alone, and the ROLLOFF case re-synchronises both builds (ecx busy with
  the hoisted `data`). 20 spellings are recorded as dead ends. New no-grade tool: tools/sd.py (self-diff of a variant against the
  baseline draft's own object -- no oracle), work/c4/rv.sh (rotation listing by source line).
- **0x00E50FD0**: a plain loop with `&& true` on the attack and phase-0 arms instead of the loop condition is worse (10126, 240 lines).
- **0x00E20600**: `&& true` 329 lines / 14430, `|| false` 309 lines / 14622, plain 355 / 14478 v 14510: per-site, not one factor.
- **0x00E40E80**: outer-loop `&& true` 223, `|| false` 236, plain 220: not the lever for the two late expanded epilogues.
- **0x00E3BB70**: `while (!T && true)` byte-neutral; five movie ctors expanded v retail's four (trace in differ line 3).

## Header debt
- Cleared for 0x0061E6D0 (hdr/README.md). tools/hmerge.py put 9 of the 40 blocks in the wrong place: inside a neighbouring `#ifdef`
  (CTCQuestCard.h, CTCInventoryMap.h x2, CBankFile.h), or it kept the default line after an `#ifndef / #else / #endif` replacement
  (CAction ctor, ConvertVector, ~CUIStateDef, CategoriesList), or added an unconditional `#define` (CGraphicDataBank.h). The
  ~CUIStateDef duplicate still compiled and cost 96 bytes (12260 v 12356) -- a wrong merge can look like a residue.
- Still open: 0x00603000 (hand729), 0x00502E90 (hand879), 0x006096E0 (hand883): one SRC_FAIL grade each. Their headers have their own
  HEAD models now (CThingCreatureDef members, CThingObjectDef, fable_map.h tree members), so the opt-ins must be re-written against
  HEAD, not merged. The mechanical merges are in work/hdr_unverified/ and work/fable_map.merged.h for reference; they are NOT in hdr/.
- 0x0078FD60 and 0x00507C30 still have no source (not reached).

## Rule slips
- Four pg.sh calls with a relative path wrote four bogus lines to ATTEMPTS.log before any compile ran; removed (no grade happened).
- The brief's order (never-graded rows first) was followed only after the coordinator's message; the first hour went into no-grade
  rotation probes on 0x00C4F600.
- differ/00c4f600.cpp, like its landed neighbours (QueueCommit 0x00C51A20, ApplyVoiceBufferProperties 0x00C4EA30), models CEAXManager /
  CEAXVoice with row-local padded structs: it needs a shared header before it can land.
- differ/00e40e80.cpp and differ/00e50fd0.cpp still carry bare `__forceinline` (not in src/).
