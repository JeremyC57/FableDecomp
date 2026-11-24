# Landing nf338 b1..b3 (Account A, 2026-11-24): T1 entries re-read as T3, 5 binder rows respelled

Agent NF338 was handed 320 T1 entries that looked untouched. All of them were already T3_PORT_QUEUE lines from nf284..nf314, so the
coordinator ruled that they be re-read as T3 work. Hand-back: `pending/2026-11-24_nf338_handback/` (NOTES.md first); tools:
`naming_batch/agent_tools/nf338/`.

STATUS: 6 done, 5 done-partial, 205 skipped-port (the recorded T3 reason still holds), 99 skipped-held (the witness is reserved), and 5 not
taken (named only in another line's prose).
- **b1 (3 rows):**
  - 0x00999110 now calls `CAFile::GetCurrentPath` (entry 149, partial: its binder 0x009994A0 is reserved by nf303).
  - Unit u62 (0x0056FAD7 + 0x006116E0) now calls `NUISystem::CManager::OnOpenMenus`.
- **b2 (1 row):** 0x00405650 now uses `NUISystem::CIME::Callback_malloc` / `Callback_DrawFans`, through a new add-only opt-in in `fable_ime.h`,
  `FABLE_RETAIL_CIME_CALLBACKS`.
  - Probe: HEAD built with the edited header is byte-identical. equiv_check shows 0 DIFF over 7 includers, and the planted control flags all 7.
  - Coordinator includer recheck: 6 / 6 RELOCATION_MATCH PASS (`../2026-11-24_landing_nf338_b2/logs/includer_recheck.log`).
- **b3 (1 row):** 0x00A3AA40 now uses the private statics `CCompressorZlib::Alloc` / `Free`.
  - It was landed with `--release-t3 nf285` (D1). nf285's entry-121 line for the same address, whose reason still holds, was re-appended
    verbatim (`b3/T3_READD.tsv`).
- **Agent proof, per row:** objsec ALL_EQ (`.drectve` included, DEF sets equal); link HEAD v new with the new spelling resolving and 0
  duplicates; `vland --recheck-landed` RELOCATION_MATCH PASS. Each row has two PDB sources (`evidence/`).
- **Tables:**
  - The run dirs' own TABLE_CHANGES were applied for the landed witnesses.
  - `TABLE_ONLY.tsv` (6 delete, 2 replace, 0 misses) removes stale lines whose witness is an unchanged row that the compiled objects
    prove already binds the PDB spelling.
  - No new multi-target symbol.
- **T3:** the six lines of finished entries were closed by hand (`T3_CLOSED_BY_HAND.tsv`: entries 62, 112, 186, 195, 223, 224). The five
  partial lines stay.
- **D2:** COORDINATION_CLAIMS line 197 (NF214) was closed under its own item key; the 2026-11-21 `done` line had the same items in a
  different order.
- **D6:** B's pending `acctB_b24` adds `engine/NUISystem_CIME.h` (row 0x00402510). If that lands, `FABLE_RETAIL_CIME_CALLBACKS` moves to
  whichever CIME header becomes canonical.
