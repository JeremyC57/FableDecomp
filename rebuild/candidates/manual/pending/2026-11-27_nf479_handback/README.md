# NF479 hand-back (2026-11-27): Codex pass 29's 159 NAME_FIXES

The owner asked for Codex's name fixes "done immediately after checks are done" (2026-11-26). This is the only naming work running: other naming is still paused (owner, 2026-11-27: "I only wanted the codex changes done now"). `README_agent.md` is the agent's report. `STATUS.tsv` gives each Codex line's outcome:
- 24 done (some partial);
- 17 done-already;
- 12 Codex-row only;
- 100 blocked;
- 6 deferred.

## Landed: 47 rows (`batches/2026-11-27_landing_nf479_b1..b5`)
| batch | landed | what |
|---|---|---|
| b1 | 3 | WordWrapText self (private const `ABE`) + binder; `?PWordWrapFont@CSubtitleRenderer@@0PBVCFontBank@@B` (2 binders); registry line 0x013B87AC removed |
| b2 | 3 | 0x006EA540 by-reference self + its only old-spelling binder 0x007E5B90 (one unit); 0x009F32A0 self (ESurfaceMultisampleType) |
| b3 | 26 | data binders: CRC32TAB (7), CCharString::NoStrings (10), NEngineInternal::GPLocalDetailGenerator (8), NEngineInternal::GPEngineEnvironment (1) |
| b4 | 12 of 13 | self respells; catalog Module 0x00687D10 -> CInputProcessGameBase, 0x00887830 -> NEnvironmentInternal::CBlendedEnvironmentTheme |
| b5 | 3 | GetStyleCardsListUIComponentPC binder; the anonymous-namespace GetPBestTargetToFaceForCreature (2 binders); stale WriteFloat table line deleted |

- **Held: 0x009AE150** (`?CompileSubDefinition@CDefinitionManager@@IAEXVCCharString@@ABV2@J@Z`). The open T3 line nf297 names it in its unit text: its call to 0x009ADAE0 is modelled on a global CDefPointer struct. The self respell does not settle that, so the row stays under the hold. Its source, self symbol and catalog line are unchanged.
- **Base check by hand.** The lander did not find NF479's `orig/` files under the names it expects, so I compared all 96 copies (48 rows + tests) with HEAD bebf762648: 0 differ.
- **Lander:** every landed row regraded MATCH / RELOCATION_MATCH PASS on the landed tree. TABLE_CHANGES applied with 0 misses and no new multi-target symbol.
- **HAND_TABLE_CHANGES (b3, b5):** applied, 0 misses.
  - **R4 granted (coordinator, on precedent):** the rewitness lines keep a binding line whose witness still binds the old spelling. The accuracy rule keeps such lines, so the tracker shows those callers disagreeing.
- **Selves and catalog:**
  - self_symbols: 15 lines updated (0x009AE150 left out);
  - catalog: 2 names (relabel from the PDB spelling) and their 2 Modules;
  - `descriptive_names.tsv`: the 0x013B87AC best guess removed (now a PDB name).
- **Checks:**
  - bindings_multi: 1 (NullCoord, as before);
  - funclets: 1,753 checked, 0 broken;
  - s units: 205 checked, 0 failed;
  - descriptive_check: 2,017 lines, 0 FAIL.
- **Tracker** (against the p29 state):
  - 17 addresses -> agree_pdb (14 split_fixable, 1 split_unknown, 1 agree_unverified, 1 agree_descriptive);
  - 0 worse.
  - NF479 predicted 18; the 18th is the held 0x009AE150.

## Not done (recorded, not acted on: naming is paused apart from Codex's fixes)
- **Parked drafts.** 0x006C20B0, 0x00843840 and 0x00A2E9A0 would add local models of shared-header classes, so they are parked. 0x004FCFE0's catalog rename waits on R3.
- **Rulings still open** (`RULINGS_NEEDED.tsv`):
  - R1: native wchar_t `_W` vs `G`;
  - R2: an inline CDefString ctor COMDAT;
  - R3: a CWorldMap.h SetMapAsLoading opt-in;
  - R5: other template instances bound at one address.
- **Rejected** (`REJECTED.tsv`): line 108's anonymous hash `?A0x8192627f` (the PDBs give `?A0xbf588e90`; the hash-free name is right and landed).
- **Locks:** NF479's 49 locks were released before landing; the lander's COORD locks were released after.
