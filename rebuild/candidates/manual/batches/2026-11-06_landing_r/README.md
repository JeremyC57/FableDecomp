# Landing 2026-11-06r: chunk 817 (3 of 6 rows landed, 3 parked)

One expert agent authored the rows, sharing the lane with the chunk 816 and 818 agents. The chunk mixed two kinds of row:
- three retries of the closest earlier parks;
- three new entity Mains of 11.9-12.6 KB, two of them ending in jump tables.

The agent proved every new name from vtable slot 1 and both PDB name sets. The landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D52E90 | NScript::CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster::Main | RELOCATION_MATCH PASS. It ends in a 4-entry jump table and started from its melee twin 0x00D58490 (chunk 815). Each phase counter gets its own block, plus one function-scope variable that every phase assigns |
| 0x00E07640 | NScript::CQ_TraderEscortScript::CDarkwoodTrader::Main | RELOCATION_MATCH PASS. It ends in a 4-entry jump table. The else-wait is written `T; for(;;){NSF; T;}`; CTimer uses `operator<(long)`; SetEntityAsRegionFollowing takes the hero first |
| 0x00E87030 | NScript::CV_FisticuffsClubScript::CTyler::Main | RELOCATION_MATCH PASS. `CDefPointer<const CScriptDef> pGlobalScriptDef` preloads esi, and the no-return arm is written second |

- **Manifest:** 0x00D52E90 was in neither the manifest nor the gap scan, so vland first refused it as OUTSIDE_MANIFEST. It was added
  to `rebuild/manifest/boundary-starts.tsv` (evidence: vtable slot, PDB name, extent) and then to the manifest with
  `crawl/manifest_add_boundaries.py --write`. Known functions are now 57,613.
- **Headers copied into the repo** (both needed by landed rows):
  - engine/CV_FisticuffsClubScript.h, new: chunk 813's draft with CFisticuffsMember, plus CTyler, behind `FABLE_RETAIL_NSCRIPT`. No
    landed source included it before.
  - engine/CQ_TraderEscortScript.h, opt-in `_DARKWOODTRADER_SETBRAINSTATE`: one declaration, additive.
  - The parked rows' overlay (the WoodsDeparture, TraderConflictGood and ChapelOfEvil drafts) stays in
    `pending/2026-11-06_linux_hand817/hdr/`.
- **vland warnings:**
  - RELOC_NAME on the STLport map dtor, on 0x0099E4C0 and on 0x00415D70 (placeholder labels);
  - RELOC_NAME on 0x00E0A510 (queued below);
  - RELOC_ALIAS on the `rand` thunk.
- **New bindings: two addresses now carry a second symbol.** In each case the new spelling is the PDB one and the old one is wrong.
  The old rows are queued for respelling:
  - 0x00E7E020: the PDB `CPredicate_And<CIsThingAlive,CIsVariableFalse>` ctor beside row 00e8b020's `CAndPredicate`, queued in
    chunk 813 (tag land0611n).
  - 0x00E83680: `CTyler::CleanupAfterFight` in its PDB scope beside row 00e83620's global `?CleanupAfterFight@CTyler@@QAEXXZ`, queued
    here.
  - No symbol is bound at a second address.
- **Names:**
  - Labels: `ghidra_out/labels_landing_2026-11-06r.tsv` holds the three new Mains plus 0x00E0A820
    `CQ_TraderEscortScript::TurnToBalv`. The catalog already had the right name; the Ghidra label was the autoname
    `Script_Darkwood_Balverine_Trader`.
  - Two catalog renames are queued under tag `land0611r`:
    - 0x00E0A510 `CTraderScript::HandleTrader` is `CQ_TraderEscortScript::CDarkwoodTrader::SetBrainState`. It stores its long argument
      at +0x20, the BrainState member.
    - 0x00E83680 `CTyler::CleanupAfterFight` needs its `NScript::CV_FisticuffsClubScript` scope.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`) and the labels are applied.
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand817/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DFE0F0 NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Main | DIFFER(9501v9501), 59 lines, 20 attempts over chunks 815 and 817 | retail cross-jumps the whole TRADERB branch, thread check included, into TRADERC. That merge happens late in VC7.1; any source-level join (a forceinline `\|\|` helper, a goto) reproduces the branch structure but shifts every frame slot by 4-16 bytes |
| 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main | DIFFER(7692v7655), 100 lines | the teleport loop's register pair is swapped; the GROBase vptr store comes before the PImp zeroing instead of after; a +37-byte exit-pad tail (GOTCHAS chunk 801 epilogue cross-jumping) |
| 0x00D647F0 NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main | DIFFER(7405v7389), 316 lines | in the phase-2 lightning branch, retail frees ebx for the Game vtable and VC7.1 spills it instead. One register choice |

Header debt: the row-local `NScript::CIsVariableFalse` (vtable 0x012EA43C) is now declared in two rows (0x00E84B80 parked,
0x00E87030 landed). It should move into its own header.
