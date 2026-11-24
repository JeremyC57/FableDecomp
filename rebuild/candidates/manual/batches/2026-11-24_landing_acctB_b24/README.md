# Landing acctB_b24 (Account A, 2026-11-24): three rows Account B verified and staged

Owner ruling (2026-11-24): "You can verify they match and pass tests, but save them in this repository for landing on the main account
later." B staged these rows in `pending/2026-11-24_acctB_b24/`; A re-graded them on its own tree and landed them.

| address | row | grade |
|---|---|---|
| 0x012133C0 | EH unwind funclet of CPhysicalPlayer::Play 0x00C271F0 (the parent's source included, funclet offset 0) | RELOCATION_MATCH PASS |
| 0x00407A70 | CUserProfileManager::SetAssignedInputKeyboard | RELOCATION_MATCH PASS |
| 0x00407BA0 | CUserProfileManager::SetAssignedInputMouse | RELOCATION_MATCH PASS |

## Decisions
- **0x012133C0 lands as the 8-byte funclet, not the 18-byte manifest extent.** That extent also covers the parent's `__ehhandler` thunk
  at 0x012133C8. The landed convention is the same for 666 such funclets: the thunk is a `manifest-gaps.tsv` line, not a row. For example,
  0x01210A48 is landed at 8 bytes with its thunk 0x01210A50 as a gap, and coverage.tsv already lists 0x012133C0 at 8 bytes. B's notes
  record that the 8-byte cut matches too.
- **The header** `engine/CUserProfileManager.h` takes B's one add-only opt-in, applied to the current header.
  - The opt-in is `FABLE_RETAIL_CUSERPROFILEMANAGER_SETASSIGNEDINPUT`: two public declarations whose spellings are in both Ego_r and
    FableWin.
  - `hdrcheck2.py`: default=same, and every existing macro is the same. No landed source defines the new macro.
  - The landed includers rechecked on the landed tree (`logs/includer_recheck.log`).
- **Pick-any** (`pickany_vs_retail.py`): the two setters' objects export no mapped copies. The funclet's object is the parent's, and
  exports Play (EQ) and a `ceil` copy (NEQ, not referenced by the leaf). The `ceil` copy is a property of the landed parent 0x00C271F0's
  object, not of this row.
- **Not landed:**
  - 0x00402510 GFMain: under repair on B's side.
  - 0x00BE4FE0: held by B until its water twin 0x00BE4660 settles the shared template text.
  - GFMain's NAME_FIXES.tsv (20 lines) is queued in `NAME_FIXES_QUEUE.tsv` as `acctB_b24 PRIORITY`.
- `bindings_multi_check.py`: one multi-target line. It is the existing `?NullCoord@CScriptThing` line from nf326 (queued); none is new.
