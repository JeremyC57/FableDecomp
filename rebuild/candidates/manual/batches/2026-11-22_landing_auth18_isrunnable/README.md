# Landing: 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable as Inferred (owner decision, 2026-11-22)

Row from AUTH18 (Ego_d text, `pending/2026-11-22_auth18/0093d270/`). **Owner decision 2026-11-22: land as Inferred** (asked together with
0x0066B370, which shows the same mechanism; that row lands once its one pick-any copy is solved).

- **Grade:** RELOCATION_MATCH, test PASS, stock flags, strict headers (`logs/vland_dry_strict_headers.log`, `logs/vland_land.log`).
- **The Inferred device:** engine/CCombatManager.h `FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY_REF_INFERRED` (nested in `_PROXIMITY`, off by
  default) makes `GetProximityForNavigation` return `const float&`. All three PDBs (Ego_d, Ego_r, FableWin; a later VC10 revision) give
  `float` by value (QBEMXZ). Retail reads the value like a reference at both call sites (0x006AAA24 in parked 0x006AA720, 0x0093D351 here:
  the manager pointer held across GetPItem, +0x28 read after it); under VC7.1 only the reference form gives that order (by value:
  DIFFER(305v295), the return-value home; mini m3 and the one-grade oracle). A third site in a second class (0x0066B370's CAnimSet
  accessors) behaves the same way, so the TLC-era headers most likely returned references here. The row source carries an `// Inferred:` line.
- **Header proof:** pin_asserts 0 pinned, assert_names_check 0 mismatches; equivalence over 57,022 landed rows: only 0x0093D270 itself differs (the first run also flagged the unwind-funclet rows, a tool artifact fixed the same day; `logs/equivalence_differing_rows.tsv`).
- **Pick-any** (`logs/pickany_vs_retail.tsv`, `logs/leafrel.txt`): leafrel 0 problems; 7 owned copies EQ. `bitset<276>::test` and
  `CThing::HasTC` were byte-identical but relocated to retail's `"bitset"` string at 0x0122E068, which had no table line: added as a first
  witness (`??_C@_06CBNGNPJL@bitset?$AA@`, witness 0x0093D270).
- **Test:** new self-contained model test (no enemy, dead enemy, not still runnable, not navigable, navigable; navigation arguments).
- **Follow-up:** the sibling 0x006AA720 SetPlayerCreatureActionForMelee (DIFFER 1218v1212, the other +0x28 read) should be regraded with a
  fresh draft on the shared headers and this opt-in (its 2026-11-03 draft hangs cl).
