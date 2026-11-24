# Landing nf329 b4 (Account A, 2026-11-24): 86 CCombatSequenceBase-family rows on 37 new generated opt-in class models

NF329 continued (rulings and route: `batches/2026-11-24_landing_nf329_b3/README.md`; hand-back `pending/2026-11-24_nf329_handback/`).
- **Headers first:** 37 `engine/CCombatSequence*.h` headers gain a `FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` opt-in model
  `: public CCombatSequenceBase`. Each selects NF325's neutral `FABLE_RETAIL_CCOMBATSEQUENCEBASE` switch, not a `_PASS27` macro. The flat
  model stays as the `#else` branch. List: `HEADERS.tsv`.
  - The emitted `??_7` probe matches retail slot for slot (`PROBES.tsv`). Slot 0 is the NF325 `??_E` / `??_G` alias.
  - `hdrcheck2`: default=same on every header.
  - `pin_asserts`: 0 to pin. `assert_names_check`: 0 mismatches.
  - equiv_check plus the planted control: `EQUIV_CHECK.tsv`, `EQUIV_CONTROL.tsv`.
- **Rows:** 86 rows move from the flattened self to the RTTI-placed PDB member, with the same per-row proof as b3.
  `land_scripted.py --release-t3 nf324` rechecked all 86 on the landed tree, and each graded symbol is the PDB symbol. The 86 nf324 T3
  lines are closed (`T3_CLOSED.tsv`).
- **Includers:** before this pass, the 118 NF329 headers had 18 landed includers. That is the agent's closure, recomputed by the
  coordinator with an anchored `#include "engine/<h>"` grep. On the tree with the b4 headers all 18 recheck PASS
  (`logs/includer_recheck.log`). An unanchored first grep counted 19 rows that do not include these headers; they recheck PASS too.
