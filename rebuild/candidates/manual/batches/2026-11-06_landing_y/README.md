# Landing 2026-11-06y: chunk 824 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows, sharing the lane with the chunk 823 and 825 agents. All six were retries of earlier parks with
proven names. The landed row is plain C++ and grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00EDD170 | NScript::CV_TempleOfLightScript::CTOL_AcolyteGood::Main | RELOCATION_MATCH PASS (130 lines before) |

- **CTOL_AcolyteGood.** The fix is chunk 813's loop form,
  `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); ... }`. VC7.1 then puts the shared
  [~altar, ~seh_me, ret] return tail after the last pad, as retail does.
- **Headers:**
  - engine/CV_TempleOfLightScript.h is copied in with chunk 811's additive opt-in `FABLE_RETAIL_CV_TEMPLEOFLIGHTSCRIPT_ACOLYTEGOOD`,
    which the landed row defines. No other landed file defines it, so every other includer keeps the default path.
  - The parked rows' opt-ins stay in `pending/2026-11-06_linux_hand824/hdr/`:
    - CV_ChapelOfEvilScript.h (chunk 814's);
    - CTCCreatureOpinionOfHero.h, the new `FABLE_RETAIL_COPINIONTRANSIENTOFFSET_BINARY`.
- **vland:** no new reloc binding; only the usual RELOC_ALIAS for the rand import thunk.
- **Names:**
  - LABELS.tsv is empty.
  - Two landed spellings are queued as `land0611y` PRIORITY:
    - 0x004793F8 is `?TransferBinaryIn@COpinionTransientOffset@@QAEXAAVCMemoryDataInputStream@@@Z` (reference). The landed 0x00685F80
      row binds the pointer spelling.
    - 0x00685F80 is the member template `??$Transfer@VCOpinionTransientOffset@@@CPersistContext@@...ABV1@@Z`. The landed caller
      0x006861C0 binds a non-template overload.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`).
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand824/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00684610 CTCCreatureOpinionOfHero::OnSerialise | DIFFER(2823v2823), 4 lines (was 122), 21 attempts | the hoisted default struct is zeroed in ctor order, not memory order. The gain came from same-TU Transfer<long> / Transfer<COpinionTransientOffset> bodies under `#pragma auto_inline(off)` |
| 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main | DIFFER(7692v7655), 36 lines, 13 attempts | teleport-loop register pair; the natural-exit ~seh_me stores come out in reverse order in this TU only; first exit pad not cross-jumped |
| 0x007A7D20 CTCBattleCharge::CreateHitEffect | DIFFER(2504v2520), 56 lines, 11 attempts | frame 0x140 v 0x148; box corners stored as computed; sound lookup registers |
| 0x008E9B70 NCreatureMode::CModeStrafe::AnimUpdate | DIFFER(1699v1705), 61 lines, 11 attempts | PullBackAmount reloaded where retail spills it. Untried: same-TU callee bodies |
| 0x00D01630 NScript::CQ_BanditCampScript::CGate1GuardOuter::Main | DIFFER(5134v5178), 78 lines, 8 + 3 diagnostic attempts | the documented MovieBase dtor blocker (4 sites called); `/d2inlS15` (diagnostic) fixes the slots but overshoots |
