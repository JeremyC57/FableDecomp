# NF329 lessons (2026-11-24)
- Re-triage by dry `gen` over every entry, not by the `// Leaf:` line: most flattened rows have no Leaf, so a Leaf test says nothing; the
  generator finding one flattened definition is the real open check (advisor).
- NF325's landed CCombatSequence models already declared every RTTI-labelled slot of their classes, so 57 of NF324's "flat-header" Q1 rows
  were switch-route rows by the time this pass started: re-read the tree before generating headers.
- The CCombatActionBase branch-1 model (FABLE_RETAIL_KRAKEN_ROAR) compiles standalone only with FABLE_RETAIL_BASE_OBJECT, _DEF_CLASS_BASE,
  _CCREATUREACTIONBASE defined and CSoundPair / CAnimationEntry declared; genmodel's --prefwd puts the forward declarations inside the
  opt-in, and the `// Needs` line carries the switches so the row generator can add them (flatten_pilot v5 reads it).
- grep -v SAME hides vtcmp's own summary line (it contains "SAME"): summarise probe logs with a script that checks slot counts and the
  slot-0 alias tail (tools/probesum.py), not by eye.
- flatten_pilot v4 treats every nf324 T3 line as a reservation; a pass that is released to those lines needs an explicit exemption keyed
  on (source nf324, column 2 in ENTRIES, no unit column).
- The 118 generated headers have no includer outside 18 landed rows (no header includes a CCombatAction / CCombatSequence leaf header),
  so the include-closure equiv_check is small; the planted control must still flag every one of those 18.
