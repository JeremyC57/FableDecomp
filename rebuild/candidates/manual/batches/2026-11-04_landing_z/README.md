# Landing 2026-11-04z: chunk 763 (follow-up on chunk 760's 25 under-budget parked rows, 54-297 bytes, C++ only), 8 landed

## Result
- **Landed 8 of 25** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): 2 MATCH and
  6 RELOCATION_MATCH, all PASS.
  - **C2DAlphaMaskInfo::Reset (0x00BE24D0) and its ctor (0x00BE2540):** these are the real PDB class; the CVO_OracleMarker label had
    been propagated. A `C4DVector(0,0,0,0)` temporary gives retail's frame.
  - **0x00441EE0:** the shared STLport vector plus a forceinline CEnginePrimitiveHandle dtor (new sub-macro `_DTOR`).
  - **GradualReorientBody (0x00863280):** the if-form short-way delta.
  - **EntityTeleportToHeroPosition (0x008956B0):** retail headers, two C2DVector locals, and PeekTC, which calls the const
    LowerBound 0x004365B0.
  - **TentacleStrikeLoop::FinishAction (0x008CF7D0):** written float-first, `(long)(TentacleSmackDownTime * GetConstantFPS())`.
  - **`_M_insert_overflow` (0x00CBC130):** the Ego_r element type `pair<unsigned __int64, CPersistInfo>`.
  - **FaceAwayFromBuildings (0x0092BF50):** same-TU comparator / GetPBestLeafThing bodies, and the constant 0.0027777f.
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile`.
- **Parked 17**, each with 10+ grades except 0x00C3A7D0 (`pending/2026-11-04_linux_hand763/differ/`).
  - 0x0093D270 matches only with a `const float&` getter that contradicts FableWin, so it stays parked on the PDB spelling.
  - 0x00C3A7D0 was skipped as "third-party: libpk". **That call is wrong:** libpk rows are in scope (chunk 761 landed several, and
    0x00C3FF70 before that), so the row goes back in the retry pool.

## Headers (4 changed; new sub-macros, all `default=same`)
- CEnginePrimitiveHandle `_DTOR`, used by the matched 0x00441EE0.
- CQuestManager `_REGIONLOCK` and CIEngine / CEngine `_CONVERTWORLDPOS`, used by parked drafts only.
- `ppcmp.py`: all 133 includers same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04z.tsv` (9). `apply_labels.py` made the C2DAlphaMaskInfo names, the 0x00CBC130
  label and the const / non-const LowerBound pair (0x004365B0 / 0x0040F020).
- **0x0062E900 reverted.** The agent's one-source proposal `~CCombatActionBase` was applied by the landing script. It was reverted
  in the catalog and manifest and queued as a PROPOSAL (`land0411z`), to be applied only after the vtable slot 0 check.
  - `finish_landing.sh` now skips any label whose evidence says proposal / one source / unproven.
- **Manifest sync:** `sync_manifest_from_catalog.py` resynced 0x008CF7D0, whose module was repeated in the name.
  `--dry-run` now prints 0 / 0 / 0.
- **Queued PRIORITY** (`land0411z`):
  - CDataOutputStream::WriteEBOOL / WriteSLONG 0x00993E30 / 0x00993EB0;
  - CPersistTraits<bool>::TransferBinaryIn 0x00403EB0;
  - GetActiveQuestFromScriptName returns the nested class;
  - 0x00CBC130 is not CMusicEntry::LoadFromScript.
- `bindings_multi_check.py`: 0.

## Coverage
46,759 genuine of 49,568; 5,831,580 function bytes (57.05%).
