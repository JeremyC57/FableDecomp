# Landing 2026-11-07h: chunk 832, expert retry pass 1 (2 of 10 rows landed, 8 parked)

The user asked for an expert reverse-engineer agent (Fable model) on parked-row retries with a larger budget (25+ attempts per row).
It took the ten closest parked rows (md 2-21).

| Row | Grade | Note |
|---|---|---|
| 0x00D6A560 NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Main | **RELOCATION_MATCH + PASS, landed** | the assault guard as a VALUE local read through the owner-accepted same-type `reinterpret_cast<const CScriptThing&>(guard).IsAlive()` hoists the shared `lea ecx,[esp+2Ch]` above the terminate jne |
| 0x00EA1720 NScript::CV_KnotholeGladeGatesScript::DoInitialBalverineAttack | **RELOCATION_MATCH + PASS, landed** | the same lever at both balverine wait sites (`// Flags: /Gr`) |
| 0x00B5AF80 RenderColorFilteredQuad | DIFFER(2863v2865), 17 | bookmark test/or flip FIXED by a clamp mix; left: the x87 product shape. A probe with `scaledContrast * 1.0f` is RELOCATION_MATCH but was not handed back (no source justification; owner question) |
| 0x00B5A3F0 ApplyColourFilter | DIFFER(2923v2916), 15 | same clamp mix fixes the flip; `or [eax+3D8h],ecx` RMW + nop remain |
| 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate | DIFFER(2340v2340), 4 | x87 / int interleave |
| 0x00CFE030 COracleTablet::Main | DIFFER(3575v3575), 8 | element-address operand order set by an earlier loop's IR numbering |
| 0x00DE8370 CRansommer::Main | DIFFER(4894v4894), 12 | latch-copy register colour |
| 0x00DD5A60 CPrisoner::Main | DIFFER(2246v2250), 21 | per-exit ~CTimer split; the two exits are one inliner bucket |
| 0x00D18DF0 CBH_GangDeputy2::Main / 0x00D0A830 CBanditKing::Main | DIFFER, 2 / 20 | MovieBase caller-size gap (documented blocker) |

No headers, labels or name fixes. Lessons: `pending/2026-11-07_linux_hand832/LESSONS.md`.
