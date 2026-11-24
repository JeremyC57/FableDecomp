# NF374 lessons

## Triage
- Tracker `spellings` columns are cut at 160 characters. Rebuild the triage from full reloc_bindings.tsv lines (tools/screen374.py).
- About 80% of T1 binder lines were stand-in or off-limits. Classifying them by demangled diff (tools/kind374.py, dm374.py) left 83 lines worth reading by hand.

## Objects and gates
- objsec must be rename-aware for binder respells. A plain section diff flags every renamed UND symbol. tools/objsec374.py tries bijections between the HEAD-only and new-only symbols, and grades `.drectve` as BYTE_EQUAL, EQUAL_UNDER_RENAME or DIFF.
  - EQUAL_UNDER_RENAME (existing /alternatename lines) is not covered by a ruling yet.
- Shared headers can carry `.drectve` side effects. For example, engine/CWoundMorphs.h adds `/DEFAULTLIB:"libcp"`. Check `.drectve` before choosing a header just for an enum.
- Member-template PDB spellings (`??$Transfer@...`) add /alternatename lines. That makes them a port per NF303 Q2; spot them before compiling.

## Table edits
- A stale table line (its witness's HEAD object already binds the PDB form) is fixed in the table only. Prove it from the object's UND or alternatename strings, not from the source text. 004de9a1's plain Transfer name appears only as an /alternatename fallback.
- land_scripted keeps only TABLE_CHANGES lines whose witness is a landed run row. A rewitness to a non-run row must go in a separate HAND file.
- The reservation check also matches table targets that equal ACCEPT addresses: 007fdeb0 is both a target and an ACCEPT binder. Never put an ACCEPT address in a QUEUE_PROPOSALS unit column.
- After an edit, check every new spelling against the table: exactly one target, either already present or added by the batch.

## Budget
- At about 120 compiles, run pairs of 10 to 15 rows. The HEAD half of a runpair also serves rewitness proofs (UND scans) for free.
