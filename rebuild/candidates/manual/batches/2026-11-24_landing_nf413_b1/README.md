# Landing nf413 b1: the last seven baked string rows spell retail's literals

| Row | Literal(s) |
|---|---|
| 0x00406F70 | `"U:\\"` (retail bytes 55 3A 5C 00, a path) plus `L"AutoSave"` and `L""`. The two wide ones went through a `wchar16` typedef that the detector does not see; the named constants are removed (NF410 r1 form). |
| 0x004D4369 | `"CTCDExperienceOrb"` |
| 0x004D66C3 | `"CTCDExplosiveTrail"` |
| 0x004D5FF2 | `"CTCDRumble"` |
| 0x008FD4BC | `"SG_MINION_REACT_TO_CORPSE"` |
| 0x00AA7230 | `"Vertex Buffer Data"` |
| 0x00ABEC70 | `"Vertex List"` |

Only the literals changed; no self changed. Every test now links its row and asserts the text.

Gates (NF413, NF410's method):
- vland: RELOCATION_MATCH PASS on both sides, 0 LITERAL_DIFF / LITERAL_UNCHECKED.
- Sections: the only new ones are `??_C` literals, and every section is byte-equal once the relocation fields are masked.
- purity returns [] for every row and test.
- A mutated literal fails all 7 tests.

Coordinator:
- 7/7 landed; reserved_check found 0.
- The T3 lines naming these rows were set aside for the landing and restored byte-identical. They stay open: the self respells and ports for 0x00406F70 / 0x00AA7230 / 0x00ABEC70 rebase on this text.
- T3 nf406 0x008FD4BC `test` is closed. The two nf321 `held` lines are amended: their literal half is fixed.
