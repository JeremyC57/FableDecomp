# acctC hand-back wfB1b (parked rows, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00652FA4 | NPlayerGui::CDrawQuickAccessIcons::Draw | 4,653 | RELOCATION_MATCH (`/GS`) | PASS |
| 0x0058578B | CTCInventory::OpenInventory | 8,081 | RELOCATION_MATCH | PASS |

Coordinator-verified (`coordverify.log`) and rule-audited by an independent agent: no hard violation.
REVIEW ITEMS:
- Both rows wrap same-TU callee bodies in `#pragma auto_inline(off)/(on)` without an `// Inline-budget:` line. This is the
  same-TU real-body idiom of docs/pipeline/GOTCHAS.md (199 of 200 sampled landed files carry no marker), not an inline-budget
  residue; ruled as review, not violation.
- 0x00652FA4 FITTED (marked on line 2): `C2DVector text;` declared one scope out; shared-header opt-in
  FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_CHECKED_LOCALS (operator-> text with named locals; same behaviour as `return Get();`;
  owner ruling wanted); `def.GuildSealMainLevelUpOffset + def.DPadRingsPos` operand order; FABLE_FORCEINLINE ToC2DCoordI.
  /GS is byte-evidenced (cookie 0x0139C8A8). Its same-TU DrawIcon body is the landed 0x00645292 statements respelt on shared
  headers (the landed row uses local stand-in classes); the landed siblings also set optimize("y", off), this row does not.
  The twin 0x00651B04 (staged in 2026-11-13_acctC_c2a) should be respelt with `long HUDRingAlpha`.
- 0x0058578B FITTED (line 13): bare-brace scopes for augmentation / suit / appearance; the bare block around the quests-entry
  action (line 332) is unmarked. Header comment lines 15-16 mention optimize("y",off) but the source only has ("s", on).
  New opt-ins FABLE_RETAIL_CTCINVENTORY_OPENINVENTORY, FABLE_RETAIL_CINPUTMANAGER_JOYSTICKS. The test covers the count loop
  and entry tests only, not the whole function.
