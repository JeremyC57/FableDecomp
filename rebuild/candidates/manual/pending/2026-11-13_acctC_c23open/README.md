# acctC open drafts after agentC23 (2026-11-14) — nothing matched

| address | proven name | grade |
|---|---|---|
| 0x00601310 | CTCInventoryMap::ConstructQuestsScreenPC (catalog: ConstructQuestsScreen_MainMenu) | DIFFER(6958v6931); `_diag` 6943v6931 with retail's call list |
| 0x0061E6D0 | CTCInventoryQuests::ConstructQuestListPC | DIFFER(12340v12356); `_diag` 12356v12356, 464/464 calls, frame packing left |
| 0x0058578B | CTCInventory::OpenInventory (catalog: vfunc_30) | DIFFER(7948v8081), ported only |

Finding: dead release asserts in inline helpers emit no bytes but grow the caller's inline budget; FableWin sizes say
CCountedPointer Get / IsNull / copy ctor / operator= each carry one. `_diag` sources use diagnostic assert headers and are
not landable as they are. Successor agentC26 tests one FableWin-backed assert set across these rows and the parked map rows.
