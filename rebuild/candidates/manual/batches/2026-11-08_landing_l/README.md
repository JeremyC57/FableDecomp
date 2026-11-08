# Landing 2026-11-08l: chunk 864 (Fable retry with research rules), 1 of its 2 matches

RELOCATION_MATCH + PASS: a read-only vland grade with a trimmed overlay (engine/CQ_EndGameBossBattleScript.h only), then `--land`.
Hand-back: `pending/2026-11-08_linux_hand864/`.

| Row | Note |
|---|---|
| 0x00D33730 NScript::CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main | the first row landed under the owner's 2026-11-08 inline-budget decision (EXTRA_RULES): `#pragma inline_depth(0)` ... `#pragma inline_depth()` round the talk branch's first thread-check return makes c2 call ~CTimer 0x00CD4470 there as retail does. Chunk 856's 103 plain-C++ grades and its /d2inlS0-S20 and /d2inlT50-T100 flag runs never reproduced the single-site call. Line 2 carries the `// Inline-budget:` evidence |

- **Not landed: 0x0050A650 CWorldMap::GetMapNavigationAreaInit.** It matches only with the new opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY,
  which puts STAND-IN noinline bodies for GetAllTCsInRoughArea / GetAllTCDsInRoughArea into the shared header. Those bodies are not
  retail's algorithm (retail's are 0x00518340 / 0x005185F0 / 0x00518900, two of them landed), so the row would fail "behaves the same as
  retail". It went back to the agent to build the header bodies from retail's logic.
- **Header landed:** CQ_EndGameBossBattleScript.h opt-in CTHEREALGUILDMASTER (the class: 0x1c, ParentClass +0x14, PMasterData +0x18).
  The change is purely additive `#ifdef` blocks and no landed file defines the macro. hdrcheck2's default comparison was vacuous
  (cpp-err), so the diff was checked by hand.
- **Names:** `ghidra_out/labels_landing_2026-11-08l.tsv` (0x00D33730; 0x0084E810 NCameraModes::CBase::AddSubComponent, FableWin.pdb + the
  landed 0x008732D0). Queue `land0811m`: the map<CCharString,CCharString> ctor / dtor 0x009AC2D0 / 0x009AC310 are landed with
  placeholder names (vland RELOC_NAME, one source: CHECK against Ego_r). The catalog sync was restricted to this landing's addresses.
