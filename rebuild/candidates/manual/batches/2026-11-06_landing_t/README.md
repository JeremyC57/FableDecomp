# Landing 2026-11-06t: chunk 819 (0 of 6 rows landed, 6 parked)

One expert agent authored the rows, sharing the lane with the chunk 818 and 820 agents. The chunk had two parts:
- three retries: CFisticuffsMember, and the two bandit-camp guards that chunk 812 parked on the MovieBase dtor;
- three new rows: the Will-training CTheRealGuildmaster::Main, CMagicman::Main, and the 18 KB quest-own CQ_ArenaScript::Main.

The agent proved every new name from its vtable slot and both PDB name sets. No row matched, so nothing was landed; this batch records
the parks, labels and lessons. Every row got 10 or more attempts.

## Parked (`pending/2026-11-06_linux_hand819/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00D5E0C0 NScript::CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main (new) | DIFFER(15372v15084), 877 -> 133 lines | VC7.1's inline budget at the exits (the chunk 809 class): the final loop's exit pad expands ~seh_me where retail calls 0x007E74D0, and the last movie ctor expands where retail calls 0x006E7B60. Diagnostic /d2inlT70-72 leaves 70 lines (a park, not a flag) |
| 0x00D09010 NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main (retry) | DIFFER(5880v5864), 203 -> 170 lines | `TalkedTo = false;` written before ClearThingHasInformation now stores from al as in retail. Left: that store is scheduled too early; the movie-dtor vptr store; one exit retail expands that we share; eax/edx colouring |
| 0x00E84B80 NScript::CV_FisticuffsClubScript::CFisticuffsMember::Main (retry) | DIFFER(8968v8836), 132 lines | ~seh_me is expanded at the natural loop exit, where retail calls 0x007E74D0 and jumps to the shared tail. CTyler's defines and includes change nothing; no /d2inlT from 60 to 100 matches |
| 0x00E40E80 NScript::CV_BordelloScript::CMagicman::Main (new) | DIFFER(14772v15104), 282 lines | retail expands two more return epilogues (DeregisterTimer x2 + ~item); one hit-exit pad; eax/edx colouring |
| 0x00D0D910 NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main (retry) | DIFFER(4928v4752), 525 lines | frame 0x110 v 0x10C; MOVIEBASE_FORCEINLINE adds a vptr store; the ~CTimer inline split. Chunk 812's first attempt already had the declared-only spec |
| 0x00F0FB70 NScript::CQ_ArenaScript::Main (new) | DIFFER(18320v18144), 2227 -> 1458 lines | the implicit movie and STB special members plus the declared-only PImp spec now give retail's expanded / called counts. Left: frame 0x2D8 v 0x2BC; retail keeps 0 in edi through the binding block; one extra exit copy |

- **Headers** (pending only):
  - the new engine/CQ_GuildTrainingWillScript.h (sizeof 0x6C; nested CTheRealGuildmaster 0x1C);
  - chunk 812's CQ_BanditCampBossBattleScript.h opt-ins. Its landed includers 0x00D038E0 and 0x00D2AA40 were rechecked and are still RELOCATION_MATCH.
- **Names:** labels in `ghidra_out/labels_landing_2026-11-06t.tsv` (the three new Mains). No name fixes.
- **Checks:** labels are applied, and the manifest sync dry run prints 0.
