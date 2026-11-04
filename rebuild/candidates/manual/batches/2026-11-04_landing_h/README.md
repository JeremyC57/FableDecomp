# Landing 2026-11-04h: chunk 745 (NScript::CV_HeroDuelScript, 4 rows)

## Result
- **Landed 4 of 4** (`logs/land.log`). All four graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  Each matched on its first agent attempt.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00E987C0 | `NScript::CV_HeroDuelScript::Main` (vtable 0x012ED0D4 slot 2) | RELOCATION_MATCH |
  | 0x00E9A970 | `NScript::CV_HeroDuelScript::FirstFleeCutscene` | RELOCATION_MATCH |
  | 0x00E9B240 | `NScript::CV_HeroDuelScript::DuelLost` | RELOCATION_MATCH |
  | 0x00E9BE60 | `NScript::CV_HeroDuelScript::Duel` | RELOCATION_MATCH |

  - All four use `/Gr`, like the landed DoMission from the same retail source file.
  - No `__asm`, helper header, pragma, `volatile` or header change. The class comes from the shared `NScript_CV_HeroDuelScript.h`.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04h.tsv`, 6 labels (4 rows + RegisterMain / Init slot confirmations). Each name has two
  sources: the vtable slot or its only caller, plus the Ego_r name set. `apply_labels.py`: 4 catalog and 4 manifest renames (namespace
  prefix).
- vland warns RELOC_NAME on 0x00CDBFB0, the cutscene cast map's destructor (`std::map<CCharString,...>::~map`); it is already queued
  (NAME_FIXES_QUEUE 534). Not changed here.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.
- Chunk notes: `pending/2026-11-04_linux_hand745/`.
