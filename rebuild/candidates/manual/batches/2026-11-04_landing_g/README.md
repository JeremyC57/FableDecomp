# Landing 2026-11-04g: chunk 744 (CQ_HangingTreeGoodScript::DoMission) + the GetGuardsHealth respelling

## Result
- **Landed 1 row** (`logs/land.log`). Graded read-only on the merged tree with no overlay first (`logs/new2.log`): RELOCATION_MATCH + PASS.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00D74430 | `NScript::CQ_HangingTreeGoodScript::DoMission` | RELOCATION_MATCH |

  - The row defines `CScriptThing::operator=` as `__forceinline` in its TU: retail expands the top-level `PartySpawnPoint` assignment
    inline, and plain `inline` or the implicit operator leaves it a call (GOTCHAS).
  - No row source or test contains `__asm`, `naked`, `_emit` or `volatile`.
- **Not landed** (staged in `pending/2026-11-04_linux_hand744/`):
  - 0x00D69480 `CQ_HangingTreeEvilScript::Main` (catalog said GoodScript: wrong class), DIFFER(2118v2118), 15 lines: register rotation
    in the failure branch only.
  - 0x00D6F4B0 `CQ_HangingTreeGoodScript::Main`, DIFFER(2106v2106), 13 lines: the same rotation.
  - 0x00D75060 FinalRound, DIFFER(5237v4044): first draft.
  - 0x00D77690 DoCutscene (3064 bytes): not attempted; NOTES.md has its structure.

## Headers
- `engine/CQ_HangingTreeGoodScript.h`: the remaining PDB methods and the ten nested CHTG_* entity scripts, behind the new
  FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT. Unchanged in the repo since the overlay base eff001d2c; hdrcheck2 `default=same`
  (`logs/hdrcheck2_pre_merge.txt`).
- `engine/CQ_HangingTreeEvilScript.h`: new (used by the parked Evil Main draft).
- **Siblings:** `includers.py` finds one landed source reaching a changed header, SpawnParty 0x00D76030. It does not define the macro;
  rechecked anyway on the merged tree (`logs/recheck.log`): RELOCATION_MATCH + PASS.

## Names
- **Respelled landed row 0x00D77340** "StoreGuardsHealth" to the PDB name `?GetGuardsHealth@CQ_HangingTreeGoodScript@NScript@@QAEXXZ`
  (both PDB name sets list Get/SetGuardsHealth and no StoreGuardsHealth; vland warned RELOC_NAME on DoMission's call). Files renamed,
  Leaf, catalog and manifest updated; it and DoMission were rechecked after the respelling (`logs/recheck_respell.log`): both
  RELOCATION_MATCH + PASS.
- **Labels:** `ghidra_out/labels_landing_2026-11-04g.tsv`, 12 labels; `apply_labels.py`: 3 catalog renames, 7 manifest renames
  (`logs/apply_labels.log`), including 0x00D69480 to `CQ_HangingTreeEvilScript::Main`.
- **Queued** (`land0411g` in `naming/NAME_FIXES_QUEUE.tsv`): the two fixes above as DONE, plus a NOTE that the manifest still lists
  parameters for 0x00D77340 that the PDB symbol does not have.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.
