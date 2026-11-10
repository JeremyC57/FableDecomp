# Job 3: the three Codex-parked rows, parked again (not landable)

The files here are Codex's retained drafts, copied unchanged from `$S/codex20in/rows/<addr>/`. agentNF made no source change. Each
draft's line-1 description, `// Grade:`, `// Leaf:` and `// Flags:` lines are Codex's. Each `_test.cpp` is Codex's exact-body fixture.
Full histories, probes and evidence are still in `$S/codex20in/rows/<addr>/` and `$S/codex20in/history/<addr>/`.

| Row | Function | Grade | Remaining diff |
|---|---|---|---|
| 0x00E5E350 | `NScript::CV_BodyGuardScript::HenchmanCommentary()` `?HenchmanCommentary@CV_BodyGuardScript@NScript@@QAEXXZ` | DIFFER(584v585), re-measured by agentNF on the current tree with `$S/tools/q.py` (`00e5e350_q_diff.txt`) | One retail instruction is missing: `mov dword ptr [esp+0Ch],0` at 0x00E5E355. Codex lists 2 alignment instructions present in our build, and the branch encodings shift with them. |
| 0x0064FDB4 | `NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt` | DIFFER(2593v2593), Codex `verification/FINAL_GRADE.json` | Frame 0x164 vs 0x16C. Local homes and the final fill geometry/UV push order differ. 114 strict / 20 loose hunks; 898/898 instructions. |
| 0x00651273 | `NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::DrawAt` | DIFFER(1677v1681), Codex `verification/FINAL_GRADE.json` | Frame 0x214 vs 0x210. Colour and geometry homes differ, and so does the operation order. 124 strict / 59 loose hunks; 601/602 instructions. |

The two DrawAt rows have no oracle in the catalog or batch TSVs, so `q.py` prints "no oracle". Their grades are Codex's measurements
through its own harness. Their builds also need Codex's header integration `$S/codex20in/hdr/` (e.g.
`engine/CEnginePrimitive2DSprite.h` with the original Layer/BlendMode enum fields). 0064fdb4 also uses the undocumented flag `/d2inlS200`
(`// Flags: /Os /d2inlS200`), which the coordinator should confirm is acceptable before any landing.

## Additional observation on 0x00E5E350 (agentNF)

- **Retail store order.** The zero at 0x00E5E355 goes to `[esp+0Ch]`. That is the home of the second `RegisterTimer` result
  (`mov [esp+0Ch],ebx` at 0x00E5E386). The first result goes to `[esp+10h]` at 0x00E5E378.
- **Which timer.** The single early exit destroys `[esp+0Ch]` first (`lea ecx,[esp+0Ch] ; call 0x00CD4470` at 0x00E5E580, then
  `[esp+10h]`). So the zeroed slot is the second-constructed timer (CheckExpensiveThingsTimer in the draft's declaration order), not
  the first as `FINAL_UNFINISHED_GUIDANCE.md` says.
- **Address-taken.** Both timers' addresses escape only through that out-of-line `~CTimer` call. VC7.1 therefore treats both as
  address-taken, and a store into either before an opaque call is not dead.
- **Ego_r.pdb locals.** register_scripts.obj lists only CheckExpensiveThingsTimer (−28), IdleConversationTimer (−32) and
  HeroLastPosition (−24). No extra local could own the zero. The frame is exactly 4+4+12 = 0x14 bytes, so no hidden temporary has a
  slot of its own either.
- **Declaration order.** Ego_r records CheckExpensiveThingsTimer first. FableWin's debug offsets (Idle −32 above Check −36) suggest
  Idle was declared first.
- **What this does not explain.** Why only one of the two timers keeps an initial store. A constructor-level zero (Codex families
  002/007/025–027) gives both timers the same treatment.
- **Untested idea.** A source in which the two timers are constructed differently (for example, one direct and one copy-initialised
  from a temporary that shares its slot) might explain it. This was not tried here, and Codex's family 005–006/015/031/033 covers
  most such spellings.
