# Native VC7.1 experiment log

Baseline: 2040 bytes, retail 2016, 128/568 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `native_debug_locals` | Use the FableWin local names, declare families before the string, and give the virtual result its long local. | 2048 | 128 |
| `v2_default_last` | Put the default body at the end of the switch to probe hot and cold case layout. | 2048 | 128 |
| `native_scoped_vector` | Place vector lifetime within a lexical block that ends before returning the selection. | 2044 | 476 |
| `native_iterator_size` | Use the vector iterator endpoints and direct iterator indexing in the selection expression. | 2048 | 128 |
| `v2_declared_push` | Keep the vector push_back owner out of line at every site and measure the lost default-arm expansion. | 1784 | 303 |
| `v4_scoped_budget` | Track vector destruction separately and measure output at the original lower inline threshold. | 1724 | 194 |
| `v4_short_finish` | Use the existing shared vector opt-in that keeps finish live through construction. | 2044 | 476 |
| `v4_late_index` | Select using post-increment in the subscript and keep the vector scoped to the lookup. | 2032 | 474 |
| `v4_disconnect_alias` | Call explicit size/visibility path by separating the measured experience from vector construction. | 2044 | 476 |
| `followup_overflow_declaration` | Use the canonical STLport vector with the overflow helper declared but defined by its original owner. | 2044 | 476 |
| `last_overflow_owner_specialization` | Keep the nontrivial CCharString overflow instance declaration-only, using the known original owner. | 2004 | 236 |

Selected `v2_scoped_vector`. 2044 versus 2016 bytes. The cases match the expected ordering and the scoped vector restores register/frame placement at entry. The default-arm realloc path and final temporary/vector lifetime still differ.

The instruction count measures alignment independently of byte equality. See RESULT.json for the byte grade.

## Continued main-chat grades

All 2,016 bytes matched. FableWin shows the returned string copy first, then destruction of the vector and local string. Giving the vector its full function-scope lifetime reproduces that order. The shared STLport overflow declaration resolves the call to 00433530 without generating a divergent duplicate implementation. The two changes together reproduce the shared cleanup for all switch arms. The table has nine exact destinations plus the default. All 125 named operands, 16 literals, ten local targets and five mapped inline copies are verified. The native test covers all experience bands, both default directions, each family entry and unsigned index wraparound.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_bandit_donor_flat | 2040 | 128 | 5 / 1 |
| round2_bandit_donor_flat_default_last | 2040 | 128 | 5 / 1 |
| round2_bandit_donor_flat_finish | 2040 | 128 | 5 / 1 |
| round2_bandit_donor_last_scoped | 2044 | 476 | 5 / 1 |
| round2_bandit_donor_scoped_switch | 2040 | 128 | 5 / 1 |
| round2_bandit_flat_ONLY | 2016 | 553 | 5 / 0 |
| round2_bandit_flat_OUTOFLINE | 2040 | 128 | 5 / 1 |
| round2_bandit_flat_SCOPE | 2040 | 128 | 5 / 1 |
| round2_bandit_flat_thresh16 | 1724 | 195 | 3 / 3 |
| round2_bandit_flat_thresh28 | 1724 | 195 | 4 / 2 |
| round2_bandit_flat_thresh40 | 1764 | 114 | 4 / 2 |
| round2_bandit_flat_thresh52 | 1724 | 195 | 5 / 1 |
| round2_bandit_last_ONLY | 2016 | 553 | 5 / 0 |
| round2_bandit_last_OUTOFLINE | 2040 | 128 | 5 / 1 |
| round2_bandit_last_SCOPE | 2040 | 128 | 5 / 1 |
| round2_bandit_last_thresh16 | 1724 | 195 | 3 / 3 |
| round2_bandit_last_thresh28 | 1724 | 195 | 4 / 2 |
| round2_bandit_last_thresh40 | 1764 | 114 | 4 / 2 |
| round2_bandit_last_thresh52 | 1724 | 195 | 5 / 1 |
| round2_bandit_scoped_ONLY | 2004 | 236 | 5 / 0 |
| round2_bandit_scoped_OUTOFLINE | 2044 | 476 | 5 / 1 |
| round2_bandit_scoped_SCOPE | 2044 | 476 | 5 / 1 |
| round2_bandit_scoped_thresh16 | 1724 | 194 | 3 / 3 |
| round2_bandit_scoped_thresh28 | 1724 | 194 | 4 / 2 |
| round2_bandit_scoped_thresh40 | 1768 | 114 | 4 / 2 |
| round2_bandit_scoped_thresh52 | 1724 | 194 | 5 / 1 |
