# Naming and header work still to do (2026-11-28, account A)

Owner: "do a full recheck of what naming / header work needs to be done on all landed and held but matching rows/functions".
Scope: all 57,393 landed rows, plus the 65 unlanded rows whose bytes match (43 Codex pass 30 hand-back, 16 held by A, 6 parked).
Per row: `ROWS.tsv`. Built by `inventory.py` / `held.py` (scratch inputs: the tracker record at this commit, `NAME_ANSWERS.tsv` from
the pass 31 build, a source scan using verify_and_land's own shared-header check).

## Landed rows (57,393)
| | rows |
|---|---:|
| no naming and no header item open | 8,124 |
| header work only | 13,495 |
| naming work only | 15,192 |
| both | 20,540 |

### Naming
| Item | Rows | What it takes |
|---|---:|---|
| Own name UNSETTLED (no proven or registered name) | 18,161 | find the original name (PDBs, strings, SDK, FSE, EgoCore), else a registered best guess; then respell row + callers |
| Own name PROVEN_RTTI, leaf not the RTTI slot name | 2,455 | respell to the vftable slot's PDB name (mostly a header port: the stand-in class must become the RTTI class) |
| Own leaf wrong (PROVEN_LANDED_WRONG) | 1,491 | self respell to the known PDB spelling |
| Own name a registered best guess | 1,237 | none unless evidence turns up (counted apart from proven) |
| Own name FOLDED (several PDB names, one body) | 44 | pick the caller's spelling |
| Row's own name proven, but other rows spell it wrong | 2,584 | caller respells |
| Calls a target whose name is UNSETTLED | 7,330 rows, 10,977 bindings | the target's naming, then a caller respell |
| Calls a target that landed rows spell wrong | 5,022 rows, 7,318 bindings | caller respells to the PDB spelling |
| Named on a T3_PORT_QUEUE line (needs a header port; hold kept) | 3,583 | header ports (largest tags: nf303 684, nf324 393, flatten2 237) |
| Catalog name differs from the leaf (saved for later) | 5,794 | `pending/2026-11-28_catalog_name_review/` |
| Leaf not checkable in full: no Leaf line 12,818, prefix Leaf 5,066, plain C Leaf 5,890 | 23,774 | a full mangled Leaf line per row, so names can be verified |
| Source uses stand-in identifiers (`_<addr>`, `Unproven`, `CVtable_`) | 9,189 | goes with the naming above |

The callee counts use `rebuild/oracles/reloc_bindings.tsv`, which misses about 3,746 live bindings (NF453), so they are a floor.

### Headers (the shared-header rule)
| Item | Rows | Classes |
|---|---:|---:|
| Writes its own copy of a class that has `engine/<Class>.h` | 28,194 | 2,208 |
| Writes its own copy of a class that another include/ file already defines (`fable_string.h`, `stlport/`, namespaced headers) | 16,140 | 977 |
| either of the two (local models to replace with the shared header) | 32,479 | |
| Writes its own copy of a PDB class that no shared header defines yet | 6,306 | 1,234 |
| Local models with pad / unknown byte arrays | 10,461 | |
| Rows with a `// Fidelity:` note | 717 | |

Largest: CThing 2,114 rows, CIntelligentPointer 1,488, CTCBase 1,125, CCombatActionBase 1,095, CDefPointer 1,033 (`CLASSES_REDEFINED.tsv`);
CCharString 5,793 (fable_string.h), CBaseIntelligentPointer 1,644, the STLport tree / iterator types (`CLASSES_IN_OTHER_HEADERS.tsv`;
some names are in another namespace: check before adopting); headers to create: `CLASSES_WITHOUT_HEADER.tsv`.
These rows landed before strict header mode; replacing a local model changes the TU's input, so each one is a regrade (and the pick-any /
strong-duplicate gates). Known related debt not recounted here: R3 strong duplicate definitions (2026-11-27 estimate 1,301 candidates
in 1,046 rows) and the 22 inline-leaf rows.

## Held rows whose bytes match (65)
- Headers: none redefines a shared-header class or a PDB class without a header (their drafts were written to the shared-header rule).
- Own names (`HELD_MATCHING_NAMES.tsv`): 60 are PDB or std names. 5 parked GameCODA / audio rows carry `Unproven` placeholder classes and
  need naming before they can land: 0x00C0AF90, 0x00C1C460, 0x00C298D0, 0x01210E95, 0x01212472. 2 Codex pass 30 rows (0x008FC050,
  0x00ACE600) have PDB names the tracker still lists as UNSETTLED.
- 6 use stand-in identifiers; 3 have a Fidelity note.
- Their callee names are not in this table (they are not landed, so the binding table has no lines for them); pass 30's 43 passed the
  name gate at its check, and the 16 held by A were name-checked by the paused agent (auth30b_archive NAME_GATE files).
