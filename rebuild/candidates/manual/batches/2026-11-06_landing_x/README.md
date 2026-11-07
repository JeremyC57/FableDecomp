# Landing 2026-11-06x: chunk 823 (0 of 6 rows landed, 6 parked)

One expert agent authored the rows, sharing the lane with the chunk 822 and 824 agents. All six were retries of earlier parks.
- Each was re-graded unchanged against today's headers, then given 11-20 new attempts, all recorded in STATUS.tsv and ATTEMPTS.log.
- None matched. Two got closer: ApplyColourFilter (23 -> 21 lines) and CSickChildsSister (now 67 lines).
- Two rows that had no test now have one: `differ/00d3d150.test.cpp` and `differ/00ec9d30.test.cpp`.
- No header changed. finish.py wrote an empty payload, so there was nothing for vland to land.

| Row | Grade | Residue |
|---|---|---|
| 0x00E90BE0 NScript::CV_GuildMasterScript::CGuildMasterGameFlow::Main | DIFFER(4977v4914), 60 lines | retail expands the movie ctor / dtor at the 2 inner loop sites and calls 0x006E7B60 / 0x006E7B80 at the 3 outer ones; an implicit or plain-inline movie ctor is expanded everywhere at any inline setting (16-way sweep) |
| 0x00D3D150 NScript::CQ_GuildTrainingScript::CAppleGirl::Main | DIFFER(4252v4252), 28 lines | register choice only, in the MANY_MORE_APPLES / NO_MORE_APPLES arms |
| 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter | DIFFER(2923v2916), 21 lines | the third `UpdateFlags \|= 2` scheduling, the bookmark test/or flip one block late, one alignment nop. A by-value `GFLimit` gets 15 lines but contradicts the PDB's const-ref symbol (kept as a probe only) |
| 0x00EC68A0 NScript::CV_SickChildScript::CSickChildsSister::Main | DIFFER(5969v5962), 67 lines | the rotated run-to-mother loop test sits after the near branch where retail puts it after the far one; one GetTimer register swap. An inner scope already fixed the ~WaveTimer / store order |
| 0x00EC9D30 NScript::CV_SickChildScript::CMansLover::Main | DIFFER(4708v4708), 58 lines | ours merges the answer==1 and ShownLetter-else terminate tests; retail keeps them apart, so the block order around the give-letter question differs |
| 0x007DB6A0 CTCTavernGameCardBlackjack::CardsMainGame | DIFFER(2544v2556), 63 lines | case 0x1C's tail is cross-jumped into the 0x17 tail where retail keeps it; two register picks in the 0x1D money check |

- **Names:**
  - All six rows use proven PDB names; there is no labels file.
  - Two landed spellings in `CEngineScreenEffectColourFilter_Render_00b5af60.cpp` are queued as `land0611x` PRIORITY:
    - the call to 0x00B5A3F0 is bound to the placeholder free function `CEngineScreenEffectColourFilter_DoRender`; the PDB name is
      `?ApplyColourFilter@CEngineScreenEffectColourFilter@@AAEXXZ`;
    - the row's own Leaf is a placeholder `__fastcall` free function; the PDB name is
      `?Render@CEngineScreenEffectColourFilter@@UAEXABVCEngineComponentRenderParams@@@Z`. That row also models the class with a
      local pad struct (header debt).
  - 0x00CBE2FF IsDistanceBetweenThingsUnder was already queued (`land0511y` and earlier).
- Parked sources and tests: `pending/2026-11-06_linux_hand823/`.
