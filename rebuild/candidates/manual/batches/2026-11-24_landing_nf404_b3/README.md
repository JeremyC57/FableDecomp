# Landing nf404 b3: five IsThingAlive stand-ins respelt to their real virtuals

| Address | New spelling |
|---|---|
| 0x008D04B0 | `?IsAlive@CGameScriptThing@@UBE_NXZ` |
| 0x0093A890 | `?IsStillValid@CAIStateGroup_BanditSitRoundCampfire@@UBE_NXZ` |
| 0x009445C0 | `?IsStillValid@CAIStateGroup_WorkSawingWood@@MBE_NXZ` |
| 0x00944CE0 | `?IsStillValid@CAIStateGroup_WorkHammerAnvil@@MBE_NXZ` |
| 0x0095EE40 | `?IsStillValid@CAIStateGroup_OpinionReactionRoot@@UBE_NXZ` |

Each name has a FableWin.pdb and an Ego_r.pdb address at the same vtable slot (NF404 NOTES). Each row's class header gets one add-only, default-off opt-in. All five rows are RELOCATION_MATCH PASS.

self_symbols: the five lines are respelt (previously the inline helper `?IsThingAlive@@...`, then the made-up `IsThingAlive_<addr>`), and column 4 is aligned.

Gates (NF404):
- vland HEAD/new;
- pk 0 NEQ / 0 reach flips;
- /FORCE link duplicates 0 -> 0;
- census by header reach. Only 0x0093AB70 was flagged, for an assert-name `__LINE__` shift; it stays RELOCATION_MATCH.

The coordinator regraded all 77 landed includers of the five headers from the repo (logs/includer_regrade.log).

Coordinator: 5/5 landed, 0 LITERAL_DIFF, T3 nf401 0x008D04B0 closed, baselines OK.
