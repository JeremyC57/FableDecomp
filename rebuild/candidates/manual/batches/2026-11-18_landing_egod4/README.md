# 2026-11-18 landing egod4: the two rows a Fable review matched (2,644 bytes)

Round 3 (`../../pending/2026-11-18_egod3/`) escalated two register-allocation residues; per the owner's rule a Fable reviewer looked
at them, read the source structure from the retail branches, and matched both (`../../pending/2026-11-18_egod4/return/fable_review/`).
Author egod4a then ported the remaining local class model and wrote the behaviour tests.

| row | name | bytes | what closed it |
|---|---|---:|---|
| 0x00BE2CD0 | CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables | 1,088 | `/GS`; the tri-list stores written in each arm of one if / else (`poly_verts[0], [1], [2]` in order, destinations swapped), not one shared `+2` store before the branch: retail's store above the branch at 0x00BE2F74..95 is VC7.1 hoisting a store both arms make, and where it sits in the arms sets the strength-reduced cursor base (0x00BE2E4E) |
| 0x0059FEE3 | CTCInventoryTrade::AddBuyItemToList | 1,556 | the "unavailable" arm written twice: outer if (affordable && (space \|\| special) && quantity > 0), inner if (!already_have_special) whose else repeats the outer else -- retail 0x0059FF9A..A0 enters the else one instruction past its head (the arms' common `push -1` hoisted, the inner else tail-merged); PeekTC with no inventory_type local (Ego_d has none); CThingPlayerCreature from the shared header (opt-ins as the landed sibling 0x005A04F7) |

- Dry run and landing, `LANDVERIFY_STRICT_HEADERS=1`: both RELOCATION_MATCH + PASS (`logs/`). No header changes (0x00BE2CD0's opt-in
  landed in egod3). Purity clean; funclets 1,355 / 0 broken; bindings_multi_check at baseline.
- Tests: 0x00BE2CD0 checks the overflow guard, a hand-worked 1x1 quad and 10 grid sizes three ways against an independent model (the
  unreachable flip / short-count arms are argued by parity and an exhaustive 16x16 check); 0x0059FEE3 drives 9 cases through every
  condition both ways with a fake menu entry and checks every call, parameter and release; 5-6 one-decision mutants fail each.
