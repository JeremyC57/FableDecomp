# Naming and header work still to do (2026-11-28, account A)

Owner: "do a full recheck of what naming / header work needs to be done on all landed and held but matching rows/functions".
Scope: all 57,393 landed rows, plus the 65 unlanded rows whose bytes match (43 Codex pass 30 hand-back, 16 held by A, 6 parked).
Per row: `ROWS.tsv`. Built by `inventory.py` / `held.py` (scratch inputs: the tracker record at this commit, `NAME_ANSWERS.tsv` from
the pass 31 build, a source scan using verify_and_land's own shared-header check).

## Landed rows (57,393)
| | rows |
|---|---:|
| nothing open (own name proven or a registered guess, no callee to respell or settle, no local class model) | 10,172 |
| header work only | 18,998 |
| naming work only | 13,186 |
| both | 15,037 |

### Naming (from the tracker's own status and tier; `ROWS.tsv` carries both)
| Bucket (own name) | Rows | What it takes |
|---|---:|---|
| done: one spelling, a PDB or std / compiler / library name | 31,421 | nothing |
| research or header port (T3: stand-ins, unknown splits, RTTI-identified) | 19,174 | find the original name, usually port the class model in a shared header, then respell row and callers |
| one spelling, not proven (agree_unverified) | 3,941 | find a second source or register a best guess |
| mechanical respell (T1 1,447, T2 27) | 1,474 | respell to the known PDB spelling (the naming lane's routine work) |
| registered best guess | 1,237 | none unless evidence turns up |
| unmeasured 102, several PDB names at one address 44 | 146 | check by hand |

| Callees | Rows | Bindings |
|---|---:|---:|
| binding spells a target differently from its proven PDB name (respell the caller) | 2,658 | 3,549 |
| binding names a target with no proven name yet | 7,330 | 10,977 |

The callee counts use `rebuild/oracles/reloc_bindings.tsv`, which misses about 3,746 live bindings (NF453), so they are a floor.
3,583 landed rows are named on a T3_PORT_QUEUE line (hold kept; largest tags nf303 684, nf324 393, flatten2 237); 5,794 have a catalog
name that differs from the leaf (`pending/2026-11-28_catalog_name_review/`); 9,189 sources use stand-in identifiers.

**Name not verifiable from the row itself (a separate column, not counted as open):** 12,818 rows have no Leaf line (they grade the catalog
name), 5,066 a prefix Leaf, 5,890 a plain C / extern Leaf (genuine C and vendor symbols, but also some bare method names such as `OnDie`).
6,072 of them are still proven by the tracker from their callers' bindings. A full mangled Leaf per row would make every name checkable.

### Headers (the shared-header rule)
| Item | Rows | Classes |
|---|---:|---:|
| Writes its own copy of a class that has `engine/<Class>.h` | 28,194 | 2,226 |
| Writes its own copy of a class that another include/ file already defines (`fable_string.h`, `stlport/`, namespaced headers) | 16,140 | 977 |
| either of the two (local models to replace with the shared header) | 32,479 | |
| Writes its own copy of a PDB class that no shared header defines yet | 6,307 | 1,235 |
| Local models with pad / unknown byte arrays | 10,461 | |
| Rows with a `// Fidelity:` note | 717 | |

Largest: CThing 2,114 rows, CIntelligentPointer 1,488, CTCBase 1,125, CCombatActionBase 1,095, CDefPointer 1,033 (`CLASSES_REDEFINED.tsv`);
CCharString 5,793 (fable_string.h), CBaseIntelligentPointer 1,644, the STLport tree / iterator types (`CLASSES_IN_OTHER_HEADERS.tsv`;
some names are in another namespace: check before adopting); headers to create: `CLASSES_WITHOUT_HEADER.tsv`.
The header counts are solid (a six-row sample: each is a real local `class X {` with the shared header not included). These rows landed before strict header mode; replacing a local model changes the TU's input, so each one is a regrade (and the pick-any /
strong-duplicate gates). Known related debt not recounted here: R3 strong duplicate definitions (2026-11-27 estimate 1,301 candidates
in 1,046 rows) and the 22 inline-leaf rows.

## Held rows whose bytes match (65): `HELD_MATCHING.tsv`
One line per row with its recorded blocker and what clears it (from pass 30's RESERVED.tsv holds, the paused agent's STATUS.tsv gates,
B's README, the pass 25 HELD.md and the parked notes). A row can have several:

| Blocker | Rows |
|---|---:|
| link / owner hold (a landed owner row or the link check) | 34 |
| an emitted copy differs from retail at its owner's address | 13 |
| name hold (Codex pass 30 name holds, naming ports) | 9 |
| needs a review or ruling | 8 |
| `Unproven` placeholder classes (GameCODA audio: 0x00C0AF90, 0x00C1C460, 0x00C298D0, 0x01210E95, 0x01212472) | 5 |
| strong duplicate of a landed row | 3 |
| all gates pass, waiting on the inline-Main ruling (0x00D05A60, 0x00D06130) | 2 |
| Dinkumware `<vector>` include order the harness lacks (0x00C298D0, 0x00C29A3C) | 2 |
| other: header opt-in not landed (0x0064FDB4), wrong name and gates not run (0x00EB5750: the PDB name is DoAskingHeroToTalkToAssassin), companion row (0x008637D0) | 3 |

No held row writes a local model of a shared-header class. Own names: 60 of 65 are PDB or std names; the 5 above are the exceptions,
and 0x008FC050 / 0x00ACE600 have PDB names the tracker still lists as UNSETTLED. Their callee names are not in this table (they are not
landed, so the binding table has no lines for them).
