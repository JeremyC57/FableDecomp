# Landing nf329 b1 (Account A, 2026-11-24): 80 CCombatActionBase-family rows on 47 new generated opt-in class models

NF329 continued (rulings and route: `batches/2026-11-24_landing_nf329_b3/README.md`; hand-back `pending/2026-11-24_nf329_handback/`).
- **Headers first:** 47 headers gain a `FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` opt-in model `: public CCombatActionBase` (or a landed derived
  base). Each one's `// Needs` line selects CCombatActionBase.h's polymorphic branch through `FABLE_RETAIL_KRAKEN_ROAR` (D-d, accepted).
  The flat model stays as the `#else` branch. List: `HEADERS.tsv`.
  - The emitted `??_7` probe matches retail slot for slot (`PROBES.tsv`).
  - `hdrcheck2`: default=same.
  - `pin_asserts` / `assert_names_check`: 0 / 0.
  - equiv_check plus the planted control: `EQUIV_CHECK.tsv`, `EQUIV_CONTROL.tsv`.
- **Rows:** 80 rows go from the flattened self to the RTTI-placed PDB member. `land_scripted.py --release-t3 nf324` rechecked them, and
  every self grades its PDB symbol. 80 nf324 T3 lines are closed (`T3_CLOSED.tsv`).
- **Includers:** the 18 pre-existing includers of the NF329 headers recheck PASS on the tree with the b4 and b1 headers
  (`logs/includer_recheck.log`).
