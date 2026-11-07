# Landing 2026-11-06q: chunk 816 (0 of 6 rows landed, 6 parked)

One expert agent authored the rows, sharing the lane with the chunk 815 and 817 agents. The six rows were unnamed gap-scan entity
Mains of 8.9-11.6 KB; three end in jump tables. The agent proved every name from vtable slot 1 (stored by the landed Alloc) and from
both PDB name sets. No row matched, so vland was not run and no source or header was landed. This batch records the parks, labels
and lessons, as chunk 810's `_k/` did.

## Parked (`pending/2026-11-06_linux_hand816/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00E2C100 NScript::CV_AmbushScamScript::CAS_Trickster::Main | DIFFER(8945v8945), 10 attempts | size exact; ONE instruction swap in the return pad at 0x00E2E38E (retail `mov ecx,ebp; push 0`, ours pushes first); every other line is a relocation. Retried in chunk 818 |
| 0x00D5AE70 NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main | DIFFER(11636v11636), 14 attempts | size and call sequence equal; the stack slots of three ints (0x44 / 0x48 / 0x4C) and two comment-loop temporaries differ. Ten declaration-order permutations had no effect. Retried in chunk 818 |
| 0x00E4B5B0 NScript::CV_BanditTollScript::CTollBandit::Main | DIFFER(10674v10594), 10 attempts | frame 0x11C, not 0x118: the state-2 pause spill gets its own slot instead of sharing warn_timer's; one return pad is placed differently. Retried in chunk 818 |
| 0x00E58D40 NScript::CV_BeggarAndChildScript::CLookoutPointBeggar::Main | DIFFER(9620v9616), 3 attempts | blocker 1 alone. With IMPLICIT_COPY, the only difference left is the predicate copy |
| 0x00D3E2E0 NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::Main | DIFFER(9028v9440), 3 attempts | blocker 1, plus the function-wide inline split: retail expands only in state 0, while the everything-inline diagnostic expands every site |
| 0x00E3BB70 NScript::CV_BordelloScript::CMadame::Main | DIFFER(10069v10149), 2 attempts | blocker 1, the per-site movie ctor/dtor split, and mixed ~CTimer exit pads. The declared-only ~CCountedPointer<MovieBase> with the movie visible took it from 887 to 715 lines |

- **Headers** (pending only):
  - engine/CQ_GuildTrainingScript.h, opt-in `_SPEEDTEST`;
  - engine/CV_BanditTollScript.h, opt-in `_TOLLBANDIT`. Its 11 landed includers were checked with g++ -E only; a VC7.1 recheck is due if the row lands;
  - engine/CV_BeggarAndChildScript.h, new: chunk 813's draft plus CLookoutPointBeggar;
  - engine/CQ_GuildTrainingSkillScript.h, new.
- **Names:**
  - Labels: `ghidra_out/labels_landing_2026-11-06q.tsv` holds the six Mains.
  - Two catalog renames are queued under tag `land0611q`:
    - 0x00E4E0E0 `CTollBanditScript::HandleBanditToll` is `CV_BanditTollScript::CTollBandit::SetUpBanditToll`.
    - 0x00E4DF20 is the nested `CV_BanditTollScript::CTollBandit::Should_I_Flee`, not the global `CTollBandit::Should_I_Flee`.
  - The three RunCutsceneMacro spellings at 0x00CBFB7D were already queued, so they were not queued again.
- **Checks:** labels are applied, and the manifest sync dry run prints 0.
